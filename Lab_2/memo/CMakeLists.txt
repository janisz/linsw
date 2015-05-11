cmake_minimum_required(VERSION 3.1)
project(memo)

set(CMAKE_C_FLAGS "-std=c99")

set(SOURCE_FILES
    main.c)

add_executable(memo ${SOURCE_FILES})