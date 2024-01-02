/*
 * oledc.c
 *
 *  Created on: Dec 30, 2023
 *      Author: David Oberleitner
 *
 *      Code adapted from example code by Mikroe https://libstock.mikroe.com/projects/view/1117/oled-c-click
 *      incl. fonts library (oledc_font.h)
 *
 *
 */
#include "oledc.h"
#include "main.h"
#include "oledc_font.h"



static uint8_t cols[ 2 ]    = { OLEDC_COL_OFF, OLEDC_COL_OFF + 95 };
static uint8_t rows[ 2 ]    = { OLEDC_ROW_OFF, OLEDC_ROW_OFF + 95 };

static uint8_t OLEDC_DEFAULT_REMAP = OLEDC_RMP_INC_HOR | OLEDC_RMP_COLOR_REV |
                                OLEDC_RMP_SEQ_RGB | OLEDC_RMP_SCAN_REV |
                                OLEDC_RMP_SPLIT_ENABLE | OLEDC_COLOR_65K;

static  uint8_t OLEDC_DEFAULT_VSL[ 3 ]       = { 0xA0, 0xB5, 0x55 };
static  uint8_t OLEDC_DEFAULT_CONTRAST[ 3 ]  = { 0x8A, 0x51, 0x8A };

void oledc_default_cfg (SPI_HandleTypeDef *hspi) {
	  // Enable CS
	  HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_RESET);

	  // Enable OLED via EN
	  HAL_GPIO_WritePin(OLED_EN_GPIO_Port, OLED_EN_Pin, GPIO_PIN_SET);

	  oledc_reset();

	  // Unlock display and turn off
	oledc_one_arg_commands(OLEDC_COMMAND_LOCK, OLEDC_DEFAULT_OLED_LOCK, hspi);
	oledc_one_arg_commands(OLEDC_COMMAND_LOCK, OLEDC_DEFAULT_CMD_LOCK, hspi);
	oledc_more_arg_commands(OLEDC_SLEEP_ON,  0,  0 , hspi);

	// Setup SSD1351
	oledc_one_arg_commands(OLEDC_SET_REMAP,       OLEDC_DEFAULT_REMAP        , hspi);
	oledc_one_arg_commands(OLEDC_MUX_RATIO,       OLEDC_DEFAULT_MUX_RATIO    , hspi);
	oledc_one_arg_commands(OLEDC_SET_START_LINE,  OLEDC_DEFAULT_START_LINE   , hspi);
	oledc_one_arg_commands(OLEDC_SET_OFFSET,      OLEDC_DEFAULT_OFFSET       , hspi);
	oledc_one_arg_commands(OLEDC_VCOMH,           OLEDC_DEFAULT_VCOMH        , hspi);
	oledc_one_arg_commands(OLEDC_CLOCK_DIV,       OLEDC_DEFAULT_DIVSET       , hspi);
	oledc_one_arg_commands(OLEDC_SET_RESET_PRECH, OLEDC_DEFAULT_PRECHARGE    , hspi);
	oledc_one_arg_commands(OLEDC_SETSEC_PRECH,    OLEDC_DEFAULT_PRECHARGE_2  , hspi);
	oledc_one_arg_commands(OLEDC_MASTER_CONTRAST, OLEDC_DEFAULT_MASTER_CONT  , hspi);

	oledc_more_arg_commands(OLEDC_CONTRAST,   OLEDC_DEFAULT_CONTRAST, 3 , hspi);
	oledc_more_arg_commands(OLEDC_VSL,        OLEDC_DEFAULT_VSL,      3 , hspi);

	// Set normal mode and turn on display
	oledc_more_arg_commands(OLEDC_MODE_NORMAL, 0, 0 , hspi);
	oledc_more_arg_commands(OLEDC_SLEEP_OFF,   0, 0 , hspi);
	oledc_fill_screen(0 , hspi);
}

void oledc_reset() {
	  HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_SET);
	  HAL_Delay(1);
	  HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_RESET);
	  HAL_Delay(1);
	  HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_SET);
	  HAL_Delay(100);
}

void oledc_one_arg_commands (uint8_t command, uint8_t args, SPI_HandleTypeDef *hspi) {
    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit_IT(hspi, &command, 1);
    HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET);
    HAL_SPI_Transmit_IT(hspi, &args, 1);
    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_SET);
}

