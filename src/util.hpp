#pragma once

// Types

typedef unsigned int  uint;
typedef unsigned char uchar;
typedef const char*   cstr;

typedef long long int int64;
typedef long int      int32;
typedef short int     int16;

typedef unsigned long long int uint64;
typedef unsigned long int      uint32;
typedef unsigned short int     uint16;

// Debug

#ifndef _DEBUG // Debug disabled

#define DEBUG_TRACE(...)
#define DEBUG_WARNING(...)
#define DEBUG_ERROR(...)

#else // Debug enabled

#include <cstdio>

#define DEBUG_TRACE(...)                         \
{                                                \
  std::fwrite("\x1b[32mTrace: ", 1, 12, stdout); \
  std::printf(__VA_ARGS__);                      \
  std::puts("\033[0m");                          \
}

#define DEBUG_WARNING(...)                         \
{                                                  \
  std::fwrite("\x1b[33mWarning: ", 1, 14, stdout); \
  std::printf(__VA_ARGS__);                        \
  std::puts("\033[0m");                            \
}

#define DEBUG_ERROR(...)                                                   \
{                                                                          \
  std::printf("\x1b[31mError in %s, line %d:\n", __FILE_NAME__, __LINE__); \
  std::printf(__VA_ARGS__);                                                \
  std::puts("\033[0m");                                                    \
}

#endif // Debug
