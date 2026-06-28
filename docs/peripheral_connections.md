# 주변장치 연결 계획

이 문서는 STM32H743VIT6, LQFP100 패키지 기준의 1차 배선 계획이다. 이전 문서의 STM32 연결 컬럼은 잘못된 핀아웃을 바탕으로 작성되었으므로 사용하지 않는다. 아래 연결은 `docs/stm32h743vi.pdf`의 STM32H742/H743 데이터시트 DS12110 Rev 11, Table 9 pin/ball definition과 Table 10 alternate functions를 기준으로 다시 정리했다.

## 선택 규칙

- 현재 `.ioc`에서 이미 사용하는 `SDMMC1`, `SWD`, SD detect 핀은 고정한다.
- 루프스테이션의 핵심 기능인 오디오 입출력은 `SAI1`으로 먼저 확보한다.
- LCD는 ST7565R 계열 쓰기 전용 SPI 장치로 보고 `MISO`를 쓰지 않는다.
- 외부 SPI ADC는 이번 배선 계획에서 제외한다.
- 버튼/LED 확장은 `I2C1` 기반 MCP23017로 묶어 GPIO 소모를 줄인다.
- GPIO 제어선은 가능하면 전용 고속 주변장치와 충돌이 적은 핀을 고른다.

## 현재 IOC에서 이미 고정된 핀

| 용도 | 주변장치 | STM32H743VI 핀 |
| --- | --- | --- |
| SWDIO | SWD | PA13 |
| SWCLK | SWD | PA14 |
| SD 데이터 0 | SDMMC1_D0 | PC8 |
| SD 데이터 1 | SDMMC1_D1 | PC9 |
| SD 데이터 2 | SDMMC1_D2 | PC10 |
| SD 데이터 3 | SDMMC1_D3 | PC11 |
| SD 클럭 | SDMMC1_CK | PC12 |
| SD 명령 | SDMMC1_CMD | PD2 |
| SD 카드 감지 | GPIO 입력 | PD4 |

## 주변장치 요약

| 모듈 | 사용할 주변장치 | STM32H743VI 핀 | 결정 |
| --- | --- | --- | --- |
| GMG12864-06D LCD, ST7565R | SPI2 송신 전용 + GPIO | PB10, PB15, PE7, PE8, PE9, PE10 | 사용 |
| MCP23017 x2 | I2C1 | PB6, PB7 | 사용 |
| PCM5102 DAC + INMP441 마이크 | SAI1 Block A/B | PE3, PE4, PE5, PE6, 선택적으로 PE2 | 사용 |
| KY-040 엔코더 | GPIO/EXTI | PB12, PB13, PB14 | 사용 |
| MCP23017 인터럽트 라인 | GPIO/EXTI | PC0, PC1 | 선택 사항 |

## LCD용 SPI 선택

LCD는 `SPI2`를 송신 전용 마스터 모드로 사용한다.

| LCD 신호 | STM32H743VI 핀 | STM32 기능 |
| --- | --- | --- |
| LCD SCL / SCK | PB10 | SPI2_SCK |
| LCD SI / MOSI | PB15 | SPI2_MOSI |
| LCD SO / MISO | 연결하지 않음 | ST7565R LCD 표시에 읽기 기능은 필요하지 않음 |

다른 SPI를 선택하지 않은 이유는 다음과 같다.

| SPI | 제외 이유 |
| --- | --- |
| SPI1 | 사용 가능하지만 LCD에 쓰면 PA5/PA6/PA7을 묶어 쓰게 된다. 현재 계획에서는 SPI2가 더 독립적으로 배치된다. |
| SPI3 | LQFP100에서 흔한 핀 조합인 PC10/PC11/PC12가 현재 `SDMMC1`과 충돌한다. |
| SPI4 | PE2/PE4/PE5/PE6 조합은 `SAI1` 오디오 핀과 충돌한다. PE11/PE12/PE13/PE14 조합도 Port E의 연속 핀을 크게 점유한다. |
| SPI6 | PA5/PA6/PA7 조합을 쓰기 쉬워 SPI1과 같은 핀 그룹을 점유한다. |

## GMG12864-06D LCD

GMG12864-06D 모듈은 ST7565R 호환 128x64 LCD로 보고, 4선식 SPI와 리셋, 데이터/명령 선택 GPIO를 사용한다.

