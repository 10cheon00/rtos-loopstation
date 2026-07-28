---
title: 전체 태스크 대상 메시지 스키마
version: 0.1.0
change_history:
  - date: 2026-07-08
    version: 0.1.0
    summary: 전체 태스크 대상 시스템 메시지와 공통 스키마를 분리해 정리함
---

# 전체 태스크 대상 메시지 스키마

이 문서는 여러 태스크가 공통으로 수신할 수 있는 시스템 제어 메시지를 정의한다. 수신 방식은 아직 확정하지 않았으므로 queue, event flag, task notification 중 하나로 후속 설계에서 결정한다.

## 1. 공통 envelope

```c
typedef struct {
    SystemMessageType type;
    TaskId source;
    uint32_t timestamp_ms;
    SystemControlPayload payload;
} SystemMessage;
```

## 2. 메시지 목록

| 메시지 종류 | 송신 태스크 | 메시지 타입 | 수신 방식 | Payload | 간단한 설명 |
| --- | --- | --- | --- | --- | --- |
| 시스템 초기화 요청 | 루프스테이션 상태 관리 태스크 | `SYSTEM_RESET_REQUEST` | TODO | `SystemControlPayload` | 상태, 오디오, 저장, 표시 관련 작업을 초기 상태로 되돌리도록 요청한다. |
| 시스템 정지 요청 | 루프스테이션 상태 관리 태스크 | `SYSTEM_STOP` | TODO | `SystemControlPayload` | 치명적 오류, 사용자 초기화, 전원 종료 대응 시 각 태스크가 진행 중 작업을 멈추고 정리하도록 요청한다. |

## 3. Payload 스키마

### 3.1 `SystemControlPayload`

| 필드 | 타입 | 설명 |
| --- | --- | --- |
| `reason` | `SystemControlReason` | 사용자 요청, fatal error, shutdown 등 |
| `error_code` | `int32_t` | 오류 기반 요청일 때 원인 코드 |
| `target_mask` | `uint32_t` | 대상 태스크 bitmask. 전체 대상이면 all bit set |
