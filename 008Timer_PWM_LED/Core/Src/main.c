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


void TIMER2_Init(void);
void GPIO_Init(void);


uint16_t brightness = 0;
int main(void)
{

	HAL_Init();
	SystemClock_Config();
	GPIO_Init();

	TIMER2_Init();

	if(HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_handler();
	}



	while(1)
	{
		while(brightness < htim2.Init.Period)
		{
			brightness +=20;
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,brightness);
			HAL_Delay(1);
			//__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,brightness);
			//HAL_Delay(1);
		}
		while(brightness > 0)
		{
			brightness = brightness -20;
			__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,brightness);
			HAL_Delay(1);
			//__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,brightness);
			//HAL_Delay(1);
		}
	}
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
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
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
	TIM_OC_InitTypeDef timPWM_init;

	htim2.Instance = TIM2;
	htim2.Init.Period = 7999;
	htim2.Init.Prescaler = 4;

	if(HAL_TIM_PWM_Init(&htim2) != HAL_OK)
	{
		Error_handler();
	}

	timPWM_init.OCMode = TIM_OCMODE_PWM1;
	timPWM_init.OCPolarity = TIM_OCPOLARITY_HIGH;

	timPWM_init.Pulse   = 0;
	if(HAL_TIM_PWM_ConfigChannel(&htim2, &timPWM_init, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_handler();
	}

}



void Error_handler(void)
{

}
