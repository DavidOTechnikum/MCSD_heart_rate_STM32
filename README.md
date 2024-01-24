# MCSD_heart_rate_STM32
AUTHORS: David OBERLEITNER, Johannes SEITZ

## SUMMARY:

Module 1, Input: MAX30101 High-Sensitivity Pulse Oximeter and Heart-Rate Sensor for Wearable Health
This module is used to measure and send vitality data regarding the heart rate and oxygen saturation using infrared and photodiodes. It is equipped with a proximity sensor and a sensor to measure the internal temperature. It is accessible through an I2C interface.
Module 2, Output: OLED C v101 click 96x96 pixel OLED Display (PSP27801, SSD1351 Controller)
This module is a 96x96 full-graphic display using organic light emitting diodes. Communication is realized through an SPI interface.
---

## Hardware

+ STM32L432KC
+ MikroE Heart rate 4 click v100
+ MikroE OLED C click board v101


## Circuitry

+ OLED C on <1>
+ Heart rate on <2>
+ Button: 3V3 wire on A6
---

## Pins

### The following pins are used for OledC/SPI:

+ PB5/D11: OLED_SDI
+ PB4/D12: OLED_SDO
+ PB3/D13: OLED_SCK
+ PA0/A0: OLED_RW
+ PA3/A2: OLED_EN
+ PB0/D3: OLED_CS
+ PB1/D6: OLED_DC
+ PC14/D7: OLED_RST

### The following pins are used for HeartRate/I2C:

### The following pin is used for Button: 

+ PA7/A6 (GPIO in)

### The following pin is used for Error LED (RGB red):

+ PA6/A5
---

## Timers

The following timers are used for this project: 

+ TIM6 for Error LED
---


## Notes and Known Issues
Output works
Input problems: I2C communication provides no data. 
---


