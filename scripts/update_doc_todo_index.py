#!/usr/bin/env python3
import datetime
import re
import sys
from pathlib import Path


OUTPUT_PATH = Path("docs/todo_index.md")
DOCS_ROOT = Path("docs")
TODO_PATTERN = re.compile(r"\bTODO\s*:|(?:^|\|)\s*TODO\s*(?:\||$)", re.IGNORECASE)


def is_excluded(path):
    return path == OUTPUT_PATH or any(
        part in {"old", "old-2"} or part.startswith("legacy_")
        for part in path.parts
    )


def markdown_files():
    return sorted(
        path
        for path in DOCS_ROOT.rglob("*.md")
        if not is_excluded(path)
    )


def strip_frontmatter(lines):
    if not lines or lines[0].strip() != "---":
        return lines, 1

    for idx in range(1, len(lines)):
        if lines[idx].strip() == "---":
            return lines[idx + 1 :], idx + 2

    return lines, 1


def normalize_todo(line):
    text = line.strip()
    text = re.sub(r"^\s*[-*]\s+", "", text)
    text = text.strip("|").strip()
    text = re.sub(r"\s*\|\s*", " | ", text)
    text = re.sub(r"^#+\s*", "", text)
    return text


def escape_table_cell(text):
    return text.replace("\\", "\\\\").replace("|", "\\|").replace("\n", " ").strip()


def collect_todos():
    todos = []
    for path in markdown_files():
        lines = path.read_text(encoding="utf-8").splitlines()
        body_lines, body_start_line = strip_frontmatter(lines)
        for offset, line in enumerate(body_lines):
            if not TODO_PATTERN.search(line):
                continue
            content = normalize_todo(line)
            if not content:
                continue
            line_number = body_start_line + offset
            todos.append((f"{path}:{line_number}", content))
    return todos


def split_frontmatter(text):
    lines = text.splitlines(keepends=True)
    if not lines or lines[0].strip() != "---":
        return None, lines

    for idx in range(1, len(lines)):
        if lines[idx].strip() == "---":
            return lines[1:idx], lines[idx + 1 :]

    return None, lines


def parse_version(frontmatter):
    if frontmatter is None:
        return None
    for line in frontmatter:
        match = re.match(r"^version:\s*(\d+)\.(\d+)\.(\d+)\s*$", line.strip())
        if match:
            return tuple(int(part) for part in match.groups())
    return None


def bump_patch(version):
    major, minor, patch = version
    return major, minor, patch + 1


def format_version(version):
    return ".".join(str(part) for part in version)


def previous_table_body(text):
    _, body = split_frontmatter(text)
    table_lines = []
    in_table = False
    for line in body:
        if line.startswith("| 파일 위치 | TODO 내용 |"):
            in_table = True
            continue
        if in_table:
            if not line.startswith("|"):
                break
            if line.startswith("| ---"):
                continue
            table_lines.append(line.rstrip("\n"))
    return table_lines


def build_table(todos):
    lines = [
        "| 파일 위치 | TODO 내용 |",
        "| --- | --- |",
    ]
    for location, content in todos:
        lines.append(f"| `{escape_table_cell(location)}` | {escape_table_cell(content)} |")
    return lines


def initial_history(today):
    return [
        f"  - date: {today}",
        "    version: 0.1.0",
        "    summary: docs 하위 문서의 TODO 항목을 모아보는 자동 생성 뷰를 추가함",
    ]


def read_history(frontmatter):
    if not frontmatter:
        return []
    history = []
    in_history = False
    for line in frontmatter:
        if line.startswith("change_history:"):
            in_history = True
            continue
        if in_history:
            if re.match(r"^[A-Za-z_][A-Za-z0-9_-]*:\s*", line):
                break
            history.append(line.rstrip("\n"))
    return history


def append_history(history, today, version):
    return [
        *history,
        f"  - date: {today}",
        f"    version: {version}",
        "    summary: 문서 TODO 목록을 최신 상태로 갱신함",
    ]


def build_document(todos, version, history):
    body = [
        "# 문서 TODO 목록",
        "",
        "이 문서는 `docs/` 하위 Markdown 문서에 남아 있는 TODO 항목을 모아 보여주는 자동 생성 뷰다.",
        "",
        f"- 생성 기준: `docs/**/*.md`, legacy 문서 폴더, `{OUTPUT_PATH}` 제외",
        f"- TODO 개수: {len(todos)}",
        "",
        *build_table(todos),
        "",
    ]
    frontmatter = [
        "---",
        "title: 문서 TODO 목록",
        f"version: {version}",
        "change_history:",
        *history,
        "---",
        "",
    ]
    return "\n".join(frontmatter + body)


def main():
    today = datetime.date.today().isoformat()
    todos = collect_todos()
    new_table = build_table(todos)[2:]

    if not OUTPUT_PATH.exists():
        OUTPUT_PATH.write_text(build_document(todos, "0.1.0", initial_history(today)), encoding="utf-8")
        print(f"[doc-todo-index] created: {OUTPUT_PATH}", file=sys.stderr)
        return 0

    existing = OUTPUT_PATH.read_text(encoding="utf-8")
    old_table = previous_table_body(existing)
    if old_table == new_table:
        return 0

    frontmatter, _ = split_frontmatter(existing)
    old_version = parse_version(frontmatter) or (0, 1, 0)
    new_version = format_version(bump_patch(old_version))
    history = read_history(frontmatter) or initial_history(today)
    history = append_history(history, today, new_version)
    OUTPUT_PATH.write_text(build_document(todos, new_version, history), encoding="utf-8")
    print(f"[doc-todo-index] updated: {OUTPUT_PATH}", file=sys.stderr)
    return 0


if __name__ == "__main__":
    sys.exit(main())
