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


void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef tim2ch1_gpio;
	//1. Enable the clock
	__HAL_RCC_TIM2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	//2. Configure a gpio to behave as timer2 channel 1
	tim2ch1_gpio.Pin  = GPIO_PIN_9;
	tim2ch1_gpio.Mode = GPIO_MODE_AF_PP;
	tim2ch1_gpio.Alternate = GPIO_AF1_TIM2;
	tim2ch1_gpio.Pull      = GPIO_NOPULL;
	tim2ch1_gpio.Speed     = GPIO_SPEED_LOW;

	HAL_GPIO_Init(GPIOA,&tim2ch1_gpio);

	//3. NVIC settings
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
	HAL_NVIC_SetPriority(TIM2_IRQn, 15,0);



}