void oledc_more_arg_commands ( uint8_t command, uint8_t *args, uint16_t args_len, SPI_HandleTypeDef *hspi) {
    uint16_t cnt;
    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit_IT(hspi, &command, 1);
    HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET);

    for ( cnt = 0; cnt < args_len; cnt++ )
    {
        HAL_SPI_Transmit_IT(hspi, &args[cnt], 1);
    }
    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_SET);
}

void oledc_fill_screen (uint16_t color, SPI_HandleTypeDef *hspi) {
    oledc_box_area(OLEDC_MIN, OLEDC_MIN, OLEDC_MAX, OLEDC_MAX, color, hspi);
}

void oledc_box_area (uint8_t start_col, uint8_t start_row, uint8_t end_col, uint8_t end_row, uint16_t color, SPI_HandleTypeDef *hspi) {
    uint8_t   cmd       = OLEDC_WRITE_RAM;
    uint16_t  cnt       = ( end_col - start_col ) * ( end_row - start_row );
    uint8_t   clr[ 2 ]  = { 0 };

    if( ( start_col > OLEDC_SCREEN_WIDTH ) ||
        ( end_col > OLEDC_SCREEN_WIDTH ) )
        return;

    if( ( start_row > OLEDC_SCREEN_HEIGHT ) ||
        ( end_row > OLEDC_SCREEN_HEIGHT ) )
        return;

    if( ( end_col < start_col ) ||
        ( end_row < start_row ) )
        return;

    cols[ 0 ] = OLEDC_COL_OFF + start_col;
    cols[ 1 ] = OLEDC_COL_OFF + end_col - 1;
    rows[ 0 ] = OLEDC_ROW_OFF + start_row;
    rows[ 1 ] = OLEDC_ROW_OFF + end_row - 1;
    clr[ 0 ] |= color >> 8;
    clr[ 1 ] |= color & 0x00FF;

    oledc_more_arg_commands(OLEDC_SET_COL_ADDRESS, cols, 2 , hspi);
    oledc_more_arg_commands(OLEDC_SET_ROW_ADDRESS, rows, 2 , hspi);
    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit_IT(hspi, &cmd, 1);
    HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET);

    while( cnt-- ) {
        HAL_SPI_Transmit_IT(hspi, &clr[0], 1);
        HAL_SPI_Transmit_IT(hspi, &clr[1], 1);
    }
    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_SET);
}

void oledc_text ( oledc_t *oledc, uint8_t *text, uint16_t x, uint16_t y , SPI_HandleTypeDef *hspi) {
    uint8_t *ptr = text;
    if ( ( x >= OLEDC_SCREEN_WIDTH ) || ( y >= OLEDC_SCREEN_HEIGHT ) ) {
        return;
    }

    oledc->x_cord = x;
    oledc->y_cord = y;

    while( *ptr ) {
        oledc_character( oledc, *ptr++ , hspi);
    }
}

void oledc_set_font ( oledc_t *oledc, const uint8_t *font_s, uint16_t color ) {
    oledc->font_obj               = font_s;
    oledc->font_first_char    = font_s[2] + (font_s[3] << 8);
    oledc->font_last_char     = font_s[4] + (font_s[5] << 8);
    oledc->font_height        = font_s[6];
    oledc->font_color         = color;
}

void oledc_pixel ( oledc_t *oledc, uint8_t col, uint8_t row, uint16_t color, SPI_HandleTypeDef *hspi) {
    uint8_t cmd       = OLEDC_WRITE_RAM;
    uint8_t clr[ 2 ]  = { 0 };

    if( ( col > OLEDC_SCREEN_WIDTH ) || ( row > OLEDC_SCREEN_HEIGHT ) ) {
        return;
    }
    cols[ 0 ] = OLEDC_COL_OFF + col;
    cols[ 1 ] = OLEDC_COL_OFF + col;
    rows[ 0 ] = OLEDC_ROW_OFF + row;
    rows[ 1 ] = OLEDC_ROW_OFF + row;
    clr[ 0 ] |= color >> 8;
    clr[ 1 ] |= color & 0x00FF;

    oledc_more_arg_commands(OLEDC_SET_COL_ADDRESS, cols, 2 , hspi);
    oledc_more_arg_commands(OLEDC_SET_ROW_ADDRESS, rows, 2 , hspi);
    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit_IT(hspi, &cmd, 1);
    HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET);
    HAL_SPI_Transmit_IT(hspi, &clr[0], 1);
    HAL_SPI_Transmit_IT(hspi, &clr[1], 1);
    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_SET);
}

