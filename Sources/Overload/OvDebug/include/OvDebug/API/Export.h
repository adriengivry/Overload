/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <OvTools/Utils/Platform.h>

#pragma warning(disable : 4251)
#pragma warning(disable : 4275)

#ifdef PLATFORM_WINDOWS
#ifdef OVDEBUG_EXPORT
#define API_OVDEBUG __declspec(dllexport)
#else
#define API_OVDEBUG __declspec(dllimport)
#endif // OVDEBUG_EXPORT
#elif PLATFORM_LINUX
#define API_OVDEBUG
#endif // PLATFORM_WINDOWS
