cmake_minimum_required(VERSION 3.26)
project(Statistical_analysis_of_digital_images)

set(CMAKE_CXX_STANDARD 23)

add_executable(Statistical_analysis_of_digital_images main.cpp
        bmp/bmp.h
        bmp/bmp.cpp
        test/correlation.h
        test/correlation.cpp
        YCbCr/YCbCr.cpp
        YCbCr/YCbCr.h)
