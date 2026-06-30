# 하드웨어 결선 기준

이 문서는 STM32H743VIT6 보드에 현재 연결한 모듈과 핀 배치를 다시 구성할 때 참고하기 위한 결선 기준이다. STM32 핀과 peripheral 설정은 `rtos-loopstation.ioc`를 우선 기준으로 삼고, 모듈 전원핀, 주소핀, 제어핀처럼 `.ioc`에 나타나지 않는 결선은 실제 테스트 과정에서 확인한 내용을 함께 적는다.

## 사용 모듈

| 모듈 | 용도 | STM32 peripheral | 현재 상태 |
| --- | --- | --- | --- |
| MCP23017 x2 | 버튼, LED용 GPIO 확장 | I2C1 | 입력/출력 polling 테스트 완료 |
| GMG12864-06D LCD | 128x64 그래픽 LCD | SPI2 + GPIO | u8g2 출력 확인 |
| 포텐셔미터 x3 | 아날로그 입력 | ADC1 | 3채널 표시 테스트 완료, 7~8채널 확장 예정 |
| SD 카드 | 저장장치 | SDMMC1 + FatFs + GPIO | 카드 인식 및 파일 입출력 확인 |
| KY-040 | 로터리 엔코더 | TIM4 encoder mode + MCP23017 GPIO | CW/CCW 회전 인식 확인 |
| INMP441 | I2S 디지털 마이크 | SAI1 Block B | SAI 수신 입력으로 사용 |
| UDA1334A | I2S DAC 출력 | SAI1 Block A | INMP441 입력 passthrough 확인 |
| PCM5102A | I2S DAC 출력 후보 | SAI1 Block A | XSMT high 조건에서 출력 확인, 향후 UDA1334A 대체 후보 |

## MCP23017 GPIO 확장

두 개의 MCP23017은 `I2C1` 버스에 함께 연결한다. I2C 주소는 모듈의 `A2/A1/A0` 핀으로 구분한다.

### STM32 연결

| MCP23017 핀 | STM32H743VIT6 연결 | STM32 기능 | 비고 |
| --- | --- | --- | --- |
| SCL | PB6 | I2C1_SCL | 3.3 V pull-up 필요 |
| SDA | PB7 | I2C1_SDA | 3.3 V pull-up 필요 |
| VDD | 3.3 V | 전원 | MCU와 같은 로직 전압 |
| VSS / GND | GND | 접지 | 공통 접지 |
| RESET | 3.3 V | 리셋 해제 | pull-up 권장 |

### 주소 및 GPIO 사용

| 장치 | A2/A1/A0 | 7-bit 주소 | 사용 GPIO |
| --- | --- | --- | --- |
| MCP23017 U1 | `100` | `0x24` | GPIOB3: KY-040 SW, GPIOB4: tact switch, GPIOB5: B4 상태 출력, GPIOB6/B7: tact switch |
| MCP23017 U2 | `101` | `0x25` | GPIOB2~B6: tact switch, GPIOB7: B6 상태 출력 |

MCP23017 입력은 내부 pull-up 또는 외부 pull-up을 사용하고, 버튼은 active-low 기준으로 처리한다.

## GMG12864-06D LCD

GMG12864-06D LCD는 ST7565R 계열로 보고 `SPI2` 송신 전용과 GPIO 제어선을 사용한다. 표시 제어는 u8g2 라이브러리로 수행한다.

| LCD 핀 | STM32H743VIT6 연결 | STM32 기능 | 비고 |
| --- | --- | --- | --- |
| SCL | PB10 | SPI2_SCK | LCD SPI clock |
| SI | PC1 | SPI2_MOSI | STM32에서 LCD로 전송 |
| CS | PE7 | GPIO output, `GMG12864_CS` | 소프트웨어 chip select |
| RSE / RST | PE8 | GPIO output, `GMG12864_RST` | LCD reset |
| RS / DC | PE9 | GPIO output, `GMG12864_DC` | command/data 선택 |
| VDD | 3.3 V | 전원 | 3.3 V 로직 |
| VSS | GND | 접지 | 공통 접지 |
| A / K | A: 3.3 V, K: GND | 백라이트 | 필요 시 저항 또는 스위칭 회로 사용 |

`SPI2`는 `.ioc`에서 master, transmit-only, 8-bit로 설정되어 있다.

## ADC1 포텐셔미터 입력

