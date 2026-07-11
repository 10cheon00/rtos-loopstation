---
title: Loop Station 기획 문서
version: 0.7.1
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
  - date: 2026-07-11
    version: 0.7.1
    summary: 이전 폴더 구조 기준 문서 목록을 제거하고 작성 원칙을 현재 문서 구조에 맞게 정리함
---

# Loop Station 기획 문서

이 폴더는 Loop Station 프로젝트 문서를 V 모델 흐름에 맞춰 정리한 문서 묶음이다.
현재 기준 문서는 `0-Hardware`, `1-Requirements`, `2-Architecture`, `3-Features` 폴더의 문서를 우선한다.

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
- 실제 결선 기준은 [0-Hardware](./0-Hardware/) 폴더의 문서를 우선 참고한다.
- 설계 공통 정보는 [2-Architecture](./2-Architecture/) 폴더의 `ARCH-*-REF-*.md` 문서를 우선 참고한다.
- 개발 중 의사결정 기록이나 과거 배선 계획은 [old](./old/) 또는 [old-2](./old-2/) 폴더를 참고한다.
