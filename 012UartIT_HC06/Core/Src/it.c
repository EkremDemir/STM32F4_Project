/*
 * it.c
 *
 *  Created on: Sep 14, 2022
 *      Author: ekrm5
 */

#include "main.h"
extern UART_HandleTypeDef huart2;

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void USART2_IRQHandler(void)
{
	HAL_UART_IRQHandler(&huart2);
}

