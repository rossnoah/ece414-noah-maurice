cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
project(Lab4)
pico_sdk_init()
add_executable(Lab4 main.c TouchScreen.c TFTMaster.c)

# pull in common dependencies
target_link_libraries(Lab4 pico_stdlib
    pico_bootsel_via_double_reset)

# create map/bin/hex file etc.
pico_add_extra_outputs(Lab4)
