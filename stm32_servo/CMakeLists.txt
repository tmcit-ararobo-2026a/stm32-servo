cmake_minimum_required(VERSION 3.22)

add_library(stm32_servo STATIC
    src/app.cpp
    ${CMAKE_SOURCE_DIR}/gn10_can/drivers/stm32_can/driver_stm32_can.cpp
)

target_include_directories(stm32_servo PUBLIC
    include
    ${CMAKE_SOURCE_DIR}/gn10_can
)
target_compile_features(stm32_servo PUBLIC cxx_std_17)

target_link_libraries(stm32_servo PUBLIC gn10_can)
target_link_libraries(stm32_servo PRIVATE stm32cubemx)