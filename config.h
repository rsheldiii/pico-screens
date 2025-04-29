/**
 * pico-screens configuration
 * 
 * This file defines the default configuration values for pico-screens.
 * Users can override these values by defining them before including pico-screens headers.
 */

#ifndef PICO_SCREENS_CONFIG_H
#define PICO_SCREENS_CONFIG_H

// Screen dimensions - can be overridden by user definitions
#ifndef PICO_SCREENS_WIDTH
#define PICO_SCREENS_WIDTH 255
#endif

#ifndef PICO_SCREENS_HEIGHT
#define PICO_SCREENS_HEIGHT 240
#endif

// Downsampling factor (percentage)
#ifndef PICO_SCREENS_DOWNSAMPLING_FACTOR
#define PICO_SCREENS_DOWNSAMPLING_FACTOR 199
#endif

#endif /* PICO_SCREENS_CONFIG_H */ 