void oledc_character (oledc_t *oledc, uint16_t ch , SPI_HandleTypeDef *hspi) {
    uint8_t     ch_width = 0;
    uint8_t     x_cnt;
    uint8_t     y_cnt;
    uint16_t    x = 0;
    uint16_t    y = 0;
    uint16_t    tmp;
    uint8_t     temp = 0;
    uint8_t     mask = 0;
    uint32_t    offset;
    const uint8_t *ch_table;
    const uint8_t *ch_bitmap;

    if( ch < oledc->font_first_char )
        return;

    if( ch > oledc->font_last_char )
        return;

    offset = 0;
    tmp = (ch - oledc->font_first_char) << 2;
    ch_table = oledc->font_obj + 8 + tmp;
    ch_width = *ch_table;

    offset = (uint32_t)ch_table[1] + ((uint32_t)ch_table[2] << 8) + ((uint32_t)ch_table[3] << 16);

    ch_bitmap = oledc->font_obj + offset;

    y = oledc->y_cord;
    for (y_cnt = 0; y_cnt < oledc->font_height; y_cnt++) {
        x = oledc->x_cord;
        mask = 0;
        for( x_cnt = 0; x_cnt < ch_width; x_cnt++ ) {
            if( !mask ) {
                temp = *ch_bitmap++;
                mask = 0x01;
            }
            if( temp & mask )
                 oledc_pixel( oledc, x, y, oledc->font_color, hspi);

            x++;
            mask <<= 1;
        }
        y++;
    }
    oledc->x_cord = x + 1;
}

//void oledc_image( oledc_t *oledc, const uint8_t* img, uint8_t col_off, uint8_t row_off, SPI_HandleTypeDef *hspi) {
//    const uint8_t *ptr = img;
//    draw_area( oledc, col_off, row_off, col_off + ptr[2], row_off + ptr[4], ptr, hspi);
//}
//
//void draw_area (oledc_t *ctx, uint8_t start_col, uint8_t start_row, uint8_t end_col, uint8_t end_row, const uint8_t *img, SPI_HandleTypeDef *hspi) {
//    uint16_t    tmp  = 0;
//    uint8_t     cmd  = OLEDC_WRITE_RAM;
//    uint8_t     frb  = 0;
//    uint8_t     srb  = 0;
//    uint16_t    cnt  = ( end_col - start_col ) * ( end_row - start_row );
//
//    const uint8_t*  ptr = img + OLEDC_IMG_HEAD;
//
//    if( ( start_col > OLEDC_SCREEN_WIDTH ) ||
//        ( end_col > OLEDC_SCREEN_WIDTH ) )
//        return;
//
//    if( ( start_row > OLEDC_SCREEN_HEIGHT ) ||
//        ( end_row > OLEDC_SCREEN_HEIGHT ) )
//        return;
//
//    if( ( end_col < start_col ) ||
//        ( end_row < start_row ) )
//        return;
//
//    cols[ 0 ] = OLEDC_COL_OFF + start_col;
//    cols[ 1 ] = OLEDC_COL_OFF + end_col - 1;
//    rows[ 0 ] = OLEDC_ROW_OFF + start_row;
//    rows[ 1 ] = OLEDC_ROW_OFF + end_row - 1;
//
//    oledc_more_arg_commands(OLEDC_SET_COL_ADDRESS, cols, 2, hspi);
//    oledc_more_arg_commands(OLEDC_SET_ROW_ADDRESS, rows, 2, hspi);
//    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_RESET);
//    HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET);
//    HAL_SPI_Transmit_IT(hspi, &cmd, 1);
//    HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET);
//
//    while( cnt-- ) {
//        frb = ptr[ tmp + 1 ];
//        srb = ptr[ tmp ];
//        HAL_SPI_Transmit_IT(hspi, &frb, 1);
//        HAL_SPI_Transmit_IT(hspi, &srb, 1);
//
//        tmp += 2;
//    }
//    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_SET);
//}

