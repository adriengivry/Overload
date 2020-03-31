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
#ifdef OVUI_EXPORT
#define API_OVUI __declspec(dllexport)
#else
#define API_OVUI __declspec(dllimport)
#endif // OVUI_EXPORT
#elif PLATFORM_LINUX
#define API_OVUI
#endif // PLATFORM_WINDOWS