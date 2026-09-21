#include "system_task.h"

#include <cstdbool>

#include "gmg12864_lcd.hpp"
#include "mcp23017.hpp"
#include "system_init_event_flag.hpp"
#include "system_initparams.h"

enum class SystemInitResult {
  SUCCESS = 0,
  FAILED,
};

static bool IsValidParams(SystemInitParams* params);
static void ErrorHandler();
static SystemInitResult Init(SystemInitParams* params);
static SystemInitResult InitSdram(SDRAM_HandleTypeDef* hsdram);

void SystemTask_Init(void* argument) {
  SystemInitParams* params = (SystemInitParams*)argument;
  if (!IsValidParams(params)) {
    ErrorHandler();
  }

  if (Init(params) == SystemInitResult::SUCCESS) {
    osEventFlagsSet(params->system_init_event, SystemInitEventFlag::Inited);
  } else {
    ErrorHandler();
  }
}

bool IsValidParams(SystemInitParams* params) { return params != NULL; }

static void ErrorHandler() {
  for (;;) {
    osDelay(1);
  }
}

/**
 * 초기화 목록
 * 1. Mcp23017
 * 2. Sdram
 * 3. Gmg12864
 */
SystemInitResult Init(SystemInitParams* params) {
  if (Mcp23017::Driver::GetInstance().Initialize(
          params->mcp23017_init_params.hi2c,
          params->mcp23017_init_params.i2c1_mutex) != Mcp23017::Status::OK) {
    return SystemInitResult::FAILED;
  }

  if (InitSdram(params->sdram_init_params.hsdram) !=
      SystemInitResult::SUCCESS) {
    return SystemInitResult::FAILED;
  }

  Gmg12864::InitParams initparams = {
      .spi_params =
          {
              .hspi = params->gmg12864_init_params.hspi,
              .CS_Pin = params->gmg12864_init_params.CS_Pin,
              .RST_Pin = params->gmg12864_init_params.RST_Pin,
              .DC_Pin = params->gmg12864_init_params.DC_Pin,
              .CS_Port = params->gmg12864_init_params.CS_Port,
              .RST_Port = params->gmg12864_init_params.RST_Port,
              .DC_Port = params->gmg12864_init_params.DC_Port,
          },
      .rotation = U8G2_R2,
  };
  if (Gmg12864::Driver::GetInstance().Initialize(&initparams) !=
      Gmg12864::Gmg12864Status::OK) {
    return SystemInitResult::FAILED;
  }

  return SystemInitResult::SUCCESS;
}

SystemInitResult InitSdram(SDRAM_HandleTypeDef* hsdram) {
  if (hsdram == nullptr || !IS_FMC_CAS_LATENCY(hsdram->Init.CASLatency)) {
    return SystemInitResult::FAILED;
  }

  // FMC CAS 필드의 인코딩을 SDRAM 모드 레지스터 M[6:4]로 변환한다.
  constexpr uint32_t mode_register_cas_position = 4U;
  const uint32_t cas_latency = (hsdram->Init.CASLatency >> FMC_SDCRx_CAS_Pos)
                               << mode_register_cas_position;

  constexpr uint32_t burst_length = 0x0000U;
  constexpr uint32_t burst_type = 0x0000U;

  const uint32_t timeout = 0xFF;
  FMC_SDRAM_CommandTypeDef command = {
      .CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1,
      .AutoRefreshNumber = 1,
  };

  // 1. Start Clock 명령 전송
  command.CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
  if (HAL_SDRAM_SendCommand(hsdram, &command, timeout) != HAL_OK) {
    return SystemInitResult::FAILED;
  }

  // 2. 명령 전송 후 100us 만큼 기다리기
  HAL_Delay(1);

  // 3. Precharge 명령 전송
  command.CommandMode = FMC_SDRAM_CMD_PALL;
  if (HAL_SDRAM_SendCommand(hsdram, &command, timeout)) {
    return SystemInitResult::FAILED;
  }

  // 4. AutoRefresh 명령 전송
  command.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
  command.AutoRefreshNumber = 8;
  if (HAL_SDRAM_SendCommand(hsdram, &command, timeout) != HAL_OK) {
    return SystemInitResult::FAILED;
  }

  // 5. LoadMode 명령 전송
  command.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
  command.AutoRefreshNumber = 1;
  command.ModeRegisterDefinition = cas_latency | burst_type | burst_length;
  if (HAL_SDRAM_SendCommand(hsdram, &command, timeout) != HAL_OK) {
    return SystemInitResult::FAILED;
  }

  // 6. RefreshRate 주기 전송
  if (HAL_SDRAM_ProgramRefreshRate(hsdram, FMC_SDRTR_COUNT) != HAL_OK) {
    return SystemInitResult::FAILED;
  }

  return SystemInitResult::SUCCESS;
}
