---
title: Loop Station 기획 문서
version: 0.7.0
change_history:
  - date: 2026-07-02
    version: 0.1.0
    summary: docs 문서 묶음의 README를 생성하고 주요 기획 문서 목록을 정리함
  - date: 2026-07-04
    version: 0.2.0
    summary: FX 설계 문서와 계층 분리 설계 문서를 문서 목록에 추가함
  - date: 2026-07-07
    version: 0.3.0
    summary: 오디오 데이터 포맷 문서를 문서 목록에 추가함
  - date: 2026-07-07
    version: 0.4.0
    summary: docs 하위 폴더 구조와 각 폴더의 목적을 명시함
  - date: 2026-07-07
    version: 0.4.1
    summary: docs 폴더 재구성에 따라 문서 링크를 갱신함
  - date: 2026-07-08
    version: 0.5.0
    summary: 태스크 메시지 설계 문서를 문서 목록에 추가함
  - date: 2026-07-10
    version: 0.6.0
    summary: TODO 목록 자동 생성 뷰와 메시지 기반 아키텍처 문서 링크를 문서 목록에 추가함
  - date: 2026-07-11
    version: 0.7.0
    summary: V 모델 문서 폴더 구조와 아키텍처 및 기능 문서 파일명 규칙을 추가함
---

# Loop Station 기획 문서

이 폴더는 Loop Station 프로젝트를 기획, 설계, 검증 관점에서 읽기 좋게 정리한 문서 묶음이다. 기존 `docs/old/project_summary.md`, `docs/old/software_design.md`, `docs/old/hardware_wiring.md`, `docs/old/peripheral_connections.md`에 흩어져 있던 내용을 문서 목적별로 다시 나누었다.

## 문서 목록

| 문서 | 목적 |
| --- | --- |
| [project_plan.md](./planning/project_plan.md) | 프로젝트 목표, 범위, 성공 기준을 정의한다. |
| [requirements.md](./planning/requirements.md) | 기능/비기능/하드웨어 요구사항을 정리한다. |
| [user_scenarios.md](./planning/user_scenarios.md) | 사용자가 시스템을 조작하는 주요 흐름을 정리한다. |
| [ui_design.md](./ui/ui_design.md) | LCD 패널, 버튼, 엔코더, LED 표시 정책을 정리한다. |
| [fx_design.md](./audio/fx_design.md) | 필수 FX와 추후 FX 후보의 설명 및 파라미터를 정리한다. |
| [audio_data_format.md](./audio/audio_data_format.md) | 저장/처리/입출력에 사용할 오디오 데이터 포맷 기준을 정리한다. |
| [software_architecture.md](./architecture/software_architecture.md) | RTOS 태스크, 상태 머신, 오디오 처리 구조를 정리한다. |
| [message-driven_architecture.md](./architecture/message-driven_architecture.md) | 태스크들이 직접 메시지를 주고받는 구조와 설계 원칙을 정리한다. |
| [task_message_design.md](./architecture/task_message_design.md) | RTOS 태스크 사이에서 오갈 수 있는 메시지 후보를 정리한다. |
| [hardware_configuration.md](./hardware/hardware_configuration.md) | 현재 검증된 모듈과 STM32 결선을 정리한다. |
| [test_plan.md](./verification/test_plan.md) | 모듈 테스트와 통합 테스트 계획을 정리한다. |
| [todo_index.md](./todo_index.md) | docs 하위 문서에 남아 있는 TODO 항목을 모아 보여준다. |

## 폴더 목록

| 폴더 | 목적 |
| --- | --- |
| [0-Hardware](./0-Hardware/) | 프로젝트에 사용하는 하드웨어 모듈, peripheral, pinout을 보관한다. |
| [1-Requirements](./1-Requirements/) | 시스템이 외부에서 어떻게 동작해야 하는지 요구사항을 보관한다. |
| [2-Architecture](./2-Architecture/) | 요구사항 분류별 설계 문서와 설계 참조 문서를 보관한다. |
| [3-Features](./3-Features/) | 설계를 실제 구현 단위로 나눈 기능 문서를 보관한다. |
| [old](./old/) | 개발 과정에서 작성한 이전 버전 문서와 기록성 문서를 보관한다. 현재 기준 문서는 V 모델 폴더의 문서를 우선한다. |
| [old-2](./old-2/) | V 모델 문서 구조로 재분류하기 전의 이전 문서 묶음을 보관한다. |

## 파일명 규칙

| 구분 | 위치 | 규칙 | 예시 |
| --- | --- | --- | --- |
| 하드웨어 문서 | `0-Hardware/` | 내용이 드러나는 kebab-case 또는 snake_case 파일명 | `hardware_modules.md` |
| 요구사항 문서 | `1-Requirements/` | 내용이 드러나는 kebab-case 파일명 | `loopstation-requirements.md` |
| 아키텍처 문서 | `2-Architecture/` | `ARCH-{요구사항명}.md` | `ARCH-AUDIO.md` |
| 아키텍처 참조 문서 | `2-Architecture/` | `ARCH-{요구사항명}-REF-{참조문서명}.md` | `ARCH-AUDIO-REF-AUDIO-FORMAT.md` |
| 기능 문서 폴더 | `3-Features/` | `ARCH-{요구사항명}/` | `ARCH-AUDIO/` |
| 기능 문서 | `3-Features/ARCH-{요구사항명}/` | `FEAT-{기능명}.md` | `FEAT-audio-passthrough.md` |

`{요구사항명}`은 요구사항 분류 ID에서 `REQ-`를 제거한 값을 사용한다.
예를 들어 `REQ-AUDIO`의 설계 문서는 `ARCH-AUDIO.md`이고, 이 설계를 구현하는 기능 문서는 `3-Features/ARCH-AUDIO/` 아래에 둔다.

## 작성 원칙

- 기존 문서에서 확인할 수 있는 내용만 확정 사항으로 적는다.
- 확인되지 않은 세부 사항은 `TODO:`로 남긴다.
- 실제 결선 기준은 [hardware_configuration.md](./hardware/hardware_configuration.md)를 우선 참고한다.
- 오디오 포맷 기준은 [audio_data_format.md](./audio/audio_data_format.md)를 우선 참고한다.
- 개발 중 의사결정 기록이나 과거 배선 계획은 [old](./old/) 폴더를 참고한다.
