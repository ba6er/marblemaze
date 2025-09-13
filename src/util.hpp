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

#define CRITICAL_TRACE(...) do {                 \
  std::fwrite("\x1b[32mTrace: ", 1, 12, stdout); \
  std::printf(__VA_ARGS__);                      \
  std::puts("\033[0m");                          \
} while(0)

#define CRITICAL_WARNING(...) do {                 \
  std::fwrite("\x1b[33mWarning: ", 1, 14, stdout); \
  std::printf(__VA_ARGS__);                        \
  std::puts("\033[0m");                            \
} while(0)

#define CRITICAL_ERROR(...) do {                                      \
  std::printf("\x1b[31mError in %s, line %d:\n", __FILE__, __LINE__); \
  std::printf(__VA_ARGS__);                                           \
  std::puts("\033[0m");                                               \
} while(0)

#define CRITICAL_ASSERT(x, ...) do {                                     \
  if (!(x)) {                                                            \
    std::printf("\x1b[31mAssert in %s, line %d:\n", __FILE__, __LINE__); \
    std::printf(__VA_ARGS__);                                            \
    std::puts("\033[0m");                                                \
    FreeSystemResources();                                               \
    std::exit(1);                                                        \
  }                                                                      \
} while(0)

#if _DEBUG==1

#define DEBUG_TRACE CRITICAL_TRACE
#define DEBUG_WARNING CRITICAL_WARNING
#define DEBUG_ERROR CRITICAL_ERROR
#define DEBUG_ASSERT CRITICAL_ASSERT

#else

#define DEBUG_TRACE(...)
#define DEBUG_WARNING(...)
#define DEBUG_ERROR(...)
#define DEBUG_ASSERT(x, ...)

#endif
