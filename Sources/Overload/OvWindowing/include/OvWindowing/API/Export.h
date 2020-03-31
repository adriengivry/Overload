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
#ifdef OVWINDOWING_EXPORT
#define API_OVWINDOWING __declspec(dllexport)
#else
#define API_OVWINDOWING __declspec(dllimport)
#endif // OVWINDOWING_EXPORT
#elif PLATFORM_LINUX
#define API_OVWINDOWING
#endif // PLATFORM_WINDOWS