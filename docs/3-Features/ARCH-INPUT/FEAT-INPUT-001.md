---
title: MCP23017 버튼 식별 매핑
version: 0.1.0
change_history:
  - date: 2026-07-28
    version: 0.1.0
    summary: 간소화된 양식으로 MCP23017 버튼 식별 매핑 기능 초안을 작성함
---

# FEAT-INPUT-001: MCP23017 버튼 식별 매핑

## 1. 연결된 상위 설계 항목

| 설계 항목 식별자 | 설계 항목 설명 |
| --- | --- |
| `ARCH-INPUT-001` | 좌, 우, Enter, Exit 버튼의 물리 상태를 읽는다. |
| `ARCH-INPUT-006` | IFX/TFX 버튼의 상태 변화를 공통 버튼 입력 경로로 감지한다. |
| `ARCH-INPUT-026` | 엔코더 누름 버튼 상태를 공통 버튼 입력 경로로 읽는다. |

## 2. 설명

MCP23017의 I2C 주소와 인터럽트가 발생한 핀 비트를 `ControlButtonId`로 변환한다.
이 기능은 버튼의 의미나 후속 동작을 판단하지 않는다.

## 3. 입력

| 입력 | 설명 |
| --- | --- |
| `uint8_t address` | 인터럽트를 발생시킨 MCP23017의 I2C 주소 |
| `uint16_t button_id_mask` | `INTFA` 또는 `INTFB`에서 확인한 핀 비트 |
| `input_button_mappings` | 장치 주소와 핀 위치별 `ControlButtonId` 매핑 |

## 4. 출력

| 출력 | 설명 |
| --- | --- |
| `ControlButtonId` | 물리 핀에 대응하는 버튼 식별자 |
| `TaskStatus` | 매핑 성공 여부 |

## 5. 구현 기준

| 항목 | 기준 |
| --- | --- |
| 설정 형식 | `InputButtonMapping` 배열에 장치 주소와 16개 핀의 `ControlButtonId`를 정의한다. |
| 검색 | `address`가 일치하는 매핑에서 `button_id_mask`의 핀 위치를 조회한다. |
| 미사용 핀 | `CONTROL_BUTTON_ID_NONE`인 핀은 유효한 버튼 이벤트로 만들지 않는다. |
| 복수 핀 | 여러 비트가 동시에 설정되면 각 핀을 누락 없이 개별 버튼 입력으로 처리할 수 있어야 한다. |

## 6. 구현 및 검증 상태

| 항목 | 상태 | 날짜 | 비고 |
| --- | --- | --- | --- |
| 구현 | 부분 구현 | - | `input_button_mappings`와 `InputTask_FindControlButtonId()`가 있으나 미사용 핀과 복수 핀 처리가 남아 있다. |
| 검증 | 부분 완료 | 2026-07-17 | 단일 장치와 단일 핀의 매핑 경로를 확인했다. |
