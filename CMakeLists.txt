cmake_minimum_required(VERSION 3.13)

# Include the hagl_hal subdirectory
add_subdirectory(hagl_hal)

# Create a pico-screens library
add_library(pico-screens INTERFACE)

target_sources(pico-screens INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/screen.c
  ${CMAKE_CURRENT_LIST_DIR}/downsampler.c
  ${CMAKE_CURRENT_LIST_DIR}/screens/shared.c
  ${CMAKE_CURRENT_LIST_DIR}/screens/st7735_128_128.c
)

target_include_directories(pico-screens INTERFACE ${CMAKE_CURRENT_LIST_DIR})

# Link against hagl_hal
target_link_libraries(pico-screens INTERFACE hagl_hal pico_stdlib hardware_spi hardware_gpio hardware_dma) 