/*
 * Helper_Func.c
 *
 *  Created on: Oct 12, 2022
 *      Author: ekrm5
 */


#include "main.h"
#include "Helper_Func.h"
#include "string.h"
#include <stdbool.h>




void clear_Rxbuffer(void);
bool isSubString(char *str11, char *str22);
int get_id(void);
int get_number(char *str);
int map(int variable, int reference_min,int reference_max,int angle_min,int angle_max);

extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart1;



char Tx_Buffer[250] = {0};
char Rx_Buffer[100] = {'\0'};
char user_info[200];
int  Rx_index = 0;


int counter2 = 0;
char Helper_Buffer[BUFFER_SIZE] = {0};
char *read =    NULL;


bool isSubString(char *str11, char *str22)
{
  //int len1 = sizeof(str11);
  int len2 = strlen(str22);

  if(Rx_index <= len2)
  {
      Rx_index = len2;
  }

  int end = Rx_index - len2 + 1;
  if((Rx_index < len2) || (Rx_index <=0 ))
  {
      return false;
  }
  for (int i = 0; i < end; i++)
  {

    bool check_found = true;


    for (int j = 0; j < len2; j++)
    {

      if (*(str22+j) != *(str11+i + j))
      {
        check_found = false;

        break;
      }
    }

    if (check_found)
    {
        return true;
    }
  }

  return false;
}

int  get_id(void)
{
    int i = 0;
    int id = 0;
    while(1)
    {
        if(((Rx_Buffer[i]) == 'I') && ((Rx_Buffer[i+1]) == 'P') && ((Rx_Buffer[i+2]) == 'D'))
        {
             id = (int)(Rx_Buffer[i+4]);
             id = id-48;
             break;
        }
        ++i;
    }
    return id;
}



int get_number(char *str)
{
    int num = 0;
    int total = 0;
    int counter;

    for (int i = 0; i < 64; i++)
    {
        if((*(str + i) == '_') && (*(str + i + 2)=='='))
        {
            counter = i + 3;
            while (*(str + counter) >= '0'  &&  (*(str + counter) <= '9'))
            {
                num =(int) (*(str + counter));
                num -=48;
                total = total*10 + num;
                ++counter;
            }

        }
    }

    return total;
}

int map(int variable, int reference_min,int reference_max,int angle_min,int angle_max)
{
    float x;
    x = reference_min + ((float)(reference_max-reference_min)/(angle_max - angle_min)) * variable;

    return (int)x;
}


void clear_Rxbuffer(void)
{
    ++counter2;
    for(int i = 0; i<Rx_index;i++)
    {
        Rx_Buffer[i] = '\0';
    }
    Rx_index = 0;

}


