/*
 * button.c
 *
 *  Created on: Dec 31, 2023
 *      Author: Dave
 */

#include "button.h"
#include "main.h"

bool debounce() {
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