현재 포텐셔미터 3개를 `ADC1`에 연결했다. 향후 7~8개까지 확장할 수 있도록 `PA0~PA7`을 우선 아날로그 입력 블록으로 사용한다.

### 현재 연결

| 입력 | STM32H743VIT6 핀 | CubeMX 신호 | 비고 |
| --- | --- | --- | --- |
| Pot 1 | PA0 | ADC1_INP16 | 현재 사용 |
| Pot 2 | PA1 | ADC1_INP17 | 현재 사용 |
| Pot 3 | PA2 | ADC1_INP14 | 현재 사용 |

### 확장 후보

| 확장 입력 | STM32H743VIT6 핀 | ADC 후보 신호 | 비고 |
| --- | --- | --- | --- |
| Pot 4 | PA3 | ADC1_INP15 | 1차 확장 |
| Pot 5 | PA4 | ADC1_INP18 | 1차 확장 |
| Pot 6 | PA5 | ADC1_INP19 | 1차 확장 |
| Pot 7 | PA6 | ADC1_INP3 | 1차 확장 |
| Pot 8 | PA7 | ADC1_INP7 | 2차 확장 |

포텐셔미터 양 끝은 `3.3 V`와 `GND`에 연결하고, wiper를 ADC 핀에 연결한다. ADC 입력에는 3.3 V를 넘는 전압을 넣지 않는다.

## SD 카드

SD 카드는 `SDMMC1` 4-bit bus로 연결하며, FatFs를 사용한다.

| SD 카드 신호 | STM32H743VIT6 핀 | STM32 기능 | 비고 |
| --- | --- | --- | --- |
| D0 | PC8 | SDMMC1_D0 | pull-up |
| D1 | PC9 | SDMMC1_D1 | 4-bit bus |
| D2 | PC10 | SDMMC1_D2 | 4-bit bus |
| D3 | PC11 | SDMMC1_D3 | 4-bit bus |
| CLK | PC12 | SDMMC1_CK | clock |
| CMD | PD2 | SDMMC1_CMD | pull-up |
| Detect | PD4 | GPIO input, `SDMMC_Detect` | pull-up, card detect |
| VCC | 모듈 사양에 맞게 연결 | 전원 | 레벨 변환/레귤레이터 유무 확인 |
| GND | GND | 접지 | 공통 접지 |

## KY-040 로터리 엔코더

KY-040의 A/B 회전 신호는 `TIM4` encoder mode로 처리한다. 푸시 스위치는 MCU GPIO를 직접 쓰지 않고 MCP23017 U1에 연결한다.

| KY-040 핀 | 연결 | 기능 | 비고 |
| --- | --- | --- | --- |
| CLK / A | PD12 | TIM4_CH1 | encoder mode |
| DT / B | PD13 | TIM4_CH2 | encoder mode |
| SW | MCP23017 U1 GPIOB3 | GPIO input | active-low, pull-up |
| + | 3.3 V | 전원 | 3.3 V 사용 |
| GND | GND | 접지 | 공통 접지 |

## SAI1 오디오 입출력

오디오 입출력은 `SAI1`에 묶는다. `SAI1 Block A`는 DAC로 송신하는 master transmit이고, `SAI1 Block B`는 INMP441에서 수신하는 synchronous slave receive이다.

| 오디오 신호 | STM32H743VIT6 핀 | STM32 기능 | 연결 대상 |
| --- | --- | --- | --- |
| BCK / SCK | PE5 | SAI1_SCK_A | INMP441 SCK, DAC BCK |
| LCK / LRCK / WS | PE4 | SAI1_FS_A | INMP441 WS, DAC LCK/LRCK |
| DAC data | PE6 | SAI1_SD_A | UDA1334A DIN 또는 PCM5102A DIN |
| MIC data | PE3 | SAI1_SD_B | INMP441 SD |

`.ioc` 기준 SAI1 clock은 PLL3에서 공급하고, `SAI1 RealAudioFreq`는 48.0 kHz로 맞춘다. 현재 MCLK는 사용하지 않는다.

### INMP441 연결

| INMP441 핀 | 연결 | 비고 |
| --- | --- | --- |
| VDD | 3.3 V | 3.3 V 사용 |
| GND | GND | 공통 접지 |
| SCK | PE5 / SAI1_SCK_A | bit clock |
| WS | PE4 / SAI1_FS_A | word select |
| SD | PE3 / SAI1_SD_B | mic data |
| L/R | GND 또는 3.3 V | GND: left slot, 3.3 V: right slot |

