# Peripheral connection plan

이 문서는 STM32H743VIT6(LQFP100) 보드에 모듈을 바로 납땜/장착하기 전에 브레드보드에서 검증할 1차 배선 계획이다.

현재 `rtos-loopstation.ioc`에서 이미 사용하는 핀은 다음과 같으므로 아래 배선에서는 제외했다.

| 용도 | STM32 핀 |
| --- | --- |
| SWD 디버그 | PA13, PA14 |
| SDMMC1 | PC8, PC9, PC10, PC11, PC12, PD2 |
| SD 카드 detect | PD4 |

## Peripheral summary

| 용도 | STM32 peripheral | STM32 핀 |
| --- | --- | --- |
| GMG12864-06D LCD | GPIO software SPI | PE7(CS), PE8(RSE), PE9(RS), PE10(SCL), PE11(SI) |
| MCP23017 x2 | I2C1 | PB6(SCL), PB7(SDA) |
| PCM5102 출력, INMP441 입력 | SAI1 | PE4(FS/LRCK/WS), PE5(SCK/BCLK), PE6(SD_A), PE3(SD_B) |
| KY-040 encoder | GPIO/EXTI | PB12, PB13, PB14 |
| 보조 GPIO | GPIO | PE12, PC0, PC1, PA0, PA1 |

## GMG12864-06D LCD

GMG12864-06D 모듈은 ST7565 계열 128x64 LCD로 보고 4-wire software SPI 방식으로 연결한다. 사용자가 확인한 모듈 핀 순서는 `CS, RSE, RS, SCL, SI, VDD, VSS, A, K, IC_SCL, IC_CS, IC_SO, IC_SI`이다.

CubeMX에서는 아래 STM32 핀을 모두 `GPIO_Output`으로 등록한다. `PE7~PE12`는 현재 계획의 SDMMC, SWD, I2C1, SAI1 오디오 핀과 겹치지 않고 한 포트에 모여 있어 LCD용으로 관리하기 쉽다.

| 순서 | GMG12864-06D 핀 | STM32 연결 | STM32 기능 | 비고 |
| ---: | --- | --- | --- | --- |
| 1 | CS | PE7 | GPIO_Output | LCD chip select |
| 2 | RSE | PE8 | GPIO_Output | reset |
| 3 | RS | PE9 | GPIO_Output | command/data 선택 |
| 4 | SCL | PE10 | GPIO_Output | software SPI clock |
| 5 | SI | PE11 | GPIO_Output | software SPI data, STM32 -> LCD |
| 6 | VDD | 3.3V | 전원 | 3.3 V 로직 기준 |
| 7 | VSS | GND | 접지 | 공통 접지 |
| 8 | A | 3.3V 또는 PE12 경유 | backlight anode | 저항 또는 트랜지스터 사용 권장 |
| 9 | K | GND | backlight cathode | 모듈 회로 확인 |
| 10 | IC_SCL | 미연결 | - | 부가 IC 인터페이스로 추정, LCD 표시 검증 단계에서는 사용하지 않음 |
| 11 | IC_CS | 미연결 | - | 부가 IC 인터페이스로 추정 |
| 12 | IC_SO | 미연결 | - | 부가 IC 인터페이스로 추정 |
| 13 | IC_SI | 미연결 | - | 부가 IC 인터페이스로 추정 |

## MCP23017 x2

두 MCP23017은 같은 I2C 버스를 공유하고, `A0~A2` 주소 핀만 다르게 묶는다. 모듈에 `NC/CS`, `NC/SO` 핀이 있으면 SPI형 MCP23S17 겸용 보드의 핀으로 보고, MCP23017 I2C 사용 시에는 연결하지 않는다.

