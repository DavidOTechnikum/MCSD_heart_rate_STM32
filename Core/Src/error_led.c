/**
  ******************************************************************************
  * @file           : error_led.c
  * @brief          : Controls a red LED checking for high/low heartrate numbers.
  * 				  Timer callback function in main.c
  ******************************************************************************
  *
  *	MCSD project winter semester 2023/2024 BIC3
  *
  *	Author: David Oberleitner
  *
  *      Code for timer-based blinking sourced from Timer Task (Homework)
  *
  ******************************************************************************
  */

#include "error_led.h"
#include "main.h"

/**
 * @brief	Function starts red blinking error LED when number is too high/low.
 * 			Used for giving feedback to pulse.
 * @param	number: 	Value to be checked.
 * @param 	htim: 		Timer handle.
 * @retval	None
 */
void error_led_number_check(uint8_t number, TIM_HandleTypeDef *htim) {
	if (number < LOW || number > HIGH) {
		error_led_blink(htim);
	} else {
		HAL_TIM_Base_Stop_IT(htim);
		HAL_GPIO_WritePin(GPIOA, ERROR_LED_Pin, GPIO_PIN_SET);
	}
}

/**
 * @brief	Function starts red error LED and starts timer for blinking.
 * 			Blinking is achieved by timer and callback function.
 * @param	htim: 	Timer handle.
 * @retval	None
 */
void error_led_blink (TIM_HandleTypeDef *htim) {
	__HAL_TIM_SET_AUTORELOAD(htim, BLINK_TIME);
	__HAL_TIM_SET_COUNTER(htim, 0);
	TIM6->EGR = 1;
	__HAL_TIM_CLEAR_FLAG(htim, TIM_FLAG_UPDATE);
	HAL_TIM_Base_Start_IT(htim);
	HAL_GPIO_TogglePin(GPIOA, ERROR_LED_Pin);
}
