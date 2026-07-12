---
title: 패널 렌더러 선택
version: 0.1.0
change_history:
  - date: 2026-07-12
    version: 0.1.0
    summary: UI_STATE_RENDER panel_id에 맞는 패널 렌더러 선택 기능 문서를 작성함
---

# 패널 렌더러 선택

## 1. 기능 범위

| 항목 | 내용 |
| --- | --- |
| 기능 ID | `FEAT-DISPLAY-004` |
| 상위 설계 문서 | `ARCH-DISPLAY.md` |
| 관련 설계 항목 | `ARCH-DISPLAY-005`, `ARCH-DISPLAY-007` |
| 주요 목적 | `panel_id`에 맞는 LCD panel renderer를 선택한다. |
| 제외 범위 | renderer 내부 drawing, LCD 전송, 패널 선택 정책 |

## 2. 연결된 상위 설계 항목

| 설계 항목 ID | 설계 항목 | 연결 내용 |
| --- | --- | --- |
| `ARCH-DISPLAY-007` | panel renderer | 홈, 트랙, FX, 설정, 진단 패널 renderer를 분리한다. |
| `ARCH-DISPLAY-005` | UI 상태 command | `panel_id`를 renderer 선택 기준으로 사용한다. |

## 3. 목적

`UI_STATE_RENDER` payload의 `panel_id`를 기준으로 해당 패널을 그릴 renderer를 선택한다.

## 4. 입력

| 입력 | 설명 |
| --- | --- |
| `panel_id` | 표시할 대상 패널 ID |
| `selected_track` | renderer에 전달할 선택 트랙 |
| `selected_item` | renderer에 전달할 선택 항목 |

## 5. 출력

| 출력 | 설명 |
| --- | --- |
| panel render request | 선택된 renderer와 rendering context |

## 6. 구현 기준

| 항목 | 기준 |
| --- | --- |
| renderer table | `panel_id`별 renderer function 또는 handler를 등록한다. |
| 지원 패널 | 홈, 트랙, FX, 설정, 진단 패널을 구분할 수 있어야 한다. |
| invalid panel | 등록되지 않은 `panel_id`는 오류로 기록하고 fallback panel 또는 오류 화면 정책으로 넘긴다. |
| context 전달 | `selected_track`, `selected_item`을 renderer context로 전달한다. |

## 7. 완료 기준

| 기준 | 확인 방법 |
| --- | --- |
| renderer 선택 | 각 `panel_id`가 기대한 renderer로 매핑되는지 확인한다. |
| context 전달 | 선택 트랙과 선택 항목이 renderer에 전달되는지 확인한다. |
| invalid 처리 | 알 수 없는 `panel_id`가 진단값으로 기록되는지 확인한다. |
