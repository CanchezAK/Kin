add_executable(test
         main.cpp
         )
pico_enable_stdio_usb(test 1)
pico_enable_stdio_uart(test 0)

 # Import pico_stdlib which imports commonly used features
 target_link_libraries(test pico_stdlib hardware_i2c pico_binary_info hardware_gpio)

 # Create map/bin/hex file etc.
 pico_add_extra_outputs(test)

 # Add path via pico_set_program_url CMAKE
 example_auto_set_url(test)