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
#ifdef OVCORE_EXPORT
#define API_OVCORE __declspec(dllexport)
#else
#define API_OVCORE __declspec(dllimport)
#endif // OVCORE_EXPORT
#elif PLATFORM_LINUX
#define API_OVCORE
#endif // PLATFORM_WINDOWS