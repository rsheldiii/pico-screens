cmake_minimum_required(VERSION 3.13)

# Configurable options for screen dimensions and downsampling
option(PICO_SCREENS_WIDTH "Width of the screen in pixels" 255)
option(PICO_SCREENS_HEIGHT "Height of the screen in pixels" 240)
option(PICO_SCREENS_DOWNSAMPLING_FACTOR "Downsampling factor as percentage (higher = more downsampling)" 199)

# Include the hagl_hal subdirectory
add_subdirectory(hagl_hal)

# Create a pico-screens library
add_library(pico-screens INTERFACE)

target_sources(pico-screens INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/screen.c
  ${CMAKE_CURRENT_LIST_DIR}/downsampler.c
  ${CMAKE_CURRENT_LIST_DIR}/screens/shared.c
  ${CMAKE_CURRENT_LIST_DIR}/screens/st7735_128_128.c
  ${CMAKE_CURRENT_LIST_DIR}/spi/spi_buffer.cpp
)

target_include_directories(pico-screens INTERFACE 
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/util
)

# Apply the configuration options to the target instead of globally
target_compile_definitions(pico-screens INTERFACE
  PICO_SCREENS_WIDTH=${PICO_SCREENS_WIDTH}
  PICO_SCREENS_HEIGHT=${PICO_SCREENS_HEIGHT}
  PICO_SCREENS_DOWNSAMPLING_FACTOR=${PICO_SCREENS_DOWNSAMPLING_FACTOR}
)

# Link against hagl_hal
target_link_libraries(pico-screens INTERFACE hagl_hal pico_stdlib hardware_spi hardware_gpio hardware_dma hardware_pio hardware_pwm) 