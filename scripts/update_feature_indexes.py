#!/usr/bin/env python3
import argparse
import datetime
import re
import sys
from pathlib import Path


DEFAULT_FEATURE_ROOT = Path("docs/3-Features")
INDEX_NAME = "index.md"
COMPLETE_STATUSES = {"완료", "구현 완료", "검증 완료", "완료됨"}
PARTIAL_IMPLEMENTATION_STATUSES = {"부분 구현"}
DATE_PATTERN = re.compile(r"^\d{4}-\d{2}-\d{2}$")
EXCLUDED_DIRECTORY_NAMES = {"old", "old-2"}


def is_excluded(path):
    return any(
        part in EXCLUDED_DIRECTORY_NAMES or part.startswith("legacy_")
        for part in path.parts
    )


def parse_args():
    parser = argparse.ArgumentParser(
        description="기능 명세 폴더의 index.md 표를 최신 상태로 갱신합니다."
    )
    parser.add_argument(
        "--root",
        type=Path,
        default=DEFAULT_FEATURE_ROOT,
        help=f"기능 명세 루트 경로 (기본값: {DEFAULT_FEATURE_ROOT})",
    )
    return parser.parse_args()


def feature_directories(root):
    if not root.exists() or is_excluded(root):
        return []
    return [
        root,
        *sorted(
            path
            for path in root.rglob("*")
            if path.is_dir() and not is_excluded(path)
        ),
    ]


def feature_files(directory):
    return sorted(
        path
        for path in directory.glob("*.md")
        if not is_excluded(path)
        and path.name != INDEX_NAME
        and path.name.startswith("FEAT-")
    )


def table_value(content, key):
    match = re.search(
        rf"^\|\s*{re.escape(key)}\s*\|\s*(.*?)\s*\|\s*$",
        content,
        re.MULTILINE,
    )
    return match.group(1).strip() if match else ""


def status_row(content, item):
    match = re.search(
        rf"^\|\s*{re.escape(item)}\s*\|\s*([^|]+?)\s*\|\s*([^|]+?)\s*\|",
        content,
        re.MULTILINE,
    )
    if not match:
        return "", ""
    return match.group(1).strip(), match.group(2).strip()


def implementation_status_section(content):
    match = re.search(
        r"^##\s+.*구현 및 검증 상태\s*$",
        content,
        re.MULTILINE,
    )
    if not match:
        return ""

    section_start = match.end()
    next_heading = re.search(r"^##\s+", content[section_start:], re.MULTILINE)
    section_end = (
        section_start + next_heading.start()
        if next_heading
        else len(content)
    )
    return content[section_start:section_end]


def description_section(content):
    match = re.search(
        r"^##\s+\d+\.\s+설명\s*$",
        content,
        re.MULTILINE,
    )
    if not match:
        return ""

    section_start = match.end()
    next_heading = re.search(r"^##\s+", content[section_start:], re.MULTILINE)
    section_end = (
        section_start + next_heading.start()
        if next_heading
        else len(content)
    )
    return content[section_start:section_end]


def first_paragraph(content):
    paragraphs = re.split(r"\n\s*\n", content.strip())
    for paragraph in paragraphs:
        value = " ".join(line.strip() for line in paragraph.splitlines()).strip()
        if value:
            return value
    return ""


def latest_change_date(content):
    dates = re.findall(
        r"^\s*-\s+date:\s*['\"]?(\d{4}-\d{2}-\d{2})['\"]?\s*$",
        content,
        re.MULTILINE,
    )
    return dates[-1] if dates else ""


def plain_text(value):
    return value.replace("`", "").replace("|", r"\|").strip()


def is_complete(status):
    return status in COMPLETE_STATUSES


def feature_record(path):
    content = path.read_text(encoding="utf-8")
    feature_id = plain_text(table_value(content, "기능 ID")) or path.stem
    purpose = plain_text(table_value(content, "주요 목적"))
    if not purpose:
        purpose = plain_text(first_paragraph(description_section(content)))
    status_section = implementation_status_section(content)
    implementation_status, implementation_date = status_row(status_section, "구현")
    verification_status, _ = status_row(status_section, "검증")
    implemented = is_complete(implementation_status)
    verified = is_complete(verification_status)
    implementation_marker = (
        "✅"
        if implemented
        else "⚠️"
        if implementation_status in PARTIAL_IMPLEMENTATION_STATUSES
        else ""
    )

    if not implemented:
        implementation_date = ""
    elif not DATE_PATTERN.fullmatch(implementation_date):
        implementation_date = latest_change_date(content)

    return {
        "id": feature_id,
        "filename": path.name,
        "purpose": purpose,
        "implemented": implemented,
        "implementation_marker": implementation_marker,
        "verified": verified,
        "implementation_date": implementation_date,
    }


