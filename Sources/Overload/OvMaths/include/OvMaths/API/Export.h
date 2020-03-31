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
#ifdef OVMATHS_EXPORT
#define API_OVMATHS __declspec(dllexport)
#else
#define API_OVMATHS __declspec(dllimport)
#endif // OVMATHS_EXPORT
#elif PLATFORM_LINUX
#define API_OVMATHS
#endif // PLATFORM_WINDOWS