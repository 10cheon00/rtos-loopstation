---
title: FX 직접 진입 parent 관계
version: 0.1.0
change_history:
  - date: 2026-07-28
    version: 0.1.0
    summary: 간소화된 양식으로 FX 직접 진입 parent 관계 기능 초안을 작성함
---
# FEAT-STATE-UI-022: FX 직접 진입 parent 관계
## 1. 연결된 상위 설계 항목
| 설계 항목 식별자 | 설계 항목 설명 |
| --- | --- |
| `ARCH-STATE-UI-022` | 직접 진입한 IFX·TFX 설정 패널의 parent를 FX 패널로 유지한다. |
## 2. 설명
출발 패널과 관계없이 IFX_SETTING과 TFX_SETTING의 Exit 목적지를 FX로 고정한다.
## 3. 입력
| 입력 | 설명 |
| --- | --- |
| IFX_SETTING 또는 TFX_SETTING | 직접 진입한 현재 패널 |
## 4. 출력
| 출력 | 설명 |
| --- | --- |
| `FX` parent panel | Exit 복귀 목적지 |
## 5. 구현 기준
| 항목 | 기준 |
| --- | --- |
| 일관성 | Enter 진입과 직접 진입이 같은 parent 관계를 사용한다. |
| 출발점 독립 | 직접 진입 이전 패널을 parent로 저장하지 않는다. |
## 6. 구현 및 검증 상태
| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 미구현 | - | FX 설정 상태와 parent 매핑이 없다. |
| 검증 | 미완료 | - | 서로 다른 출발 패널에서 Exit 목적지를 검증한다. |
