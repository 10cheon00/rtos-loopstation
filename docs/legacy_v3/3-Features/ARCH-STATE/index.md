---
title: ARCH-STATE 기능 명세 목록
version: 0.1.0
change_history:
  - date: 2026-07-28
    version: 0.1.0
    summary: 폴더 내 기능 명세의 목적과 구현 및 검증 현황을 조회하는 인덱스를 작성함
---

# ARCH-STATE 기능 명세 목록

| 기능 식별자 | 주요 목적 | 구현 여부 | 검증 상태 | 구현 일자 |
| --- | --- | --- | --- | --- |
| [`FEAT-STATE-001`](FEAT-STATE-001.md) | 정적으로 구성된 상태 머신 인스턴스를 식별자별로 등록하고 조회한다. |  |  |  |
| [`FEAT-STATE-002`](FEAT-STATE-002.md) | 현재 상태, 전용 context, 식별 정보와 허용 상태 집합을 독립된 인스턴스로 구성한다. |  |  |  |
| [`FEAT-STATE-003`](FEAT-STATE-003.md) | canonical state의 쓰기 경로를 상태 관리 태스크 내부 API로 제한한다. |  |  |  |
| [`FEAT-STATE-004`](FEAT-STATE-004.md) | 상태 정의를 정적 수명으로 제공하여 런타임 동적 할당 없이 참조를 전환한다. |  |  |  |
| [`FEAT-STATE-005`](FEAT-STATE-005.md) | 상태 머신 전용 설계가 정한 초기 상태와 context를 공통 인스턴스에 안전하게 주입한다. |  |  |  |
| [`FEAT-STATE-006`](FEAT-STATE-006.md) | 등록 완료 후 첫 이벤트보다 먼저 초기 상태의 on_enter를 정확히 한 번 호출한다. |  |  |  |
| [`FEAT-STATE-007`](FEAT-STATE-007.md) | 상태 관리 태스크가 state_event_queue의 단일 소비자로 blocking 대기하고 이벤트를 순서대로 읽는다. |  |  |  |
| [`FEAT-STATE-008`](FEAT-STATE-008.md) | 잘못된 type, payload와 식별자가 상태 머신에 전달되기 전에 공통 규칙으로 거부한다. |  |  |  |
| [`FEAT-STATE-009`](FEAT-STATE-009.md) | StateEvent의 type과 payload를 조사해 대상 상태 머신 종류와 인스턴스를 결정한다. |  |  |  |
| [`FEAT-STATE-010`](FEAT-STATE-010.md) | 분류된 인스턴스의 현재 상태가 구현한 on_message에 context와 이벤트를 전달한다. |  |  |  |
| [`FEAT-STATE-011`](FEAT-STATE-011.md) | on_enter, on_message, on_exit의 책임과 금지 사항을 공통 계약으로 정의한다. |  |  |  |
| [`FEAT-STATE-012`](FEAT-STATE-012.md) | 구체 상태 타입을 몰라도 공통 함수 시그니처로 세 생명주기를 호출할 수 있게 한다. |  |  |  |
| [`FEAT-STATE-013`](FEAT-STATE-013.md) | TRANSITION의 다음 상태가 대상 상태 머신에 등록된 유효한 상태인지 확인한다. |  |  |  |
| [`FEAT-STATE-014`](FEAT-STATE-014.md) | 검증된 전이를 on_exit, current state 변경, on_enter 순서로 실행한다. |  |  |  |
| [`FEAT-STATE-015`](FEAT-STATE-015.md) | 한 이벤트의 전체 pipeline을 끝낸 뒤 다음 이벤트를 dequeue하여 상태 변경 순서를 보존한다. |  |  |  |
| [`FEAT-STATE-016`](FEAT-STATE-016.md) | 전이 검증과 on_exit 완료 후 canonical state 참조를 한 번만 변경한다. |  |  |  |
| [`FEAT-STATE-017`](FEAT-STATE-017.md) | 생명주기, 파라미터 변경, 결과 확인과 필요한 후속 command enqueue가 끝난 뒤 다음 dequeue를 허용한다. |  |  |  |
| [`FEAT-STATE-018`](FEAT-STATE-018.md) | HANDLED, TRANSITION, IGNORED, REJECTED, ERROR를 공통 타입으로 정의한다. |  |  |  |
| [`FEAT-STATE-019`](FEAT-STATE-019.md) | on_message 결과를 검사해 완료, 전이, 상태 유지 또는 오류 경로를 선택한다. |  |  |  |
| [`FEAT-STATE-020`](FEAT-STATE-020.md) | IGNORED, REJECTED, ERROR에서 마지막으로 확정된 canonical state를 유지한다. |  |  |  |
| [`FEAT-STATE-021`](FEAT-STATE-021.md) | 상위 오류 정책이 실패 위치와 원인을 판단하도록 정형화된 상태 오류 정보를 생성한다. |  |  |  |
| [`FEAT-STATE-022`](FEAT-STATE-022.md) | 상태 머신에 필요한 RTOS 및 command 자원을 태스크 생성 전에 외부에서 준비한다. |  |  |  |
| [`FEAT-STATE-023`](FEAT-STATE-023.md) | 태스크 자체와 각 상태 머신의 의존성을 정형화된 초기화 인자로 구분한다. |  |  |  |
| [`FEAT-STATE-024`](FEAT-STATE-024.md) | 상태 관리 태스크 초기화 시 전용 자원이 필요한 상태 머신에만 해당 인자를 전달한다. |  |  |  |
| [`FEAT-STATE-025`](FEAT-STATE-025.md) | 각 상태 머신이 검증한 의존 자원만 자신의 전용 context에 보관한다. |  |  |  |
