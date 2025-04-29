#ifdef __cplusplus
extern "C" {
#endif

#ifndef __SCREEN_SHARED__
#define __SCREEN_SHARED__

#include "pico.h"
#include "../config.h"

// Screen dimensions - using configurable values
#ifndef SCREENWIDTH
#define SCREENWIDTH PICO_SCREENS_WIDTH
#endif

#ifndef SCREENHEIGHT 
#define SCREENHEIGHT PICO_SCREENS_HEIGHT
#endif

#define DOOM_WIDTH SCREENWIDTH 
#define DOOM_HEIGHT SCREENHEIGHT

// Downsampling factor (percentage) - using configurable value
#ifndef DOWNSAMPLING_FACTOR_OUT_OF_100
#define DOWNSAMPLING_FACTOR_OUT_OF_100 PICO_SCREENS_DOWNSAMPLING_FACTOR
#endif

#define DOWNSAMPLED_WIDTH ((SCREENWIDTH * 100 / DOWNSAMPLING_FACTOR_OUT_OF_100))
#define DOWNSAMPLED_HEIGHT ((SCREENHEIGHT * 100 / DOWNSAMPLING_FACTOR_OUT_OF_100))

#define DOWNSAMPLING_OFFSET_WIDTH ((SCREENWIDTH - (SCREENWIDTH * 100 / DOWNSAMPLING_FACTOR_OUT_OF_100)) >> 1)
#define DOWNSAMPLING_OFFSET_HEIGHT ((SCREENHEIGHT - (SCREENHEIGHT * 100 / DOWNSAMPLING_FACTOR_OUT_OF_100)) >> 1)

void clearDownsampleBuffers(void);

uint16_t colorToGreyscale(uint16_t pixel);

void areaAverageHandleDownsampling(uint16_t *src, int scanline, void (*callback)(uint16_t *, int));
void areaAverageHandleFrameStart(void);

void nearestNeighborHandleDownsampling(uint16_t *src, int scanline, void (*callback)(uint16_t *, int));
void nearestNeighborHandleFrameStart(void);

#endif

#ifdef __cplusplus
}
#endif