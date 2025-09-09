#pragma once

// Free system resources, implemented in main.cpp

void FreeSystemResources();

// Types

#include <cstdint>

using uchar = uint8_t;
using uint = unsigned int;

// Debug

#include <cstdio>
#include <cstdlib>

#define DEBUG_TRACE(...) do {                    \
  std::fwrite("\x1b[32mTrace: ", 1, 12, stdout); \
  std::printf(__VA_ARGS__);                      \
  std::puts("\033[0m");                          \
} while(0)

#define DEBUG_WARNING(...) do {                    \
  std::fwrite("\x1b[33mWarning: ", 1, 14, stdout); \
  std::printf(__VA_ARGS__);                        \
  std::puts("\033[0m");                            \
} while(0)

#define DEBUG_ERROR(...) do {                                         \
  std::printf("\x1b[31mError in %s, line %d:\n", __FILE__, __LINE__); \
  std::printf(__VA_ARGS__);                                           \
  std::puts("\033[0m");                                               \
} while(0)

#define DEBUG_ASSERT(x, ...) do {                                        \
  if (!(x)) {                                                            \
    std::printf("\x1b[31mAssert in %s, line %d:\n", __FILE__, __LINE__); \
    std::printf(__VA_ARGS__);                                            \
    std::puts("\033[0m");                                                \
    FreeSystemResources();                                               \
    std::exit(1);                                                        \
  }                                                                      \
} while(0)
