---
title: ARCH-DISPLAY 기능 명세 목록
version: 0.1.1
change_history:
  - date: 2026-07-28
    version: 0.1.0
    summary: 폴더 내 기능 명세의 목적과 구현 및 검증 현황을 조회하는 인덱스를 작성함
  - date: 2026-07-28
    version: 0.1.1
    summary: 기능 명세 인덱스를 최신 상태로 갱신함
---

# ARCH-DISPLAY 기능 명세 목록

| 기능 식별자 | 주요 목적 | 구현 여부 | 검증 상태 | 구현 일자 |
| --- | --- | --- | --- | --- |
| [`FEAT-DISPLAY-001`](FEAT-DISPLAY-001.md) | 현재 UI 패널을 표시 구조가 그릴 수 있도록 panel ID와 바인딩된 모든 파라미터를 포함하는 UI_STATE_RENDER payload를 정의한다. |  |  |  |
| [`FEAT-DISPLAY-002`](FEAT-DISPLAY-002.md) | UI render command를 누락 없이 순서대로 표시 구조에 전달한다. |  |  |  |
| [`FEAT-DISPLAY-003`](FEAT-DISPLAY-003.md) | 표시 구조가 display_command_queue에서 UI_STATE_RENDER command를 순서대로 읽는다. |  |  |  |
| [`FEAT-DISPLAY-004`](FEAT-DISPLAY-004.md) | panel_id에 맞는 LCD panel renderer를 선택한다. |  |  |  |
| [`FEAT-DISPLAY-005`](FEAT-DISPLAY-005.md) | 표시 구조가 command의 panel ID와 바인딩 파라미터를 별도 canonical state 없이 현재 렌더링 입력으로 사용한다. |  |  |  |
| [`FEAT-DISPLAY-006`](FEAT-DISPLAY-006.md) | 선택된 panel renderer가 LCD에 출력할 panel frame을 구성한다. |  |  |  |
| [`FEAT-DISPLAY-007`](FEAT-DISPLAY-007.md) | 생성된 LCD panel frame을 실제 LCD 출력으로 반영한다. |  |  |  |
| [`FEAT-DISPLAY-008`](FEAT-DISPLAY-008.md) | 파라미터 변경이 완료되면 현재 패널의 모든 바인딩 파라미터를 전달하고 패널 전체 frame을 다시 그리도록 요청한다. | ✅ |  | 2026-07-28 |
| [`FEAT-display-mailbox-update`](FEAT-display-mailbox-update.md) |  |  |  |  |
| [`FEAT-lcd-panel-rendering`](FEAT-lcd-panel-rendering.md) |  |  |  |  |
| [`FEAT-led-status-rendering`](FEAT-led-status-rendering.md) |  |  |  |  |
| [`FEAT-level-meter-rendering`](FEAT-level-meter-rendering.md) |  |  |  |  |
