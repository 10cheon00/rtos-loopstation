---
title: Repository Instructions
version: 0.4.0
change_history:
  - date: 2026-07-08
    version: 0.1.0
    summary: 문서 변경 이력 자동화 hook 운영 규칙을 추가함
  - date: 2026-07-10
    version: 0.2.0
    summary: 문서 TODO 목록 자동 생성 hook 운영 규칙을 추가함
  - date: 2026-07-28
    version: 0.3.0
    summary: 기능 명세 인덱스 자동 갱신 hook 운영 규칙을 추가함
  - date: 2026-07-31
    version: 0.4.0
    summary: 모든 문서 자동화 hook이 old 및 legacy 문서를 수정하지 않도록 제외 규칙을 명시함
---

# Repository Instructions

## Documentation Change History

Apply this rule only when a request changes Markdown documentation and the change is actually reflected in one or more `.md` files.

Before finishing a documentation edit:

1. Run `git diff -- <file>` for each changed Markdown file.
2. Summarize the meaningful documentation changes from the diff.
3. Update the file frontmatter.
4. Bump the document `version` using SemVer:
   - `patch`: typos, wording cleanup, small clarifications, corrected calculations.
   - `minor`: new sections, new requirements, new design decisions, or materially expanded content.
   - `major`: incompatible changes to documented architecture, formats, interfaces, or project policy.
5. Add one row to `change_history` with:
   - `date`: current date in `YYYY-MM-DD`.
   - `version`: the updated document version.
   - `summary`: concise Korean summary of the change.

When multiple Markdown files are changed, apply the version bump and `change_history` update independently to each changed file.

The project-local Codex `Stop` hook runs `scripts/check-md-change-history.sh` to validate and, when possible, automatically fix missing Markdown frontmatter, `version`, and `change_history` entries.
It must exclude files under `docs/old/`, `docs/old-2/`, and `docs/legacy_*/`.

After that, the same `Stop` hook runs `scripts/update-feature-indexes.sh` to synchronize every `docs/3-Features/**/index.md` table with the feature specifications in its directory. It adds missing feature records and updates implementation status, verification status, and implementation dates from each feature specification.
It must not traverse or update feature indexes under `docs/old/`, `docs/old-2/`, or `docs/legacy_*/`.

Finally, the same `Stop` hook runs `scripts/update-doc-todo-index.sh` to regenerate `docs/todo_index.md`, excluding Markdown files under `docs/old/`, `docs/old-2/`, and `docs/legacy_*/`.

Frontmatter format:

```yaml
---
title: 문서 제목
version: 0.1.0
change_history:
  - date: 2026-07-08
    version: 0.1.0
    summary: 문서 변경점 요약
---
```

Do not apply this rule to files under `docs/old/`, `docs/old-2/`, or `docs/legacy_*/`.
