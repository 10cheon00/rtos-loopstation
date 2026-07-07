# 하드웨어 구성 문서

이 문서는 현재 프로젝트에서 검증한 하드웨어 모듈과 STM32H743VIT6 peripheral 연결을 기획 문서 관점에서 요약한다. 상세 결선은 [hardware_wiring.md](../old/hardware_wiring.md)를 기준으로 한다.

## 1. 하드웨어 구성 요약

| 모듈 | STM32 peripheral | 주요 핀 | 상태 |
| --- | --- | --- | --- |
| MCP23017 x2 | I2C1 | PB6, PB7 | 버튼/LED GPIO 확장 |
| GMG12864-06D LCD | SPI2 + GPIO | PB10, PC1, PE7, PE8, PE9 | u8g2 출력 확인 |
| 포텐셔미터 x3 | ADC1 | PA0, PA1, PA2 | 3채널 테스트 완료 |
| SD 카드 | SDMMC1 + GPIO | PC8~PC12, PD2, PD4 | FatFs 파일 입출력 확인 |
| KY-040 | TIM4 + MCP23017 | PD12, PD13, MCP23017 GPIOB3 | 회전 인식 확인 |
| INMP441 | SAI1 Block B | PE3, PE4, PE5 | 마이크 입력 |
| PCM5102A | SAI1 Block A | PE4, PE5, PE6, XSMT | 최종 DAC, XSMT high 조건에서 출력 확인 |

## 2. Peripheral 사용 현황

| Peripheral | 용도 |
| --- | --- |
| I2C1 | MCP23017 두 개 연결 |
| SPI2 | GMG12864-06D LCD 송신 전용 |
| ADC1 | 포텐셔미터 입력 |
| SDMMC1 | SD 카드 |
| TIM4 | KY-040 로터리 엔코더 A/B |
| SAI1 Block A | I2S DAC 출력 |
| SAI1 Block B | INMP441 I2S 마이크 입력 |
| GPIO | LCD 제어선, SD card detect |

## 3. 모듈별 핀 연결

### 3.1 MCP23017

| 신호 | STM32 연결 | 비고 |
| --- | --- | --- |
| SCL | PB6 / I2C1_SCL | 3.3 V pull-up |
| SDA | PB7 / I2C1_SDA | 3.3 V pull-up |
| U1 address | A2/A1/A0 = `100` | 7-bit `0x24` |
| U2 address | A2/A1/A0 = `101` | 7-bit `0x25` |

### 3.2 LCD

| LCD 신호 | STM32 연결 |
| --- | --- |
| SCL | PB10 / SPI2_SCK |
| SI | PC1 / SPI2_MOSI |
| CS | PE7 / GPIO |
| RST | PE8 / GPIO |
| DC | PE9 / GPIO |

### 3.3 ADC 포텐셔미터

| 입력 | STM32 연결 |
| --- | --- |
| Pot 1 | PA0 / ADC1_INP16 |
| Pot 2 | PA1 / ADC1_INP17 |
| Pot 3 | PA2 / ADC1_INP14 |

확장 후보: PA3, PA4, PA5, PA6, PA7

### 3.4 SD 카드

| SD 신호 | STM32 연결 |
| --- | --- |
| D0 | PC8 / SDMMC1_D0 |
| D1 | PC9 / SDMMC1_D1 |
| D2 | PC10 / SDMMC1_D2 |
| D3 | PC11 / SDMMC1_D3 |
| CLK | PC12 / SDMMC1_CK |
| CMD | PD2 / SDMMC1_CMD |
| Detect | PD4 / GPIO input |

SD카드 모듈은 보드에 내장된 모듈을 사용한다.

### 3.5 KY-040

| KY-040 신호 | 연결 |
| --- | --- |
| CLK / A | PD12 / TIM4_CH1 |
| DT / B | PD13 / TIM4_CH2 |
| SW | MCP23017 U1 GPIOB3 |

### 3.6 SAI1 오디오

SAI1의 sample rate, slot width, 내부 sample 변환 정책은 [audio_data_format.md의 4. SAI 입출력 변환 정책](../audio/audio_data_format.md#4-sai-입출력-변환-정책)을 기준으로 한다. 이 절은 물리 결선만 정리한다.

| 오디오 신호 | STM32 연결 | 연결 대상 |
| --- | --- | --- |
| BCK / SCK | PE5 / SAI1_SCK_A | INMP441, PCM5102A |
| LCK / WS | PE4 / SAI1_FS_A | INMP441, PCM5102A |
| DAC data | PE6 / SAI1_SD_A | PCM5102A DIN |
| MIC data | PE3 / SAI1_SD_B | INMP441 SD |

INMP441:

| INMP441 핀 | 연결 |
| --- | --- |
| SCK | PE5 |
| WS | PE4 |
| SD | PE3 |
| L/R | GND 또는 3.3 V |

PCM5102A:

| PCM5102A 핀 | 연결 |
| --- | --- |
| BCK | PE5 |
| LCK / LRCK | PE4 |
| DIN | PE6 |
| XSMT | 3.3 V |
| SCK | GND 또는 모듈 기본 상태 |
| FMT / FLT / DEMP | GND 또는 모듈 기본 상태 |

## 4. 전원 및 신호 주의사항

- 모든 GND는 공통으로 묶는다.
- I2C pull-up은 3.3 V 기준으로 둔다.
- STM32 GPIO에 5 V pull-up 신호가 직접 들어가지 않게 한다.
- ADC 입력에는 3.3 V를 넘는 전압을 넣지 않는다.
- PCM5102A는 `XSMT`가 3.3 V에 연결되어야 출력된다.
- SAI1 MCLK는 현재 사용하지 않는다.

## 5. 미정 사항

- TODO: 최종 PCB 또는 커넥터 핀맵
