cmake_minimum_required(VERSION 3.14)
set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "g++")

project(my_project)

file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.c)
list(REMOVE_ITEM SRC_FILES ${PROJECT_SOURCE_DIR}/src/main.c)

add_executable(
        test_tokenizer
        tests/test_tokenizer.c
        ${SRC_FILES}
)

add_test(
        NAME test_tokenizer
        COMMAND $<TARGET_FILE:test_tokenizer>
)

enable_testing()

#target_include_directories(test_tokenizer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)


#set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

#include(FetchContent)
#FetchContent_Declare(
#        googletest
#        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
#)
#FetchContent_MakeAvailable(googletest)
#
#enable_testing()
#
#add_executable(
#        test_tokenizer
#        tests/test_tokenizer.cpp
#)
#target_link_libraries(
#        test_tokenizer
#        GTest::gtest_main
#)
#
#include(GoogleTest)
#gtest_discover_tests(test_tokenizer)