| GMG12864-06D 핀 | STM32H743VI 연결 | STM32 기능 | 비고 |
| --- | --- | --- | --- |
| CS | PE7 | GPIO_Output | LCD 칩 선택, 소프트웨어로 제어 |
| RSE / RST | PE8 | GPIO_Output | LCD 리셋 |
| RS / DC | PE9 | GPIO_Output | 명령/데이터 선택 |
| SCL | PB10 | SPI2_SCK | SPI 클럭 |
| SI | PB15 | SPI2_MOSI | STM32에서 LCD로 전송 |
| VDD | 3.3 V | 전원 | 3.3 V 로직 사용 |
| VSS | GND | 접지 | 공통 접지 |
| A | 3.3 V 또는 PE10 경유 | 백라이트 양극 | 모듈 회로에 맞춰 저항/트랜지스터 사용 |
| K | GND | 백라이트 음극 | 모듈 회로 확인 |
| IC_SCL | 연결하지 않음 | - | 부가 인터페이스로 보고 1차 표시 검증에서는 사용하지 않음 |
| IC_CS | 연결하지 않음 | - | 부가 인터페이스 |
| IC_SO | 연결하지 않음 | - | 부가 인터페이스 |
| IC_SI | 연결하지 않음 | - | 부가 인터페이스 |

CubeMX 설정 시작점은 다음과 같다.

| 항목 | 설정 |
| --- | --- |
| SPI2 모드 | 마스터, 송신 전용 |
| NSS | Software |
| 데이터 크기 | 8-bit |
| CPOL/CPHA | 우선 mode 0으로 시작하고, 표시가 깨지면 모듈 동작에 맞춰 조정 |
| GPIO | PE7, PE8, PE9를 output으로 설정 |

## MCP23017 GPIO 확장

두 개의 MCP23017은 같은 I2C 버스를 공유한다. 주소 핀 `A0~A2`를 다르게 묶어 두 장치를 구분한다.

| MCP23017 핀 | STM32H743VI 연결 | STM32 기능 | 비고 |
| --- | --- | --- | --- |
| VDD | 3.3 V | 전원 | MCU와 같은 로직 전압 |
| VSS / GND | GND | 접지 | 공통 접지 |
| SCL | PB6 | I2C1_SCL | 3.3 V pull-up 필요 |
| SDA | PB7 | I2C1_SDA | 3.3 V pull-up 필요 |
| RESET | 3.3 V 또는 예비 GPIO | 리셋 | 10 kOhm 정도 pull-up 권장 |
| A0/A1/A2, U1 | GND/GND/GND | 주소 설정 | 7-bit 주소 `0x20` |
| A0/A1/A2, U2 | 3.3 V/GND/GND | 주소 설정 | 7-bit 주소 `0x21` |
| INTA/INTB, 선택 사항 | PC0 / PC1 | GPIO_EXTI | 인터럽트 기반 입력 처리가 필요할 때만 사용 |

MCP23017 모듈 보드에 `CS`, `SO`처럼 SPI용으로 보이는 핀이 있어도, I2C 모드에서는 연결하지 않는다.

## PCM5102 DAC 및 INMP441 마이크

오디오는 `SAI1`을 사용한다. DAC 출력과 마이크 입력을 하나의 오디오 주변장치 그룹에 묶어 LCD/SPI 선택이 오디오 핀을 침범하지 않도록 한다.

| 오디오 신호 | STM32H743VI 연결 | STM32 기능 | 연결 모듈 |
| --- | --- | --- | --- |
| BCLK / SCK | PE5 | SAI1_SCK_A | PCM5102와 INMP441 공유 |
| LRCK / WS / FS | PE4 | SAI1_FS_A | PCM5102와 INMP441 공유 |
| DAC 데이터 출력 | PE6 | SAI1_SD_A | STM32에서 PCM5102 DIN으로 출력 |
| 마이크 데이터 입력 | PE3 | SAI1_SD_B | INMP441 SD에서 STM32로 입력 |
| MCLK, 선택 사항 | PE2 | SAI1_MCLK_A | 대부분의 PCM5102 모듈에서는 필요하지 않음 |

권장 SAI 구성은 다음과 같다.

| 블록 | 역할 |
| --- | --- |
| SAI1 Block A | PCM5102용 마스터 송신 |
| SAI1 Block B | INMP441용 동기 수신 |

PCM5102 모듈 연결:

