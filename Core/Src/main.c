/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "u8g2.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ENCODER_DISPLAY_INTERVAL_MS 50U
#define MCP23017_ADDR_A2A1A0_100 0x24U
#define MCP23017_HAL_ADDR(addr7) ((uint16_t)((addr7) << 1))
#define MCP23017_REG_IODIRB 0x01U
#define MCP23017_REG_GPPUB 0x0DU
#define MCP23017_REG_GPIOB 0x13U
#define ENCODER_SW_MCP23017_B3 GPIO_PIN_3

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

I2C_HandleTypeDef hi2c1;

SD_HandleTypeDef hsd1;

SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim4;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
static void MX_GPIO_Init(void);
static void MX_SDMMC1_SD_Init(void);
static void MX_SPI2_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM4_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static u8g2_t lcd;
static int32_t encoder_position;
static uint16_t encoder_last_count;
static int16_t encoder_last_delta;
static uint8_t encoder_switch_ready;
static uint8_t encoder_switch_pressed;

static void GMG12864_DelayCycles(volatile uint32_t cycles) {
  while (cycles-- > 0U) {
    __NOP();
  }
}

static uint8_t GMG12864_U8x8ByteHwSpi(u8x8_t* u8x8, uint8_t msg,
                                      uint8_t arg_int, void* arg_ptr) {
  (void)u8x8;

  switch (msg) {
    case U8X8_MSG_BYTE_INIT:
      break;

    case U8X8_MSG_BYTE_SEND:
      if (HAL_SPI_Transmit(&hspi2, (uint8_t*)arg_ptr, arg_int, HAL_MAX_DELAY) !=
          HAL_OK) {
        Error_Handler();
      }
      break;

    case U8X8_MSG_BYTE_SET_DC:
      HAL_GPIO_WritePin(GMG12864_DC_GPIO_Port, GMG12864_DC_Pin,
                        arg_int != 0U ? GPIO_PIN_SET : GPIO_PIN_RESET);
      break;

    case U8X8_MSG_BYTE_START_TRANSFER:
      HAL_GPIO_WritePin(GMG12864_CS_GPIO_Port, GMG12864_CS_Pin, GPIO_PIN_RESET);
      break;

    case U8X8_MSG_BYTE_END_TRANSFER:
      HAL_GPIO_WritePin(GMG12864_CS_GPIO_Port, GMG12864_CS_Pin, GPIO_PIN_SET);
      break;

    default:
      return 0;
  }

  return 1;
}

