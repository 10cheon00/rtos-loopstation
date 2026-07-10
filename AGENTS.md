---
title: Repository Instructions
version: 0.2.0
change_history:
  - date: 2026-07-08
    version: 0.1.0
    summary: 문서 변경 이력 자동화 hook 운영 규칙을 추가함
  - date: 2026-07-10
    version: 0.2.0
    summary: 문서 TODO 목록 자동 생성 hook 운영 규칙을 추가함
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

After that, the same `Stop` hook runs `scripts/update-doc-todo-index.sh` to regenerate `docs/todo_index.md`, which lists TODO items from Markdown files under `docs/` except `docs/old/`.

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

Do not apply this rule to files under `docs/old/`.
