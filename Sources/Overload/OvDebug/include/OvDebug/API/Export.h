/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#pragma warning(disable : 4251)
#pragma warning(disable : 4275)

#ifdef OVDEBUG_EXPORT
#ifdef PLATFORM_WINDOWS
#define API_OVDEBUG __declspec(dllexport)
#else
#define API_OVDEBUG
#endif // PLATFORM_WINDOWS
#else
#ifdef PLATFORM_WINDOWS
#define API_OVDEBUG __declspec(dllimport)
#else
#define API_OVDEBUG
#endif // PLATFORM_WINDOWS
#endif // OVDEBUG_EXPORT
