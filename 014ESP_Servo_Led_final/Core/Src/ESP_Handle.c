/*
 * ESP_Handle.c
 *
 *  Created on: Oct 12, 2022
 *      Author: ekrm5
 */


#include <string.h>
#include "main.h"
#include "ESP_Handle.h"
#include "Helper_Func.h"



extern char Tx_Buffer[250];
extern char Rx_Buffer[100];
extern char user_info[200];
extern int  Rx_index;


void ESP_Init(void);
void ESP_Server_Start(void);
int ESP_Server_Send(char *str, int id);

extern TIM_HandleTypeDef htim2;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart1;

char *Basic_inclusion = "<!DOCTYPE HTML><html><head>\
    <title>ESP Input Form</title>\
    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\
    </head><body>\
    <br><a href=\" ?pin=on\"><button type=\"button\"> LED ON</button></a><br>\
    <br><a href=\" ?pin=off\"><button type=\"button\">LED OFF</button></a><br><br>\
    <form action=\"/get\">\
      Motor_1 angle: <input type=\"text\" name=\"motor_a\">\
      <input type=\"submit\" value=\"Submit\">\
    </form><br>\
    <form action=\"/get\">\
      Motor_2 angle: <input type=\"text\" name=\"motor_b\">\
      <input type=\"submit\" value=\"Submit\">\
    </form><br>\
  </body></html>";




#define ssid "eko"
#define psw  "ekosyf666"
int angle;
int previousAngle_a = 0;
int previousAngle_b = 0;

int difference;

char dataSend[1024] = {0};



void ESP_Init(void)
{

/***********RESETTING**************************************************/
    sprintf(Tx_Buffer, "AT+RST\r\n"); //Reset the ESP8266
    HAL_UART_Transmit_IT(&huart1,(uint8_t*)Tx_Buffer, strlen(Tx_Buffer));
    HAL_Delay(3000); //Wait 2 second to reset the esp

        /***************INFO***************************/
    sprintf(user_info,"RESETTING\r\n");
    HAL_UART_Transmit_IT(&huart2,(uint8_t*)user_info, strlen(user_info));
    HAL_Delay(100);

    clear_Rxbuffer();


/**************************CHECK it is run or not**************************************************/
    sprintf(Tx_Buffer, "AT\r\n");//wait until esp work properly
    HAL_UART_Transmit_IT(&huart1,(uint8_t*)Tx_Buffer, strlen(Tx_Buffer));
    //HAL_Delay(500);
    while(!isSubString(Rx_Buffer, "OK"));

    clear_Rxbuffer(); // clear garbage value of received from esp

     /***************INFO***************************/
    sprintf(user_info,"AT---->OK\r\n");
    HAL_UART_Transmit_IT(&huart2,(uint8_t*)user_info, strlen(user_info));
    HAL_Delay(100);


/**************************Set ESP as client mode**************************************************/
    sprintf(Tx_Buffer, "AT+CWMODE=1\r\n");//wait until esp work properly
    HAL_UART_Transmit_IT(&huart1,(uint8_t*)Tx_Buffer, strlen(Tx_Buffer));
    while(!isSubString(Rx_Buffer, "OK"));
    clear_Rxbuffer();

     /***************INFO***************************/
    sprintf(user_info,"CW MODE---->1\r\n");
    HAL_UART_Transmit_IT(&huart2,(uint8_t*)user_info, strlen(user_info));
    HAL_Delay(100);


    /**************************Set ESP as client mode**************************************************/
    sprintf(Tx_Buffer, "AT+CIFSR\r\n");//wait until esp work properly
    HAL_UART_Transmit_IT(&huart1,(uint8_t*)Tx_Buffer, strlen(Tx_Buffer));
    while(!isSubString(Rx_Buffer, "OK"));
    HAL_Delay(100);
    HAL_UART_Transmit_IT(&huart2,(uint8_t*)Rx_Buffer, BUFFER_SIZE);
    HAL_Delay(100);
    clear_Rxbuffer();


/**************************Handle the WIFI connection**************************************************/
    sprintf(Tx_Buffer, "AT+CWJAP=\"%s\",\"%s\"\r\n",ssid,psw);
    HAL_UART_Transmit_IT(&huart1,(uint8_t*)Tx_Buffer, strlen(Tx_Buffer));
    //HAL_Delay(5000); //wait 5 seconds for connection of ESP with WIFI
    while(!isSubString(Rx_Buffer, "OK"));
    clear_Rxbuffer(); // clear garbage value of received from esp

     /***************INFO***************************/
    sprintf(user_info, "AT+CWJAP=\"%s\",\"%s\"\r\n",ssid,psw);
    HAL_UART_Transmit_IT(&huart2,(uint8_t*)user_info, strlen(user_info));
    HAL_Delay(100);
    sprintf(user_info,"Connected\r\n");
    HAL_UART_Transmit_IT(&huart2,(uint8_t*)user_info, strlen(user_info));
    HAL_Delay(100);


/**************************Enable Multiple Connection**************************************************/
    sprintf(Tx_Buffer, "AT+CIPMUX=1\r\n");//wait until esp work properly
    HAL_UART_Transmit_IT(&huart1,(uint8_t*)Tx_Buffer, strlen(Tx_Buffer));
    //HAL_Delay(500);
    while(!isSubString(Rx_Buffer, "OK"));

    clear_Rxbuffer();

     /***************INFO***************************/
    sprintf(user_info,"CIPMUX---->OK\r\n");
    HAL_UART_Transmit_IT(&huart2,(uint8_t*)user_info, strlen(user_info));
    HAL_Delay(100);




/**************************Enable Multiple Connection**************************************************/
    sprintf(Tx_Buffer, "AT+CIPSERVER=1,80\r\n");//wait until esp work properly
    HAL_UART_Transmit_IT(&huart1,(uint8_t*)Tx_Buffer, strlen(Tx_Buffer));
    //HAL_Delay(500);
    while(!isSubString(Rx_Buffer, "OK"));
    //state = true;
    clear_Rxbuffer();

    /***************INFO***************************/
    sprintf(user_info,"CIPSERVER---->OK\r\n");
    HAL_UART_Transmit_IT(&huart2,(uint8_t*)user_info, strlen(user_info));
    HAL_Delay(500);
    sprintf(user_info,"All necessary initialization are complete....\r\n");
    HAL_UART_Transmit_IT(&huart2,(uint8_t*)user_info, strlen(user_info));


    clear_Rxbuffer();

    sprintf(dataSend, Basic_inclusion);




}


