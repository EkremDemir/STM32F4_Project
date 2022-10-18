/*
 * it.c
 *
 *  Created on: Sep 11, 2022
 *      Author: ekrm5
 */
#include "main.h"

extern TIM_HandleTypeDef htim2;

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void TIM2_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim2);
}

