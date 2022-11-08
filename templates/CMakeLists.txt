project(templates)

add_executable(tester "init.cpp")

target_link_libraries(tester 
PRIVATE icm20948)