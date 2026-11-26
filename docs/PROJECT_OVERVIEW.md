---
title: RTOS LoopStation 프로젝트 개요
version: 1.0.0
change_history:
  - date: 2026-11-26
    version: 1.0.0
    summary: RTOS LoopStation 프로젝트 초기 개요 문서 생성
---

# RTOS LoopStation 프로젝트 개요

## 개요

RTOS LoopStation은 **FreeRTOS CMSIS V2 포트**를 기반으로 한 **실시간 오디오 처리 시스템**으로, 마이크 입력 수신, 트랙 녹음, 반복 재생, 오버다빙 (Overdubbing), FX 효과, 다양한 사용자 입력 (LCD 패널, 로터리 인크리먼트, 부트언) 을 처리하는 임베디드 소프트웨어입니다.

본 프로젝트는 참조 설계인 **RC-505 MK1**를 기반으로 개발되었습니다.

## 하드웨어 플랫폼

- **마이크로컨트롤러**: STM32H743VIT6 (Cortex-M7)
- **RTOS**: FreeRTOS V10.6.2 (CMSIS RTOS V2 포트)
- **오버레이**: STM32CubeIDE