static uint8_t GMG12864_U8x8GpioDelay(u8x8_t* u8x8, uint8_t msg,
                                      uint8_t arg_int, void* arg_ptr) {
  (void)u8x8;
  (void)arg_ptr;

  switch (msg) {
    case U8X8_MSG_GPIO_AND_DELAY_INIT:
      HAL_GPIO_WritePin(GMG12864_CS_GPIO_Port, GMG12864_CS_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(GMG12864_RST_GPIO_Port, GMG12864_RST_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(GMG12864_DC_GPIO_Port, GMG12864_DC_Pin, GPIO_PIN_RESET);
      break;

    case U8X8_MSG_DELAY_MILLI:
      HAL_Delay(arg_int);
      break;

    case U8X8_MSG_DELAY_10MICRO:
      GMG12864_DelayCycles((uint32_t)arg_int * 1600U);
      break;

    case U8X8_MSG_DELAY_100NANO:
      GMG12864_DelayCycles((uint32_t)arg_int * 16U);
      break;

    case U8X8_MSG_DELAY_NANO:
      break;

    case U8X8_MSG_GPIO_RESET:
      HAL_GPIO_WritePin(GMG12864_RST_GPIO_Port, GMG12864_RST_Pin,
                        arg_int != 0U ? GPIO_PIN_SET : GPIO_PIN_RESET);
      break;

    case U8X8_MSG_GPIO_CS:
      HAL_GPIO_WritePin(GMG12864_CS_GPIO_Port, GMG12864_CS_Pin,
                        arg_int != 0U ? GPIO_PIN_SET : GPIO_PIN_RESET);
      break;

    case U8X8_MSG_GPIO_DC:
      HAL_GPIO_WritePin(GMG12864_DC_GPIO_Port, GMG12864_DC_Pin,
                        arg_int != 0U ? GPIO_PIN_SET : GPIO_PIN_RESET);
      break;

    default:
      return 0;
  }

  return 1;
}

static HAL_StatusTypeDef MCP23017_WriteReg(uint8_t addr7, uint8_t reg,
                                           uint8_t value) {
  return HAL_I2C_Mem_Write(&hi2c1, MCP23017_HAL_ADDR(addr7), reg,
                           I2C_MEMADD_SIZE_8BIT, &value, 1, HAL_MAX_DELAY);
}

static HAL_StatusTypeDef MCP23017_ReadReg(uint8_t addr7, uint8_t reg,
                                          uint8_t* value) {
  return HAL_I2C_Mem_Read(&hi2c1, MCP23017_HAL_ADDR(addr7), reg,
                          I2C_MEMADD_SIZE_8BIT, value, 1, HAL_MAX_DELAY);
}

static HAL_StatusTypeDef EncoderSwitch_Init(void) {
  uint8_t value;

  if (MCP23017_ReadReg(MCP23017_ADDR_A2A1A0_100, MCP23017_REG_IODIRB,
                       &value) != HAL_OK) {
    return HAL_ERROR;
  }
  value |= ENCODER_SW_MCP23017_B3;
  if (MCP23017_WriteReg(MCP23017_ADDR_A2A1A0_100, MCP23017_REG_IODIRB,
                        value) != HAL_OK) {
    return HAL_ERROR;
  }

  if (MCP23017_ReadReg(MCP23017_ADDR_A2A1A0_100, MCP23017_REG_GPPUB,
                       &value) != HAL_OK) {
    return HAL_ERROR;
  }
  value |= ENCODER_SW_MCP23017_B3;
  if (MCP23017_WriteReg(MCP23017_ADDR_A2A1A0_100, MCP23017_REG_GPPUB,
                        value) != HAL_OK) {
    return HAL_ERROR;
  }

  return HAL_OK;
}

static void EncoderSwitch_Poll(void) {
  uint8_t gpio_b;

  if (!encoder_switch_ready) {
    return;
  }

  if (MCP23017_ReadReg(MCP23017_ADDR_A2A1A0_100, MCP23017_REG_GPIOB,
                       &gpio_b) != HAL_OK) {
    encoder_switch_ready = 0U;
    encoder_switch_pressed = 0U;
    return;
  }

  encoder_switch_pressed =
      (gpio_b & ENCODER_SW_MCP23017_B3) == 0U ? 1U : 0U;
}

static void LCD_DrawEncoderStatus(void) {
  char line1[22];
  char line2[22];
  char line3[22];
  char line4[22];
  const char* direction = "STOP";

  if (encoder_last_delta > 0) {
    direction = "CW";
  } else if (encoder_last_delta < 0) {
    direction = "CCW";
  }

  (void)snprintf(line1, sizeof(line1), "CNT: %5u",
                 (unsigned int)__HAL_TIM_GET_COUNTER(&htim4));
  (void)snprintf(line2, sizeof(line2), "POS: %ld", (long)encoder_position);
  (void)snprintf(line3, sizeof(line3), "DIR: %s d=%d", direction,
                 (int)encoder_last_delta);
  if (encoder_switch_ready) {
    (void)snprintf(line4, sizeof(line4), "SW : %s",
                   encoder_switch_pressed ? "PUSH" : "OPEN");
  } else {
    (void)snprintf(line4, sizeof(line4), "SW : MCP ERR");
  }

  u8g2_ClearBuffer(&lcd);
  u8g2_SetFont(&lcd, u8g2_font_6x10_tf);
  u8g2_DrawStr(&lcd, 0, 10, "TIM4 encoder test");
  u8g2_DrawStr(&lcd, 0, 22, line1);
  u8g2_DrawStr(&lcd, 0, 34, line2);
  u8g2_DrawStr(&lcd, 0, 46, line3);
  u8g2_DrawStr(&lcd, 0, 58, line4);
  u8g2_SendBuffer(&lcd);
}

static void LCD_DrawEncoderError(const char* message) {
  u8g2_ClearBuffer(&lcd);
  u8g2_SetFont(&lcd, u8g2_font_6x10_tf);
  u8g2_DrawStr(&lcd, 0, 12, "Encoder error");
  u8g2_DrawStr(&lcd, 0, 28, message);
  u8g2_SendBuffer(&lcd);
}

static void Encoder_Poll(void) {
  static uint32_t last_display_ms;
  uint32_t now_ms = HAL_GetTick();
  uint16_t count = (uint16_t)__HAL_TIM_GET_COUNTER(&htim4);
  int16_t delta = (int16_t)(count - encoder_last_count);

  encoder_last_count = count;

  if (delta != 0) {
    encoder_last_delta = delta;
    encoder_position += delta;
  }

  if ((now_ms - last_display_ms) < ENCODER_DISPLAY_INTERVAL_MS) {
    return;
  }
  last_display_ms = now_ms;

  EncoderSwitch_Poll();
  LCD_DrawEncoderStatus();
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SDMMC1_SD_Init();
  MX_FATFS_Init();
  MX_SPI2_Init();
  MX_I2C1_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */

  u8g2_Setup_st7565_erc12864_alt_f(&lcd, U8G2_R0, GMG12864_U8x8ByteHwSpi,
                                   GMG12864_U8x8GpioDelay);
  u8g2_InitDisplay(&lcd);
  u8g2_SetPowerSave(&lcd, 0);
  u8g2_SetContrast(&lcd, 80);

  if (HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL) != HAL_OK) {
    LCD_DrawEncoderError("TIM4 start failed");
    Error_Handler();
  }
  __HAL_TIM_SET_COUNTER(&htim4, 0U);
  encoder_last_count = 0U;
  encoder_last_delta = 0;
  encoder_position = 0;
  encoder_switch_ready = EncoderSwitch_Init() == HAL_OK ? 1U : 0U;
  EncoderSwitch_Poll();
  LCD_DrawEncoderStatus();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    Encoder_Poll();
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 60;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 5;
  RCC_OscInitStruct.PLL.PLLR = 42;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x307075B1;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief SDMMC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SDMMC1_SD_Init(void)
{

  /* USER CODE BEGIN SDMMC1_Init 0 */

  /* USER CODE END SDMMC1_Init 0 */

  /* USER CODE BEGIN SDMMC1_Init 1 */

  /* USER CODE END SDMMC1_Init 1 */
  hsd1.Instance = SDMMC1;
  hsd1.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING;
  hsd1.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;
  hsd1.Init.BusWide = SDMMC_BUS_WIDE_4B;
  hsd1.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd1.Init.ClockDiv = 24;
  if (HAL_SD_Init(&hsd1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SDMMC1_Init 2 */

  /* USER CODE END SDMMC1_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES_TXONLY;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 0x0;
  hspi2.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  hspi2.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
  hspi2.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
  hspi2.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi2.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi2.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
  hspi2.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
  hspi2.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
  hspi2.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
  hspi2.Init.IOSwap = SPI_IO_SWAP_DISABLE;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 0;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 65535;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI1;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 0;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 0;
  if (HAL_TIM_Encoder_Init(&htim4, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GMG12864_CS_Pin|GMG12864_RST_Pin|GMG12864_DC_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : GMG12864_CS_Pin GMG12864_RST_Pin GMG12864_DC_Pin */
  GPIO_InitStruct.Pin = GMG12864_CS_Pin|GMG12864_RST_Pin|GMG12864_DC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : SDMMC_Detect_Pin */
  GPIO_InitStruct.Pin = SDMMC_Detect_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(SDMMC_Detect_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

 /* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x0;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1) {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line
     number, ex: printf("Wrong parameters value: file %s on line %d\r\n", file,
     line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
