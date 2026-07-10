---
title: 하드웨어 모듈 목록
version: 0.1.0
change_history:
  - date: 2026-07-11
    version: 0.1.0
    summary: V 모델 문서 구조의 하드웨어 단계에 맞춰 사용 모듈, peripheral, pinout, 비고를 표로 정리함
---

# 하드웨어 모듈 목록

이 문서는 Loop Station 프로젝트에 사용한 하드웨어 모듈과 STM32H743VIT6 연결 정보를 정리한다.

| 모듈명 | 연결 peripheral | peripheral 연결 pinout | 비고 |
| --- | --- | --- | --- |
| MCP23017 x2 | `I2C1` | `PB6 / I2C1_SCL`, `PB7 / I2C1_SDA` | 버튼 입력과 LED 출력 GPIO 확장에 사용한다. U1 주소는 `0x24`, U2 주소는 `0x25`를 기준으로 한다. |
| GMG12864-06D LCD | `SPI2`, `GPIO` | `PB10 / SPI2_SCK`, `PC1 / SPI2_MOSI`, `PE7 / GPIO_Output / CS`, `PE8 / GPIO_Output / RST`, `PE9 / GPIO_Output / DC` | SPI 송신 전용으로 사용한다. `u8g2` 기반 출력 테스트를 완료했다. |
| 포텐셔미터 x3 | `ADC1` | `PA0 / ADC1_INP16`, `PA1 / ADC1_INP17`, `PA2 / ADC1_INP14` | FX 파라미터 또는 아날로그 조작값 입력에 사용한다. 확장 후보 핀은 `PA3`, `PA4`, `PA5`, `PA6`, `PA7`이다. |
| SD 카드 | `SDMMC1`, `GPIO` | `PC8 / SDMMC1_D0`, `PC9 / SDMMC1_D1`, `PC10 / SDMMC1_D2`, `PC11 / SDMMC1_D3`, `PC12 / SDMMC1_CK`, `PD2 / SDMMC1_CMD`, `PD4 / GPIO_Input / SDMMC_Detect` | 4-bit SD bus와 card detect 입력을 사용한다. FatFs 파일 입출력 대상이다. |
| KY-040 로터리 엔코더 | `TIM4`, MCP23017 GPIO | `PD12 / TIM4_CH1`, `PD13 / TIM4_CH2`, MCP23017 U1 `GPIOB3 / SW` | 회전 입력은 TIM4 encoder mode로 처리하고 push 입력은 MCP23017 GPIO로 처리한다. |
| INMP441 I2S 마이크 | `SAI1 Block B` | `PE3 / SAI1_SD_B`, `PE4 / SAI1_FS_A`, `PE5 / SAI1_SCK_A` | SAI1 Block B를 synchronous slave RX로 사용한다. `WS`와 `SCK`는 DAC와 공유한다. |
| PCM5102A I2S DAC | `SAI1 Block A`, `GPIO` | `PE4 / SAI1_FS_A`, `PE5 / SAI1_SCK_A`, `PE6 / SAI1_SD_A`, `XSMT / 3.3 V` | 최종 오디오 출력 DAC다. `XSMT`는 high 상태여야 출력된다. `MCLK`는 사용하지 않는다. |
| SWD 디버그 포트 | `DEBUG` | `PA13 / SWDIO`, `PA14 / SWCLK` | 펌웨어 다운로드와 디버깅에 사용한다. |
