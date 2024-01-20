/**
  ******************************************************************************
  * @file           : button.c
  * @brief          : Contains button debouncing function.
  ******************************************************************************
  *
  *	MCSD project winter semester 2023/2024 BIC3
  *
  *	Author: David Oberleitner
  *
  ******************************************************************************
  */

#include "button.h"
#include "main.h"

/**
 * @brief	Button debouncing function.
 * @param	None.
 * @retval	None.
 */
bool button_debounce() {
	GPIO_PinState button;
	uint8_t counter = 0;

	button = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7);
	while (button == 0) {
		counter++;
		if (counter > DEBOUNCE) {
			return true;
		}
	}
	return false;
}
