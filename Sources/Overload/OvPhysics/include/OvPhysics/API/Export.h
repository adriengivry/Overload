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
#ifdef OVPHYSICS_EXPORT
#define API_OVPHYSICS __declspec(dllexport)
#else
#define API_OVPHYSICS __declspec(dllimport)
#endif // OVPHYSICS_EXPORT
#elif PLATFORM_LINUX
#define API_OVPHYSICS
#endif // PLATFORM_WINDOWS