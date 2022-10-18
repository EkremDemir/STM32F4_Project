/*
 * main.c
 *
 *  Created on: Sep 11, 2022
 *      Author: ekrm5
 */

#include "main.h"

TIM_HandleTypeDef htim2;


void SystemClock_Config(void);
void Error_handler(void);

//void LSE_Configuration(void);
void TIMER2_Init(void);
void GPIO_Init(void);

uint32_t numDifference = 0;

double timer2_cnt_freq=0;
double timer2_cnt_res=0;
double user_signal_time_period =0;
volatile double user_signal_freq=0;
uint32_t input_captures[2] = {0};
uint8_t count = 1;

uint8_t cnt = 0;
volatile uint8_t is_capture_done = FALSE;


int main(void)
{

	HAL_Init();
	SystemClock_Config();
	GPIO_Init();

	TIMER2_Init();

	HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_2);

	uint32_t temp = HAL_RCC_GetHCLKFreq();
	temp = HAL_RCC_GetPCLK1Freq();
	temp = HAL_RCC_GetPCLK2Freq();
	temp = HAL_RCC_GetSysClockFreq();

	(void)temp;

	while(1)
	{
		cnt  = TIM2->CNT;
		(void)cnt;
		if(is_capture_done)
		{
			if(input_captures[1] > input_captures[0])
			{
				numDifference = input_captures[1] - input_captures[0];
			}
			else
			{
				numDifference = (0xFFFFFFFF  - input_captures[0]) + input_captures[1];
			}

			timer2_cnt_freq = (HAL_RCC_GetPCLK1Freq() * 2 ) / (htim2.Init.Prescaler + 1);
			timer2_cnt_res = 1/ timer2_cnt_freq;
			user_signal_time_period = numDifference * timer2_cnt_res;
			user_signal_freq = 1/user_signal_time_period ;

			is_capture_done = FALSE;
		}

	}


//	LSE_Configuration();

	while(1);
	return 0;
}

void SystemClock_Config(void)
{
  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSE, RCC_MCODIV_5);
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
/*
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 50;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
*/
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
	  Error_handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV4;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
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
	TIM_IC_InitTypeDef timer2ICconfig;

	htim2.Instance = TIM2;

	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period      = 0xFFFFFFFF-1;
	htim2.Init.Prescaler   = 1;

	if(HAL_TIM_IC_Init(&htim2) != HAL_OK)
	{
		Error_handler();
	}

	timer2ICconfig.ICPolarity = TIM_ICPOLARITY_RISING;
	timer2ICconfig.ICPrescaler = TIM_ICPSC_DIV1;
	timer2ICconfig.ICSelection = TIM_ICSELECTION_DIRECTTI;
	timer2ICconfig.ICFilter    = 0;
	HAL_TIM_IC_ConfigChannel(&htim2, &timer2ICconfig, TIM_CHANNEL_2);
}


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
#if 1
	if(!is_capture_done)
	{
		if(count == 1)
		{
			input_captures[0] = __HAL_TIM_GET_COMPARE(htim,TIM_CHANNEL_2);
			count++;
		}
		else if(count == 2)
		{
			input_captures[1] = __HAL_TIM_GET_COMPARE(htim,TIM_CHANNEL_2);
			count = 1;
			is_capture_done = TRUE;
		}
	}
#endif
}

void Error_handler(void)
{

}
