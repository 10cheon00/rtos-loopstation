# 주변장치 연결 계획

이 문서는 STM32H743VIT6, LQFP100 패키지 기준의 1차 배선 계획이다. 이전 문서의 STM32 연결 컬럼은 잘못된 핀아웃을 바탕으로 작성되었으므로 사용하지 않는다. 아래 연결은 `docs/stm32h743vi.pdf`의 STM32H742/H743 데이터시트 DS12110 Rev 11, Table 9 pin/ball definition과 Table 10 alternate functions를 기준으로 다시 정리했다.

## 선택 규칙

- 현재 `.ioc`에서 이미 사용하는 `SDMMC1`, `SWD`, SD detect 핀은 고정한다.
- 루프스테이션의 핵심 기능인 오디오 입출력은 `SAI1`으로 먼저 확보한다.
- LCD는 ST7565R 계열 쓰기 전용 SPI 장치로 보고 `MISO`를 쓰지 않는다.
- 외부 SPI ADC는 이번 배선 계획에서 제외한다.
- 버튼/LED 확장은 `I2C1` 기반 MCP23017로 묶어 GPIO 소모를 줄인다.
- 아날로그 입력은 지금 3개만 쓰더라도 향후 7개 이상 확장할 수 있도록 같은 ADC 인스턴스의 연속적인 핀 그룹을 우선 확보한다.
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
| LCD SPI 클럭 | SPI2_SCK | PB10 |
| LCD SPI 데이터 | SPI2_MOSI | PC1 |
| LCD 제어선 | GPIO 출력 | PE7, PE8, PE9 |
| MCP23017 버스 | I2C1 | PB6, PB7 |
| 로터리 엔코더 A/B | TIM4 encoder mode | PD12, PD13 |

## 주변장치 요약

| 모듈 | 사용할 주변장치 | STM32H743VI 핀 | 결정 |
| --- | --- | --- | --- |
| GMG12864-06D LCD, ST7565R | SPI2 송신 전용 + GPIO | PB10, PC1, PE7, PE8, PE9, PE10 | 사용 |
| MCP23017 x2 | I2C1 | PB6, PB7 | 사용 |
| PCM5102 DAC + INMP441 마이크 | SAI1 Block A/B | PE3, PE4, PE5, PE6, 선택적으로 PE2 | 사용 |
| KY-040 엔코더 | TIM4 encoder + MCP23017 GPIO | PD12, PD13, MCP23017 `0x24` GPIOB3 | 사용 |
| ADC 입력 x3, 향후 x7 이상 | ADC1 scan + DMA | PA0, PA1, PA2, 예비 PA3~PA7 | 사용 예정 |
| MCP23017 인터럽트 라인 | GPIO/EXTI | PC0 | 선택 사항 |

## LCD용 SPI 선택

LCD는 `SPI2`를 송신 전용 마스터 모드로 사용한다.

| LCD 신호 | STM32H743VI 핀 | STM32 기능 |
| --- | --- | --- |
| LCD SCL / SCK | PB10 | SPI2_SCK |
| LCD SI / MOSI | PC1 | SPI2_MOSI |
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
| SI | PC1 | SPI2_MOSI | STM32에서 LCD로 전송 |
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
| A2/A1/A0, U1 | 3.3 V/GND/GND | 주소 설정 | 7-bit 주소 `0x24`, 사용자 표기 `100` |
| A2/A1/A0, U2 | 3.3 V/GND/3.3 V | 주소 설정 | 7-bit 주소 `0x25`, 사용자 표기 `101` |
| U1 GPIOB3 | KY-040 SW | GPIO input | 엔코더 푸시 스위치, pull-up, active-low |
| INTA/INTB, 선택 사항 | PC0 | GPIO_EXTI | 인터럽트 기반 입력 처리가 필요할 때만 사용 |

MCP23017 모듈 보드에 `CS`, `SO`처럼 SPI용으로 보이는 핀이 있어도, I2C 모드에서는 연결하지 않는다.

## ADC 아날로그 입력 확장 계획

ADC 입력은 `ADC1` 하나를 기준으로 regular scan + DMA로 묶는다. 지금 필요한 3개 입력은 `ADC1`의 PA0~PA2에 배치하고, 향후 7개 이상이 필요해지면 같은 포트 A의 인접 핀을 순서대로 확장한다.

이 선택의 핵심은 다음과 같다.

- `PA0~PA7`은 현재 `.ioc`에서 `SDMMC1`, `SPI2`, `I2C1`, `TIM4`, `SWD`, LCD GPIO와 충돌하지 않는다.
- 하나의 ADC 인스턴스에서 scan sequence를 늘리는 방식이므로, 입력 3개에서 7개 이상으로 늘려도 코드 구조가 크게 바뀌지 않는다.
- 포트 A의 연속 핀을 아날로그 입력 블록으로 예약하면 배선과 커넥터 배치가 단순해진다.
- `PC1`은 현재 LCD `SPI2_MOSI`로 사용 중이므로 ADC 후보에서 제외한다.
- `PC0`은 ADC 입력으로도 쓸 수 있지만 MCP23017 인터럽트 예비선으로 남긴다.

