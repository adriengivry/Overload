/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#pragma warning(disable : 4251)
#pragma warning(disable : 4275)

#ifdef OVMATHS_EXPORT
#define API_OVMATHS __declspec(dllexport)
#else
#define API_OVMATHS __declspec(dllimport)
#endif // OVMATHS_EXPORT