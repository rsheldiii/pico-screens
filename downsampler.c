#include "pico.h"
#include "screens/shared.h"
// Remove the dependency on i_video.h
// #include "i_video.h"

// Use shared.h constants instead of redefining them
// SCREENWIDTH, SCREENHEIGHT, DOWNSAMPLED_WIDTH and DOWNSAMPLED_HEIGHT are all defined there

// Buffer for processing a scanline
static uint16_t buffer[DOWNSAMPLED_WIDTH];

void downsample_scanline(uint16_t *src, uint16_t *dest) {
    for (uint8_t x = 0; x < DOWNSAMPLED_WIDTH; x++) {
      // Calculate the range of source pixels to average for this destination pixel
      // Using integer-based scaling as in shared.h
      uint16_t start = (x * DOWNSAMPLING_FACTOR_OUT_OF_100) / 100;
      uint16_t end = ((x + 1) * DOWNSAMPLING_FACTOR_OUT_OF_100) / 100;
      uint8_t total = end - start;
      
      if (total == 0) total = 1; // Avoid division by zero

      // Accumulate the sum of the source pixels in this range
      uint32_t r_sum = 0, g_sum = 0, b_sum = 0;
      for (uint16_t current_pixel = start; current_pixel < end; current_pixel++) {
        r_sum += (src[current_pixel] & 0b1111100000000000) >> 11;
        g_sum += (src[current_pixel] & 0b0000011111100000) >> 5;
        b_sum += (src[current_pixel] & 0b0000000000011111);
      }

      dest[x] = ((r_sum / total) << 11) | ((g_sum / total) << 5) | (b_sum / total);
    }
}