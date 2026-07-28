---
title: IFX 설정 패널 직접 전이
version: 0.1.0
change_history:
  - date: 2026-07-28
    version: 0.1.0
    summary: 간소화된 양식으로 IFX 설정 패널 직접 전이 기능 초안을 작성함
---
# FEAT-STATE-UI-020: IFX 설정 패널 직접 전이
## 1. 연결된 상위 설계 항목
| 설계 항목 식별자 | 설계 항목 설명 |
| --- | --- |
| `ARCH-STATE-UI-020` | IFX 조작 시 선택 bus를 갱신하고 IFX 설정 패널로 전이한다. |
## 2. 설명
IFX 직접 진입 이벤트를 `IFX_SETTING` 전이와 IFX bus 선택 갱신으로 처리한다.
## 3. 입력
| 입력 | 설명 |
| --- | --- |
| IFX 직접 진입 이벤트, UI context | 전이와 context 갱신 정보 |
## 4. 출력
| 출력 | 설명 |
| --- | --- |
| `IFX_SETTING`, transition flag | IFX 설정 패널 전이 결과 |
## 5. 구현 기준
| 항목 | 기준 |
| --- | --- |
| context | 전이 전에 selected FX bus를 IFX로 갱신한다. |
| 재입력 | 이미 IFX_SETTING이면 정의된 동일 상태 정책을 적용한다. |
## 6. 구현 및 검증 상태
| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 미구현 | - | IFX_SETTING 상태와 직접 전이가 없다. |
| 검증 | 미완료 | - | 모든 출발 패널과 재입력을 검증한다. |
