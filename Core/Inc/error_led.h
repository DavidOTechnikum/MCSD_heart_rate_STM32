/*
 * error_led.h
 *
 *  Created on: Jan 2, 2024
 *      Author: Dave
 */

#ifndef INC_ERROR_LED_H_
#define INC_ERROR_LED_H_

#include "stm32l4xx_hal.h"
#include "main.h"

#define BLINK_TIME 500
#define LOW 10
#define HIGH 99

void number_check(uint8_t number, TIM_HandleTypeDef *htim);
void error_led_blink (TIM_HandleTypeDef *htim);

#endif /* INC_ERROR_LED_H_ */
