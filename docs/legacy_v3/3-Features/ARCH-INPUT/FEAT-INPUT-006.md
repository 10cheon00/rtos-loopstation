---
title: button state snapshot 갱신
version: 0.2.0
change_history:
  - date: 2026-07-12
    version: 0.1.0
    summary: stable 버튼 상태를 modifier snapshot에 반영하는 기능 문서를 작성함
  - date: 2026-07-17
    version: 0.2.0
    summary: button state snapshot 기능의 미구현 상태를 기록함
---

# button state snapshot 갱신

## 1. 기능 범위

| 항목 | 내용 |
| --- | --- |
| 기능 ID | `FEAT-INPUT-006` |
| 상위 설계 문서 | `ARCH-INPUT.md` |
| 관련 설계 항목 | `ARCH-INPUT-022`, `ARCH-INPUT-023`, `ARCH-INPUT-027`, `ARCH-INPUT-032` |
| 주요 목적 | 확정된 버튼 상태를 modifier 생성용 snapshot에 반영한다. |
| 제외 범위 | 엔코더 회전 delta 계산, modifier 의미 해석, 상태 관리 canonical state 갱신 |

## 2. 연결된 상위 설계 항목

| 설계 항목 ID | 설계 항목 | 연결 내용 |
| --- | --- | --- |
| `ARCH-INPUT-023` | modifier snapshot | 엔코더 회전 event의 `modifier_mask` 입력으로 사용된다. |
| `ARCH-INPUT-027` | button state snapshot 갱신 | 엔코더 push press/release 상태를 snapshot에 반영한다. |
| `ARCH-INPUT-032` | button state snapshot | 입력 처리 구조가 소유하는 최신 버튼 상태다. |

## 3. 목적

엔코더 회전 event를 만들 때 회전 순간 눌려 있던 버튼 상태를 `modifier_mask`로 복사할 수 있도록, 버튼별 stable 상태를 snapshot으로 유지한다.

## 4. 입력

| 입력 | 설명 |
| --- | --- |
| stable button edge | debounce 후 확정된 버튼 ID와 상태 |

## 5. 출력

| 출력 | 설명 |
| --- | --- |
| `button_state_snapshot` | 현재 눌려 있는 버튼 상태 bit mask 또는 동등한 구조 |

## 6. 구현 기준

| 항목 | 기준 |
| --- | --- |
| press 처리 | `PRESSED` edge 확정 시 해당 버튼 bit를 set한다. |
| release 처리 | `RELEASED` edge 확정 시 해당 버튼 bit를 clear한다. |
| 소유권 | snapshot은 사용자 컨트롤 처리 구조가 소유한다. |
| 사용 범위 | 상태 관리 구조의 canonical state가 아니며, modifier snapshot 생성을 위한 입력 계층 상태로만 사용한다. |

## 7. 완료 기준

| 기준 | 확인 방법 |
| --- | --- |
| press 반영 | press 확정 시 해당 bit가 set되는지 확인한다. |
| release 반영 | release 확정 시 해당 bit가 clear되는지 확인한다. |
| modifier 연계 | 엔코더 회전 기능이 snapshot을 읽어 `modifier_mask`를 만들 수 있는지 확인한다. |

## 8. 구현 및 검증 상태

| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 미구현 | - | InputTask에 `button_state_snapshot` 저장 및 갱신 구조가 없다. |
| 검증 | 미완료 | - | 구현 후 검증한다. |
