---
title: ARCH-STATE-UI 기능 명세 목록
version: 0.1.0
change_history:
  - date: 2026-07-28
    version: 0.1.0
    summary: 폴더 내 기능 명세의 목적과 구현 및 검증 현황을 조회하는 인덱스를 작성함
---

# ARCH-STATE-UI 기능 명세 목록

| 기능 식별자 | 주요 목적 | 구현 여부 | 검증 상태 | 구현 일자 |
| --- | --- | --- | --- | --- |
| [`FEAT-STATE-UI-001`](FEAT-STATE-UI-001.md) | 각 UI 패널을 State 인스턴스로 정의하고 UiPanelId로 조회할 수 있게 한다. | ⚠️ |  |  |
| [`FEAT-STATE-UI-002`](FEAT-STATE-UI-002.md) | 패널 전이에 필요한 사용자 선택값을 UI 상태 머신 전용 context로 유지한다. |  |  |  |
| [`FEAT-STATE-UI-003`](FEAT-STATE-UI-003.md) | UI 상태 머신 시작 시 UI_STATE_HOME_PANEL을 현재 상태로 설정하고 on_enter()를 호출한다. | ✅ |  | 2026-07-28 |
| [`FEAT-STATE-UI-004`](FEAT-STATE-UI-004.md) | HOME 진입 완료 후 StateTask가 홈 패널 ID와 바인딩 파라미터로 최초 표시 명령을 만든다. |  |  |  |
| [`FEAT-STATE-UI-005`](FEAT-STATE-UI-005.md) | SETTING, HOME, FX, TRACK의 비순환 인접 관계를 정적으로 정의한다. | ⚠️ |  |  |
| [`FEAT-STATE-UI-006`](FEAT-STATE-UI-006.md) | 좌 버튼 이벤트를 현재 상위 패널의 왼쪽 인접 상태 전이로 처리한다. | ⚠️ |  |  |
| [`FEAT-STATE-UI-007`](FEAT-STATE-UI-007.md) | 우 버튼 이벤트를 현재 상위 패널의 오른쪽 인접 상태 전이로 처리한다. | ⚠️ |  |  |
| [`FEAT-STATE-UI-008`](FEAT-STATE-UI-008.md) | SETTING의 좌 조작과 TRACK의 우 조작에서 유효 상태를 유지하고 경계 입력으로 처리한다. |  |  |  |
| [`FEAT-STATE-UI-009`](FEAT-STATE-UI-009.md) | StateTask가 상위 패널 전이를 마친 뒤 current panel과 바인딩 파라미터를 표시 명령으로 전송한다. | ⚠️ |  |  |
| [`FEAT-STATE-UI-010`](FEAT-STATE-UI-010.md) | Enter 버튼 이벤트와 현재 상위 패널의 선택 context로 진입할 하위 패널을 결정한다. |  |  |  |
| [`FEAT-STATE-UI-011`](FEAT-STATE-UI-011.md) | SETTING의 선택 항목을 SYSTEM_SETTING 또는 HARDWARE_DIAGNOSTIC 상태로 변환한다. |  |  |  |
| [`FEAT-STATE-UI-012`](FEAT-STATE-UI-012.md) | FX 패널의 선택 bus를 IFX_SETTING 또는 TFX_SETTING 상태로 변환한다. |  |  |  |
| [`FEAT-STATE-UI-013`](FEAT-STATE-UI-013.md) | TRACK에서 Enter 이벤트가 발생하면 선택 트랙을 유지하고 TRACK_SETTING으로 전이한다. |  |  |  |
| [`FEAT-STATE-UI-014`](FEAT-STATE-UI-014.md) | StateTask가 Enter 전이를 마친 뒤 하위 panel ID와 바인딩 파라미터를 표시 명령으로 전송한다. | ⚠️ |  |  |
| [`FEAT-STATE-UI-015`](FEAT-STATE-UI-015.md) | 각 하위 UiPanelId를 Exit 시 복귀할 상위 UiPanelId에 연결한다. |  |  |  |
| [`FEAT-STATE-UI-016`](FEAT-STATE-UI-016.md) | 하위 상태가 Exit 버튼 이벤트를 받으면 parent 매핑의 상태 ID와 transition flag를 반환한다. |  |  |  |
| [`FEAT-STATE-UI-017`](FEAT-STATE-UI-017.md) | 하위 패널 진입과 Exit 전이 동안 상위 패널의 선택 항목을 초기화하지 않는다. |  |  |  |
| [`FEAT-STATE-UI-018`](FEAT-STATE-UI-018.md) | StateTask가 Exit 전이를 마친 뒤 parent panel ID와 유지된 표시값을 렌더링 명령으로 전송한다. | ⚠️ |  |  |
| [`FEAT-STATE-UI-019`](FEAT-STATE-UI-019.md) | 현재 패널과 관계없이 IFX·TFX 버튼 이벤트를 전역 UI 전이 대상으로 분류한다. |  |  |  |
| [`FEAT-STATE-UI-020`](FEAT-STATE-UI-020.md) | IFX 직접 진입 이벤트를 IFX_SETTING 전이와 IFX bus 선택 갱신으로 처리한다. |  |  |  |
| [`FEAT-STATE-UI-021`](FEAT-STATE-UI-021.md) | TFX 직접 진입 이벤트를 TFX_SETTING 전이와 TFX bus 선택 갱신으로 처리한다. |  |  |  |
| [`FEAT-STATE-UI-022`](FEAT-STATE-UI-022.md) | 출발 패널과 관계없이 IFX_SETTING과 TFX_SETTING의 Exit 목적지를 FX로 고정한다. |  |  |  |
| [`FEAT-STATE-UI-023`](FEAT-STATE-UI-023.md) | StateTask가 IFX·TFX 직접 전이를 마친 뒤 선택된 FX 설정 패널의 전체 렌더링을 요청한다. | ⚠️ |  |  |
