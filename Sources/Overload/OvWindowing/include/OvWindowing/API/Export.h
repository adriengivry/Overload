/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#pragma warning(disable : 4251)
#pragma warning(disable : 4275)

#ifdef OVWINDOWING_EXPORT
#define API_OVWINDOWING __declspec(dllexport)
#else
#define API_OVWINDOWING __declspec(dllimport)
#endif // OVWINDOWING_EXPORT