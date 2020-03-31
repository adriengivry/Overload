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
#ifdef OVRENDERING_EXPORT
#define API_OVRENDERING __declspec(dllexport)
#else
#define API_OVRENDERING __declspec(dllimport)
#endif // OVRENDERING_EXPORT
#elif PLATFORM_LINUX
#define API_OVRENDERING
#endif // PLATFORM_WINDOWS