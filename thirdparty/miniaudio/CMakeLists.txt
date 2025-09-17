cmake_minimum_required(VERSION 3.18.4)

project(miniaudio)

add_library(miniaudio)

target_sources(miniaudio PRIVATE src/miniaudio.c)
target_include_directories(miniaudio PUBLIC include)
