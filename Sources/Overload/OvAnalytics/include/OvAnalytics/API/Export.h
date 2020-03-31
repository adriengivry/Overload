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
#ifdef OVANALYTICS_EXPORT
#define API_OVANALYTICS __declspec(dllexport)
#else
#define API_OVANALYTICS __declspec(dllimport)
#endif // OVANALYTICS_EXPORT
#elif PLATFORM_LINUX
#define API_OVANALYTICS
#endif // PLATFORM_WINDOWS