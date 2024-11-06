cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
project(Lab6)
pico_sdk_init()
add_executable(Lab6)

pico_generate_pio_header(Lab6 ${CMAKE_CURRENT_LIST_DIR}/SPIPIO.pio)

# pull in common dependencies
target_link_libraries(Lab6
    pico_stdlib
    hardware_adc
    hardware_gpio
    hardware_pio
    hardware_irq
    hardware_pwm
    pico_bootsel_via_double_reset
    pwm_pin
    PIO_input_capture_2a
    ic
    ic_pio
)

target_sources(Lab6 PRIVATE
    main.c
    ts_lcd
    TouchScreen.c
    TFTMaster.c
    glcdfont.c
    timer.c
)

# create map/bin/hex file etc.
pico_add_extra_outputs(Lab6)