def build_table(records):
    lines = [
        "| 기능 식별자 | 주요 목적 | 구현 여부 | 검증 상태 | 구현 일자 |",
        "| --- | --- | --- | --- | --- |",
    ]
    for record in records:
        identifier = f"[`{record['id']}`]({record['filename']})"
        verified = "✅" if record["verified"] else ""
        lines.append(
            f"| {identifier} | {record['purpose']} | {record['implementation_marker']} | "
            f"{verified} | {record['implementation_date']} |"
        )
    return lines


def split_frontmatter(text):
    lines = text.splitlines()
    if not lines or lines[0].strip() != "---":
        return [], lines

    for index in range(1, len(lines)):
        if lines[index].strip() == "---":
            return lines[1:index], lines[index + 1 :]

    return [], lines


def parse_version(frontmatter):
    for line in frontmatter:
        match = re.match(r"^version:\s*(\d+)\.(\d+)\.(\d+)\s*$", line.strip())
        if match:
            return tuple(int(part) for part in match.groups())
    return 0, 1, 0


def format_version(version):
    return ".".join(str(part) for part in version)


def bump_patch(version):
    major, minor, patch = version
    return major, minor, patch + 1


def read_history(frontmatter):
    history = []
    in_history = False
    for line in frontmatter:
        if line.startswith("change_history:"):
            in_history = True
            continue
        if in_history:
            if re.match(r"^[A-Za-z_][A-Za-z0-9_-]*:\s*", line):
                break
            history.append(line)
    return history


def current_table(text):
    lines = text.splitlines()
    for index, line in enumerate(lines):
        if line == "| 기능 식별자 | 주요 목적 | 구현 여부 | 검증 상태 | 구현 일자 |":
            table = [line]
            for table_line in lines[index + 1 :]:
                if not table_line.startswith("|"):
                    break
                table.append(table_line)
            return table
    return []


def initial_history(today):
    return [
        f"  - date: {today}",
        "    version: 0.1.0",
        "    summary: 폴더 내 기능 명세의 목적과 구현 및 검증 현황을 조회하는 인덱스를 작성함",
    ]


def append_history(history, today, version):
    return [
        *history,
        f"  - date: {today}",
        f"    version: {version}",
        "    summary: 기능 명세 인덱스를 최신 상태로 갱신함",
    ]


def build_document(title, version, history, table):
    lines = [
        "---",
        f"title: {title}",
        f"version: {version}",
        "change_history:",
        *history,
        "---",
        "",
        f"# {title}",
        "",
        *table,
        "",
    ]
    return "\n".join(lines)


def update_index(directory, root, today):
    index_path = directory / INDEX_NAME
    title = (
        "기능 명세 목록"
        if directory == root and root == DEFAULT_FEATURE_ROOT
        else f"{directory.name} 기능 명세 목록"
    )
    records = [feature_record(path) for path in feature_files(directory)]
    new_table = build_table(records)

    if not index_path.exists():
        index_path.write_text(
            build_document(title, "0.1.0", initial_history(today), new_table),
            encoding="utf-8",
        )
        print(f"[feature-index] created: {index_path}", file=sys.stderr)
        return True

    existing = index_path.read_text(encoding="utf-8")
    if current_table(existing) == new_table:
        return False

    frontmatter, _ = split_frontmatter(existing)
    new_version = format_version(bump_patch(parse_version(frontmatter)))
    history = read_history(frontmatter) or initial_history(today)
    history = append_history(history, today, new_version)
    index_path.write_text(
        build_document(title, new_version, history, new_table),
        encoding="utf-8",
    )
    print(f"[feature-index] updated: {index_path}", file=sys.stderr)
    return True


def main():
    args = parse_args()
    root = args.root
    if not root.exists():
        print(f"[feature-index] feature root not found: {root}", file=sys.stderr)
        return 1

    today = datetime.date.today().isoformat()
    try:
        for directory in feature_directories(root):
            update_index(directory, root, today)
    except (OSError, UnicodeError) as exc:
        print(f"[feature-index] failed: {exc}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
