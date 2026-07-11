---
title: 하드웨어 모듈 목록
version: 0.3.0
change_history:
  - date: 2026-07-11
    version: 0.1.0
    summary: V 모델 문서 구조의 하드웨어 단계에 맞춰 사용 모듈, peripheral, pinout, 비고를 표로 정리함
  - date: 2026-07-11
    version: 0.2.0
    summary: 요구사항의 제어부, FX부, 트랙부 용어에 맞춰 하드웨어 모듈을 재분류함
  - date: 2026-07-11
    version: 0.3.0
    summary: FX부와 트랙부의 버튼 및 포텐셔미터 구성을 실제 조작명 기준으로 수정함
---

# 하드웨어 모듈 목록

이 문서는 Loop Station 프로젝트에 사용한 하드웨어 모듈과 STM32H743VIT6 연결 정보를 정리한다.
하나의 모듈이 여러 구역의 역할을 함께 담당하는 경우에는 각 구역 표에 중복 기재한다.

## 1. 제어부

제어부는 패널 탐색, 선택, 값 조절과 같이 시스템 조작의 기준이 되는 입력을 담당한다.

| 모듈명 | 연결 peripheral | peripheral 연결 pinout | 비고 |
| --- | --- | --- | --- |
| MCP23017 x2 | `I2C1` | `PB6 / I2C1_SCL`, `PB7 / I2C1_SDA` | 제어부의 Enter 버튼, Exit 버튼, 좌 버튼, 우 버튼 입력에 사용한다. U1 주소는 `0x24`, U2 주소는 `0x25`를 기준으로 한다. |
| KY-040 로터리 엔코더 | `TIM4`, MCP23017 GPIO | `PD12 / TIM4_CH1`, `PD13 / TIM4_CH2`, MCP23017 U1 `GPIOB3 / SW` | 제어부의 로터리 엔코더 회전 조작과 로터리 엔코더 푸시 버튼 입력에 사용한다. |

## 2. FX부

FX부는 FX 활성화와 FX 파라미터 조작 입력을 담당한다.

| 모듈명 | 연결 peripheral | peripheral 연결 pinout | 비고 |
| --- | --- | --- | --- |
| MCP23017 x2 | `I2C1` | `PB6 / I2C1_SCL`, `PB7 / I2C1_SDA` | FX부의 IFX 활성화 버튼, TFX 활성화 버튼 입력에 사용한다. U1 주소는 `0x24`, U2 주소는 `0x25`를 기준으로 한다. |
| 포텐셔미터 x2 | `ADC1` | `PA0 / ADC1_INP16`, `PA1 / ADC1_INP17` | FX부의 IFX 파라미터 조절 노브, TFX 파라미터 조절 노브 입력에 사용한다. |

## 3. 트랙부

트랙부는 트랙 조작, 트랙 상태 표시, 트랙 볼륨 조작 입력을 담당한다.

| 모듈명 | 연결 peripheral | peripheral 연결 pinout | 비고 |
| --- | --- | --- | --- |
| MCP23017 x2 | `I2C1` | `PB6 / I2C1_SCL`, `PB7 / I2C1_SDA` | 트랙부의 녹음/재생 버튼, 정지 버튼, 트랙 조작 버튼 입력과 트랙 상태 LED 출력에 사용한다. U1 주소는 `0x24`, U2 주소는 `0x25`를 기준으로 한다. |
| 포텐셔미터 x1 | `ADC1` | `PA2 / ADC1_INP14` | 트랙부의 트랙 볼륨 조절 노브 입력에 사용한다. |

## 4. 표시부

표시부는 LCD 화면 출력과 LED 피드백 출력을 담당한다.

| 모듈명 | 연결 peripheral | peripheral 연결 pinout | 비고 |
| --- | --- | --- | --- |
| GMG12864-06D LCD | `SPI2`, `GPIO` | `PB10 / SPI2_SCK`, `PC1 / SPI2_MOSI`, `PE7 / GPIO_Output / CS`, `PE8 / GPIO_Output / RST`, `PE9 / GPIO_Output / DC` | SPI 송신 전용으로 사용한다. `u8g2` 기반 출력 테스트를 완료했다. |
| MCP23017 x2 | `I2C1` | `PB6 / I2C1_SCL`, `PB7 / I2C1_SDA` | 표시부의 IFX 활성화 LED, TFX 활성화 LED, 트랙 상태 LED 출력에 사용한다. U1 주소는 `0x24`, U2 주소는 `0x25`를 기준으로 한다. |

## 5. 저장부

저장부는 트랙 오디오 파일과 설정 파일을 저장하고 읽는 역할을 담당한다.

| 모듈명 | 연결 peripheral | peripheral 연결 pinout | 비고 |
| --- | --- | --- | --- |
| SD 카드 | `SDMMC1`, `GPIO` | `PC8 / SDMMC1_D0`, `PC9 / SDMMC1_D1`, `PC10 / SDMMC1_D2`, `PC11 / SDMMC1_D3`, `PC12 / SDMMC1_CK`, `PD2 / SDMMC1_CMD`, `PD4 / GPIO_Input / SDMMC_Detect` | 4-bit SD bus와 card detect 입력을 사용한다. FatFs 파일 입출력 대상이다. |

## 6. 오디오 입출력부

오디오 입출력부는 외부 오디오 입력과 최종 오디오 출력을 담당한다.

| 모듈명 | 연결 peripheral | peripheral 연결 pinout | 비고 |
| --- | --- | --- | --- |
| INMP441 I2S 마이크 | `SAI1 Block B` | `PE3 / SAI1_SD_B`, `PE4 / SAI1_FS_A`, `PE5 / SAI1_SCK_A` | SAI1 Block B를 synchronous slave RX로 사용한다. `WS`와 `SCK`는 DAC와 공유한다. |
| PCM5102A I2S DAC | `SAI1 Block A`, `GPIO` | `PE4 / SAI1_FS_A`, `PE5 / SAI1_SCK_A`, `PE6 / SAI1_SD_A`, `XSMT / 3.3 V` | 최종 오디오 출력 DAC다. `XSMT`는 high 상태여야 출력된다. `MCLK`는 사용하지 않는다. |

## 7. 개발 및 디버그

개발 및 디버그 항목은 제품 동작 요구사항의 직접 구성 요소는 아니지만 펌웨어 다운로드와 디버깅에 필요하다.

| 모듈명 | 연결 peripheral | peripheral 연결 pinout | 비고 |
| --- | --- | --- | --- |
| SWD 디버그 포트 | `DEBUG` | `PA13 / SWDIO`, `PA14 / SWCLK` | 펌웨어 다운로드와 디버깅에 사용한다. |
