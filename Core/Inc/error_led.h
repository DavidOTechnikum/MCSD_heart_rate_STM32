/*
/**
  ******************************************************************************
  * @file           : error_led.h
  * @brief          : Header for error_led.c file.
  *                   This file contains the includes, defines, and prototypes of error_led.c.
  ******************************************************************************
  *
  *	MCSD project winter semester 2023/2024 BIC3
  *
  *	Author: David Oberleitner
  *
  ******************************************************************************
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
