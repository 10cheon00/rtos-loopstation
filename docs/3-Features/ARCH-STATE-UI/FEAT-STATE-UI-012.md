---
title: FX 하위 패널 전이
version: 0.1.0
change_history:
  - date: 2026-07-28
    version: 0.1.0
    summary: 간소화된 양식으로 FX 하위 패널 전이 기능 초안을 작성함
---
# FEAT-STATE-UI-012: FX 하위 패널 전이
## 1. 연결된 상위 설계 항목
| 설계 항목 식별자 | 설계 항목 설명 |
| --- | --- |
| `ARCH-STATE-UI-012` | 선택 FX bus에 따라 IFX 또는 TFX 설정 패널로 전이한다. |
## 2. 설명
FX 패널의 선택 bus를 `IFX_SETTING` 또는 `TFX_SETTING` 상태로 변환한다.
## 3. 입력
| 입력 | 설명 |
| --- | --- |
| 선택 FX bus, Enter 이벤트 | 하위 FX 패널 결정 정보 |
## 4. 출력
| 출력 | 설명 |
| --- | --- |
| 목적 `StateId`, transition flag | 선택된 FX 설정 상태 |
## 5. 구현 기준
| 항목 | 기준 |
| --- | --- |
| 매핑 | IFX와 TFX를 각각 대응 설정 패널에 연결한다. |
| context | 전이 후에도 선택 FX bus를 유지한다. |
## 6. 구현 및 검증 상태
| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 미구현 | - | FX 상태와 선택 bus context가 없다. |
| 검증 | 미완료 | - | IFX·TFX 선택 전이를 검증한다. |
