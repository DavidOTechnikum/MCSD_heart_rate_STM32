/*
 * error_led.h
 *
 *  Created on: Jan 2, 2024
 *      Author: David Oberleitner
 *
 *      Code for timer-based blinking sourced from Timer Task (Homework)
 */

#ifndef INC_ERROR_LED_H_
#define INC_ERROR_LED_H_

#include "stm32l4xx_hal.h"
#include "main.h"

#define BLINK_TIME 500
#define LOW 50
#define HIGH 74

void error_led_number_check(uint8_t number, TIM_HandleTypeDef *htim);
void error_led_blink (TIM_HandleTypeDef *htim);

#endif /* INC_ERROR_LED_H_ */
