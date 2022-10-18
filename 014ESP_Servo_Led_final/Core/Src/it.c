/*
 * it.c
 *
 *  Created on: Sep 11, 2022
 *      Author: ekrm5
 */
#include "main.h"

extern TIM_HandleTypeDef htim2;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart1;

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void TIM2_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim2);
}

void USART1_IRQHandler(void)
{
	HAL_UART_IRQHandler(&huart1);
}

void USART2_IRQHandler(void)
{
	HAL_UART_IRQHandler(&huart2);
}
