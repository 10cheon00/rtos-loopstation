---
title: LCD 출력 commit
version: 0.1.0
change_history:
  - date: 2026-07-12
    version: 0.1.0
    summary: 생성된 LCD panel frame을 실제 LCD 출력으로 반영하는 기능 문서를 작성함
---

# LCD 출력 commit

## 1. 기능 범위

| 항목 | 내용 |
| --- | --- |
| 기능 ID | `FEAT-DISPLAY-007` |
| 상위 설계 문서 | `ARCH-DISPLAY.md` |
| 관련 설계 항목 | `ARCH-DISPLAY-007`, `ARCH-DISPLAY-018` |
| 주요 목적 | 생성된 LCD panel frame을 실제 LCD 출력으로 반영한다. |
| 제외 범위 | LCD driver 초기화 세부 구현, panel renderer 선택, 표시값 변경 정책 |

## 2. 연결된 상위 설계 항목

| 설계 항목 ID | 설계 항목 | 연결 내용 |
| --- | --- | --- |
| `ARCH-DISPLAY-007` | panel renderer | renderer가 만든 frame을 LCD에 출력한다. |
| `ARCH-DISPLAY-018` | 전체 표시 경로 | 표시 구조의 최종 출력이 LCD로 전달된다. |

## 3. 목적

패널 renderer가 만든 frame buffer 또는 draw command list를 LCD driver 출력으로 commit해 사용자가 선택된 패널을 볼 수 있게 한다.

## 4. 입력

| 입력 | 설명 |
| --- | --- |
| LCD frame buffer 또는 draw command list | renderer가 생성한 화면 출력 결과 |

## 5. 출력

| 출력 | 설명 |
| --- | --- |
| LCD output | 실제 LCD에 반영된 패널 화면 |

## 6. 구현 기준

| 항목 | 기준 |
| --- | --- |
| commit 단위 | `ARCH-DISPLAY-007`의 패널 전환 출력은 전체 frame commit을 기본으로 한다. |
| 실패 처리 | LCD 전송 실패는 진단 counter 또는 런타임 오류 보고와 연결한다. |
| 동시성 | LCD commit 중 다른 render request와 buffer 소유권이 충돌하지 않아야 한다. |
| 완료 표시 | commit 완료 후 필요하면 현재 표시 frame metadata를 갱신한다. |

## 7. 완료 기준

| 기준 | 확인 방법 |
| --- | --- |
| LCD 반영 | `UI_STATE_RENDER` 처리 후 LCD에 새 패널이 출력되는지 확인한다. |
| frame 일치 | renderer가 만든 frame과 LCD 출력 내용이 일치하는지 확인한다. |
| 실패 기록 | LCD commit 실패 시 오류가 기록되는지 확인한다. |
