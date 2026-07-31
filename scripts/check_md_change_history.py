#!/usr/bin/env python3
import datetime
import re
import subprocess
import sys
from pathlib import Path


DEFAULT_SUMMARY = "문서 변경 사항 반영"
EXCLUDED_DIRECTORY_NAMES = {"old", "old-2"}


def is_excluded(path):
    return any(
        part in EXCLUDED_DIRECTORY_NAMES or part.startswith("legacy_")
        for part in path.parts
    )


def run_git(args):
    return subprocess.run(
        ["git", *args],
        check=True,
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    ).stdout


def changed_markdown_files():
    output = run_git(["diff", "--name-only", "--", "*.md"])
    files = []
    for line in output.splitlines():
        path = line.strip()
        if not path:
            continue
        file_path = Path(path)
        if is_excluded(file_path):
            continue
        if file_path.exists() and file_path.suffix == ".md":
            files.append(file_path)
    return files


def split_frontmatter(text, path):
    lines = text.splitlines(keepends=True)
    if not lines or lines[0].strip() != "---":
        return [], lines, False

    for idx in range(1, len(lines)):
        if lines[idx].strip() == "---":
            return lines[1:idx], lines[idx + 1 :], True

    raise ValueError(f"{path}: YAML frontmatter 시작 표시는 있지만 닫는 --- 표시가 없습니다.")


def find_title(body, path):
    for line in body:
        match = re.match(r"^#\s+(.+?)\s*$", line)
        if match:
            return match.group(1).strip()
    return path.stem.replace("_", " ").replace("-", " ").strip() or "문서"


def version_index(frontmatter):
    for idx, line in enumerate(frontmatter):
        if re.match(r"^version:\s*", line):
            return idx
    return None


def parse_version(line):
    match = re.match(r"^version:\s*['\"]?(\d+)\.(\d+)\.(\d+)['\"]?\s*$", line.strip())
    if not match:
        return None
    return tuple(int(part) for part in match.groups())


def bump_version(version, level):
    major, minor, patch = version
    if level == "major":
        return major + 1, 0, 0
    if level == "minor":
        return major, minor + 1, 0
    return major, minor, patch + 1


def format_version(version):
    return ".".join(str(part) for part in version)


def diff_for(path):
    return run_git(["diff", "--", str(path)])


def classify_bump(diff):
    added_lines = [
        line
        for line in diff.splitlines()
        if line.startswith("+") and not line.startswith("+++")
    ]
    joined = "\n".join(added_lines)
    if re.search(r"BREAKING CHANGE|호환되지 않는 변경|incompatible change", joined, re.IGNORECASE):
        return "major"
    if any(re.match(r"^\+\s{0,3}#{1,6}\s+\S+", line) for line in added_lines):
        return "minor"
    return "patch"


def summarize_diff(path, diff):
    if path.name == "AGENTS.md":
        return "문서 변경 이력 자동화 hook 운영 규칙을 추가함"

    added_headings = []
    for line in diff.splitlines():
        if line.startswith("+") and not line.startswith("+++") and re.match(r"^\+\s{0,3}#{1,6}\s+\S+", line):
            heading = line[1:].strip().lstrip("#").strip()
            if heading:
                added_headings.append(heading)

    if added_headings:
        return f"{added_headings[0]} 관련 문서 내용을 추가함"

    if re.search(r"^\+.*(표|메시지|정책|설정|구조|흐름)", diff, re.MULTILINE):
        return "문서의 설계 및 설명 내용을 갱신함"

    return DEFAULT_SUMMARY


def change_history_range(frontmatter):
    start = None
    for idx, line in enumerate(frontmatter):
        if re.match(r"^change_history:\s*(?:\[\]\s*)?$", line):
            start = idx
            break
    if start is None:
        return None

    end = len(frontmatter)
    for idx in range(start + 1, len(frontmatter)):
        if re.match(r"^[A-Za-z_][A-Za-z0-9_-]*:\s*", frontmatter[idx]):
            end = idx
            break
    return start, end


def today_item_range(frontmatter, history_start, history_end, today):
    for idx in range(history_start + 1, history_end):
        if re.match(rf"^\s*-\s+date:\s+['\"]?{re.escape(today)}['\"]?\s*$", frontmatter[idx]):
            end = history_end
            for next_idx in range(idx + 1, history_end):
                if re.match(r"^\s*-\s+date:\s+", frontmatter[next_idx]):
                    end = next_idx
                    break
            return idx, end
    return None


