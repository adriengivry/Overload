/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#pragma warning(disable : 4251)
#pragma warning(disable : 4275)

#ifdef OVUI_EXPORT
#define API_OVUI __declspec(dllexport)
#else
#define API_OVUI __declspec(dllimport)
#endif // OVUI_EXPORT