cmake_minimum_required(VERSION 3.20.0)

list(APPEND BOARD_ROOT ${CMAKE_CURRENT_LIST_DIR})

# zephyr_library()
# zephyr_library_sources(board.c)

find_package(Zephyr REQUIRED)
project(my_zephyr_app)

# add_subdirectory(drivers/sensor)
target_sources(app PRIVATE src/main.cpp drivers/sensor/led_sensor.c)
