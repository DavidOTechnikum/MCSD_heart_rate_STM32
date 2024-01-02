/*
 * oledc.h
 *
 *  Created on: Dec 30, 2023
 *      Author: David Oberleitner
 */

#ifndef INC_OLEDC_H_
#define INC_OLEDC_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "main.h"
#include <stdlib.h>
#include <string.h>

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

typedef struct {
const uint8_t*   font_obj;
uint16_t         font_color;
uint8_t          font_orientation;
uint16_t         font_first_char;
uint16_t         font_last_char;
uint16_t         font_height;
uint16_t         x_cord;
uint16_t         y_cord;

} oledc_t;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void oledc_default_cfg (SPI_HandleTypeDef *hspi1);
void oledc_reset();
void oledc_one_arg_commands (uint8_t command, uint8_t args, SPI_HandleTypeDef *hspi1);
void oledc_more_arg_commands ( uint8_t command, uint8_t *args, uint16_t args_le, SPI_HandleTypeDef *hspi1 );
void oledc_fill_screen (uint16_t color, SPI_HandleTypeDef *hspi1);
void box_area (uint8_t start_col, uint8_t start_row, uint8_t end_col, uint8_t end_row, uint16_t color, SPI_HandleTypeDef *hspi1);
void oledc_text ( oledc_t *oledc, uint8_t *text, uint16_t x, uint16_t y , SPI_HandleTypeDef *hspi1);
void oledc_set_font ( oledc_t *oledc, const uint8_t *font_s, uint16_t color );
void pixel ( oledc_t *oledc, uint8_t col, uint8_t row, uint16_t color, SPI_HandleTypeDef *hspi1);
void character ( oledc_t *oledc, uint16_t ch, SPI_HandleTypeDef *hspi1);
void oledc_image( oledc_t *oledc, const uint8_t* img, uint8_t col_off, uint8_t row_off, SPI_HandleTypeDef *hspi1);
void draw_area (oledc_t *oledc, uint8_t start_col, uint8_t start_row, uint8_t end_col, uint8_t end_row, const uint8_t *img, SPI_HandleTypeDef *hspi1);
void oledc_rectangle (uint8_t col_off, uint8_t row_off, uint8_t col_end, uint8_t row_end, uint16_t color, SPI_HandleTypeDef *hspi1);
void oledc_numbers_fade(oledc_t *oledc, uint8_t* numbers, SPI_HandleTypeDef *hspi1);
void oledc_text_fade(oledc_t *oledc, uint8_t* text, SPI_HandleTypeDef *hspi1);
void oledc_update_number(oledc_t *oledc, uint8_t* numbers, SPI_HandleTypeDef *hspi1, TIM_HandleTypeDef *htim);
void oledc_change_mode(oledc_t *oledc,  uint8_t *numbers, uint8_t *text, SPI_HandleTypeDef *hspi1);


