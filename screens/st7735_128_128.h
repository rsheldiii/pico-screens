// DESCRIPTION:
// LED / LCD screen-specific logic

#ifndef __ST7735_128_128__
#define __ST7735_128_128__

#include "shared.h"

#include "pico.h"

#include <stdlib.h>
#include "mipi_display.h"

#define LCD_WIDTH 128
#define LCD_HEIGHT 128

#define SCREEN_WIDTH_OFFSET ((LCD_WIDTH - (SCREENWIDTH * 100 / DOWNSAMPLING_FACTOR_OUT_OF_100)) / 2)
#define SCREEN_HEIGHT_OFFSET ((LCD_HEIGHT - (SCREENHEIGHT * 100 / DOWNSAMPLING_FACTOR_OUT_OF_100)) / 2)


void st7735_128_128_initScreen(void);
void st7735_128_128_handleFrameStart(uint8_t frame);
void st7735_128_128_handleScanline(uint16_t *line, int scanline);
uint16_t doSomeBullshit(uint16_t color);
uint16_t swapBytesInPixel(uint16_t color);
uint16_t rgb555_to_rgb565(uint16_t color);

#endif