void ESP_Server_Start(void)
{
    //HAL_UART_Receive_IT(&huart1,(uint8_t*)TempRx, 1);
    int id = 0;

    while(!isSubString(Rx_Buffer, "HTTP"));
    id = get_id();
    //while()



    HAL_UART_Transmit_IT(&huart2,(uint8_t*)Rx_Buffer,BUFFER_SIZE);


    if(isSubString(Rx_Buffer, "pin=on"))
    {
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);


    }
    else if(isSubString(Rx_Buffer, "pin=off"))
    {
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);



    }
    else if(isSubString(Rx_Buffer, "motor_a"))
    {
        angle = get_number(Rx_Buffer);
        angle = map(angle, 180, 300, 30, 120);

        difference = angle - previousAngle_a;
        if(difference < 0)
        {
            difference *= -1;
            while(difference > 0)
            {
                __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,angle  + difference);
                HAL_Delay(10);
                --difference;
            }
            previousAngle_a = angle;
        }
        else
        {

            while(difference > 0)
            {
                __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,angle - difference);
                HAL_Delay(10);
                --difference;
            }
            previousAngle_a = angle;
        }
    }
    else if(isSubString(Rx_Buffer, "motor_b"))
    {

        angle = get_number(Rx_Buffer);
        angle = map(angle, 130, 470, 0, 180);

        difference = angle - previousAngle_b;
        if(difference < 0)
        {
            difference *= -1;
            while(difference > 0)
            {
                __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,angle  + difference);
                HAL_Delay(10);
                --difference;
            }
            previousAngle_b = angle;
        }
        else
        {

            while(difference > 0)
            {
                __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,angle - difference);
                HAL_Delay(10);
                --difference;
            }
            previousAngle_b = angle;
        }

    }

    else if((isSubString(Rx_Buffer, "favicon.ico")) == 1)
    {
        HAL_Delay(50);
        clear_Rxbuffer();

    }

    else
    {
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);


    }


    ESP_Server_Send(dataSend,id);



}


int ESP_Server_Send(char *str, int id)
{
    int len = strlen(str);
    char data[100] = {0};
    clear_Rxbuffer();

    HAL_UART_Transmit_IT(&huart2,(uint8_t*)Rx_Buffer,BUFFER_SIZE);
    sprintf(data, "AT+CIPSEND=%d,%d\r\n",id,len);
    HAL_Delay(50);
    clear_Rxbuffer();
    HAL_UART_Transmit_IT(&huart1,(uint8_t*)data, strlen(data));
    HAL_Delay(50);
    while(!isSubString(Rx_Buffer,">"))
    {

        if(isSubString(Rx_Buffer,"ERROR"))
        {
            HAL_UART_Transmit_IT(&huart1,(uint8_t*)str, strlen(data));
            HAL_Delay(50);
            break;
        }
    }
    //while(!isSubString(Rx_Buffer,">"));
    HAL_UART_Transmit_IT(&huart2,(uint8_t*)Rx_Buffer,BUFFER_SIZE);
    HAL_Delay(50);
    clear_Rxbuffer();


    HAL_UART_Transmit_IT(&huart1,(uint8_t*)str, strlen(str));
    HAL_Delay(50);
    while(!isSubString(Rx_Buffer,"SEND OK"))
    {

        if(isSubString(Rx_Buffer,"ERROR"))
        {
            HAL_UART_Transmit_IT(&huart1,(uint8_t*)str, strlen(str));
            HAL_Delay(50);
            break;
        }
    }
    //HAL_UART_Transmit_IT(&huart1,(uint8_t*)str, strlen(str));
    //while(!isSubString(Rx_Buffer,"SEND OK"));
    HAL_UART_Transmit_IT(&huart2,(uint8_t*)Rx_Buffer,BUFFER_SIZE);
    HAL_Delay(50);




    clear_Rxbuffer();
    sprintf(data, "AT+CIPCLOSE=%d\r\n",id);
    HAL_UART_Transmit_IT(&huart1,(uint8_t*)data, strlen(data));
    while(!isSubString(Rx_Buffer,"OK"))
    {

        if(isSubString(Rx_Buffer,"ERROR"))
        {
            HAL_UART_Transmit_IT(&huart1,(uint8_t*)str, strlen(data));
            HAL_Delay(50);
            break;
        }
    }
    //while(!isSubString(Rx_Buffer,"OK"));
    HAL_UART_Transmit_IT(&huart2,(uint8_t*)Rx_Buffer,BUFFER_SIZE);
    clear_Rxbuffer();
    return 1;

}

