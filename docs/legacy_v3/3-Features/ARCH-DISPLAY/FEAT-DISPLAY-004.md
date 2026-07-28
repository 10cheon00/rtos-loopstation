---
title: 패널 렌더러 선택
version: 1.0.1
change_history:
  - date: 2026-07-12
    version: 0.1.0
    summary: UI_STATE_RENDER panel_id에 맞는 패널 렌더러 선택 기능 문서를 작성함
  - date: 2026-07-17
    version: 0.2.0
    summary: 기능 구현 및 검증 상태 표를 추가함
  - date: 2026-07-26
    version: 1.0.0
    summary: 자기완결형 UI_STATE_RENDER command와 바인딩 파라미터 전달 계약을 반영함
  - date: 2026-07-28
    version: 1.0.1
    summary: 문서 변경 사항 반영
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
| `ARCH-DISPLAY-005` | UI render command | `panel_id`를 renderer 선택 기준으로 사용하고 바인딩 파라미터를 renderer 입력으로 전달한다. |

## 3. 목적

`UI_STATE_RENDER` payload의 `panel_id`를 기준으로 해당 패널을 그릴 renderer를 선택한다.

## 4. 입력

| 입력 | 설명 |
| --- | --- |
| `panel_id` | 표시할 대상 패널 ID |
| bound parameters | 해당 패널 renderer에 전달할 모든 `Parameter` 값 |

## 5. 출력

| 출력 | 설명 |
| --- | --- |
| panel render request | 선택된 renderer와 바인딩 파라미터 배열 |

## 6. 구현 기준

| 항목 | 기준 |
| --- | --- |
| renderer table | `panel_id`별 renderer function 또는 handler를 등록한다. |
| 지원 패널 | 홈, 트랙, FX, 설정, 진단 패널을 구분할 수 있어야 한다. |
| invalid panel | 등록되지 않은 `panel_id`는 오류로 기록하고 fallback panel 또는 오류 화면 정책으로 넘긴다. |
| 파라미터 전달 | command에 복사된 바인딩 파라미터 배열을 선택된 renderer에 전달한다. |

## 7. 완료 기준

| 기준 | 확인 방법 |
| --- | --- |
| renderer 선택 | 각 `panel_id`가 기대한 renderer로 매핑되는지 확인한다. |
| 파라미터 전달 | command의 모든 바인딩 파라미터가 renderer에 전달되는지 확인한다. |
| invalid 처리 | 알 수 없는 `panel_id`가 진단값으로 기록되는지 확인한다. |

## 8. 구현 및 검증 상태

| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 미확인 | - | 이번 구현 판정 범위에 포함하지 않았다. |
| 검증 | 미확인 | - | 구현 확인 후 기록한다. |
