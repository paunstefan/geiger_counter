set(NAME geiger_counter)

file(GLOB geiger_counter_SRC
     "*.h"
     "*.c"
)

add_executable(${NAME} ${geiger_counter_SRC})

# pull in common dependencies
target_link_libraries(${NAME} pico_stdlib hardware_i2c)

pico_enable_stdio_usb(${NAME} 1)
pico_enable_stdio_uart(${NAME} 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(${NAME})