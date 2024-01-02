/*
 * error_led.c
 *
 *  Created on: Jan 2, 2024
 *      Author: Dave
 */

#include "error_led.h"
#include "main.h"

void number_check(uint8_t number, TIM_HandleTypeDef *htim) {
	uint8_t low = 10;
	uint8_t high = 99;

	if (number < low || number > high) {
		error_led_blink(htim);
	} else {
		HAL_TIM_Base_Stop_IT(htim);
		HAL_GPIO_WritePin(GPIOA, ERROR_LED_Pin, GPIO_PIN_SET);
	}
}

void error_led_blink (TIM_HandleTypeDef *htim) {
	__HAL_TIM_SET_AUTORELOAD(htim, BLINK_TIME);
	__HAL_TIM_SET_COUNTER(htim, 0);
	TIM6->EGR = 1;
	__HAL_TIM_CLEAR_FLAG(htim, TIM_FLAG_UPDATE);
	HAL_TIM_Base_Start_IT(htim);
	HAL_GPIO_TogglePin(GPIOA, ERROR_LED_Pin);
}
