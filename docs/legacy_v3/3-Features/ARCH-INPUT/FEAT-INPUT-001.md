---
title: MCP23017 버튼 pin 식별 테이블
version: 0.3.0
change_history:
  - date: 2026-07-12
    version: 0.1.0
    summary: 버튼 EXTI source를 ButtonId로 변환하는 기능 문서를 작성함
  - date: 2026-07-12
    version: 0.2.0
    summary: EXTI source가 아닌 MCP23017 pin bit를 ButtonId로 변환하도록 수정함
  - date: 2026-07-12
    version: 0.2.1
    summary: 상위 설계 항목명을 버튼 변경 감지 기준으로 맞춤
  - date: 2026-07-17
    version: 0.3.0
    summary: 버튼 매핑 구현 현황과 검증 상태를 추가함
---

# MCP23017 버튼 pin 식별 테이블

## 1. 기능 범위

| 항목 | 내용 |
| --- | --- |
| 기능 ID | `FEAT-INPUT-001` |
| 상위 설계 문서 | `ARCH-INPUT.md` |
| 관련 설계 항목 | `ARCH-INPUT-001`, `ARCH-INPUT-006`, `ARCH-INPUT-026` |
| 주요 목적 | MCP23017에서 읽은 interrupt pin bit 또는 GPIO bit를 `ButtonId`로 변환한다. |
| 제외 범위 | debounce, 버튼 의미 해석, `CONTROL_BUTTON` 생성 |

## 2. 연결된 상위 설계 항목

| 설계 항목 ID | 설계 항목 | 연결 내용 |
| --- | --- | --- |
| `ARCH-INPUT-001` | 버튼 변경 감지 | 제어부 버튼 입력 source를 식별한다. |
| `ARCH-INPUT-006` | FX 버튼 감지 | IFX/TFX 버튼 입력 source를 식별한다. |
| `ARCH-INPUT-026` | encoder push 변경 감지 | 엔코더 push 입력 source를 식별한다. |

## 3. 목적

MCP23017의 interrupt line은 여러 버튼 입력을 하나의 MCU interrupt로 모을 수 있다.
따라서 사용자 컨트롤 처리 태스크가 I2C로 MCP23017의 interrupt 상태를 읽은 뒤, 어떤 pin bit가 어떤 버튼인지 변환할 수 있어야 한다.
이 기능은 MCP23017 device, port, pin bit와 `ButtonId`를 연결하는 정적 테이블을 제공한다.

## 4. 입력

| 입력 | 설명 |
| --- | --- |
| MCP23017 device id | interrupt를 발생시킨 MCP23017 장치 식별자 |
| port id | MCP23017의 port A/B 식별자 |
| interrupt pin bit 또는 GPIO bit | I2C로 읽은 `INTF`, `INTCAP`, `GPIO` 기반 pin bit |
| raw level | MCP23017에서 읽은 버튼 물리 level |

## 5. 출력

| 출력 | 설명 |
| --- | --- |
| `ButtonId` | 상태 관리 구조와 입력 처리 구조가 공유하는 버튼 식별자 |
| normalized raw state | active high/low 차이를 정규화한 raw pressed/released 상태 |

## 6. 구현 기준

| 항목 | 기준 |
| --- | --- |
| pin mapping | MCP23017 device, port, pin bit 조합마다 정확히 하나의 `ButtonId`에 매핑한다. |
| unknown pin | 매핑되지 않은 pin bit는 event로 만들지 않고 진단 counter만 증가시킨다. |
| active level | 버튼별 active level을 테이블에 포함해 `pressed`/`released` 정규화가 가능해야 한다. |
| 초기 버튼 목록 | `BUTTON_LEFT`, `BUTTON_RIGHT`, `BUTTON_ENTER`, `BUTTON_EXIT`, `BUTTON_IFX`, `BUTTON_TFX`, `BUTTON_TRACK_REC_PLAY`, `BUTTON_TRACK_STOP`, `BUTTON_TRACK_CONTROL`, `BUTTON_ENCODER_PUSH`를 표현할 수 있어야 한다. |

## 7. 완료 기준

| 기준 | 확인 방법 |
| --- | --- |
| pin 변환 | 각 MCP23017 pin bit가 기대한 `ButtonId`로 변환되는지 확인한다. |
| active level 정규화 | active high/low 입력이 동일한 pressed/released 의미로 정규화되는지 확인한다. |
| unknown pin 처리 | 매핑되지 않은 pin bit가 event를 만들지 않고 오류로 기록되는지 확인한다. |

## 8. 구현 및 검증 상태

| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 부분 구현 | - | `input_button_mappings`와 `InputTask_FindControlButtonId()`를 구현했으나 unknown pin 진단과 복수 pin event 처리가 남아 있다. |
| 검증 | 부분 완료 | 2026-07-17 | 단일 pin의 slave address 및 pin mask 매핑 경로를 확인했다. |
