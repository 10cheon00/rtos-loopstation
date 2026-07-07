# 요구사항 정의서

## 1. 기능 요구사항

### 1.1 오디오 입출력

| ID | 요구사항 | 근거 |
| --- | --- | --- |
| FR-AUDIO-001 | 시스템은 INMP441에서 입력된 I2S 오디오 데이터를 수신할 수 있어야 한다. | `hardware_wiring.md`, `software_design.md` |
| FR-AUDIO-002 | 시스템은 SAI1 Block A를 통해 I2S DAC로 오디오 데이터를 출력할 수 있어야 한다. | `hardware_wiring.md` |
| FR-AUDIO-003 | 시스템은 마이크 입력을 DAC 출력으로 passthrough할 수 있어야 한다. | 현재 하드웨어 검증 내용 |
| FR-AUDIO-004 | 시스템은 입력 오디오에 IFX를 적용할 수 있는 구조를 가져야 한다. | `project_summary.md`, `software_design.md` |
| FR-AUDIO-005 | 시스템은 출력 오디오에 TFX를 적용할 수 있는 구조를 가져야 한다. | `project_summary.md`, `software_design.md` |

### 1.2 트랙

| ID | 요구사항 | 근거 |
| --- | --- | --- |
| FR-TRACK-001 | 트랙은 `IDLE`, `RECORDING`, `PLAYING`, `OVERDUBBING`, `STOPPED` 상태를 가져야 한다. | `project_summary.md`, `software_design.md` |
| FR-TRACK-002 | `IDLE` 상태에서 녹음/재생 버튼을 누르면 `RECORDING`으로 전환해야 한다. | `project_summary.md` |
| FR-TRACK-003 | `RECORDING` 상태에서 녹음/재생 버튼을 누르면 `PLAYING`으로 전환해야 한다. | `project_summary.md` |
| FR-TRACK-004 | `PLAYING` 상태에서 녹음/재생 버튼을 누르면 `OVERDUBBING`으로 전환해야 한다. | `project_summary.md` |
| FR-TRACK-005 | `OVERDUBBING` 상태에서 녹음/재생 버튼을 누르면 `PLAYING`으로 전환해야 한다. | `project_summary.md` |
| FR-TRACK-006 | 정지 버튼은 녹음, 재생, 오버더빙을 중단하고 `STOPPED`로 전환해야 한다. | `project_summary.md` |
| FR-TRACK-007 | `STOPPED` 상태에서 녹음/재생 버튼을 누르면 `PLAYING`으로 전환해야 한다. | `project_summary.md` |
| FR-TRACK-008 | `STOPPED` 상태에서 정지 버튼을 길게 누르거나 짧은 시간 안에 연속 입력하면 트랙 데이터를 삭제해야 한다. | `project_summary.md` |

### 1.3 저장장치

| ID | 요구사항 | 근거 |
| --- | --- | --- |
| FR-STORAGE-001 | 시스템은 SD 카드를 마운트할 수 있어야 한다. | `project_summary.md`, `hardware_wiring.md` |
| FR-STORAGE-002 | 시스템은 FatFs를 사용해 파일을 읽고 쓸 수 있어야 한다. | `project_summary.md`, `hardware_wiring.md` |
| FR-STORAGE-003 | 녹음된 트랙 오디오 데이터를 저장장치에 저장할 수 있어야 한다. | `project_summary.md`, `software_design.md` |
| FR-STORAGE-004 | 저장된 트랙 오디오 데이터를 재생을 위해 읽을 수 있어야 한다. | `software_design.md` |

파일 형식:

- TODO: 트랙 오디오 파일 형식
- TODO: 샘플 포맷
- TODO: 파일명 규칙
- TODO: 메타데이터 저장 방식

### 1.4 사용자 입력

| ID | 요구사항 | 근거 |
| --- | --- | --- |
| FR-INPUT-001 | 시스템은 MCP23017에 연결된 버튼 입력, LED 출력을 처리할 수 있어야 한다. | `hardware_wiring.md` |
| FR-INPUT-002 | 시스템은 KY-040 로터리 엔코더의 CW/CCW 회전을 처리할 수 있어야 한다. | `hardware_wiring.md` |
| FR-INPUT-003 | 시스템은 KY-040 푸시 스위치를 MCP23017 입력으로 처리할 수 있어야 한다. | `hardware_wiring.md` |
| FR-INPUT-004 | 시스템은 ADC1에 연결된 포텐셔미터 값을 읽을 수 있어야 한다. | `hardware_wiring.md` |

### 1.5 표시 및 UI

| ID | 요구사항 | 근거 |
| --- | --- | --- |
| FR-UI-001 | 시스템은 LCD에 현재 상태와 설정 정보를 표시할 수 있어야 한다. | `project_summary.md` |
| FR-UI-002 | 시스템은 홈, 트랙, FX, 설정 패널을 제공해야 한다. | `software_design.md` |
| FR-UI-003 | 시스템은 LED로 IFX, TFX, 트랙 상태를 표시해야 한다. | `project_summary.md` |
| FR-UI-004 | 시스템은 사용자 조작에 의한 설정값의 변경을 표시해야 한다. | `project_summary.md` |

## 2. 비기능 요구사항

| ID | 요구사항 | 상태 |
| --- | --- | --- |
| NFR-RT-001 | 오디오 처리 태스크는 가장 높은 우선순위를 가져야 한다. | 정의됨 |
| NFR-RT-002 | 오디오 처리 경로에서는 지연이 큰 작업을 수행하지 않아야 한다. | 정의됨 |
| NFR-RT-003 | 저장장치 입출력은 오디오 처리와 분리된 태스크에서 수행해야 한다. | 정의됨 |
| NFR-RT-004 | 태스크 간 통신은 큐, 세마포어, 뮤텍스 등을 사용해야 한다. | 정의됨 |
| NFR-AUDIO-001 | 오디오 지연 시간 목표 | TODO: 허용 지연 시간 |
| NFR-AUDIO-002 | 오디오 dropout 허용 기준 | TODO: dropout 기준 |
| NFR-STORAGE-001 | SD 카드 오류 처리 정책 | TODO: 오류 처리 기준 |

## 3. 하드웨어 요구사항

| ID | 요구사항 |
| --- | --- |
| HR-001 | MCU는 STM32H743VIT6를 사용한다. |
| HR-002 | LCD는 GMG12864-06D를 SPI2로 연결한다. |
| HR-003 | 버튼/LED 확장은 MCP23017 두 개를 I2C1에 연결한다. |
| HR-004 | 오디오 입출력은 SAI1 Block A/B를 사용한다. |
| HR-005 | 저장장치는 SDMMC1 기반 SD 카드를 사용하고 보드에 내장된 SD카드 모듈을 사용해 인식한다. |
| HR-006 | 로터리 엔코더는 KY-040을 사용하고 TIM4 encoder mode를 사용한다. |
| HR-007 | 포텐셔미터들은 ADC1에 연결한다. |

## 4. 미정 사항

### 4.1 RTOS 종류 및 구체 설정

RTOS는 STM32CubeMX에서 선택 가능한 FreeRTOS를 선택한다.

- TODO: 최종 트랙 파일 포맷
- TODO: 오디오 버퍼 크기
- FX별 파라미터 목록은 [fx_design.md](./fx_design.md)를 기준으로 한다.
- TODO: UI 최종 화면 레이아웃