### UDA1334A 연결

UDA1334A는 현재 정상 출력이 확인된 DAC 모듈이다.

| UDA1334A 핀 | 연결 | 비고 |
| --- | --- | --- |
| VIN | 3.3 V 또는 모듈 지원 전원 | 모듈 사양 확인 |
| GND | GND | 공통 접지 |
| BCK | PE5 / SAI1_SCK_A | bit clock |
| LCK | PE4 / SAI1_FS_A | LR clock |
| DIN | PE6 / SAI1_SD_A | DAC data |

### PCM5102A 연결

PCM5102A는 UDA1334A 대체 후보로 테스트했다. `XSMT`를 3.3 V에 연결해야 출력이 나온다.

| PCM5102A 핀 | 연결 | 비고 |
| --- | --- | --- |
| VCC | 3.3 V 또는 모듈 지원 전원 | 모듈 사양 확인 |
| GND | GND | 디지털 공통 접지 |
| BCK | PE5 / SAI1_SCK_A | bit clock |
| LCK / LRCK | PE4 / SAI1_FS_A | word select |
| DIN | PE6 / SAI1_SD_A | DAC data |
| XSMT | 3.3 V | mute 해제, 필수 |
| SCK | GND 또는 모듈 기본 상태 | MCLK 미사용 구성 |
| FMT | GND 또는 모듈 기본 상태 | I2S format |
| FLT | GND 또는 모듈 기본 상태 | filter mode |
| DEMP | GND 또는 모듈 기본 상태 | de-emphasis disable |
| LOUT | 라인 출력 L | 별도 잭 사용 시 tip |
| ROUT | 라인 출력 R | 별도 잭 사용 시 ring |
| AGND | 라인 출력 GND | 별도 잭 사용 시 sleeve |
| A3V3 | 연결하지 않음 | 모듈 내부 아날로그 전원 노드로 취급 |

## 디버그 및 공통 핀

| 용도 | STM32H743VIT6 핀 | 비고 |
| --- | --- | --- |
| SWDIO | PA13 | 디버그 |
| SWCLK | PA14 | 디버그 |
| 공통 GND | GND | 모든 모듈 GND 공통 |
| 로직 전원 | 3.3 V | 5 V pull-up 모듈은 직접 연결하지 않음 |

## 핀 사용 요약

| STM32H743VIT6 핀 | 현재 용도 |
| --- | --- |
| PA0 | ADC1_INP16, Pot 1 |
| PA1 | ADC1_INP17, Pot 2 |
| PA2 | ADC1_INP14, Pot 3 |
| PA13 | SWDIO |
| PA14 | SWCLK |
| PB6 | I2C1_SCL, MCP23017 |
| PB7 | I2C1_SDA, MCP23017 |
| PB10 | SPI2_SCK, LCD |
| PC1 | SPI2_MOSI, LCD |
| PC8 | SDMMC1_D0 |
| PC9 | SDMMC1_D1 |
| PC10 | SDMMC1_D2 |
| PC11 | SDMMC1_D3 |
| PC12 | SDMMC1_CK |
| PD2 | SDMMC1_CMD |
| PD4 | SD card detect |
| PD12 | TIM4_CH1, encoder A |
| PD13 | TIM4_CH2, encoder B |
| PE3 | SAI1_SD_B, INMP441 data |
| PE4 | SAI1_FS_A, audio LCK/WS |
| PE5 | SAI1_SCK_A, audio BCK/SCK |
| PE6 | SAI1_SD_A, DAC data |
| PE7 | LCD CS |
| PE8 | LCD reset |
| PE9 | LCD DC |

## 배선 체크리스트

- 모든 모듈의 GND를 STM32 보드 GND와 공통으로 묶는다.
- STM32 GPIO에 5 V pull-up 신호가 직접 들어가지 않게 한다.
- I2C pull-up은 3.3 V 기준으로 둔다.
- PCM5102A는 `XSMT`를 3.3 V에 연결해야 출력된다.
- INMP441의 `L/R` 상태와 코드의 수신 slot 선택이 일치해야 한다.
- SDMMC, SAI, SPI 클럭선은 가능하면 아날로그 입력선과 떨어뜨려 배선한다.
