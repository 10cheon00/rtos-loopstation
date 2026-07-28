---
title: 소프트웨어 아키텍처
version: 0.5.0
change_history:
  - date: 2026-07-02
    version: 0.1.0
    summary: 소프트웨어 아키텍처 문서를 생성하고 RTOS 태스크, 상태 머신, 오디오 처리 구조를 분리해 정리함
  - date: 2026-07-04
    version: 0.2.0
    summary: 레이어드 아키텍처 정리에 맞춰 소프트웨어 구조 설명을 보강함
  - date: 2026-07-07
    version: 0.2.1
    summary: 트랙별 TFX 적용 후 믹싱하도록 오디오 처리 다이어그램을 수정함
  - date: 2026-07-07
    version: 0.3.0
    summary: PCM5102A를 DAC 모듈로 선택하면서 UDA1334 관련 내용을 제거함
  - date: 2026-07-07
    version: 0.4.0
    summary: 오디오 포맷을 44.1 kHz, 16-bit, stereo 기준으로 반영함
  - date: 2026-07-07
    version: 0.4.1
    summary: 오디오 포맷 문서와의 참조 링크를 추가함
  - date: 2026-07-07
    version: 0.4.2
    summary: docs 폴더 재구성에 따라 문서 위치와 내부 링크를 갱신함
  - date: 2026-07-07
    version: 0.5.0
    summary: 전체 파이프라인 설명을 위해 입력, 처리, 저장, 재생 경로별 플로우차트를 분리함
---

# 소프트웨어 아키텍처

## 1. 설계 원칙

루프스테이션은 실시간 오디오 처리가 가장 중요하다. 따라서 오디오 처리 경로는 높은 우선순위에서 동작해야 하며, 저장장치 접근이나 UI 갱신처럼 지연이 큰 작업과 분리되어야 한다.

주요 원칙:

- 실행 시간과 우선순위에 따라 태스크를 분리한다.
- 오디오 처리 태스크가 가장 높은 우선순위를 가진다.
- 오디오 처리 태스크에서는 blocking I/O나 긴 계산을 피한다.
- 태스크 간 통신은 큐, 세마포어, 뮤텍스 등을 사용한다.
- 공유 데이터는 최소화한다.

## 2. 전체 파이프라인

루프스테이션의 오디오 처리는 실시간 입력 경로, 트랙 반복 재생 경로, 최종 출력 경로가 믹서에서 만나는 구조로 본다. 녹음은 입력 경로에서 저장장치로 분기되는 별도 경로이며, 반복 재생은 트랙 경로의 반복 재생 처리에서 담당한다.

### 2.1 전체 오디오 경로 개요

```mermaid
flowchart LR
    subgraph INPUT["실시간 입력 경로"]
        INPUT_AUDIO["외부 오디오 입력"]
        IFX["IFX"]
        PASSTHROUGH["입력 패스스루"]
        RECORD["녹음"]
        INPUT_AUDIO --> IFX
        IFX --> PASSTHROUGH
        IFX --> RECORD
    end

    subgraph TRACK["트랙 반복 재생 경로"]
        TRACK_DATA["저장된 트랙"]
        LOOP["반복 재생 처리"]
        TFX["TFX"]
        TGAIN["트랙 음량"]
        TRACK_DATA --> LOOP --> TFX --> TGAIN
        LOOP -. "끝에 도달하면 처음으로 이동" .-> LOOP
    end

    subgraph OUTPUT["출력 경로"]
        MIXER["믹싱"]
        MASTER["마스터 음량"]
        LIMIT["최종 출력 제한"]
        OUT["오디오 출력"]
        MIXER --> MASTER --> LIMIT --> OUT
    end

    PASSTHROUGH --> MIXER
    TGAIN --> MIXER
    RECORD --> TRACK_DATA
```

### 2.2 입력 처리

입력 처리는 외부에서 들어온 실시간 오디오에 IFX를 적용한다. IFX 이후의 신호는 녹음 여부와 입력 패스스루 설정을 각각 독립적으로 검사해 녹음 경로와 믹싱 경로로 분기한다.

```mermaid
flowchart LR
    INPUT_AUDIO["외부 오디오 입력"]
    IFX["IFX"]
    RECORD_STATE{"RECORDING 또는 OVERDUBBING 상태?"}
    PASSTHROUGH{"입력 패스스루 사용?"}
    RECORD["녹음"]
    MIX["믹싱"]

    INPUT_AUDIO --> IFX
    IFX --> RECORD_STATE
    IFX --> PASSTHROUGH
    RECORD_STATE -->|예| RECORD
    PASSTHROUGH -->|예| MIX
```

### 2.3 트랙 반복 재생

트랙 반복 재생은 저장된 트랙을 현재 재생 위치부터 읽고, 트랙 끝에 도달하면 다시 처음부터 읽는다. 트랙별 FX와 음량을 적용한 뒤 믹서로 전달한다.

```mermaid
flowchart LR
    TRACK["저장된 트랙"]
    LOOP["반복 재생 처리"]
    END{"트랙 끝인가?"}
    TFX["TFX"]
    TGAIN["트랙 음량"]
    MIX["믹싱"]

    TRACK --> LOOP --> END
    END -->|아니오| TFX
    END -->|예: 처음으로 이동| LOOP
    TFX --> TGAIN --> MIX
```

### 2.4 출력 처리

출력 처리는 입력 패스스루 경로와 트랙 반복 재생 경로에서 전달된 오디오를 합산하고, 마스터 음량과 최종 출력 제한을 적용한 뒤 출력 장치로 전달한다.

