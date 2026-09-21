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
    // TODO:
    // 시스템 초기화 성공 시 다른 태스크들이 시작할 수 있도록,
    //  이벤트를 발생 시킨 후 이 태스크 종료
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
  return SystemInitResult::SUCCESS;
}
