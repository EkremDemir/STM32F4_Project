/*
 * main.c
 *
 *  Created on: Sep 14, 2022
 *      Author: ekrm5
 */


#include "main.h"
#include <string.h>
#include "stdbool.h"

void SystemClockConfig(void);
void UART2_Init(void);
void Error_Handler(void);
void GPIO_Init(void);

UART_HandleTypeDef huart2;

char rx_buffer[3],tx_buffer[50];
char info1_buffer[50] = "Led is on\n";
char info2_buffer[50] = "Led is off\n";
char info3_buffer[50] = "Led already is on\n";
char info4_buffer[50] = "Led already is off\n";

bool led_state = false;


int main(void)
{
	HAL_Init();
	SystemClockConfig();

	GPIO_Init();
	UART2_Init();

	HAL_UART_Receive_IT(&huart2,(uint8_t*) rx_buffer, sizeof(rx_buffer));



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
	  RCC_OscInitStruct.PLL.PLLN = 100;
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

	  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
	  {
	    Error_Handler();
	  }
}


void UART2_Init(void)
{
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 9600;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits   = UART_STOPBITS_1;
	huart2.Init.Parity     = UART_PARITY_NONE;
	huart2.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	huart2.Init.Mode       = UART_MODE_TX_RX;

	if(HAL_UART_Init(&huart2) != HAL_OK)
	{
		Error_Handler();
	}
}


void GPIO_Init(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef Ledgpio;

	Ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
	Ledgpio.Pin  = GPIO_PIN_5;
	Ledgpio.Speed = GPIO_SPEED_LOW;
	Ledgpio.Pull  = GPIO_NOPULL;

	HAL_GPIO_Init(GPIOA,&Ledgpio);



}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(rx_buffer[0] == 'o' && rx_buffer[1] == 'n')
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
		HAL_Delay(10);
		if(led_state != true)
		{
			HAL_UART_Transmit(huart,(uint8_t *)info1_buffer, 50, 500);
			led_state = true;
		}
		else
		{
			HAL_UART_Transmit(huart,(uint8_t *)info3_buffer, 50, 500);
		}
		rx_buffer[0] =0;
		rx_buffer[1] =0;

	}
	else if(rx_buffer[0] == 'o' && rx_buffer[1] == 'f' && rx_buffer[2] == 'f')
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		HAL_Delay(10);
		if(led_state != false)
		{
			HAL_UART_Transmit(huart,(uint8_t *)info2_buffer, 50, 500);
			led_state = false;
		}
		else
		{
			HAL_UART_Transmit(huart,(uint8_t *)info4_buffer, 50, 500);

		}
		rx_buffer[0] =0;
		rx_buffer[1] =0;
		rx_buffer[3] =0;
	}
	HAL_UART_Receive_IT(&huart2,(uint8_t*) rx_buffer, sizeof(rx_buffer));
}

void Error_Handler(void)
{
	while(1);
}
