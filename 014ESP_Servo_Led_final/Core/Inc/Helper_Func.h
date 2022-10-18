/*
 * Helper_Func.h
 *
 *  Created on: Oct 12, 2022
 *      Author: ekrm5
 */

#ifndef INC_HELPER_FUNC_H_
#define INC_HELPER_FUNC_H_

#include "stdbool.h"

/**
 * Clear the data which stored in Rxbuffer 
 */
void clear_Rxbuffer(void);

/**
 * Searches a specific text within another text
 * 
 * @param str11 takes the whole text
 * @param str22 takes the text that will be searched
 * @return failure or success
 */
bool isSubString(char *str11, char *str22);

/**
 * Clear the data which stored in Rxbuffer 
 * @return the ID number of device which is stored in Rxbuffer
 */
int get_id(void);


/**
 * Take ID number of connected device
 * @return the ID number of device which is stored in Rxbuffer
 */
int get_id(void);

/**
 * Take motor angle from received data
 * @param str11 Received data
 * @return angle of the motor
 */

int get_number(char *str);


/**
 * Convert received angle to pulse value of the PWM signal
 * 
 * @param variable the number which converted to PWM signal
 * @param reference_min the number which minimum value of PWM signal
 * @param reference_max the number which maximum value of PWM signal
 * @param angle_min the number which minimum value of received angle from the server
 * @param angle_max the number which minimum value of received angle from the server
 * @return converted number
 */
int map(int variable, int reference_min,int reference_max,int angle_min,int angle_max);


#endif /* INC_HELPER_FUNC_H_ */
