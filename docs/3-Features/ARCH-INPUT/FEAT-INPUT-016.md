---
title: 엔코더 modifier snapshot
version: 0.1.0
change_history:
  - date: 2026-07-28
    version: 0.1.0
    summary: 간소화된 양식으로 엔코더 modifier snapshot 기능 초안을 작성함
---

# FEAT-INPUT-016: 엔코더 modifier snapshot

## 1. 연결된 상위 설계 항목

| 설계 항목 식별자 | 설계 항목 설명 |
| --- | --- |
| `ARCH-INPUT-022` | 접점 흔들림 제거가 끝난 버튼별 눌림/떼짐 상태를 보관한다. |
| `ARCH-INPUT-023` | 회전 시점에 눌린 보조 버튼 상태를 현재 버튼 상태 모음에서 복사한다. |
| `ARCH-INPUT-030` | 현재 엔코더 누름 상태를 회전 이벤트의 보조 버튼 정보로 제공한다. |
| `ARCH-INPUT-032` | 접점 흔들림 제거가 끝난 버튼 상태를 보관하고 엔코더 회전 시점에 복사한다. |

## 2. 설명

엔코더 회전 이벤트를 만들 때 `button_state_snapshot`에서 현재 눌린 보조 버튼 상태를 `modifier_mask`로 복사한다.

## 3. 입력

| 입력 | 설명 |
| --- | --- |
| `button_state_snapshot` | debounce가 끝난 현재 버튼 상태 |
| 엔코더 식별자 | 회전한 엔코더와 관련 modifier를 선택하는 식별자 |

## 4. 출력

| 출력 | 설명 |
| --- | --- |
| `modifier_mask` | 회전 이벤트 생성 시점에 눌려 있던 보조 버튼 정보 |

## 5. 구현 기준

| 항목 | 기준 |
| --- | --- |
| 복사 시점 | signed delta가 확정된 뒤 이벤트 payload를 만들기 직전에 snapshot을 읽는다. |
| 상태 출처 | debounce가 완료된 `button_state_snapshot`만 사용한다. |
| 일관성 | snapshot 조회 중 갱신으로 서로 다른 시점의 버튼 상태가 섞이지 않아야 한다. |
| 책임 제한 | modifier에 따른 변화 배율이나 동작 의미를 입력 계층에서 결정하지 않는다. |

## 6. 구현 및 검증 상태

| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 미구현 | - | InputTask에 버튼 snapshot과 `modifier_mask` 생성 코드가 없다. |
| 검증 | 미완료 | - | 엔코더 push의 press/release와 회전을 조합해 검증한다. |
