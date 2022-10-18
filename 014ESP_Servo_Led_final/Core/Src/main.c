/*
 * main.c
 *
 *  Created on: Sep 11, 2022
 *      Author: ekrm5
 */

#include "main.h"
#include "ESP_Handle.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>





TIM_HandleTypeDef htim2;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart1;
uint32_t ccr_content;

void SystemClock_Config(void);
void Error_handler(void);

//Timer initialization for PWM  feature
void TIMER2_Init(void);

//UART initialization for serial communication of ESP8266 wifi module
void UART1_Init(void);

//UART initialization for serial communication of USB-TTL coverter. 
//Purpose of USB-TTL is to observe the flow of the code
void UART2_Init(void);

//GPIO initialization of pin for LEDs
void GPIO_Init(void);



char TempRx[1] = {0};
bool state = false;

//holds data of WIFI module
extern char Rx_Buffer[100];
extern int  Rx_index;
char *temp = NULL;



int main(void)
{

	HAL_Init();
	SystemClock_Config();
	GPIO_Init();

	TIMER2_Init();

	UART2_Init();
	UART1_Init();
	
	//Start PWM for channel 1 and channel 2
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	
	//Start the interrput for serial communication
	HAL_UART_Receive_IT(&huart1,(uint8_t*)TempRx, 1);
	
	//Initialize Wifi module for "AT" commands of WIFI
	ESP_Init();

	while(1)
	{
		//Start the communication and process the data 
		ESP_Server_Start();

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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
	  Error_handler();
  }
}




void GPIO_Init(void)
{
	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_InitTypeDef Ledgpio;

	Ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
	Ledgpio.Pin  = GPIO_PIN_12;
	Ledgpio.Speed = GPIO_SPEED_LOW;
	Ledgpio.Pull  = GPIO_NOPULL;

	HAL_GPIO_Init(GPIOD,&Ledgpio);



}




void TIMER2_Init(void)
{
	TIM_OC_InitTypeDef timPWM_init;

	htim2.Instance = TIM2;
	htim2.Init.Period = 3599;
	htim2.Init.Prescaler = 221;

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

	timPWM_init.Pulse   = 0;
	if(HAL_TIM_PWM_ConfigChannel(&htim2, &timPWM_init, TIM_CHANNEL_2) != HAL_OK)
	{
		Error_handler();
	}

}

void UART2_Init(void)
{
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits   = UART_STOPBITS_1;
	huart2.Init.Parity     = UART_PARITY_NONE;
	huart2.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	huart2.Init.Mode       = UART_MODE_TX_RX;

	if(HAL_UART_Init(&huart2) != HAL_OK)
	{
		Error_handler();
	}
}

void UART1_Init(void)
{
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits   = UART_STOPBITS_1;
	huart1.Init.Parity     = UART_PARITY_NONE;
	huart1.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	huart1.Init.Mode       = UART_MODE_TX_RX;

	if(HAL_UART_Init(&huart1) != HAL_OK)
	{
		Error_handler();
	}
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart1)
	{
		if((TempRx[0] != '\0') && (Rx_index < BUFFER_SIZE))
		{
			*(Rx_Buffer + (++Rx_index)) = TempRx[0];
		}

		HAL_UART_Receive_IT(huart,(uint8_t*)TempRx, 1);

	}
}

void Error_handler(void)
{

}
