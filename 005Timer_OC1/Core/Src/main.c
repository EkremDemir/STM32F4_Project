/*
 * main.c
 *
 *  Created on: Sep 11, 2022
 *      Author: ekrm5
 */

#include "main.h"

TIM_HandleTypeDef htim2;
uint32_t ccr_content;

void SystemClock_Config(void);
void Error_handler(void);

//void LSE_Configuration(void);
void TIMER2_Init(void);
void GPIO_Init(void);

uint32_t pulse_value1 = 40000;
uint32_t pulse_value2 = 20000;
uint32_t pulse_value3 = 10000;
uint32_t pulse_value4 = 5000;

int main(void)
{

	HAL_Init();
	SystemClock_Config();
	GPIO_Init();

	TIMER2_Init();

	if(HAL_TIM_OC_Start_IT(&htim2, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_handler();
	}

	if(HAL_TIM_OC_Start_IT(&htim2, TIM_CHANNEL_2) != HAL_OK)
	{
		Error_handler();
	}

	if(HAL_TIM_OC_Start_IT(&htim2, TIM_CHANNEL_3) != HAL_OK)
	{
		Error_handler();
	}

	if(HAL_TIM_OC_Start_IT(&htim2, TIM_CHANNEL_4) != HAL_OK)
	{
		Error_handler();
	}




	while(1);
	return 0;
}

void SystemClock_Config(void)
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
		Error_handler();
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
		Error_handler();
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





void TIMER2_Init(void)
{
	TIM_OC_InitTypeDef tim2Oc_init;

	htim2.Instance = TIM2;
	htim2.Init.Period = 0xFFFFFFFF;
	htim2.Init.Prescaler = 1;

	if(HAL_TIM_OC_Init(&htim2) != HAL_OK)
	{
		Error_handler();
	}

	tim2Oc_init.OCMode = TIM_OCMODE_TOGGLE;
	tim2Oc_init.OCPolarity = TIM_OCPOLARITY_HIGH;

	tim2Oc_init.Pulse = pulse_value1;
	HAL_TIM_OC_ConfigChannel(&htim2, &tim2Oc_init, TIM_CHANNEL_1);

	tim2Oc_init.Pulse = pulse_value2;
	HAL_TIM_OC_ConfigChannel(&htim2, &tim2Oc_init, TIM_CHANNEL_2);

	tim2Oc_init.Pulse = pulse_value3;
	HAL_TIM_OC_ConfigChannel(&htim2, &tim2Oc_init, TIM_CHANNEL_3);

	tim2Oc_init.Pulse = pulse_value4;
	HAL_TIM_OC_ConfigChannel(&htim2, &tim2Oc_init, TIM_CHANNEL_4);

}

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
	//TIM2_CH1 toggling with frequency = 500hz
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	{
		ccr_content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
		__HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_1,ccr_content+ pulse_value1);


	}
	//TIM2_CH2 toggling with frequency = 1k
	else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
	{
		ccr_content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
		__HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_2, ccr_content + pulse_value2);

	}
	//TIM2_CH3 toggling with frequency = 2k
	else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
	{
		ccr_content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);
		__HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_3, ccr_content + pulse_value3);

	}
	//TIM2_CH4 toggling with frequency = 4k
	else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
	{
		ccr_content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);
		__HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_4, ccr_content + pulse_value4);

	}
}


void Error_handler(void)
{

}