/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define OLED_RST_Pin GPIO_PIN_14
#define OLED_RST_GPIO_Port GPIOC
#define OLED_RW_Pin GPIO_PIN_0
#define OLED_RW_GPIO_Port GPIOA
#define VCP_TX_Pin GPIO_PIN_2
#define VCP_TX_GPIO_Port GPIOA
#define OLED_EN_Pin GPIO_PIN_3
#define OLED_EN_GPIO_Port GPIOA
#define OLED_CS_Pin GPIO_PIN_0
#define OLED_CS_GPIO_Port GPIOB
#define OLED_DC_Pin GPIO_PIN_1
#define OLED_DC_GPIO_Port GPIOB
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define VCP_RX_Pin GPIO_PIN_15
#define VCP_RX_GPIO_Port GPIOA
#define OLED_SCK_Pin GPIO_PIN_3
#define OLED_SCK_GPIO_Port GPIOB
#define OLED_SDO_Pin GPIO_PIN_4
#define OLED_SDO_GPIO_Port GPIOB
#define OLED_SDI_Pin GPIO_PIN_5
#define OLED_SDI_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/**
 * \defgroup error_code Error Code
 * \{
 */
#define OLEDC_RETVAL  uint8_t

#define OLEDC_OK           0x00
#define OLEDC_INIT_ERROR   0xFF
/** \} */

/**
 * \defgroup font_dir Font Direction
 * \{
 */
#define OLEDC_FO_HORIZONTAL       0x00
#define OLEDC_FO_VERTICAL         0x01
#define OLEDC_FO_VERTICAL_COLUMN  0x02
/** \} */

/**
 * \defgroup remamp_set Remamp Settings
 * \{
 */
#define OLEDC_RMP_INC_HOR         0x00
#define OLEDC_RMP_INC_VER         0x01
#define OLEDC_RMP_COLOR_NOR       0x00
#define OLEDC_RMP_COLOR_REV       0x02
#define OLEDC_RMP_SEQ_RGB         0x00
#define OLEDC_RMP_SEQ_BGR         0x04
#define OLEDC_RMP_SCAN_NOR        0x00
#define OLEDC_RMP_SCAN_REV        0x10
#define OLEDC_RMP_SPLIT_DISABLE   0x00
#define OLEDC_RMP_SPLIT_ENABLE    0x20
#define OLEDC_COLOR_65K           0x00
#define OLEDC_COLOR_262K          0x80
#define OLEDC_IMG_HEAD            0x06
/** \} */

/**
 * \defgroup dev_proper Device Properties
 * \{
 */
#define OLEDC_SCREEN_WIDTH    0x60
#define OLEDC_SCREEN_HEIGHT   0x60
#define OLEDC_SCREEN_SIZE     0x2400
#define OLEDC_ROW_OFF         0x00
#define OLEDC_COL_OFF         0x10
/** \} */

/**
 * \defgroup commands Commands
 * \{
 */
#define OLEDC_SET_COL_ADDRESS   0x15
#define OLEDC_SET_ROW_ADDRESS   0x75
#define OLEDC_WRITE_RAM         0x5C
#define OLEDC_READ_RAM          0x5D
#define OLEDC_SET_REMAP         0xA0
#define OLEDC_SET_START_LINE    0xA1
#define OLEDC_SET_OFFSET        0xA2
#define OLEDC_MODE_OFF          0xA4
#define OLEDC_MODE_ON           0xA5
#define OLEDC_MODE_NORMAL       0xA6
#define OLEDC_MODE_INVERSE      0xA7
#define OLEDC_FUNCTION          0xAB
#define OLEDC_SLEEP_ON          0xAE
#define OLEDC_SLEEP_OFF         0xAF
#define OLEDC_NOP               0xB0
#define OLEDC_SET_RESET_PRECH   0xB1
#define OLEDC_ENHANCEMENT       0xB2
#define OLEDC_CLOCK_DIV         0xB3
#define OLEDC_VSL               0xB4
#define OLEDC_GPIO              0xB5
#define OLEDC_SETSEC_PRECH      0xB6
#define OLEDC_GREY_SCALE        0xB8
#define OLEDC_LUT               0xB9
#define OLEDC_PRECH_VOL         0xBB
#define OLEDC_VCOMH             0xBE
#define OLEDC_CONTRAST          0xC1
#define OLEDC_MASTER_CONTRAST   0xC7
#define OLEDC_MUX_RATIO         0xCA
#define OLEDC_COMMAND_LOCK      0xFD
#define OLEDC_SCROLL_HOR        0x96
#define OLEDC_START_MOV         0x9E
#define OLEDC_STOP_MOV          0x9F


#define OLEDC_DEFAULT_START_LINE 0x00
#define OLEDC_DEFAULT_MUX_RATIO  95
#define OLEDC_DEFAULT_ 0x80
#define OLEDC_DEFAULT_OFFSET     0x20

#define OLEDC_DEFAULT_OLED_LOCK     0x12
#define OLEDC_DEFAULT_CMD_LOCK      0xB1
#define OLEDC_DEFAULT_DIVSET        0xF1
#define OLEDC_DEFAULT_PRECHARGE     0x32
#define OLEDC_DEFAULT_VCOMH         0x05
#define OLEDC_DEFAULT_MASTER_CONT   0xCF
#define OLEDC_DEFAULT_PRECHARGE_2   0x01
/** \} */

#define OLEDC_DUMMY 0
#define OLEDC_MAX 96

//static uint8_t cols[ 2 ]    = { OLEDC_COL_OFF, OLEDC_COL_OFF + 95 };
//static uint8_t rows[ 2 ]    = { OLEDC_ROW_OFF, OLEDC_ROW_OFF + 95 };
//
//static uint8_t OLEDC_DEFAULT_REMAP = OLEDC_RMP_INC_HOR | OLEDC_RMP_COLOR_REV |
//                                OLEDC_RMP_SEQ_RGB | OLEDC_RMP_SCAN_REV |
//                                OLEDC_RMP_SPLIT_ENABLE | OLEDC_COLOR_65K;
//
//static  uint8_t OLEDC_DEFAULT_VSL[ 3 ]       = { 0xA0, 0xB5, 0x55 };
//static  uint8_t OLEDC_DEFAULT_CONTRAST[ 3 ]  = { 0x8A, 0x51, 0x8A };

/* USER CODE END Private defines */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

//static oledc_t oledc;


//#define text1 "Hello"
//#define text2  "this is the demo"
//#define text3  "for OLED C click"

/* USER CODE END PV */
#endif /* INC_OLEDC_H_ */
