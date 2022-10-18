/*
 * it.c
 *
 *  Created on: Sep 14, 2022
 *      Author: ekrm5
 */

#include "main.h"

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

