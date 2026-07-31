---
title: FX 직접 진입 snapshot 갱신
version: 0.2.0
change_history:
  - date: 2026-07-28
    version: 0.1.0
    summary: 간소화된 양식으로 FX 직접 진입 후 렌더링 기능 초안을 작성함
  - date: 2026-07-31
    version: 0.2.0
    summary: 직접 진입 후 렌더링 요청을 최신 FX 설정 패널 snapshot 갱신 기능으로 변경함
---
# FEAT-STATE-UI-023: FX 직접 진입 snapshot 갱신
## 1. 연결된 상위 설계 항목
| 설계 항목 식별자 | 설계 항목 설명 |
| --- | --- |
| `ARCH-STATE-UI-023` | StateTask가 직접 진입 완료 후 FX 설정 패널 값으로 최신 UI 표시 snapshot을 덮어쓴다. |
## 2. 설명
StateTask가 IFX·TFX 직접 전이를 마친 뒤 선택된 FX 설정 패널과 바인딩 파라미터를 최신 `DisplaySnapshot.ui_state`에 게시한다.
## 3. 입력
| 입력 | 설명 |
| --- | --- |
| FX 설정 `UiPanelId`, FX bus context, 바인딩 파라미터 | 직접 전이된 패널과 현재 표시값 |
## 4. 출력
| 출력 | 설명 |
| --- | --- |
| 갱신된 `DisplaySnapshot.ui_state` | DisplayTask가 다음 표시 주기에 사용할 FX 설정 패널 snapshot |
## 5. 구현 기준
| 항목 | 기준 |
| --- | --- |
| 후처리 | FX bus context 갱신과 전이가 끝난 뒤 snapshot을 게시한다. |
| snapshot | 선택된 FX 설정 패널의 바인딩 파라미터만 포함한다. |
| 출력 분리 | snapshot 게시 함수는 renderer나 LCD·LED 출력 함수를 호출하지 않는다. |
## 6. 구현 및 검증 상태
| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 부분 구현 | - | 공통 snapshot 게시 경로만 있고 FX 직접 전이는 없다. |
| 검증 | 미완료 | - | IFX·TFX 직접 진입 화면을 검증한다. |
