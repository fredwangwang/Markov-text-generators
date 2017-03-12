cmake_minimum_required(VERSION 3.6)
project(markov_model)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/main.cpp src/model.cpp src/model.h)
add_executable(markov_model ${SOURCE_FILES})