//void oledc_rectangle (uint8_t col_off, uint8_t row_off, uint8_t col_end, uint8_t row_end, uint16_t color, SPI_HandleTypeDef *hspi) {
//    box_area(col_off, row_off, col_end, row_end, color, hspi);
//}

void oledc_numbers_fade(oledc_t *oledc, uint8_t* numbers, SPI_HandleTypeDef *hspi) {
	  for (int i = NUM_ROW; i <= OLEDC_MAX; i += NUM_FADE) {
		  if (i+NUM_OFFSET < OLEDC_MAX) {
			  oledc_box_area(NUM_COL, i, NUM_ROW_END, i+NUM_OFFSET, BACKGROUND, hspi);
		  } else {
			  oledc_box_area(NUM_COL, i, NUM_ROW_END, OLEDC_MAX, BACKGROUND, hspi);
		  }
		  oledc_text(oledc, numbers, NUM_COL, i, hspi);
		  HAL_Delay(200);
	  }
}

void oledc_text_fade(oledc_t *oledc, uint8_t* text, SPI_HandleTypeDef *hspi) {
	  for (int i = TEXT_COL; i <= OLEDC_MAX; i += TEXT_FADE) {
		  oledc_box_area(i-TEXT_FADE, TEXT_ROW, OLEDC_MAX, TEXT_ROW_END, BACKGROUND, hspi);
		  oledc_text(oledc, text, i, TEXT_ROW, hspi);
		  HAL_Delay(100);
	  }
}

void oledc_update_number(oledc_t *oledc, uint8_t* numbers, SPI_HandleTypeDef *hspi, TIM_HandleTypeDef *htim) {
	oledc_box_area (NUM_COL, NUM_ROW, NUM_COL_END, NUM_ROW_END, BACKGROUND, hspi);
	oledc_text(oledc, numbers, NUM_COL, NUM_ROW, hspi);
	char *ptr;
	uint8_t number = strtol((char*)numbers, &ptr, 10);
	error_led_number_check(number, htim);
}


void oledc_change_mode(oledc_t *oledc, uint8_t *numbers, uint8_t *text, SPI_HandleTypeDef *hspi) {
	uint8_t pulse[] = "PULSE";
	uint8_t oxygen[] = "OXYGEN";

	if (strcmp((char*)text, (char*)pulse) == 0) {
		oledc_numbers_fade(oledc, numbers, hspi);
		oledc_text_fade(oledc, text, hspi);
		oledc_text(oledc, numbers, NUM_COL, NUM_ROW, hspi);
		oledc_text(oledc, oxygen, TEXT_COL, TEXT_ROW, hspi);
	} else {
		oledc_numbers_fade(oledc, numbers, hspi);
		oledc_text_fade(oledc, text, hspi);
		oledc_text(oledc, numbers, NUM_COL, NUM_ROW, hspi);
		oledc_text(oledc, pulse, TEXT_COL, TEXT_ROW, hspi);
	}
}

void oledc_error_message(oledc_t *oledc, SPI_HandleTypeDef *hspi) {
	uint8_t error_message[] = "ERROR";
	oledc_set_font(oledc, guiFont_Exo_2_Condensed15x23_Regular, 0);
	oledc_fill_screen(ERROR_BACKGROUND, hspi);
	oledc_text(oledc, error_message, ERROR_TEXT_COL, TEXT_ROW, hspi);
}

void oledc_start_screen(bool startup, oledc_t *oledc, SPI_HandleTypeDef *hspi) {
	uint8_t welcome[] = "WELCOME";
	uint8_t pulse[] = "PULSE";
	oledc_set_font(oledc, guiFont_Tahoma_14_Regular, 0);

	if (startup) {
		oledc_fill_screen(WELCOME_BACKGROUND, hspi);
		oledc_text(oledc, welcome, WELCOME_TEXT_COL, TEXT_ROW, hspi);
		HAL_Delay(1000);
	}

	oledc_fill_screen(BACKGROUND, hspi);
	oledc_text(oledc, pulse, TEXT_COL, TEXT_ROW, hspi);
}
