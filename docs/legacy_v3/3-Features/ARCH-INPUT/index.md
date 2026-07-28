---
title: ARCH-INPUT 기능 명세 목록
version: 0.1.0
change_history:
  - date: 2026-07-28
    version: 0.1.0
    summary: 폴더 내 기능 명세의 목적과 구현 및 검증 현황을 조회하는 인덱스를 작성함
---

# ARCH-INPUT 기능 명세 목록

| 기능 식별자 | 주요 목적 | 구현 여부 | 검증 상태 | 구현 일자 |
| --- | --- | --- | --- | --- |
| [`FEAT-INPUT-001`](FEAT-INPUT-001.md) | MCP23017에서 읽은 interrupt pin bit 또는 GPIO bit를 ButtonId로 변환한다. |  |  |  |
| [`FEAT-INPUT-002`](FEAT-INPUT-002.md) | MCP23017 INT 라인 assert를 ISR에서 최소 정보로 기록해 사용자 컨트롤 처리 태스크에 전달한다. | ✅ | ✅ | 2026-07-17 |
| [`FEAT-INPUT-003`](FEAT-INPUT-003.md) | ISR에서 발생한 MCP23017 INT event를 사용자 컨트롤 처리 태스크가 나중에 순서대로 읽을 수 있게 보관한다. |  |  |  |
| [`FEAT-INPUT-004`](FEAT-INPUT-004.md) | input_event_queue에 쌓인 interrupt event를 읽고 I2C로 MCP23017 버튼 상태를 확인한다. |  |  |  |
| [`FEAT-INPUT-005`](FEAT-INPUT-005.md) | MCP23017에서 읽은 raw button transition 중 bounce를 제거하고 stable press/release를 확정한다. |  |  |  |
| [`FEAT-INPUT-006`](FEAT-INPUT-006.md) | 확정된 버튼 상태를 modifier 생성용 snapshot에 반영한다. |  |  |  |
| [`FEAT-INPUT-007`](FEAT-INPUT-007.md) | 확정된 버튼 edge를 상태 관리 구조가 해석할 수 있는 payload로 만든다. |  |  |  |
| [`FEAT-INPUT-008`](FEAT-INPUT-008.md) | CONTROL_BUTTON message를 상태 관리 구조의 state_event_queue로 전송한다. |  |  |  |
| [`FEAT-INPUT-009`](FEAT-INPUT-009.md) | MCP23017 INT queue overflow, 알 수 없는 pin bit, I2C read 실패, 전송 실패를 진단 가능하게 기록한다. |  |  |  |
| [`FEAT-encoder-event-processing`](FEAT-encoder-event-processing.md) |  |  |  |  |
| [`FEAT-potentiometer-sampling`](FEAT-potentiometer-sampling.md) |  |  |  |  |
| [`FEAT-track-reset-gesture`](FEAT-track-reset-gesture.md) |  |  |  |  |
