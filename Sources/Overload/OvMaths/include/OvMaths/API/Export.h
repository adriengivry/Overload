/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#pragma warning(disable : 4251)
#pragma warning(disable : 4275)

#ifdef OVMATHS_EXPORT
#ifdef PLATFORM_WINDOWS
#define API_OVMATHS __declspec(dllexport)
#else
#define API_OVMATHS
#endif // PLATFORM_WINDOWS
#else
#ifdef PLATFORM_WINDOWS
#define API_OVMATHS __declspec(dllimport)
#else
#define API_OVMATHS
#endif // PLATFORM_WINDOWS
#endif // OVMATHS_EXPORT