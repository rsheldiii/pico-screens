# pico-screens Examples

Here are examples of how to use the pico-screens library with different configuration options.

## Basic Usage

```c
#include <pico-screens/screen.h>

int main() {
    // Initialize screen
    I_initScreen();
    
    // Create a buffer for a single line
    uint16_t line_buffer[SCREENWIDTH];
    
    // Main loop
    while (1) {
        // Signal the start of a new frame
        I_handleFrameStart(frame_counter);
        
        // For each line of the screen
        for (int y = 0; y < SCREENHEIGHT; y++) {
            // Fill the line buffer with some pattern
            for (int x = 0; x < SCREENWIDTH; x++) {
                // Example: create a gradient pattern
                line_buffer[x] = ((x * 31) / SCREENWIDTH) << 11 | 
                                 ((y * 63) / SCREENHEIGHT) << 5 | 
                                 ((x+y) * 31) / (SCREENWIDTH+SCREENHEIGHT);
            }
            
            // Send the line to the screen
            I_handleScanline(line_buffer, y);
        }
        
        // Signal the end of the frame
        I_handleFrameEnd(frame_counter++);
    }
}
```

## Configuring via CMake

In your project's CMakeLists.txt:

```cmake
# Include the pico-screens library
add_subdirectory(pico-screens)

# Configure screen dimensions and downsampling
set(PICO_SCREENS_WIDTH 320)
set(PICO_SCREENS_HEIGHT 240)
set(PICO_SCREENS_DOWNSAMPLING_FACTOR 150)

# Your application
add_executable(my_app main.c)

target_link_libraries(my_app PRIVATE
    pico_stdlib
    pico-screens
)
```

## Configuring via Preprocessor Defines

In your application code, before including any pico-screens headers:

```c
#define PICO_SCREENS_WIDTH 160
#define PICO_SCREENS_HEIGHT 128 
#define PICO_SCREENS_DOWNSAMPLING_FACTOR 200

#include <pico-screens/screen.h>

int main() {
    // Your code here
}
```

## Hardware-specific Configuration

For different hardware setups, you can use conditional compilation:

```cmake
# In CMakeLists.txt
if(DEFINED HARDWARE_PROFILE)
    if(HARDWARE_PROFILE STREQUAL "PICO_DISPLAY")
        # Pico Display Pack configuration
        set(PICO_SCREENS_WIDTH 240)
        set(PICO_SCREENS_HEIGHT 135)
        set(PICO_SCREENS_DOWNSAMPLING_FACTOR 100)
        
    elseif(HARDWARE_PROFILE STREQUAL "TINY_DISPLAY")
        # Small ST7735 display configuration
        set(PICO_SCREENS_WIDTH 128)
        set(PICO_SCREENS_HEIGHT 128)
        set(PICO_SCREENS_DOWNSAMPLING_FACTOR 200)
        
    elseif(HARDWARE_PROFILE STREQUAL "HDMI_OUT")
        # HDMI output configuration
        set(PICO_SCREENS_WIDTH 640)
        set(PICO_SCREENS_HEIGHT 480)
        set(PICO_SCREENS_DOWNSAMPLING_FACTOR 100)
    endif()
endif()
```

Then build with:

```bash
cmake -DHARDWARE_PROFILE=TINY_DISPLAY ..
``` 