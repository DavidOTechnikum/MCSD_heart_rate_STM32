/*
 * oledc.c
 *
 *  Created on: Dec 30, 2023
 *      Author: David Oberleitner
 *
 *      Code adapted from example code by Mikroe https://libstock.mikroe.com/projects/view/1117/oled-c-click
 *      incl. fonts library (oledc_font.h)
 */
#include "oledc.h"
#include "main.h"

static uint8_t cols[ 2 ]    = { OLEDC_COL_OFF, OLEDC_COL_OFF + 95 };
static uint8_t rows[ 2 ]    = { OLEDC_ROW_OFF, OLEDC_ROW_OFF + 95 };

static uint8_t OLEDC_DEFAULT_REMAP = OLEDC_RMP_INC_HOR | OLEDC_RMP_COLOR_REV |
                                OLEDC_RMP_SEQ_RGB | OLEDC_RMP_SCAN_REV |
                                OLEDC_RMP_SPLIT_ENABLE | OLEDC_COLOR_65K;

static  uint8_t OLEDC_DEFAULT_VSL[ 3 ]       = { 0xA0, 0xB5, 0x55 };
static  uint8_t OLEDC_DEFAULT_CONTRAST[ 3 ]  = { 0x8A, 0x51, 0x8A };

void oledc_default_cfg (SPI_HandleTypeDef *hspi1) {
	  // Enable CS
	  HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_RESET);

	  // Enable OLED via EN
	  HAL_GPIO_WritePin(OLED_EN_GPIO_Port, OLED_EN_Pin, GPIO_PIN_SET);

	  oledc_reset();

	  // Unlock display and turn off
	oledc_one_arg_commands(OLEDC_COMMAND_LOCK, OLEDC_DEFAULT_OLED_LOCK, hspi1);
	oledc_one_arg_commands(OLEDC_COMMAND_LOCK, OLEDC_DEFAULT_CMD_LOCK, hspi1);
	oledc_more_arg_commands(OLEDC_SLEEP_ON,  0,  0 , hspi1);

	// Setup SSD1351
	oledc_one_arg_commands(OLEDC_SET_REMAP,       OLEDC_DEFAULT_REMAP        , hspi1);
	oledc_one_arg_commands(OLEDC_MUX_RATIO,       OLEDC_DEFAULT_MUX_RATIO    , hspi1);
	oledc_one_arg_commands(OLEDC_SET_START_LINE,  OLEDC_DEFAULT_START_LINE   , hspi1);
	oledc_one_arg_commands(OLEDC_SET_OFFSET,      OLEDC_DEFAULT_OFFSET       , hspi1);
	oledc_one_arg_commands(OLEDC_VCOMH,           OLEDC_DEFAULT_VCOMH        , hspi1);
	oledc_one_arg_commands(OLEDC_CLOCK_DIV,       OLEDC_DEFAULT_DIVSET       , hspi1);
	oledc_one_arg_commands(OLEDC_SET_RESET_PRECH, OLEDC_DEFAULT_PRECHARGE    , hspi1);
	oledc_one_arg_commands(OLEDC_SETSEC_PRECH,    OLEDC_DEFAULT_PRECHARGE_2  , hspi1);
	oledc_one_arg_commands(OLEDC_MASTER_CONTRAST, OLEDC_DEFAULT_MASTER_CONT  , hspi1);

	oledc_more_arg_commands(OLEDC_CONTRAST,   OLEDC_DEFAULT_CONTRAST, 3 , hspi1);
	oledc_more_arg_commands(OLEDC_VSL,        OLEDC_DEFAULT_VSL,      3 , hspi1);

	// Set normal mode and turn on display
	oledc_more_arg_commands(OLEDC_MODE_NORMAL, 0, 0 , hspi1);
	oledc_more_arg_commands(OLEDC_SLEEP_OFF,   0, 0 , hspi1);
	oledc_fill_screen(0 , hspi1);
}

void oledc_reset() {
	  HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_SET);
	  HAL_Delay(1);
	  HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_RESET);
	  HAL_Delay(1);
	  HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_SET);
	  HAL_Delay(100);
}

