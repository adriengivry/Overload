/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#pragma warning(disable : 4251)
#pragma warning(disable : 4275)

#ifdef OVTOOLS_EXPORT
#ifdef PLATFORM_WINDOWS
#define API_OVTOOLS __declspec(dllexport)
#else
#define API_OVTOOLS
#endif // PLATFORM_WINDOWS
#else
#ifdef PLATFORM_WINDOWS
#define API_OVTOOLS __declspec(dllimport)
#else
#define API_OVTOOLS
#endif // PLATFORM_WINDOWS
#endif // OVTOOLS_EXPORT