| MCP23017 모듈 핀 | STM32 연결 | STM32 기능 | 비고 |
| --- | --- | --- | --- |
| VDD | 3.3V | 전원 | STM32와 같은 전압 |
| VSS / GND | GND | 접지 | 공통 접지 |
| SCL | PB6 | I2C1_SCL | 4.7 kOhm 정도 pull-up 권장 |
| SDA | PB7 | I2C1_SDA | 4.7 kOhm 정도 pull-up 권장 |
| NC/CS | 미연결 | - | MCP23017 I2C 모드에서는 chip select를 사용하지 않음 |
| NC/SO | 미연결 | - | MCP23017 I2C 모드에서는 SPI serial out을 사용하지 않음 |
| RESET | 3.3V | reset 비활성 | 10 kOhm pull-up 권장, 필요 시 GPIO로 변경 |
| A0/A1/A2 - U1 | GND/GND/GND | 주소 설정 | 7-bit I2C 주소 `0x20` |
| A0/A1/A2 - U2 | 3.3V/GND/GND | 주소 설정 | 7-bit I2C 주소 `0x21` |
| INTA/INTB - U1 | PC0 / PC1 | GPIO_EXTI 입력 | 선택 사항 |
| INTA/INTB - U2 | PA0 / PA1 | GPIO_EXTI 입력 | 선택 사항 |
| GPA0~GPB7 | 버튼/LED 등 | 확장 GPIO | 입력은 pull-up 정책 별도 결정 |

## MCP3008 ADC

MCP3008을 계속 사용한다면 별도 SPI peripheral 또는 STM32 내장 ADC 사용을 다시 검토한다. LCD는 GPIO software SPI로 분리했으므로 SPI1을 LCD가 점유하지 않는다.

| MCP3008 핀 | STM32 연결 | STM32 기능 | 비고 |
| --- | --- | --- | --- |
| VDD | 3.3V | 전원 | 입력 범위도 3.3 V 이하 |
| VREF | 3.3V | ADC 기준전압 | 더 낮은 기준전압을 쓰면 입력 범위도 낮아짐 |
| AGND | GND | 아날로그 접지 | DGND와 공통 접지 |
| DGND | GND | 디지털 접지 | AGND와 공통 접지 |
| CLK | PA5 | SPI1_SCK | LCD와 분리됨, MCP3008을 유지할 때만 사용 |
| DOUT | PA6 | SPI1_MISO | ADC -> STM32 |
| DIN | PA7 | SPI1_MOSI | STM32 -> ADC |
| CS / SHDN | PB0 | GPIO_Output | ADC chip select |
| CH0~CH7 | 아날로그 입력 | ADC 입력 | 노브/센서 출력은 0~3.3 V 범위로 제한 |

## KY-040 rotary encoder

KY-040 보드에 이미 pull-up 저항이 있는 경우가 많지만, 모듈별 차이가 있으므로 STM32 내부 pull-up도 켜는 쪽으로 시작한다.

| KY-040 핀 | STM32 연결 | STM32 기능 | 비고 |
| --- | --- | --- | --- |
| + | 3.3V | 전원 | 5 V 대신 3.3 V 사용 |
| GND | GND | 접지 | 공통 접지 |
| CLK / A | PB12 | GPIO_EXTI 입력 | 내부 pull-up |
| DT / B | PB13 | GPIO_EXTI 입력 | 내부 pull-up |
| SW | PB14 | GPIO_EXTI 입력 | 내부 pull-up |

## PCM5102 DAC

PCM5102는 I2S 형식의 오디오 DAC이다. STM32 쪽은 SAI1 Block A를 master transmit로 쓰는 구성이 적합하다.

| PCM5102 모듈 핀 | STM32 연결 | STM32 기능 | 비고 |
| --- | --- | --- | --- |
| VIN / VCC | 3.3V | 전원 | 모듈이 5 V 입력 레귤레이터형인지 확인 |
| GND | GND | 접지 | 공통 접지 |
| BCK / BCLK | PE5 | SAI1_SCK_A | INMP441와 clock 공유 |
| LCK / LRCK / WS | PE4 | SAI1_FS_A | INMP441와 frame sync 공유 |
| DIN | PE6 | SAI1_SD_A | STM32 -> DAC |
| SCK | 미연결 | - | 대부분 PCM5102 모듈은 system clock 없이 동작 가능 |
| XSMT | 3.3V | mute 해제 | 10 kOhm pull-up 권장 |
| FLT | GND 또는 3.3V | 필터 선택 | 모듈 기본값 우선 |
| DMP | GND 또는 3.3V | de-emphasis | 보통 GND |
| FMT | GND 또는 3.3V | I2S format | 모듈 기본 I2S 설정 확인 |