void oledc_one_arg_commands (uint8_t command, uint8_t args, SPI_HandleTypeDef *hspi1) {
    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit_IT(hspi1, &command, 1);
    HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET);
    HAL_SPI_Transmit_IT(hspi1, &args, 1);
    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_SET);
}

void oledc_more_arg_commands ( uint8_t command, uint8_t *args, uint16_t args_len, SPI_HandleTypeDef *hspi1) {
    uint16_t cnt;
    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit_IT(hspi1, &command, 1);
    HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET);

    for ( cnt = 0; cnt < args_len; cnt++ )
    {
        HAL_SPI_Transmit_IT(hspi1, &args[cnt], 1);
    }
    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_SET);
}

void oledc_fill_screen (uint16_t color, SPI_HandleTypeDef *hspi1) {
    box_area(0, 0, 96, 96, color , hspi1);
}

void box_area (uint8_t start_col, uint8_t start_row, uint8_t end_col, uint8_t end_row, uint16_t color, SPI_HandleTypeDef *hspi1) {
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

    oledc_more_arg_commands(OLEDC_SET_COL_ADDRESS, cols, 2 , hspi1);
    oledc_more_arg_commands(OLEDC_SET_ROW_ADDRESS, rows, 2 , hspi1);
    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit_IT(hspi1, &cmd, 1);
    HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET);

    while( cnt-- ) {
        HAL_SPI_Transmit_IT(hspi1, &clr[0], 1);
        HAL_SPI_Transmit_IT(hspi1, &clr[1], 1);
    }
    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_SET);
}

void oledc_text ( oledc_t *ctx, uint8_t *text, uint16_t x, uint16_t y , SPI_HandleTypeDef *hspi1) {
    uint8_t *ptr = text;
    if ( ( x >= OLEDC_SCREEN_WIDTH ) || ( y >= OLEDC_SCREEN_HEIGHT ) ) {
        return;
    }

    ctx->x_cord = x;
    ctx->y_cord = y;

    while( *ptr ) {
        character( ctx, *ptr++ , hspi1);
    }
}

void oledc_set_font ( oledc_t *ctx, const uint8_t *font_s, uint16_t color ) {
    ctx->font_obj               = font_s;
    ctx->font_first_char    = font_s[2] + (font_s[3] << 8);
    ctx->font_last_char     = font_s[4] + (font_s[5] << 8);
    ctx->font_height        = font_s[6];
    ctx->font_color         = color;
}

void pixel ( oledc_t *ctx, uint8_t col, uint8_t row, uint16_t color, SPI_HandleTypeDef *hspi1) {
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

    oledc_more_arg_commands(OLEDC_SET_COL_ADDRESS, cols, 2 , hspi1);
    oledc_more_arg_commands(OLEDC_SET_ROW_ADDRESS, rows, 2 , hspi1);
    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit_IT(hspi1, &cmd, 1);
    HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET);
    HAL_SPI_Transmit_IT(hspi1, &clr[0], 1);
    HAL_SPI_Transmit_IT(hspi1, &clr[1], 1);
    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_SET);
}

void character ( oledc_t *ctx, uint16_t ch , SPI_HandleTypeDef *hspi1) {
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

    if( ch < ctx->font_first_char )
        return;

    if( ch > ctx->font_last_char )
        return;

    offset = 0;
    tmp = (ch - ctx->font_first_char) << 2;
    ch_table = ctx->font_obj + 8 + tmp;
    ch_width = *ch_table;

    offset = (uint32_t)ch_table[1] + ((uint32_t)ch_table[2] << 8) + ((uint32_t)ch_table[3] << 16);

    ch_bitmap = ctx->font_obj + offset;

    y = ctx->y_cord;
    for (y_cnt = 0; y_cnt < ctx->font_height; y_cnt++) {
        x = ctx->x_cord;
        mask = 0;
        for( x_cnt = 0; x_cnt < ch_width; x_cnt++ ) {
            if( !mask ) {
                temp = *ch_bitmap++;
                mask = 0x01;
            }
            if( temp & mask )
                 pixel( ctx, x, y, ctx->font_color, hspi1);

            x++;
            mask <<= 1;
        }
        y++;
    }
    ctx->x_cord = x + 1;
}

