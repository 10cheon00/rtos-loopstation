---
title: ARCH-DISPLAY 기능 명세 목록
version: 0.1.1
change_history:
  - date: 2026-07-28
    version: 0.1.0
    summary: 폴더 내 기능 명세의 목적과 구현 및 검증 현황을 조회하는 인덱스를 작성함
  - date: 2026-07-31
    version: 0.1.1
    summary: 기능 명세 인덱스를 최신 상태로 갱신함
---

# ARCH-DISPLAY 기능 명세 목록

| 기능 식별자 | 주요 목적 | 구현 여부 | 검증 상태 | 구현 일자 |
| --- | --- | --- | --- | --- |
| [`FEAT-DISPLAY-001`](FEAT-DISPLAY-001.md) | 현재 UiPanelId, 해당 패널의 모든 Parameter, FX와 트랙 LED 상태를 하나의 DisplaySnapshot으로 표현한다. | ⚠️ |  |  |
| [`FEAT-DISPLAY-002`](FEAT-DISPLAY-002.md) | 최신 UI·FX·트랙 상태로 만든 DisplaySnapshot을 길이 1 display_snapshot_mailbox에 게시하고 기존 원소가 있으면 덮어쓴다. | ⚠️ |  |  |
| [`FEAT-DISPLAY-003`](FEAT-DISPLAY-003.md) | DisplayTask가 정해진 표시 주기에 display_snapshot_mailbox를 비차단으로 읽고, 성공하면 마지막 유효 snapshot을 갱신한다. | ⚠️ |  |  |
| [`FEAT-DISPLAY-004`](FEAT-DISPLAY-004.md) | 마지막으로 유효하게 취득한 DisplaySnapshot.ui_state.panel_id를 renderer table에 적용해 패널 renderer를 선택한다. | ⚠️ |  |  |
| [`FEAT-DISPLAY-005`](FEAT-DISPLAY-005.md) | DisplaySnapshot.ui_state.parameters를 선택된 renderer의 읽기 전용 입력으로 전달한다. | ⚠️ |  |  |
| [`FEAT-DISPLAY-006`](FEAT-DISPLAY-006.md) | DisplayTask가 선택한 renderer는 표시 주기마다 u8g2 buffer를 지우고 최신 snapshot으로 패널의 전체 레이아웃과 표시값을 다시 그린다. | ⚠️ |  |  |
| [`FEAT-DISPLAY-007`](FEAT-DISPLAY-007.md) | DisplayTask가 완성된 u8g2 frame buffer를 LCD 통신 경로로 전송하고 완료 후에만 다음 표시 주기의 출력을 허용한다. | ⚠️ |  |  |
| [`FEAT-DISPLAY-008`](FEAT-DISPLAY-008.md) | DisplayTask가 정해진 표시 주기마다 마지막으로 유효하게 취득한 DisplaySnapshot으로 현재 패널 전체를 렌더링한다. | ⚠️ |  |  |
| [`FEAT-DISPLAY-009`](FEAT-DISPLAY-009.md) | IFX와 TFX의 최신 활성화 상태를 DisplaySnapshot에 보관해 DisplayTask가 표시 주기마다 읽을 수 있게 한다. |  |  |  |
| [`FEAT-DISPLAY-010`](FEAT-DISPLAY-010.md) | DisplayTask가 표시 주기마다 최신 FX 표시 snapshot의 활성화 상태를 IFX와 TFX LED의 on/off 출력으로 변환한다. |  |  |  |
| [`FEAT-DISPLAY-011`](FEAT-DISPLAY-011.md) | 모든 트랙의 최신 상태를 DisplaySnapshot에 보관해 DisplayTask가 표시 주기마다 읽을 수 있게 한다. |  |  |  |
| [`FEAT-DISPLAY-012`](FEAT-DISPLAY-012.md) | DisplayTask가 표시 주기마다 최신 트랙 표시 snapshot의 상태를 트랙별 LED 색상 출력값으로 변환한다. |  |  |  |
