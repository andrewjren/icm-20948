cmake_minimum_required(VERSION 3.10)
project(icm-20948-interface)

set(src 
"${CMAKE_CURRENT_SOURCE_DIR}/bus/I2cStrategy.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/Icm20948.cpp")

add_library(icm20948 STATIC ${src})

target_include_directories(icm20948
PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}/core
${CMAKE_CURRENT_SOURCE_DIR}/bus
)

target_link_libraries(icm20948
PUBLIC
i2c)