/*
 * main.c
 *
 *  Created on: Sep 11, 2022
 *      Author: ekrm5
 */

#include "main.h"

TIM_HandleTypeDef htim6;

void SystemClockConfig(void);
void Error_Handler(void);

void GPIO_Init(void);
void TIMER6_Init(void);

int main(void)
{
 	HAL_Init();
	SystemClockConfig();
	GPIO_Init();
	TIMER6_Init();



	//	Start the timer
	HAL_TIM_Base_Start_IT(&htim6);

	while(1);
	return 0;
}


void SystemClockConfig(void)
{

	  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	  /** Configure the main internal regulator output voltage
	  */
	  __HAL_RCC_PWR_CLK_ENABLE();
	  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	  /** Initializes the RCC Oscillators according to the specified parameters
	  * in the RCC_OscInitTypeDef structure.
	  */
	  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	  RCC_OscInitStruct.PLL.PLLM = 8;
	  RCC_OscInitStruct.PLL.PLLN = 160;
	  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	  RCC_OscInitStruct.PLL.PLLQ = 7;
	  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  /** Initializes the CPU, AHB and APB buses clocks
	  */
	  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
	                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	  {
	    Error_Handler();
	  }
}

void TIMER6_Init(void)
{
	//uint32_t pre =  HAL_RCC_GetHCLKFreq() /1000000;
	htim6.Instance = TIM6;
    htim6.Init.Prescaler = 4;
    htim6.Init.Period    = 15;
    htim6.Init.CounterMode = TIM_COUNTERMODE_UP;

    if(HAL_TIM_Base_Init(&htim6) != HAL_OK)
    {
    	Error_Handler();
    }

}

void GPIO_Init(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef Ledgpio;

	Ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
	Ledgpio.Pin  = GPIO_PIN_1;
	Ledgpio.Speed = GPIO_SPEED_LOW;
	Ledgpio.Pull  = GPIO_NOPULL;

	HAL_GPIO_Init(GPIOA,&Ledgpio);



}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
}
void Error_Handler(void)
{
	while(1);
}
