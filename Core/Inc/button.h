/*
/**
  ******************************************************************************
  * @file           : button.h
  * @brief          : Header for button.c file.
  *                   This file contains the includes, defines, and prototypes of button.c.
  ******************************************************************************
  *
  *	MCSD project winter semester 2023/2024 BIC3
  *
  *	Author: David Oberleitner
  *
  ******************************************************************************
  */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include "stm32l4xx_hal.h"
#include "main.h"
#include <stdbool.h>

#define DEBOUNCE 5

bool button_debounce();

#endif /* INC_BUTTON_H_ */
