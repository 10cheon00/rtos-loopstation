---
title: LCD 패널 frame 생성
version: 0.2.0
change_history:
  - date: 2026-07-12
    version: 0.1.0
    summary: 선택된 panel renderer가 LCD 출력 frame을 생성하는 기능 문서를 작성함
  - date: 2026-07-17
    version: 0.2.0
    summary: 기능 구현 및 검증 상태 표를 추가함
---

# LCD 패널 frame 생성

## 1. 기능 범위

| 항목 | 내용 |
| --- | --- |
| 기능 ID | `FEAT-DISPLAY-006` |
| 상위 설계 문서 | `ARCH-DISPLAY.md` |
| 관련 설계 항목 | `ARCH-DISPLAY-007`, `ARCH-DISPLAY-008`, `ARCH-DISPLAY-011` |
| 주요 목적 | 선택된 panel renderer가 LCD에 출력할 panel frame을 구성한다. |
| 제외 범위 | LCD driver 전송, 패널 전환 command queue 처리, 표시값 변경 정책 |

## 2. 연결된 상위 설계 항목

| 설계 항목 ID | 설계 항목 | 연결 내용 |
| --- | --- | --- |
| `ARCH-DISPLAY-007` | panel renderer | panel id별 화면 구성을 그린다. |
| `ARCH-DISPLAY-008` | 선택 항목 표시 | 선택 track/item을 강조 표현에 반영한다. |
| `ARCH-DISPLAY-011` | 패널 전환 초기 payload | 패널 전환 시 필요한 초기 표시값과 함께 frame을 구성할 수 있다. |

## 3. 목적

선택된 panel renderer가 패널 레이아웃, 제목, 선택 강조, 기본 표시 영역을 구성해 LCD 출력 frame을 만든다.

## 4. 입력

| 입력 | 설명 |
| --- | --- |
| panel render request | 선택된 renderer와 display context |
| display panel context | `panel_id`, `selected_track`, `selected_item` |

## 5. 출력

| 출력 | 설명 |
| --- | --- |
| LCD frame buffer 또는 draw command list | LCD driver에 전달할 화면 출력 결과 |

## 6. 구현 기준

| 항목 | 기준 |
| --- | --- |
| renderer 분리 | 홈, 트랙, FX, 설정, 진단 패널 renderer를 분리한다. |
| 선택 강조 | `selected_track`, `selected_item`이 있는 경우 해당 UI 요소를 강조한다. |
| 초기 frame | `ARCH-DISPLAY-007`에 따라 패널 전환에 필요한 기본 frame을 생성한다. |
| 표시값 payload | `ARCH-DISPLAY-010`의 표시값 payload는 이 기능의 필수 입력이 아니다. |

## 7. 완료 기준

| 기준 | 확인 방법 |
| --- | --- |
| panel별 frame | 각 `panel_id`에 대해 기본 frame이 생성되는지 확인한다. |
| 선택 강조 | 선택 track/item이 frame에 반영되는지 확인한다. |
| renderer 독립성 | 특정 패널 renderer 변경이 다른 패널 renderer에 영향을 주지 않는지 확인한다. |

## 8. 구현 및 검증 상태

| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 미확인 | - | 이번 구현 판정 범위에 포함하지 않았다. |
| 검증 | 미확인 | - | 구현 확인 후 기록한다. |
