#include "pico.h"
// Remove the dependency on i_video.h
// #include "i_video.h"

// Define the necessary constants directly in this file
#ifndef SCREENWIDTH
#define SCREENWIDTH 256
#endif

#ifndef SCREENHEIGHT
#define SCREENHEIGHT 240
#endif

#ifndef DOWNSAMPLING_FACTOR
#define DOWNSAMPLING_FACTOR 2.0
#endif

// Define these as macros instead of variable constants
#define DOWNSAMPLED_WIDTH (SCREENWIDTH / DOWNSAMPLING_FACTOR)
#define DOWNSAMPLED_HEIGHT (SCREENHEIGHT / DOWNSAMPLING_FACTOR)

// Use a fixed size buffer - 128 is SCREENWIDTH/2 which should be sufficient
#define MAX_DOWNSAMPLED_WIDTH 128
static uint16_t buffer[MAX_DOWNSAMPLED_WIDTH];

void downsample_scanline(uint16_t *src, uint16_t *dest) {
    for (uint8_t x = 0; x < DOWNSAMPLED_WIDTH; x++) {
      // Calculate the range of source pixels to average for this destination pixel
      uint8_t start = (uint8_t)(x * DOWNSAMPLING_FACTOR);
      uint8_t end = (uint8_t)((x + 1) * DOWNSAMPLING_FACTOR);
      // we don't just use DOWNSAMPLING_FACTOR for this because it might be like 2.6 and right now we only do integer downsampling
      uint8_t total = end - start;

      // Accumulate the sum of the source pixels in this range
      uint32_t r_sum = 0, g_sum = 0, b_sum = 0;
      for (uint8_t current_pixel = start; current_pixel < end; current_pixel++) {
        r_sum += (src[current_pixel] & 0b1111100000000000) >> 11;
        g_sum += (src[current_pixel] & 0b0000011111100000) >> 5;
        b_sum += (src[current_pixel] & 0b0000000000011111);
      }

      dest[x] = ((r_sum / total) << 11) | ((g_sum / total) << 5) | (b_sum / total);
    }
}