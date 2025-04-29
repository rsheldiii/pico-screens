# pico-screens

A library for handling different screen types on the Raspberry Pi Pico.

## Configuration

This library has several configuration options that can be set to target different hardware:

### Screen Dimensions

You can configure the screen dimensions in one of three ways:

1. **CMake Variables** - Set the parameters when calling cmake:
   ```
   cmake -DPICO_SCREENS_WIDTH=320 -DPICO_SCREENS_HEIGHT=240 ..
   ```

2. **Preprocessor Defines** - Define these before including any pico-screens headers:
   ```c
   #define PICO_SCREENS_WIDTH 320
   #define PICO_SCREENS_HEIGHT 240
   #include <pico-screens/screen.h>
   ```

3. **Direct Override** - Define the actual constants used by the library:
   ```c
   #define SCREENWIDTH 320
   #define SCREENHEIGHT 240
   #include <pico-screens/screen.h>
   ```

### Downsampling Factor

The downsampling factor controls how much the source image is scaled. Higher values mean more downsampling (smaller output).

Set it as a percentage (e.g., 300 means the output will be 1/3 the size of the input):

```c
#define PICO_SCREENS_DOWNSAMPLING_FACTOR 300
```

or via CMake:

```
cmake -DPICO_SCREENS_DOWNSAMPLING_FACTOR=300 ..
```

## Supported Screens

The library currently supports the following screens:
- ST7735 (128x128)
- More screens to be added

## Usage

Include the library in your CMakeLists.txt:

```cmake
target_link_libraries(your_app PRIVATE pico-screens)
```

Initialize the screen in your code:

```c
#include <pico-screens/screen.h>

int main() {
    // Initialize hardware
    // ...
    
    // Initialize screen
    I_initScreen();
    
    while (1) {
        // Get your frame data
        uint16_t* frameData = get_frame_data();
        
        // Start a new frame
        I_handleFrameStart(frameCounter);
        
        // Send each scanline
        for (int y = 0; y < SCREENHEIGHT; y++) {
            I_handleScanline(&frameData[y * SCREENWIDTH], y);
        }
        
        // End the frame
        I_handleFrameEnd(frameCounter++);
    }
}
``` 