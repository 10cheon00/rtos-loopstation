---
title: 패널 파라미터 mailbox 갱신 검토 기록
version: 0.2.0
change_history:
  - date: 2026-07-26
    version: 0.1.0
    summary: 자기완결형 UI_STATE_RENDER 도입으로 패널 파라미터 mailbox 기능이 대체되었음을 기록함
  - date: 2026-07-28
    version: 0.2.0
    summary: 패널 파라미터 mailbox 갱신 검토 기록 관련 문서 내용을 추가함
---

# 패널 파라미터 mailbox 갱신 검토 기록

## 1. 기존 작성 대상

이 파일은 `track_param_mailbox`, `fx_param_mailbox`, `system_mailbox`, `diagnostic_mailbox`에서 현재 패널의 파라미터 payload를 받아 표시 상태를 갱신하는 기능 문서로 작성할 예정이었다.

## 2. 검토 및 구현 결과

StateTask가 하나의 `StateEvent` 처리를 완료한 후 현재 UI panel ID와 그 패널에 바인딩된 모든 파라미터를 하나의 `UI_STATE_RENDER` command에 복사하도록 설계를 변경했다.
이에 따라 패널 파라미터 mailbox를 별도로 두고 DisplayTask가 값을 조합하거나 최신화하는 기능은 역할이 중복되어 필요하지 않다.

## 3. 대체 설계

| 기존 설계 ID | 검토 결과 | 대체 설계 |
| --- | --- | --- |
| `ARCH-DISPLAY-021` | `track_param_mailbox`를 구현하지 않는다. | `ARCH-DISPLAY-005`, `ARCH-DISPLAY-008`, `ARCH-DISPLAY-010` |
| `ARCH-DISPLAY-023` | `fx_param_mailbox`를 구현하지 않는다. | `ARCH-DISPLAY-005`, `ARCH-DISPLAY-008`, `ARCH-DISPLAY-010` |
| `ARCH-DISPLAY-024` | `system_mailbox`를 구현하지 않는다. | `ARCH-DISPLAY-005`, `ARCH-DISPLAY-008`, `ARCH-DISPLAY-010` |
| `ARCH-DISPLAY-025` | 일반 UI 파라미터용 `diagnostic_mailbox`를 구현하지 않는다. | `ARCH-DISPLAY-005`, 별도 비동기 telemetry가 필요하면 신규 설계 |

## 4. 보존 이유

기존 기능 작성 대상이 삭제된 이유와 대체 경로를 추적할 수 있도록 파일을 유지한다.
이 문서는 구현 대상 기능 명세가 아니라 검토 기록이며 구현 완료 판정 대상에서 제외한다.
