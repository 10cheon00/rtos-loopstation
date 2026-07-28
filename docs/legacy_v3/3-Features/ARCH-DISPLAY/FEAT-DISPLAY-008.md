---
title: 파라미터 변경 시 전체 패널 재렌더링
version: 0.1.0
change_history:
  - date: 2026-07-28
    version: 0.1.0
    summary: 파라미터 변경 후 현재 패널 전체를 다시 그리는 기능 명세를 작성함
---

# 파라미터 변경 시 전체 패널 재렌더링

## 1. 기능 범위

| 항목 | 내용 |
| --- | --- |
| 기능 ID | `FEAT-DISPLAY-008` |
| 상위 설계 문서 | `ARCH-DISPLAY.md` |
| 관련 설계 항목 | `ARCH-DISPLAY-007`, `ARCH-DISPLAY-009`, `ARCH-DISPLAY-010`, `ARCH-DISPLAY-019`, `ARCH-DISPLAY-028` |
| 주요 목적 | 파라미터 변경이 완료되면 현재 패널의 모든 바인딩 파라미터를 전달하고 패널 전체 frame을 다시 그리도록 요청한다. |
| 제외 범위 | 개별 panel renderer의 레이아웃 구현, 비동기 telemetry 갱신, 부분 영역 렌더링 |

## 2. 연결된 상위 설계 항목

| 설계 항목 ID | 설계 항목 | 연결 내용 |
| --- | --- | --- |
| `ARCH-DISPLAY-007` | panel renderer | 패널 전환과 같은 renderer를 사용해 현재 패널 전체를 다시 그린다. |
| `ARCH-DISPLAY-009` | 전체 패널 재렌더링 조건 | StateEvent 처리 결과에 파라미터 변경 flag가 있으면 render를 요청한다. |
| `ARCH-DISPLAY-010` | 전체 패널 표시 payload | 현재 패널에 바인딩된 모든 파라미터를 `UI_STATE_RENDER`에 복사한다. |
| `ARCH-DISPLAY-019` | `display_command_queue` | 생성한 `UI_STATE_RENDER` command를 표시 구조에 전달한다. |
| `ARCH-DISPLAY-028` | 전체 패널 frame 재생성 | frame buffer를 초기화하고 현재 패널의 전체 레이아웃과 표시값을 다시 그린다. |

## 3. 목적

StateTask가 파라미터 변경을 완료한 뒤 현재 패널의 표시 내용과 canonical parameter state가 일치하도록 전체 패널 재렌더링을 요청한다.
표시 구조는 변경된 파라미터가 나타나는 일부 영역만 수정하지 않고, 패널 전환과 동일한 `UI_STATE_RENDER` 처리 경로를 사용해 현재 패널의 전체 frame을 다시 생성하고 LCD에 commit한다.

## 4. 입력

| 입력 | 설명 |
| --- | --- |
| StateEvent 처리 결과 flag | 파라미터 변경이 완료되었음을 나타내는 `PARAMETER_UPDATED` flag |
| current panel ID | UI 상태 머신의 현재 상태에 대응하는 panel ID |
| panel-parameter binding | 현재 패널에 연결된 `ParameterId` 목록 |
| parameter store | binding에 대응하는 최신 `Parameter` 값 |

## 5. 출력

| 출력 | 설명 |
| --- | --- |
| `UI_STATE_RENDER` command | 현재 panel ID와 모든 바인딩 파라미터를 포함하는 전체 패널 render 요청 |
| LCD 전체 panel frame | 기존 패널 출력 파이프라인이 frame buffer를 다시 구성해 commit한 결과 |

## 6. 구현 기준

| 항목 | 기준 |
| --- | --- |
| 판정 시점 | StateTask가 하나의 StateEvent에 따른 상태 전이와 파라미터 변경을 모두 처리한 후 결과 flag를 검사한다. |
| render 조건 | 결과에 `PARAMETER_UPDATED`가 포함되면 현재 패널의 render를 요청한다. |
| 중복 억제 | 하나의 StateEvent에서 `PARAMETER_UPDATED`와 `TRANSITION`이 함께 발생해도 `UI_STATE_RENDER`는 한 번만 생성한다. |
| payload 구성 | current panel ID와 해당 패널에 바인딩된 모든 `Parameter` 값을 하나의 command에 값으로 복사한다. |
| queue 전달 | 완성된 command를 `display_command_queue`에 저장하고 전송 실패를 호출자에게 반환한다. |
| renderer 재사용 | DisplayTask는 패널 전환과 같은 panel renderer 선택 및 frame 생성 경로를 사용한다. |
| 전체 redraw | panel renderer는 frame buffer를 초기화하고 전체 레이아웃과 모든 표시값을 다시 그린 뒤 LCD에 commit한다. |
| 부분 갱신 제외 | 변경된 파라미터 ID나 화면 좌표를 이용한 부분 영역 갱신 경로를 추가하지 않는다. |
| 저장소 격리 | DisplayTask는 canonical parameter store를 조회하지 않고 command payload만 사용한다. |

## 7. 완료 기준

| 기준 | 확인 방법 |
| --- | --- |
| 변경 감지 | 파라미터 변경 결과가 발생하면 `UI_STATE_RENDER`가 한 번 생성되는지 확인한다. |
| payload 완전성 | 현재 panel ID와 모든 바인딩 파라미터가 command 전송 전후로 유지되는지 확인한다. |
| 전체 frame 생성 | 파라미터 하나만 변경해도 renderer가 frame buffer를 초기화하고 현재 패널 전체를 다시 그리는지 확인한다. |
| LCD 반영 | 전체 frame commit 후 변경값과 나머지 패널 요소가 모두 정상적으로 표시되는지 확인한다. |
| 불필요한 render 방지 | 파라미터 변경이나 패널 전이가 없는 이벤트에는 render command를 생성하지 않는지 확인한다. |
| 단일 요청 | 하나의 이벤트에 변경과 전이가 함께 포함되어도 render command가 중복 생성되지 않는지 확인한다. |

## 8. 구현 및 검증 상태

| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 완료 | 2026-07-28 | StateTask의 결과 flag 판정, `UI_STATE_RENDER` 생성과 DisplayTask의 panel renderer 호출 경로가 구현되어 있다. |
| 검증 | 부분 완료 | 2026-07-28 | 코드 분석으로 전체 패널 buffer 초기화와 commit 경로를 확인했으며 실제 장치에서 빠른 연속 변경과 중복 요청 여부 검증이 남아 있다. |
