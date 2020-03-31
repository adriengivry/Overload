#pragma once

#ifdef _WIN32
// If this macro is define than the current platform is Windows.
#define PLATFORM_WINDOWS
#elif __linux
// If this macro is define than the current platform is Linux-based.
#define PLATFORM_LINUX
#endif