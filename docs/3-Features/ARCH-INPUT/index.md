---
title: ARCH-INPUT 기능 명세 목록
version: 0.1.3
change_history:
  - date: 2026-07-28
    version: 0.1.0
    summary: 폴더 내 기능 명세의 목적과 구현 및 검증 현황을 조회하는 인덱스를 작성함
  - date: 2026-07-28
    version: 0.1.1
    summary: 부분 구현 기능의 구현 여부를 경고 기호로 표시함
  - date: 2026-07-28
    version: 0.1.2
    summary: 아날로그 조작값과 엔코더 회전 기능 명세를 목록에 추가함
  - date: 2026-07-28
    version: 0.1.3
    summary: 세분화된 아날로그 및 엔코더 입력 기능 명세를 목록에 반영함
---

# ARCH-INPUT 기능 명세 목록

| 기능 식별자 | 주요 목적 | 구현 여부 | 검증 상태 | 구현 일자 |
| --- | --- | --- | --- | --- |
| [`FEAT-INPUT-001`](FEAT-INPUT-001.md) | MCP23017의 I2C 주소와 인터럽트가 발생한 핀 비트를 ControlButtonId로 변환한다. 이 기능은 버튼의 의미나 후속 동작을 판단하지 않는다. | ⚠️ |  |  |
| [`FEAT-INPUT-002`](FEAT-INPUT-002.md) | MCP23017의 INT 신호로 EXTI가 발생하면 ISR에서 인터럽트 출처와 발생 시각만 기록한다. I2C 통신과 버튼 판정은 InputTask에서 수행한다. | ✅ | ✅ | 2026-07-17 |
| [`FEAT-INPUT-003`](FEAT-INPUT-003.md) | ISR에서 생성된 InputEvent를 InputTask가 실행될 때까지 순서대로 보관한다. 큐는 버튼 인터럽트와 엔코더 회전처럼 입력 종류가 다른 이벤트를 하나의 계약으로 전달한다. | ⚠️ |  |  |
| [`FEAT-INPUT-004`](FEAT-INPUT-004.md) | Mcp23017IntEvent의 EXTI 핀으로 MCP23017 주소를 찾고, 태스크 문맥에서 INTF와 INTCAP 레지스터를 읽어 변경된 핀과 버튼의 눌림 상태를 구한다. | ⚠️ |  |  |
| [`FEAT-INPUT-005`](FEAT-INPUT-005.md) | MCP23017에서 읽은 버튼 상태의 접점 흔들림을 제거하고 실제 눌림 또는 떼짐으로 확정된 변화만 후속 기능에 전달한다. |  |  |  |
| [`FEAT-INPUT-006`](FEAT-INPUT-006.md) | debounce가 끝난 버튼 상태를 입력 계층의 스냅샷으로 보관한다. 엔코더 회전 이벤트는 회전 시점의 스냅샷에서 눌린 보조 버튼 정보를 가져간다. |  |  |  |
| [`FEAT-INPUT-007`](FEAT-INPUT-007.md) | 확정된 버튼 식별자와 눌림 상태를 상태 관리 구조가 공통으로 받을 수 있는 ControlButtonPayload로 변환한다. 버튼에 따른 판단과 행동은 포함하지 않는다. | ⚠️ |  |  |
| [`FEAT-INPUT-008`](FEAT-INPUT-008.md) | ControlButtonPayload를 StateEvent에 담아 state_event_queue로 전송한다. 전송 이후 이벤트의 해석과 상태 변경은 상태 관리 구조가 담당한다. | ⚠️ |  |  |
| [`FEAT-INPUT-009`](FEAT-INPUT-009.md) | 버튼 입력 경로에서 발생한 큐 포화, 장치·핀 식별 실패, I2C 통신 실패, 상태 이벤트 전송 실패를 종류별로 기록한다. |  |  |  |
| [`FEAT-INPUT-010`](FEAT-INPUT-010.md) | FX부와 트랙부의 ADC 채널을 조작기 식별자에 연결하고 정해진 주기로 원시값을 읽는다. |  |  |  |
| [`FEAT-INPUT-011`](FEAT-INPUT-011.md) | 현재 엔코더 카운터와 직전 카운터의 차이를 wrap-around를 고려한 signed delta로 변환한다. | ⚠️ |  |  |
| [`FEAT-INPUT-012`](FEAT-INPUT-012.md) | 현재 ADC 원시값을 조작기별 직전 전송값과 비교해 후속 이벤트를 생성할 의미 있는 변화인지 판정한다. |  |  |  |
| [`FEAT-INPUT-013`](FEAT-INPUT-013.md) | 의미 있는 ADC 원시값을 후속 구조가 장치별 ADC 해상도를 몰라도 사용할 수 있는 공통 범위로 변환한다. |  |  |  |
| [`FEAT-INPUT-014`](FEAT-INPUT-014.md) | 조작기 식별자와 원시값, 정규화된 값, 감지 시각을 하나의 아날로그 조작 이벤트로 만들어 state_event_queue에 전송한다. |  |  |  |
| [`FEAT-INPUT-015`](FEAT-INPUT-015.md) | 엔코더 타이머 인터럽트가 발생하면 ISR에서 카운터, 엔코더 식별자와 감지 시각만 InputEvent로 기록한다. | ⚠️ |  |  |
| [`FEAT-INPUT-016`](FEAT-INPUT-016.md) | 엔코더 회전 이벤트를 만들 때 button_state_snapshot에서 현재 눌린 보조 버튼 상태를 modifier_mask로 복사한다. |  |  |  |
| [`FEAT-INPUT-017`](FEAT-INPUT-017.md) | 엔코더 식별자, signed delta, modifier_mask와 감지 시각을 회전 StateEvent로 만들어 state_event_queue에 전송한다. | ⚠️ |  |  |
