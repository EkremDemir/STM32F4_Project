/*
 * main.c
 *
 *  Created on: Sep 11, 2022
 *      Author: ekrm5
 */

#include <string.h>
#include "stm32f4xx_hal.h"
#include "main.h"


void GPIO_Init(void);
void Error_handler(void);
void TIMER2_Init(void);
void TIMER6_Init(void);
void LSE_Configuration(void);

void SystemClock_Config(void);

TIM_HandleTypeDef htimer2;
TIM_HandleTypeDef htimer6;


uint32_t input_captures[2] = {0};
uint8_t count=1;
volatile uint8_t is_capture_done = FALSE;
uint32_t capture_difference =0;
double timer2_cnt_freq=0;
double timer2_cnt_res=0;
double user_signal_time_period =0;
double user_signal_freq=0;
char usr_msg[100];


int main(void)
{

	HAL_Init();

	//SystemClockConfig(SYS_CLOCK_FREQ_50_MHZ);

	SystemClock_Config();

	GPIO_Init();



	TIMER6_Init();

	TIMER2_Init();

	LSE_Configuration();

    HAL_TIM_Base_Start_IT(&htimer6);

	HAL_TIM_IC_Start_IT(&htimer2,TIM_CHANNEL_1);

	while(1)
	{
		if(is_capture_done)
		{
			if(input_captures[1] > input_captures[0])
				capture_difference = input_captures[1] - input_captures[0];
			else
				capture_difference = (0XFFFFFFFF -input_captures[0]) + input_captures[1];


		timer2_cnt_freq = (HAL_RCC_GetPCLK1Freq() * 2 ) / (htimer2.Init.Prescaler + 1);
		timer2_cnt_res = 1/ timer2_cnt_freq;
		user_signal_time_period = capture_difference * timer2_cnt_res;
		user_signal_freq = 1/user_signal_time_period ;


		is_capture_done = FALSE;

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
	GPIO_InitTypeDef ledgpio;
	ledgpio.Pin = GPIO_PIN_5;
	ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
	ledgpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA,&ledgpio);
}






 void TIMER2_Init(void)
{
	 TIM_IC_InitTypeDef timer2IC_Config;

	 htimer2.Instance = TIM2;
	 htimer2.Init.CounterMode = TIM_COUNTERMODE_UP;
	 htimer2.Init.Period = 0xFFFFFFFF;
	 htimer2.Init.Prescaler = 1;
	 if ( HAL_TIM_IC_Init(&htimer2) != HAL_OK)
	 {
		 Error_handler();
	 }

	 timer2IC_Config.ICFilter = 0 ;
	 timer2IC_Config.ICPolarity = TIM_ICPOLARITY_RISING;
	 timer2IC_Config.ICPrescaler = TIM_ICPSC_DIV1;
	 timer2IC_Config.ICSelection = TIM_ICSELECTION_DIRECTTI;
	 if ( HAL_TIM_IC_ConfigChannel(&htimer2, &timer2IC_Config, TIM_CHANNEL_1) != HAL_OK)
	 {
		 Error_handler();
	 }



}


 void TIMER6_Init(void)
 {
 	//uint32_t pre =  HAL_RCC_GetHCLKFreq() /1000000;
 	 htimer6.Instance = TIM6;
     htimer6.Init.Prescaler = 0;
     htimer6.Init.Period    = 1200;
     htimer6.Init.CounterMode = TIM_COUNTERMODE_UP;

     if(HAL_TIM_Base_Init(&htimer6) != HAL_OK)
     {
    	 Error_handler();
     }

 }


void LSE_Configuration(void)
{

#if 0
	RCC_OscInitTypeDef Osc_Init;
	Osc_Init.OscillatorType = RCC_OSCILLATORTYPE_LSE;
	Osc_Init.LSEState = RCC_LSE_ON;
	if (HAL_RCC_OscConfig(&Osc_Init) != HAL_OK)
	{
		Error_handler();
	}
#endif

	HAL_RCC_MCOConfig(RCC_MCO1,RCC_MCO1SOURCE_HSI,RCC_MCODIV_4);

}


 void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
#if 1
 if(! is_capture_done)
 {
	 if(count == 1)
	 {
		 input_captures[0] = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1);
		 count++;
	 }
	 else if (count == 2)
	 {
		 input_captures[1] = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1);
		 count =1;
		 is_capture_done = TRUE;
	 }

 }
#endif

}


 /*
 void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
 {
 	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
 }
 */

void Error_handler(void)
{
	while(1);
}