권장 시작 배치는 다음과 같다.

| 순번 | STM32H743VI 핀 | CubeMX 신호 | 용도 |
| --- | --- | --- | --- |
| ADC 입력 1 | PA0 | ADC1_INP16 | 즉시 사용 |
| ADC 입력 2 | PA1 | ADC1_INP17 | 즉시 사용 |
| ADC 입력 3 | PA2 | ADC1_INP14 | 즉시 사용 |

향후 확장용으로 다음 핀을 같은 ADC1 scan sequence에 추가한다.

| 확장 순번 | STM32H743VI 핀 | CubeMX 신호 | 비고 |
| --- | --- | --- | --- |
| ADC 입력 4 | PA3 | ADC1_INP15 | 1차 확장 |
| ADC 입력 5 | PA4 | ADC1_INP18 | 1차 확장 |
| ADC 입력 6 | PA5 | ADC1_INP19 | 1차 확장 |
| ADC 입력 7 | PA6 | ADC1_INP3 | 1차 확장 |
| ADC 입력 8 | PA7 | ADC1_INP7 | 2차 확장 |

PA0~PA7을 모두 사용한 뒤에도 입력이 더 필요하면 아래 후보를 검토한다.

| 후보 | STM32H743VI 핀 | CubeMX 신호 | 주의점 |
| --- | --- | --- | --- |
| 추가 후보 1 | PC4 | ADC1_INP4 또는 ADC2_INP4 | SDMMC와 충돌하지 않음 |
| 추가 후보 2 | PC5 | ADC1_INP8 또는 ADC2_INP8 | SDMMC와 충돌하지 않음 |
| 추가 후보 3 | PB0 | ADC1_INP9 또는 ADC2_INP9 | Port B 사용량 증가 |
| 추가 후보 4 | PB1 | ADC1_INP5 또는 ADC2_INP5 | Port B 사용량 증가 |

ADC 설정 방향은 다음과 같다.

| 항목 | 권장 설정 |
| --- | --- |
| ADC 인스턴스 | `ADC1` |
| 변환 방식 | regular scan sequence |
| 데이터 이동 | DMA circular buffer |
| 트리거 | 우선 software start 또는 timer trigger, 이후 샘플링 주기 확정 시 타이머 트리거 |
| 샘플링 시간 | 소스 임피던스가 높으면 길게 설정 |
| 기준 전압 | `VDDA/VREF+` 노이즈와 디커플링 확인 |

PA5~PA7은 `SPI1` 후보 핀이기도 하다. 앞으로 고속 SPI 센서나 외부 SPI ADC를 다시 추가할 가능성이 커지면, PA5~PA7을 모두 ADC로 확정하기 전에 SPI1 사용 여부를 다시 판단한다. 현재 계획에서는 외부 SPI ADC를 제외했고 LCD도 `SPI2`를 사용하므로 ADC 확장 블록으로 예약한다.

## PCM5102 DAC 및 INMP441 마이크

오디오는 `SAI1`을 사용한다. DAC 출력과 마이크 입력을 하나의 오디오 주변장치 그룹에 묶어 LCD/SPI 선택이 오디오 핀을 침범하지 않도록 한다.

| 오디오 신호 | STM32H743VI 연결 | STM32 기능 | 연결 모듈 |
| --- | --- | --- | --- |
| BCK | PE5 | SAI1_SCK_A | PCM5102와 INMP441 공유 |
| LCK | PE4 | SAI1_FS_A | PCM5102와 INMP441 공유 |
| DAC 데이터 출력 | PE6 | SAI1_SD_A | STM32에서 PCM5102 DIN으로 출력 |
| 마이크 데이터 입력 | PE3 | SAI1_SD_B | INMP441 SD에서 STM32로 입력 |

권장 SAI 구성은 다음과 같다.

| 블록 | 역할 |
| --- | --- |
| SAI1 Block A | PCM5102용 마스터 송신 |
| SAI1 Block B | INMP441용 동기 수신 |

PCM5102 모듈 우선 연결:

| PCM5102 모듈 핀 | 연결 | 비고 |
| --- | --- | --- |
| VCC | 3.3 V 또는 모듈 지원 전원 | 모듈 보드에 레귤레이터가 있는지 확인 |
| GND | GND | 디지털 공통 접지 |
| BCK | PE5 | SAI1_SCK_A |
| LCK | PE4 | SAI1_FS_A |
| DIN | PE6 | SAI1_SD_A |
| SCK | GND | PCM5102의 system clock 입력. MCLK 없이 내부 PLL을 쓰는 3-wire I2S 구성에서는 떠 있게 두지 않고 GND로 고정 |
| XSMT | 3.3 V | 음소거 해제 |
| FMT | GND | I2S 입력 형식 선택 |
| FLT | GND | normal latency filter 선택 |
| DEMP | GND | de-emphasis 비활성화 |
| A3V3 | 연결하지 않음 | 모듈의 아날로그 3.3 V 출력/노드로 보이면 외부 전원 입력으로 쓰지 않음 |
| ROUT | 헤드폰 앰프 또는 라인 입력 R | PCM5102 오른쪽 아날로그 출력 |
| LOUT | 헤드폰 앰프 또는 라인 입력 L | PCM5102 왼쪽 아날로그 출력 |
| AGND | 헤드폰 앰프 또는 라인 입력 GND | 아날로그 출력 기준 접지 |

