/*
 * button.h
 *
 *  Created on: Dec 31, 2023
 *      Author: Dave
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include "stm32l4xx_hal.h"
#include "main.h"
#include <stdbool.h>

#define DEBOUNCE 5

bool button_debounce();

#endif /* INC_BUTTON_H_ */
