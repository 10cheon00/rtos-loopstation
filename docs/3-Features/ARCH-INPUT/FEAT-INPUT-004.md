---
title: MCP23017 버튼 상태 조회
version: 0.1.0
change_history:
  - date: 2026-07-28
    version: 0.1.0
    summary: 간소화된 양식으로 MCP23017 버튼 상태 조회 기능 초안을 작성함
---

# FEAT-INPUT-004: MCP23017 버튼 상태 조회

## 1. 연결된 상위 설계 항목

| 설계 항목 식별자 | 설계 항목 설명 |
| --- | --- |
| `ARCH-INPUT-001` | 좌, 우, Enter, Exit 버튼의 물리 상태를 읽는다. |
| `ARCH-INPUT-006` | IFX/TFX 버튼의 상태 변화를 공통 버튼 입력 경로로 감지한다. |
| `ARCH-INPUT-026` | 엔코더 누름 버튼 상태를 공통 버튼 입력 경로로 읽는다. |
| `ARCH-INPUT-031` | 버튼의 접점 흔들림을 제거하고 안정된 눌림/떼짐 이벤트를 만든다. |

## 2. 설명

`Mcp23017IntEvent`의 EXTI 핀으로 MCP23017 주소를 찾고, 태스크 문맥에서 `INTF`와 `INTCAP` 레지스터를 읽어 변경된 핀과 버튼의 눌림 상태를 구한다.

## 3. 입력

| 입력 | 설명 |
| --- | --- |
| `Mcp23017IntEvent` | `gpio_pin`과 `timestamp_tick`을 가진 인터럽트 기록 |
| `input_mcp23017_devices` | EXTI 핀과 MCP23017 주소를 연결하는 `Mcp23017DeviceConfig` 배열 |
| `I2C_HandleTypeDef *hi2c` | MCP23017 레지스터를 읽는 I2C 핸들 |

## 4. 출력

| 출력 | 설명 |
| --- | --- |
| `uint8_t address` | 인터럽트를 발생시킨 MCP23017 주소 |
| `uint16_t button_id_mask` | `INTFA` 또는 `INTFB`에서 확인한 핀 비트 |
| `ControlButtonState` | `CONTROL_BUTTON_STATE_PRESSED` 또는 `CONTROL_BUTTON_STATE_RELEASED` |
| `TaskStatus` | 주소 조회와 레지스터 읽기의 성공 여부 |

## 5. 구현 기준

| 항목 | 기준 |
| --- | --- |
| 실행 문맥 | `Mcp23017_ReadRegister()`는 ISR이 아닌 `InputTask`에서 호출한다. |
| 레지스터 | `INTFA`/`INTFB`로 변경 핀을 찾고 `INTCAPA`/`INTCAPB`로 인터럽트 시점의 상태를 읽는다. |
| active level | pull-up 버튼 입력의 low를 `CONTROL_BUTTON_STATE_PRESSED`, high를 `CONTROL_BUTTON_STATE_RELEASED`로 변환한다. |
| 복수 핀 | 하나의 캡처에 여러 변경 비트가 있으면 각 비트의 상태를 개별적으로 계산한다. |
| 오류 | 주소를 찾지 못하거나 I2C 읽기가 실패하면 `TASK_STATUS_ERROR`를 반환한다. |

## 6. 구현 및 검증 상태

| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 부분 구현 | - | 주소 조회와 `INTF`/`INTCAP` 읽기는 있으나 복수 핀 처리와 일부 오류 판정이 남아 있다. |
| 검증 | 부분 완료 | 2026-07-17 | GPIOB 버튼의 press/release 캡처 값을 확인했다. |