```mermaid
flowchart LR
    INPUT_SRC["입력 패스스루 신호"]
    TRACK_SRC["트랙 재생 신호"]
    MIX["믹싱"]
    MASTER["마스터 음량"]
    LIMIT["최종 출력 제한"]
    OUT["오디오 출력"]

    INPUT_SRC --> MIX
    TRACK_SRC --> MIX
    MIX --> MASTER --> LIMIT --> OUT
```

## 3. RTOS 태스크 구성

| 우선순위 | 태스크 | 역할 |
| --- | --- | --- |
| 1 | 오디오 처리 태스크 | 오디오 입력, FX, 믹싱, 출력 처리 |
| 2 | 저장 장치 입출력 태스크 | 트랙 파일 저장 및 읽기 |
| 3 | 사용자 컨트롤 처리 태스크 | 버튼, 엔코더, 노브 입력 처리 |
| 4 | 루프스테이션 상태 관리 태스크 | 트랙 상태, FX 상태, 시스템 상태 관리 |
| 5 | LED/디스플레이 처리 태스크 | LCD와 LED 표시 갱신 |

TODO: 실제 RTOS 우선순위 값

## 4. 태스크 간 통신

현재 문서에서 확인된 시나리오는 다음과 같다.

| 송신자 | 수신자 | 메시지 예 |
| --- | --- | --- |
| 사용자 컨트롤 처리 태스크 | 루프스테이션 상태 관리 태스크 | 트랙 상태 전이 요청 |
| 루프스테이션 상태 관리 태스크 | 오디오 처리 태스크 | 녹음 시작, 녹음 종료, 재생 시작, 재생 종료 |
| 루프스테이션 상태 관리 태스크 | LED/디스플레이 처리 태스크 | 상태 표시 갱신 요청 |
| 오디오 처리 태스크 | 저장 장치 입출력 태스크 | 오디오 버퍼 저장 요청 |
| 저장 장치 입출력 태스크 | 오디오 처리 태스크 | 저장된 트랙 데이터 스트리밍 |

TODO: 메시지 구조체 정의

TODO: 큐 길이와 버퍼 정책

TODO: 태스크 간 동기화 방식

## 5. 상태 머신

### 5.1 시스템 상태

```mermaid
graph LR
    NO_POWER["NO_POWER"]
    NOT_INIT["NOT_INIT"]
    RUNNING["RUNNING"]
    END["END"]

    NO_POWER -->|전원 인가| NOT_INIT
    NOT_INIT -->|초기화 완료| RUNNING
    RUNNING -->|종료 이벤트| END
```

### 5.2 트랙 상태

```mermaid
graph TD
    NOT_INIT["NOT_INIT"]
    IDLE["IDLE"]
    RECORDING["RECORDING"]
    PLAYING["PLAYING"]
    OVERDUBBING["OVERDUBBING"]
    STOPPED["STOPPED"]

    NOT_INIT -->|초기화 완료| IDLE
    IDLE -->|녹음/재생 버튼| RECORDING
    RECORDING -->|녹음/재생 버튼| PLAYING
    PLAYING -->|녹음/재생 버튼| OVERDUBBING
    OVERDUBBING -->|녹음/재생 버튼| PLAYING
    RECORDING -->|정지 버튼| STOPPED
    PLAYING -->|정지 버튼| STOPPED
    OVERDUBBING -->|정지 버튼| STOPPED
    STOPPED -->|녹음/재생 버튼| PLAYING
    STOPPED -->|정지 버튼 길게 누름 또는 연속 입력| IDLE
```

## 6. 오디오 처리 구조

현재 하드웨어 테스트에서는 다음 구성이 확인되었다.

- `SAI1 Block B`: INMP441 입력 수신
- `SAI1 Block A`: PCM5102A 출력 송신
- SAI 입출력 포맷과 sample rate는 [audio_data_format.md의 4. SAI 입출력 변환 정책](../audio/audio_data_format.md#4-sai-입출력-변환-정책)을 기준으로 한다.
- 현재 테스트 코드는 polling 기반 passthrough

목표 오디오 파이프라인은 [2. 전체 파이프라인](#2-전체-파이프라인)을 기준으로 한다.

DMA circular buffer 구조와 오디오 버퍼 크기는 [audio_data_format.md의 6. 버퍼 단위](../audio/audio_data_format.md#6-버퍼-단위)를 기준으로 한다.

샘플 포맷과 fixed/floating point 처리 방식은 [audio_data_format.md의 3. 내부 처리 포맷](../audio/audio_data_format.md#3-내부-처리-포맷)을 기준으로 한다.

mixing gain 정책은 [audio_data_format.md의 7. FX 및 믹싱과의 관계](../audio/audio_data_format.md#7-fx-및-믹싱과의-관계)를 기준으로 한다.

## 7. 저장 장치 입출력 구조

SD 카드는 `SDMMC1`과 FatFs로 접근한다. 녹음된 트랙 데이터는 저장 장치 입출력 태스크를 통해 파일로 저장한다.

TODO: 파일 포맷

TODO: 파일 flush 정책

TODO: 재생 스트리밍 버퍼 구조

TODO: SD 카드 오류 복구 방식

## 8. FX 처리 구조

FX는 입력용 IFX와 출력용 TFX로 구분한다.

프로토타입 FX 후보:

- LPF
- HPF
- EQ
- Reverb

추가 후보:

- Flanger
- Phaser
- Chorus
- Delay

각 FX의 설명과 파라미터는 [fx_design.md](../audio/fx_design.md)를 기준으로 한다.