| PCM5102 핀 | 연결 | 비고 |
| --- | --- | --- |
| VIN / VCC | 3.3 V 또는 모듈 지원 전원 | 모듈 보드에 레귤레이터가 있는지 확인 |
| GND | GND | 공통 접지 |
| BCK / BCLK | PE5 | SAI1_SCK_A |
| LCK / LRCK / WS | PE4 | SAI1_FS_A |
| DIN | PE6 | SAI1_SD_A |
| SCK | 연결하지 않음 또는 필요 시 PE2 | 대부분의 PCM5102 보드는 외부 MCLK 없이 동작 가능 |
| XSMT | 3.3 V | 음소거 해제 |
| FMT / FLT / DMP | 모듈 기본값 또는 고정 연결 | 모듈 보드 기본 설정 확인 |

INMP441 모듈 연결:

| INMP441 핀 | 연결 | 비고 |
| --- | --- | --- |
| VDD | 3.3 V | 3.3 V 사용 |
| GND | GND | 공통 접지 |
| SCK / BCLK | PE5 | 오디오 bit clock 공유 |
| WS / LRCK | PE4 | 오디오 frame sync 공유 |
| SD | PE3 | SAI1_SD_B |
| L/R | GND 또는 3.3 V | 좌/우 채널 선택 |

## KY-040 로터리 엔코더

엔코더는 GPIO EXTI로 처리한다. 아래 핀들은 SDMMC, SWD, SAI1, I2C1, LCD SPI2 SCK/MOSI와 겹치지 않는다.

| KY-040 핀 | STM32H743VI 연결 | STM32 기능 | 비고 |
| --- | --- | --- | --- |
| + | 3.3 V | 전원 | 3.3 V 사용 |
| GND | GND | 접지 | 공통 접지 |
| CLK / A | PB12 | GPIO_EXTI | pull-up |
| DT / B | PB13 | GPIO_EXTI | pull-up |
| SW | PB14 | GPIO_EXTI | pull-up |

PB13/PB14에도 SPI2 alternate function이 있지만, 이 설계에서는 LCD에 `PB10`, `PB15`만 사용하므로 활성 충돌은 없다.

## 핀 할당 요약

| STM32H743VI 핀 | 할당 용도 |
| --- | --- |
| PA13 | SWDIO |
| PA14 | SWCLK |
| PB6 | I2C1_SCL |
| PB7 | I2C1_SDA |
| PB10 | LCD SPI2_SCK |
| PB12 | 엔코더 A |
| PB13 | 엔코더 B |
| PB14 | 엔코더 스위치 |
| PB15 | LCD SPI2_MOSI |
| PC0 | MCP23017 인터럽트, 선택 사항 |
| PC1 | MCP23017 인터럽트, 선택 사항 |
| PC8 | SDMMC1_D0 |
| PC9 | SDMMC1_D1 |
| PC10 | SDMMC1_D2 |
| PC11 | SDMMC1_D3 |
| PC12 | SDMMC1_CK |
| PD2 | SDMMC1_CMD |
| PD4 | SD 카드 감지 |
| PE2 | SAI1_MCLK_A, 선택 사항 |
| PE3 | INMP441 데이터, SAI1_SD_B |
| PE4 | 오디오 LRCK/WS, SAI1_FS_A |
| PE5 | 오디오 BCLK, SAI1_SCK_A |
| PE6 | PCM5102 데이터, SAI1_SD_A |
| PE7 | LCD CS |
| PE8 | LCD 리셋 |
| PE9 | LCD RS/DC |
| PE10 | LCD 백라이트 제어, 선택 사항 |

## 배선 전 확인 사항

- 모든 모듈의 GND는 STM32 보드 GND와 공통으로 묶는다.
- GPIO, I2C, SPI 신호는 3.3 V 로직으로 운용한다.
- 5 V pull-up이 걸린 모듈은 STM32 핀에 직접 연결하지 않는다.
- I2C pull-up은 3.3 V에 연결한다.
- 여러 I2C 모듈에 pull-up이 이미 달려 있으면 전체 등가 저항이 너무 낮아질 수 있으므로 확인한다.
- LCD `CS`는 하드웨어 NSS가 아니라 GPIO로 직접 제어한다.
- `SPI4`는 LCD에 사용하지 않는다. LQFP100에서 `SPI4`의 유효 핀들이 선택한 `SAI1` 오디오 핀 그룹과 겹치거나 Port E를 크게 점유한다.
- 이후 다른 SPI 모듈을 추가할 때는 해당 모듈이 `MISO`를 요구하는지 먼저 확인한다. 쓰기 전용 디스플레이와 읽기/쓰기 센서는 버스 제약이 다르다.

## 참고 자료

- STMicroelectronics, `STM32H742xI/G STM32H743xI/G` datasheet DS12110 Rev 11, January 2026: `docs/stm32h743vi.pdf`
- 현재 CubeMX 프로젝트: `rtos-loopstation.ioc`
