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
#include <stdbool.h>
#include <stdio.h>
#include "u8g2.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MCP23017_BASE_ADDR 0x20U
#define MCP23017_ADDR_A2A1A0_100 (MCP23017_BASE_ADDR | 0x04U)
#define MCP23017_ADDR_A2A1A0_101 (MCP23017_BASE_ADDR | 0x05U)
#define MCP23017_HAL_ADDR(addr7) ((uint16_t)((addr7) << 1))

#define MCP23017_REG_IODIRA 0x00U
#define MCP23017_REG_IODIRB 0x01U
#define MCP23017_REG_GPPUA 0x0CU
#define MCP23017_REG_GPPUB 0x0DU
#define MCP23017_REG_GPIOB 0x13U
#define MCP23017_REG_OLATB 0x15U

#define MCP23017_100_B4_INPUT GPIO_PIN_4
#define MCP23017_100_B5_OUTPUT GPIO_PIN_5
#define MCP23017_100_STATELESS_MASK (GPIO_PIN_6 | GPIO_PIN_7)
#define MCP23017_100_INPUT_MASK \
  (MCP23017_100_B4_INPUT | MCP23017_100_STATELESS_MASK)

#define MCP23017_101_B7_INPUT GPIO_PIN_7
#define MCP23017_101_B6_OUTPUT GPIO_PIN_6
#define MCP23017_101_STATELESS_MASK \
  (GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5)
#define MCP23017_101_INPUT_MASK \
  (MCP23017_101_B7_INPUT | MCP23017_101_STATELESS_MASK)

#define MCP23017_POLL_INTERVAL_MS 20U
#define MCP23017_DEBOUNCE_MS 40U

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

I2C_HandleTypeDef hi2c1;

SD_HandleTypeDef hsd1;

SPI_HandleTypeDef hspi2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
static void MX_GPIO_Init(void);
static void MX_SDMMC1_SD_Init(void);
static void MX_SPI2_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
typedef struct {
  bool stable_pressed;
  bool last_raw_pressed;
  uint32_t last_change_ms;
} ButtonDebounce;

static u8g2_t lcd;
static ButtonDebounce mcp100_b4_button;
static ButtonDebounce mcp101_b7_button;
static bool mcp100_b5_state;
static bool mcp101_b6_state;
static uint8_t mcp100_olatb;
static uint8_t mcp101_olatb;
static uint8_t mcp100_last_stateless = 0xffU;
static uint8_t mcp101_last_stateless = 0xffU;
static bool mcp23017_ready;

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

static HAL_StatusTypeDef MCP23017_SetBOutput(uint8_t addr7, uint8_t* olatb,
                                             uint8_t mask, bool on) {
  if (on) {
    *olatb |= mask;
  } else {
    *olatb &= (uint8_t)~mask;
  }

  return MCP23017_WriteReg(addr7, MCP23017_REG_OLATB, *olatb);
}

static HAL_StatusTypeDef MCP23017_InitDevice(uint8_t addr7, uint8_t iodirb,
                                             uint8_t gppub, uint8_t* olatb) {
  *olatb = 0U;

  if (MCP23017_WriteReg(addr7, MCP23017_REG_IODIRA, 0xffU) != HAL_OK) {
    return HAL_ERROR;
  }
  if (MCP23017_WriteReg(addr7, MCP23017_REG_GPPUA, 0x00U) != HAL_OK) {
    return HAL_ERROR;
  }
  if (MCP23017_WriteReg(addr7, MCP23017_REG_OLATB, *olatb) != HAL_OK) {
    return HAL_ERROR;
  }
  if (MCP23017_WriteReg(addr7, MCP23017_REG_IODIRB, iodirb) != HAL_OK) {
    return HAL_ERROR;
  }
  if (MCP23017_WriteReg(addr7, MCP23017_REG_GPPUB, gppub) != HAL_OK) {
    return HAL_ERROR;
  }

  return HAL_OK;
}