## INMP441 MEMS microphone

INMP441은 I2S 출력 마이크이다. PCM5102와 같은 SAI1 clock/frame을 공유하고, SAI1 Block B를 synchronous receive로 둔다.

| INMP441 모듈 핀 | STM32 연결 | STM32 기능 | 비고 |
| --- | --- | --- | --- |
| VDD | 3.3V | 전원 | 3.3 V 사용 |
| GND | GND | 접지 | 공통 접지 |
| SCK / BCLK | PE5 | SAI1_SCK_A 공유 | STM32 master clock |
| WS / LRCK | PE4 | SAI1_FS_A 공유 | frame sync |
| SD | PE3 | SAI1_SD_B | mic -> STM32 |
| L/R | GND 또는 3.3V | 채널 선택 | GND=left, 3.3V=right로 시작 |

## LQFP100 package pin cross-reference

실제 개발보드 커넥터 실크가 MCU 핀명을 그대로 노출하지 않을 수 있으므로, 보드 회로도에서 아래 MCU 핀명과 커넥터 위치를 다시 대조한다.

| STM32 핀 | LQFP100 핀 번호 | 이번 계획의 용도 |
| --- | ---: | --- |
| PE2 | 1 | 예비: SAI1_MCLK_A 가능 |
| PE3 | 2 | INMP441 SD, SAI1_SD_B |
| PE4 | 3 | SAI1_FS_A |
| PE5 | 4 | SAI1_SCK_A |
| PE6 | 5 | PCM5102 DIN, SAI1_SD_A |
| PC0 | 15 | MCP23017 U1 INTA |
| PC1 | 16 | MCP23017 U1 INTB |
| PA0 | 22 | MCP23017 U2 INTA |
| PA1 | 23 | MCP23017 U2 INTB |
| PA5 | 29 | SPI1_SCK |
| PA6 | 30 | SPI1_MISO |
| PA7 | 31 | SPI1_MOSI |
| PB0 | 34 | MCP3008 CS |
| PE7 | 38 | GMG12864-06D CS |
| PE8 | 39 | GMG12864-06D RSE |
| PE9 | 40 | GMG12864-06D RS |
| PE10 | 41 | GMG12864-06D SCL |
| PE11 | 42 | GMG12864-06D SI |
| PE12 | 43 | GMG12864-06D backlight control 예비 |
| PB12 | 51 | KY-040 CLK |
| PB13 | 52 | KY-040 DT |
| PB14 | 53 | KY-040 SW |
| PB6 | 92 | I2C1_SCL |
| PB7 | 93 | I2C1_SDA |

## Notes before wiring

- 모든 모듈의 GND는 STM32 보드 GND와 반드시 공통으로 묶는다.
- STM32H743의 I/O는 3.3 V 기준으로 운용한다. 5 V 출력 모듈이나 5 V pull-up이 걸린 I2C/SPI 모듈은 그대로 연결하지 않는다.
- I2C pull-up은 3.3 V에 연결한다. 모듈에 이미 pull-up이 여러 개 달려 있으면 전체 등가 저항이 너무 낮아질 수 있으므로 확인한다.
- GMG12864-06D LCD는 GPIO software SPI로 시작한다. 나중에 LCD 갱신 속도가 부족하면 hardware SPI 전환을 검토한다.
- 오디오 clock은 PCM5102와 INMP441이 공유하므로 SAI1 설정에서 Block A를 master transmit, Block B를 synchronous slave receive로 시작한다.

## References

- STMicroelectronics, `STM32H742xI/G STM32H743xI/G` datasheet DS12110 Rev 11, January 2026: https://www.st.com/resource/en/datasheet/stm32h743vi.pdf
- 현재 CubeMX 설정: `rtos-loopstation.ioc`
