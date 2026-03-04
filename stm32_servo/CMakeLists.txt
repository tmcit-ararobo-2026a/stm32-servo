cmake_minimum_required(VERSION 3.22)

add_library(stm32_servo STATIC
    src/app.cpp
)

target_include_directories(stm32_servo PUBLIC include)
target_compile_features(stm32_servo PUBLIC cxx_std_17)

target_link_libraries(stm32_servo PUBLIC gn10_can)