cmake_minimum_required(VERSION 3.18.4)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
	message(STATUS "This is a release build")
else()
	message(STATUS "This is a debug build")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

project(marblemaze)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

add_subdirectory(thirdparty/glfw-3.4)
add_subdirectory(thirdparty/glad)
add_subdirectory(thirdparty/miniaudio)
add_subdirectory(thirdparty/stb_image)
add_subdirectory(thirdparty/stb_truetype)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(${PROJECT_NAME})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
	target_compile_definitions(${PROJECT_NAME} PUBLIC _RES_PATH="./res/")
	target_compile_definitions(${PROJECT_NAME} PUBLIC _DEBUG=0)
else()
	target_compile_definitions(${PROJECT_NAME} PUBLIC _RES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/res/")
	target_compile_definitions(${PROJECT_NAME} PUBLIC _DEBUG=1)
endif()

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Wno-unused-parameter)
target_sources(${PROJECT_NAME} PRIVATE "${SOURCE_FILES}")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/")
target_link_libraries(${PROJECT_NAME} PRIVATE glfw glad miniaudio stb_image stb_truetype)