PCM5102의 실제 I2S 입력은 `BCK`, `LCK`, `DIN` 세 신호다. `SCK`, `FMT`, `FLT`, `DEMP`, `XSMT`는 데이터 전송선이 아니라 동작 모드를 정하는 제어 핀이므로 floating 상태로 두지 않는다.

PCM5102 단독 출력 테스트가 끝난 뒤 INMP441을 추가한다. 이때 INMP441은 `PE5`의 BCK와 `PE4`의 LCK를 PCM5102와 공유하고, 마이크 데이터선만 `PE3`의 `SAI1_SD_B`로 입력한다.

## KY-040 로터리 엔코더

엔코더 A/B는 `TIM4` encoder mode로 처리한다. 푸시 스위치는 이미 쓰는 MCP23017 입력 확장 쪽으로 보내 MCU GPIO를 추가 소모하지 않는다.

| KY-040 핀 | STM32H743VI 연결 | STM32 기능 | 비고 |
| --- | --- | --- | --- |
| + | 3.3 V | 전원 | 3.3 V 사용 |
| GND | GND | 접지 | 공통 접지 |
| CLK / A | PD12 | TIM4_CH1 | encoder mode |
| DT / B | PD13 | TIM4_CH2 | encoder mode |
| SW | MCP23017 `0x24` GPIOB3 | I2C GPIO input | pull-up, active-low |

PD12/PD13은 현재 `SDMMC1`, `SWD`, `I2C1`, `SPI2`, LCD GPIO와 겹치지 않는다. 엔코더 스위치는 MCP23017의 `A0~A2 = 100` 장치, 7-bit 주소 `0x24`의 Port B bit 3에 연결한다.

## 핀 할당 요약

| STM32H743VI 핀 | 할당 용도 |
| --- | --- |
| PA13 | SWDIO |
| PA14 | SWCLK |
| PA0 | ADC1_INP16, 아날로그 입력 1 |
| PA1 | ADC1_INP17, 아날로그 입력 2 |
| PA2 | ADC1_INP14, 아날로그 입력 3 |
| PA3 | ADC1_INP15, 아날로그 입력 확장 |
| PA4 | ADC1_INP18, 아날로그 입력 확장 |
| PA5 | ADC1_INP19, 아날로그 입력 확장 |
| PA6 | ADC1_INP3, 아날로그 입력 확장 |
| PA7 | ADC1_INP7, 아날로그 입력 확장 |
| PB6 | I2C1_SCL |
| PB7 | I2C1_SDA |
| PB10 | LCD SPI2_SCK |
| PC1 | LCD SPI2_MOSI |
| PC0 | MCP23017 인터럽트, 선택 사항 |
| PC4 | ADC 추가 후보 |
| PC5 | ADC 추가 후보 |
| PC8 | SDMMC1_D0 |
| PC9 | SDMMC1_D1 |
| PC10 | SDMMC1_D2 |
| PC11 | SDMMC1_D3 |
| PC12 | SDMMC1_CK |
| PD2 | SDMMC1_CMD |
| PD4 | SD 카드 감지 |
| PD12 | 엔코더 A, TIM4_CH1 |
| PD13 | 엔코더 B, TIM4_CH2 |
| PE2 | SAI1_MCLK_A, 선택 사항 |
| PE3 | INMP441 데이터, SAI1_SD_B |
| PE4 | 오디오 LCK, SAI1_FS_A |
| PE5 | 오디오 BCK, SAI1_SCK_A |
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
- ADC 입력으로 쓰는 핀에는 3.3 V를 넘는 전압을 직접 넣지 않는다. 포텐셔미터나 센서 출력이 5 V 범위라면 분압 또는 버퍼 회로가 필요하다.
- ADC 입력은 디지털 신호선, LCD SPI, SDMMC 클럭과 최대한 멀리 배선하고, 필요하면 RC 저역통과 필터와 별도 아날로그 그라운드 배치를 검토한다.

## 참고 자료

- STMicroelectronics, `STM32H742xI/G STM32H743xI/G` datasheet DS12110 Rev 11, January 2026: `docs/stm32h743vi.pdf`
- STM32CubeMX MCU database: `STM32H743VITx.xml`
- 현재 CubeMX 프로젝트: `rtos-loopstation.ioc`
