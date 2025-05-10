#include "pico/stdlib.h"
#include "st7735_128_128.h"

// lifted from hagl_hal_single.c
static void put_pixel(int16_t x0, int16_t y0, color_t color)
{
    mipi_display_write(x0, y0, 1, 1, (uint8_t *) &color);
}

// Function to convert a single RGB555 color to RGB565
// and swap the byte order
uint16_t doSomeBullshit(uint16_t color) {
    uint16_t temp = 0;
    temp |= ((color & 0b0000000001100000) << 9); // grab least significant 2 green bits and bit shift to convert from RGB555
    // temp is currently 0b
    temp |= ((color & 0b0000000000011111) << 8); // grab 5 blue bits
    temp |= ((color & 0b0111111110000000) >> 7); // grab 5 red bits and 3 green bits. MSB is 0 in RGB555
    // bits look like GGGBBBBB RRRRRGGG
    return temp & 0b1110111101111011; // remove top bit from green and red, bottom from blue. the color is blown out otherwise for some reason
}
uint16_t rgb555_to_rgb565(uint16_t color) {
    uint16_t r = (color & 0x7C00) >> 10; // Extract red component (5 bits)
    uint16_t g = (color & 0x03E0) >> 5;  // Extract green component (5 bits)
    uint16_t b = (color & 0x001F);       // Extract blue component (5 bits)

    // Convert RGB555 to RGB565
    uint16_t rgb565 = (r << 11) | (g << 1) | b;

    return rgb565;
}    


    // just swap the bytes in each pixel
uint16_t swapBytesInPixel(uint16_t color) {
    return (((color) << 8) & 0xFF00) | (((color) >> 8) & 0xFF);
}

static void blit(int16_t x0, int16_t y0, uint16_t width, uint16_t height, uint16_t *src)
{
    // swap the bytes in each pixel
    for (uint16_t i = 0; i < width * height; i++) {
        // src[i] = doSomeBullshit(src[i]);   // TODO temporary, reinstate
        // src[i] = rgb555_to_rgb565(src[i]); 
        src[i] = swapBytesInPixel(src[i]);
    }
    mipi_display_write(x0, y0, width, height, (uint8_t *) src);
}




void st7735_128_128_initScreen(void) {

    mipi_display_init();

    // for(uint8_t y = 0; y < 80; y++) {
    //     blit(x,y,1,1, data);
    // }
    // initialize 128 pixel row to blue
    uint16_t lcd_row[LCD_WIDTH];

    // fill blue row with 0b0001111100000000
    for (uint8_t i = 0; i < LCD_WIDTH; i++) {
        lcd_row[i] = 0b1111111111111111;
    }

    for(uint8_t y = 0; y < LCD_HEIGHT; y++) {
        blit(0, y, LCD_WIDTH, 1, lcd_row);
    }

    sleep_ms(500);

    // fill blue row with 0b0001111100000000
    for (uint8_t i = 0; i < LCD_WIDTH; i++) {
        lcd_row[i] = 0b0001111100000000;
    }

    for(uint8_t y = 0; y < LCD_HEIGHT; y++) {
        blit(0, y, LCD_WIDTH, 1, lcd_row);
    }

    sleep_ms(500);
}

void st7735_128_128_handleFrameStart(uint8_t frame) {
    // areaAverageHandleFrameStart();
    nearestNeighborHandleFrameStart();
}

void st7735_128_128_blit(uint16_t *downsampled_line, int scanline) {
    // make random pixel white
    // put_pixel(rand() % LCD_WIDTH, scanline, 0b1111111111111111);

    // for (uint8_t x = 0; x < DOWNSAMPLED_WIDTH; x++) {
    //     uint16_t color = downsampled_line[x];
    //     // st7735 expects least significant byte first, but the normal msb / lsb order in each byte
    //     downsampled_line[x] = (((color) << 8) & 0xFF00) | (((color) >> 8) & 0xFF);
    //     // put_pixel(x, scanline, downsampled_line[x]);
    // }
    // blit(0, scanline, 85, 1, downsampled_line);
    blit(SCREEN_WIDTH_OFFSET,  SCREEN_HEIGHT_OFFSET + scanline,DOWNSAMPLED_WIDTH, 1, downsampled_line);
}

void downsample_3x(uint16_t* src, uint16_t* dest) {
    for (uint8_t i = 0; i < 255; i+=3) {
        dest[i/3] = src[i];
    }
}

void st7735_128_128_handleScanline(uint16_t *line, int scanline) {
    // bootstrap downsampling method
    // uint16_t downsampled_line[85] = {0}; // TODO  2x magic number
    // downsample_3x(line, downsampled_line);
    // blit(0,scanline,85,1, downsampled_line); // known "working" but not actually working
    
    // pixel-by-pixel method
    // for (uint8_t i = 0; i < 255; i+=3) {
        // put_pixel(i/3, scanline, line[i]);    
    // }
    // put_pixel(rand() % 128, rand() % 128, 0b1111111111111111);
    nearestNeighborHandleDownsampling(line, scanline, st7735_128_128_blit);
    // areaAverageHandleDownsampling(line, scanline, st7735_128_128_blit);

    // // bootstrap downsampling method
    // uint16_t downsampled_line[85] = {0}; // TODO2x magic number
    // downsample_3x(line, downsampled_line);
    // blit(0,scanline,85,1, downsampled_line); // known "working" but not actually working
    // put_pixel(rand() % LCD_WIDTH, rand() % 80, 0b1111111111111111);
}