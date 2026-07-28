---
title: 패널 표시 payload 적용
version: 1.1.0
change_history:
  - date: 2026-07-12
    version: 0.1.0
    summary: 패널 전환 시 표시 구조의 현재 패널 context를 갱신하는 기능 문서를 작성함
  - date: 2026-07-17
    version: 0.2.0
    summary: 기능 구현 및 검증 상태 표를 추가함
  - date: 2026-07-26
    version: 1.0.0
    summary: 자기완결형 UI_STATE_RENDER command와 바인딩 파라미터 전달 계약을 반영함
  - date: 2026-07-28
    version: 1.1.0
    summary: 패널 표시 payload 적용 관련 문서 내용을 추가함
---

# 패널 표시 payload 적용

## 1. 기능 범위

| 항목 | 내용 |
| --- | --- |
| 기능 ID | `FEAT-DISPLAY-005` |
| 상위 설계 문서 | `ARCH-DISPLAY.md` |
| 관련 설계 항목 | `ARCH-DISPLAY-005`, `ARCH-DISPLAY-008`, `ARCH-DISPLAY-012` |
| 주요 목적 | 표시 구조가 command의 panel ID와 바인딩 파라미터를 별도 canonical state 없이 현재 렌더링 입력으로 사용한다. |
| 제외 범위 | 상태 관리 구조의 canonical UI state 결정, renderer drawing |

## 2. 연결된 상위 설계 항목

| 설계 항목 ID | 설계 항목 | 연결 내용 |
| --- | --- | --- |
| `ARCH-DISPLAY-005` | UI render command | 현재 `panel_id`와 바인딩된 모든 파라미터를 받는다. |
| `ARCH-DISPLAY-008` | 패널 파라미터 payload | command의 파라미터를 renderer 입력으로 사용한다. |
| `ARCH-DISPLAY-012` | 표시 구조 최소 상태 | command 자체만으로 frame을 생성하고 canonical 파라미터 저장소를 조회하지 않는다. |

## 3. 목적

표시 구조는 수신한 command의 panel ID와 바인딩 파라미터를 해당 render 실행 동안의 입력으로 사용한다.
표시 대상 선별은 StateTask가 command 생성 전에 수행하므로 DisplayTask는 UI 상태나 파라미터 저장소를 별도로 최신화하지 않는다.

## 4. 입력

| 입력 | 설명 |
| --- | --- |
| `UiStateRenderPayload` | 현재 panel ID와 바인딩된 모든 파라미터 값을 담은 payload |

## 5. 출력

| 출력 | 설명 |
| --- | --- |
| renderer 입력 | 현재 `panel_id`와 바인딩 파라미터 배열 |

## 6. 구현 기준

| 항목 | 기준 |
| --- | --- |
| 적용 시점 | `UI_STATE_RENDER` command를 dequeue한 뒤 renderer 호출 전에 payload를 검증한다. |
| 비영속 입력 | command payload를 별도 canonical UI context로 복제하지 않는다. |
| 저장소 격리 | DisplayTask는 LoopStation parameter store에 접근하지 않는다. |
| 일관성 | renderer에는 동일한 command에 들어 있던 panel ID와 파라미터를 함께 전달한다. |

## 6.1 검토 및 구현 결과

기존 설계의 “패널 선택 context 갱신” 기능은 DisplayTask가 UI 상태와 선택값을 별도로 보관한다는 전제를 사용했다.
자기완결형 `UI_STATE_RENDER`가 panel ID와 모든 바인딩 파라미터를 전달하도록 변경되어 별도 display context 저장 기능은 필요하지 않다.
이 문서는 삭제하지 않고 command payload 검증과 renderer 입력 적용 기능으로 범위를 변경한다.

## 7. 완료 기준

| 기준 | 확인 방법 |
| --- | --- |
| panel 적용 | `UI_STATE_RENDER`의 `panel_id`가 renderer 선택에 사용되는지 확인한다. |
| 파라미터 적용 | payload의 모든 바인딩 파라미터가 renderer 입력으로 전달되는지 확인한다. |
| 불필요 상태 제거 | DisplayTask가 별도 UI context나 파라미터 저장소를 참조하지 않는지 확인한다. |

## 8. 구현 및 검증 상태

| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 미확인 | - | 이번 구현 판정 범위에 포함하지 않았다. |
| 검증 | 미확인 | - | 구현 확인 후 기록한다. |