def ensure_today_item_fields(frontmatter, item_start, item_end, version, summary):
    version_seen = False
    summary_seen = False
    summary_valid = False

    idx = item_start + 1
    while idx < item_end:
        if re.match(r"^\s+version:\s*", frontmatter[idx]):
            version_seen = True
            if not re.match(r"^\s+version:\s*\S+", frontmatter[idx]):
                frontmatter[idx] = f"    version: {version}\n"
            idx += 1
            continue
        if re.match(r"^\s+summary:\s*", frontmatter[idx]):
            summary_seen = True
            value = re.sub(r"^\s+summary:\s*", "", frontmatter[idx]).strip()
            value = value.strip("'\"")
            if value:
                summary_valid = True
            else:
                frontmatter[idx] = f"    summary: {summary}\n"
                summary_valid = True
            idx += 1
            continue
        idx += 1

    insert_at = item_start + 1
    if not version_seen:
        frontmatter.insert(insert_at, f"    version: {version}\n")
        item_end += 1
        insert_at += 1
    if not summary_seen:
        frontmatter.insert(insert_at, f"    summary: {summary}\n")
    elif not summary_valid:
        raise ValueError("오늘 change_history 항목의 summary를 보정하지 못했습니다.")


def add_history_item(frontmatter, history_start, history_end, today, version, summary):
    item = [
        f"  - date: {today}\n",
        f"    version: {version}\n",
        f"    summary: {summary}\n",
    ]
    insert_at = history_end
    while insert_at > history_start + 1 and frontmatter[insert_at - 1].strip() == "":
        insert_at -= 1
    frontmatter[insert_at:insert_at] = item


def ensure_frontmatter(path, today):
    original_text = path.read_text(encoding="utf-8")
    frontmatter, body, had_frontmatter = split_frontmatter(original_text, path)
    diff = diff_for(path)
    summary = summarize_diff(path, diff)
    bump = classify_bump(diff)

    changed = False
    if not had_frontmatter:
        title = find_title(body, path)
        frontmatter = [
            f"title: {title}\n",
            "version: 0.1.0\n",
            "change_history:\n",
            f"  - date: {today}\n",
            "    version: 0.1.0\n",
            f"    summary: {summary}\n",
        ]
        changed = True
    else:
        v_idx = version_index(frontmatter)
        if v_idx is None:
            insert_at = 1 if frontmatter and re.match(r"^title:\s*", frontmatter[0]) else 0
            frontmatter.insert(insert_at, "version: 0.1.0\n")
            current_version = (0, 1, 0)
            changed = True
        else:
            current_version = parse_version(frontmatter[v_idx])
            if current_version is None:
                raise ValueError(f"{path}: version 필드가 SemVer 형식이 아닙니다.")

        history = change_history_range(frontmatter)
        if history is None:
            history_start = len(frontmatter)
            frontmatter.append("change_history:\n")
            history_end = len(frontmatter)
            history = (history_start, history_end)
            changed = True
        else:
            history_start, _ = history
            if re.match(r"^change_history:\s*\[\]\s*$", frontmatter[history_start]):
                frontmatter[history_start] = "change_history:\n"
                changed = True

        history_start, history_end = history
        item = today_item_range(frontmatter, history_start, history_end, today)
        if item is None:
            if v_idx is not None:
                new_version = bump_version(current_version, bump)
                frontmatter[v_idx] = f"version: {format_version(new_version)}\n"
            else:
                new_version = current_version
            add_history_item(frontmatter, history_start, history_end, today, format_version(new_version), summary)
            changed = True
        else:
            top_version = parse_version(frontmatter[version_index(frontmatter)]) if version_index(frontmatter) is not None else (0, 1, 0)
            before = list(frontmatter)
            ensure_today_item_fields(frontmatter, item[0], item[1], format_version(top_version), summary)
            changed = changed or before != frontmatter

    if changed:
        new_text = "---\n" + "".join(frontmatter) + "---\n" + "".join(body)
        path.write_text(new_text, encoding="utf-8")
        print(f"[md-change-history] updated: {path}", file=sys.stderr)


def main():
    try:
        files = changed_markdown_files()
    except subprocess.CalledProcessError as exc:
        print(f"[md-change-history] git diff failed: {exc.stderr.strip()}", file=sys.stderr)
        return 1

    if not files:
        return 0

    today = datetime.date.today().isoformat()
    errors = []
    for path in files:
        try:
            ensure_frontmatter(path, today)
        except Exception as exc:
            errors.append(f"{path}: {exc}")

    if errors:
        print("[md-change-history] failed to validate/update Markdown change history:", file=sys.stderr)
        for error in errors:
            print(f"  - {error}", file=sys.stderr)
        return 1

    return 0


if __name__ == "__main__":
    sys.exit(main())
