#include "stm32_lpm_if.h"
#include "gpio.h"
#include "platform.h"
#include "stm32_lpm.h"
#include "sys_sensors.h"
#include "usart.h"
#include "usart_if.h"

const struct UTIL_LPM_Driver_s UTIL_PowerDriver = {
    PWR_EnterSleepMode, PWR_ExitSleepMode, PWR_EnterStopMode,
    PWR_ExitStopMode,   PWR_EnterOffMode,  PWR_ExitOffMode,
};

void PWR_EnterOffMode(void) {}

void PWR_ExitOffMode(void) {}

void PWR_EnterStopMode(void) {
  HAL_SuspendTick();
  GPIO_InitTypeDef GPIO_InitStructure = {0};
  GPIO_InitStructure.Pin = GPIO_PIN_All;
  GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
  __HAL_RCC_GPIOA_CLK_DISABLE();
  HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
  __HAL_RCC_GPIOC_CLK_DISABLE();
  GPIO_InitStructure.Pin = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_5 |
                           GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 |
                           GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 |
                           GPIO_PIN_14 | GPIO_PIN_15;
  GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
  HAL_GPIO_WritePin(BQ24210_PG_Port, BQ24210_PG_Pin, GPIO_PIN_RESET);
  __HAL_RCC_GPIOB_CLK_DISABLE();

  HAL_UART_MspDeInit(&hlpuart1);
  LL_PWR_ClearFlag_C1STOP_C1STB();
  HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);
}

void PWR_ExitStopMode(void) {
  HAL_ResumeTick();
  MX_GPIO_Init();
  HAL_UART_MspInit(&hlpuart1);
  vcom_Resume();
}

void PWR_EnterSleepMode(void) {
  HAL_SuspendTick();
  HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
}

void PWR_ExitSleepMode(void) { HAL_ResumeTick(); }