static HAL_StatusTypeDef MCP23017_InitAll(void) {
  mcp100_b5_state = false;
  mcp101_b6_state = false;

  if (MCP23017_InitDevice(MCP23017_ADDR_A2A1A0_100, MCP23017_100_INPUT_MASK,
                          MCP23017_100_INPUT_MASK, &mcp100_olatb) != HAL_OK) {
    return HAL_ERROR;
  }

  if (MCP23017_InitDevice(MCP23017_ADDR_A2A1A0_101, MCP23017_101_INPUT_MASK,
                          MCP23017_101_INPUT_MASK, &mcp101_olatb) != HAL_OK) {
    return HAL_ERROR;
  }

  mcp100_b4_button.last_raw_pressed = false;
  mcp100_b4_button.stable_pressed = false;
  mcp100_b4_button.last_change_ms = HAL_GetTick();
  mcp101_b7_button.last_raw_pressed = false;
  mcp101_b7_button.stable_pressed = false;
  mcp101_b7_button.last_change_ms = HAL_GetTick();

  return HAL_OK;
}

static bool MCP23017_DebouncePressedEdge(ButtonDebounce* button,
                                         bool raw_pressed, uint32_t now_ms) {
  if (raw_pressed != button->last_raw_pressed) {
    button->last_raw_pressed = raw_pressed;
    button->last_change_ms = now_ms;
  }

  if ((raw_pressed != button->stable_pressed) &&
      ((now_ms - button->last_change_ms) >= MCP23017_DEBOUNCE_MS)) {
    button->stable_pressed = raw_pressed;
    return raw_pressed;
  }

  return false;
}

static bool MCP23017_IsPressed(uint8_t gpio_value, uint8_t mask) {
  return (gpio_value & mask) == 0U;
}

static void MCP23017_BuildPressedLine(char* line, size_t line_size,
                                      const char* prefix, uint8_t pressed_mask,
                                      const uint8_t* pins, size_t pin_count) {
  int written = snprintf(line, line_size, "%s:", prefix);
  bool any_pressed = false;

  if (written < 0) {
    return;
  }

  for (size_t i = 0; i < pin_count; i++) {
    uint8_t pin = pins[i];

    if ((pressed_mask & (uint8_t)(1U << pin)) != 0U) {
      int remain = (int)line_size - written;

      if (remain > 0) {
        written += snprintf(&line[written], (size_t)remain, " B%u", pin);
      }
      any_pressed = true;
    }
  }

  if (!any_pressed && written < (int)line_size) {
    (void)snprintf(&line[written], line_size - (size_t)written, " none");
  }
}

static void LCD_DrawMCP23017Status(uint8_t mcp100_stateless,
                                   uint8_t mcp101_stateless) {
  static const uint8_t mcp100_pins[] = {6U, 7U};
  static const uint8_t mcp101_pins[] = {2U, 3U, 4U, 5U};
  char line1[22];
  char line2[22];
  char line3[22];

  MCP23017_BuildPressedLine(line1, sizeof(line1), "100", mcp100_stateless,
                            mcp100_pins, sizeof(mcp100_pins));
  MCP23017_BuildPressedLine(line2, sizeof(line2), "101", mcp101_stateless,
                            mcp101_pins, sizeof(mcp101_pins));
  (void)snprintf(line3, sizeof(line3), "T 100B5=%u 101B6=%u",
                 mcp100_b5_state ? 1U : 0U, mcp101_b6_state ? 1U : 0U);

  u8g2_ClearBuffer(&lcd);
  u8g2_SetFont(&lcd, u8g2_font_6x10_tf);
  u8g2_DrawStr(&lcd, 0, 10, "MCP23017 polling");
  u8g2_DrawStr(&lcd, 0, 24, line1);
  u8g2_DrawStr(&lcd, 0, 38, line2);
  u8g2_DrawStr(&lcd, 0, 52, line3);
  u8g2_SendBuffer(&lcd);
}

static void LCD_DrawMCP23017Error(const char* message) {
  u8g2_ClearBuffer(&lcd);
  u8g2_SetFont(&lcd, u8g2_font_6x10_tf);
  u8g2_DrawStr(&lcd, 0, 12, "MCP23017 error");
  u8g2_DrawStr(&lcd, 0, 28, message);
  u8g2_SendBuffer(&lcd);
}

