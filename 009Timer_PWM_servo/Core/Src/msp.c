/*
 * msp.c
 *
 *  Created on: Sep 11, 2022
 *      Author: ekrm5
 */
#include "stm32f4xx_hal.h"

extern TIM_HandleTypeDef htim2;
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


void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef tim2OC_ch_gpios;

	//1. enable peripheral clock
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_TIM2_CLK_ENABLE();

	//2. configure gpıos to behave as timer to channel 1,2,3 and 4
	//                              CH1        CH2           CH3         CH4
	tim2OC_ch_gpios.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
	tim2OC_ch_gpios.Mode = GPIO_MODE_AF_PP;
	tim2OC_ch_gpios.Alternate = GPIO_AF1_TIM2;
	tim2OC_ch_gpios.Pull      = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA,&tim2OC_ch_gpios);



	//3. NVIV settings
	HAL_NVIC_SetPriority(TIM2_IRQn, 15,0);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
}


