---
title: ARCH-DISPLAY 기능 명세 목록
version: 0.1.0
change_history:
  - date: 2026-07-28
    version: 0.1.0
    summary: 폴더 내 기능 명세의 목적과 구현 및 검증 현황을 조회하는 인덱스를 작성함
---

# ARCH-DISPLAY 기능 명세 목록

| 기능 식별자 | 주요 목적 | 구현 여부 | 검증 상태 | 구현 일자 |
| --- | --- | --- | --- | --- |
| [`FEAT-DISPLAY-001`](FEAT-DISPLAY-001.md) | 현재 UiPanelId와 해당 패널의 모든 Parameter를 UiStateRenderPayload로 표현한다. | ⚠️ |  |  |
| [`FEAT-DISPLAY-002`](FEAT-DISPLAY-002.md) | UiStateRenderPayload를 DisplayCommand에 담아 display_command_queue로 전송한다. | ✅ |  | 2026-07-28 |
| [`FEAT-DISPLAY-003`](FEAT-DISPLAY-003.md) | DisplayTask가 display_command_queue에서 DisplayCommand를 순서대로 꺼내 종류별 처리기로 전달한다. | ⚠️ |  |  |
| [`FEAT-DISPLAY-004`](FEAT-DISPLAY-004.md) | UiStateRenderPayload.panel_id를 ui_panel_render_mappings에 적용해 패널 renderer를 선택한다. | ⚠️ |  |  |
| [`FEAT-DISPLAY-005`](FEAT-DISPLAY-005.md) | UiStateRenderPayload.parameter를 선택된 renderer의 입력으로 전달한다. | ⚠️ |  |  |
| [`FEAT-DISPLAY-006`](FEAT-DISPLAY-006.md) | 선택된 renderer가 u8g2 buffer를 지우고 패널의 전체 레이아웃과 표시값을 다시 그린다. | ⚠️ |  |  |
| [`FEAT-DISPLAY-007`](FEAT-DISPLAY-007.md) | 완성된 u8g2 frame buffer를 LCD 통신 경로로 전송한다. | ⚠️ |  |  |
| [`FEAT-DISPLAY-008`](FEAT-DISPLAY-008.md) | StateTask가 이벤트 처리를 끝낸 뒤 전이 또는 파라미터 변경 flag가 있으면 현재 패널 전체의 렌더링을 요청한다. | ✅ |  | 2026-07-28 |
| [`FEAT-DISPLAY-009`](FEAT-DISPLAY-009.md) | IFX와 TFX의 최신 활성화 상태를 하나의 표시 snapshot으로 수신한다. |  |  |  |
| [`FEAT-DISPLAY-010`](FEAT-DISPLAY-010.md) | FX 표시 snapshot의 활성화 상태를 IFX와 TFX LED의 on/off 출력으로 변환한다. |  |  |  |
| [`FEAT-DISPLAY-011`](FEAT-DISPLAY-011.md) | 모든 트랙의 최신 상태를 하나의 표시 snapshot으로 수신한다. |  |  |  |
| [`FEAT-DISPLAY-012`](FEAT-DISPLAY-012.md) | 트랙 표시 snapshot의 상태를 트랙별 LED 색상 출력값으로 변환한다. |  |  |  |
