#include "stm32h7xx.h"
#include "FreeRTOS.h"
#include "SEGGER_SYSVIEW.h"
#include "SEGGER_RTT.h"
extern const SEGGER_SYSVIEW_OS_API SYSVIEW_X_OS_TraceAPI;
extern unsigned char __rtt_start__, __rtt_end__;
static void describe(void) {
  SEGGER_SYSVIEW_SendSysDesc("N=rtos-loopstation,D=STM32H743,O=FreeRTOS");
  SEGGER_SYSVIEW_SendSysDesc("I#15=SysTick");
}
void SEGGER_SYSVIEW_Conf(void) {
  /* SRAM4 is configured non-cacheable by MPU_Config. Explicitly clear NOLOAD. */
  for (unsigned char *p = &__rtt_start__; p < &__rtt_end__; ++p) *p = 0;
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->LAR = 0xC5ACCE55;
  DWT->CYCCNT = 0;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
  SEGGER_RTT_Init();
  SEGGER_SYSVIEW_Init(SystemCoreClock, SystemCoreClock, &SYSVIEW_X_OS_TraceAPI, describe);
  SEGGER_SYSVIEW_SetRAMBase(0x20000000);
  /* Host starts recording; no buffer filling before the Windows connection. */
}
