cmake_minimum_required(VERSION 3.12.0)

include(CTest)

add_executable(${PROJECT_NAME}_tests)
target_sources(${PROJECT_NAME}_tests
    PRIVATE
    accessprivate_test.cpp)

target_link_libraries(${PROJECT_NAME}_tests ${PROJECT_NAME})
add_test(NAME ${PROJECT_NAME}_testsuite COMMAND ${PROJECT_NAME}_tests)
