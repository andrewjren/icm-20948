project(templates)

find_package(Threads REQUIRED)

add_executable(tester "init.cpp")

target_link_libraries(tester 
PRIVATE 
icm20948
Threads::Threads)