static void MCP23017_Poll(void) {
  static uint32_t last_poll_ms;
  uint32_t now_ms = HAL_GetTick();
  uint8_t gpio100 = 0xffU;
  uint8_t gpio101 = 0xffU;
  uint8_t mcp100_stateless;
  uint8_t mcp101_stateless;
  bool should_redraw = false;

  if ((now_ms - last_poll_ms) < MCP23017_POLL_INTERVAL_MS) {
    return;
  }
  last_poll_ms = now_ms;

  if (!mcp23017_ready) {
    return;
  }

  if (MCP23017_ReadReg(MCP23017_ADDR_A2A1A0_100, MCP23017_REG_GPIOB,
                       &gpio100) != HAL_OK) {
    mcp23017_ready = false;
    LCD_DrawMCP23017Error("read 0x24 failed");
    return;
  }

  if (MCP23017_ReadReg(MCP23017_ADDR_A2A1A0_101, MCP23017_REG_GPIOB,
                       &gpio101) != HAL_OK) {
    mcp23017_ready = false;
    LCD_DrawMCP23017Error("read 0x25 failed");
    return;
  }

  if (MCP23017_DebouncePressedEdge(
          &mcp100_b4_button,
          MCP23017_IsPressed(gpio100, MCP23017_100_B4_INPUT), now_ms)) {
    mcp100_b5_state = !mcp100_b5_state;
    if (MCP23017_SetBOutput(MCP23017_ADDR_A2A1A0_100, &mcp100_olatb,
                            MCP23017_100_B5_OUTPUT,
                            mcp100_b5_state) != HAL_OK) {
      mcp23017_ready = false;
      LCD_DrawMCP23017Error("write 0x24 failed");
      return;
    }
    should_redraw = true;
  }

  if (MCP23017_DebouncePressedEdge(
          &mcp101_b7_button,
          MCP23017_IsPressed(gpio101, MCP23017_101_B7_INPUT), now_ms)) {
    mcp101_b6_state = !mcp101_b6_state;
    if (MCP23017_SetBOutput(MCP23017_ADDR_A2A1A0_101, &mcp101_olatb,
                            MCP23017_101_B6_OUTPUT,
                            mcp101_b6_state) != HAL_OK) {
      mcp23017_ready = false;
      LCD_DrawMCP23017Error("write 0x25 failed");
      return;
    }
    should_redraw = true;
  }

  mcp100_stateless = (uint8_t)(~gpio100) & MCP23017_100_STATELESS_MASK;
  mcp101_stateless = (uint8_t)(~gpio101) & MCP23017_101_STATELESS_MASK;

  if ((mcp100_stateless != mcp100_last_stateless) ||
      (mcp101_stateless != mcp101_last_stateless)) {
    mcp100_last_stateless = mcp100_stateless;
    mcp101_last_stateless = mcp101_stateless;
    should_redraw = true;
  }

  if (should_redraw) {
    LCD_DrawMCP23017Status(mcp100_stateless, mcp101_stateless);
  }
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
  /* USER CODE BEGIN 2 */

  u8g2_Setup_st7565_erc12864_alt_f(&lcd, U8G2_R0, GMG12864_U8x8ByteHwSpi,
                                   GMG12864_U8x8GpioDelay);
  u8g2_InitDisplay(&lcd);
  u8g2_SetPowerSave(&lcd, 0);
  u8g2_SetContrast(&lcd, 80);

  u8g2_ClearBuffer(&lcd);
  u8g2_SetFont(&lcd, u8g2_font_6x10_tf);
  u8g2_DrawStr(&lcd, 0, 12, "Hello");
  u8g2_DrawStr(&lcd, 0, 28, "Loopstation");
  u8g2_SendBuffer(&lcd);

  if (MCP23017_InitAll() == HAL_OK) {
    mcp23017_ready = true;
    LCD_DrawMCP23017Status(0U, 0U);
  } else {
    mcp23017_ready = false;
    LCD_DrawMCP23017Error("init failed");
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    MCP23017_Poll();
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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

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
