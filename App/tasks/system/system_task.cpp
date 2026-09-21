#include "system_task.h"

#include <cstdbool>

#include "mcp23017.hpp"
#include "system_init_event_flag.hpp"
#include "system_initparams.h"

static bool IsValidParams(SystemInitParams* params);
static void ErrorHandler();
static TaskStatus Init(SystemInitParams* params);

void SystemTask_Init(void* argument) {
  SystemInitParams* params = (SystemInitParams*)argument;
  if (!IsValidParams(params)) {
    ErrorHandler();
  }

  if (Init(params) == TASK_STATUS_OK) {
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

TaskStatus Init(SystemInitParams* params) { return TASK_STATUS_OK; }
