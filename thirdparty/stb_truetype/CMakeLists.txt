cmake_minimum_required(VERSION 3.18.4)

project(stb_truetype)

add_library(stb_truetype)

target_sources(stb_truetype PRIVATE src/stb_truetype.c)
target_include_directories(stb_truetype PUBLIC include)
