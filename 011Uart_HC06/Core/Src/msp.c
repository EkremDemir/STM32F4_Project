/*
 * msp.c
 *
 *  Created on: Sep 14, 2022
 *      Author: ekrm5
 */

#include "main.h"


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

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	//Enable clock paths
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_USART2_CLK_ENABLE();

	//configure pins for usart pins   PA2->TX, PA3->RX
	GPIO_InitTypeDef  gpio_usart2;

	gpio_usart2.Pin = GPIO_PIN_2 | GPIO_PIN_3;
	gpio_usart2.Mode = GPIO_MODE_AF_PP;
	gpio_usart2.Alternate = GPIO_AF7_USART2;
	gpio_usart2.Pull      = GPIO_PULLUP;
	gpio_usart2.Speed     = GPIO_SPEED_MEDIUM;

	HAL_GPIO_Init(GPIOA,&gpio_usart2);

	//configure the nvic
	HAL_NVIC_SetPriority(USART2_IRQn, 15,0);
	HAL_NVIC_EnableIRQ(USART2_IRQn);

}

