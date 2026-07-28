---
title: UI 선택 context
version: 0.1.0
change_history:
  - date: 2026-07-28
    version: 0.1.0
    summary: 간소화된 양식으로 UI 선택 context 기능 초안을 작성함
---
# FEAT-STATE-UI-002: UI 선택 context
## 1. 연결된 상위 설계 항목
| 설계 항목 식별자 | 설계 항목 설명 |
| --- | --- |
| `ARCH-STATE-UI-002` | 선택 항목, 선택 트랙, 선택 FX bus를 UI 상태 머신 context에 보관한다. |
## 2. 설명
패널 전이에 필요한 사용자 선택값을 UI 상태 머신 전용 context로 유지한다.
## 3. 입력
| 입력 | 설명 |
| --- | --- |
| 선택 변경 `StateEvent` | 항목·트랙·FX bus 선택 변경 |
## 4. 출력
| 출력 | 설명 |
| --- | --- |
| UI context | 최신 선택 항목, 트랙과 FX bus |
## 5. 구현 기준
| 항목 | 기준 |
| --- | --- |
| 소유권 | context는 UI 상태 머신이 소유하고 상태 파일의 static 변수로 분산하지 않는다. |
| 초기화 | StateTask 초기화 인자로 전달된 기본값으로 시작한다. |
## 6. 구현 및 검증 상태
| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 미구현 | - | `StateMachine.context`는 있으나 UI 선택 context가 정의·주입되지 않았다. |
| 검증 | 미완료 | - | 선택 갱신과 패널 전이 후 유지 여부를 검증한다. |