void oledc_image( oledc_t *ctx, const uint8_t* img, uint8_t col_off, uint8_t row_off, SPI_HandleTypeDef *hspi1) {
    const uint8_t *ptr = img;
    draw_area( ctx, col_off, row_off, col_off + ptr[2], row_off + ptr[4], ptr, hspi1);
}

void draw_area (oledc_t *ctx, uint8_t start_col, uint8_t start_row, uint8_t end_col, uint8_t end_row, const uint8_t *img, SPI_HandleTypeDef *hspi1) {
    uint16_t    tmp  = 0;
    uint8_t     cmd  = OLEDC_WRITE_RAM;
    uint8_t     frb  = 0;
    uint8_t     srb  = 0;
    uint16_t    cnt  = ( end_col - start_col ) * ( end_row - start_row );

    const uint8_t*  ptr = img + OLEDC_IMG_HEAD;

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

    oledc_more_arg_commands(OLEDC_SET_COL_ADDRESS, cols, 2, hspi1);
    oledc_more_arg_commands(OLEDC_SET_ROW_ADDRESS, rows, 2, hspi1);
    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit_IT(hspi1, &cmd, 1);
    HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET);

    while( cnt-- ) {
        frb = ptr[ tmp + 1 ];
        srb = ptr[ tmp ];
        HAL_SPI_Transmit_IT(hspi1, &frb, 1);
        HAL_SPI_Transmit_IT(hspi1, &srb, 1);

        tmp += 2;
    }
    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_SET);
}

void oledc_rectangle (uint8_t col_off, uint8_t row_off, uint8_t col_end, uint8_t row_end, uint16_t color, SPI_HandleTypeDef *hspi1) {
    box_area(col_off, row_off, col_end, row_end, color, hspi1);
}

void oledc_numbers_fade(oledc_t *oledc, uint8_t* numbers, SPI_HandleTypeDef *hspi1) {
	  for (int i = 40; i <= OLEDC_MAX; i += 5) {
		  if (i+14 < OLEDC_MAX) {
			  oledc_rectangle(40, i, 70, i+14, 0xF800, hspi1);
		  } else {
			  oledc_rectangle(40, i, 70, 96, 0xF800, hspi1);
		  }
		  oledc_text(oledc, numbers, 40, i, hspi1);
		  HAL_Delay(200);
	  }
}

void oledc_text_fade(oledc_t *oledc, uint8_t* text, SPI_HandleTypeDef *hspi1) {
	  for (int i = 20; i <= OLEDC_MAX; i += 4) {
		  oledc_rectangle(i-4, 20, 96, 39, 0xF800, hspi1);
		  oledc_text(oledc, text, i, 20, hspi1);
		  HAL_Delay(100);
	  }
}

void oledc_update_number(oledc_t *oledc, uint8_t* numbers, SPI_HandleTypeDef *hspi1, TIM_HandleTypeDef *htim) {
	oledc_rectangle (40, 40, 70, 70, 0xF800, hspi1);
	oledc_text(oledc, numbers, 40, 40, hspi1);
	char *ptr;
	uint8_t number = strtol((char*)numbers, &ptr, 10);
	number_check(number, htim);
}


void oledc_change_mode(oledc_t *oledc, uint8_t *numbers, uint8_t *text, SPI_HandleTypeDef *hspi1) {
	uint8_t pulse[] = "PULSE";
	uint8_t oxygen[] = "OXYGEN";

	if (strcmp((char*)text, (char*)pulse) == 0) {
		oledc_numbers_fade(oledc, numbers, hspi1);
		oledc_text_fade(oledc, text, hspi1);
		oledc_text(oledc, numbers, 40, 40, hspi1);
		oledc_text(oledc, oxygen, 20, 20, hspi1);
	} else {
		oledc_numbers_fade(oledc, numbers, hspi1);
		oledc_text_fade(oledc, text, hspi1);
		oledc_text(oledc, numbers, 40, 40, hspi1);
		oledc_text(oledc, pulse, 20, 20, hspi1);
	}
}
