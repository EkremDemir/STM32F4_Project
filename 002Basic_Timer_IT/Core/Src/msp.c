/*
 * msp.c
 *
 *  Created on: Sep 11, 2022
 *      Author: ekrm5
 */
#include "stm32f4xx_hal.h"
void HAL_MspInit(void)
{
	//do low level processor specific inits
	//1. set up priority grouping of arm
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	//2. Enable reqired system exception
	SCB->SHCSR |= 0x7 << 16; //usage fault, memory fault and bus fault

	//3. configure the priority for system exceptions

	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
	HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);


}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	//1. Enable the clock
	__HAL_RCC_TIM6_CLK_ENABLE();

	//2. Enable the IRQ number
	HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);

	//3. Configure priority for TIM6_DAC_IRQn
	HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 15, 0);

}
