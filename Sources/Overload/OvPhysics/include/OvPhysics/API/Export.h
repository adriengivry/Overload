/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#pragma warning(disable : 4251)
#pragma warning(disable : 4275)

#ifdef OVPHYSICS_EXPORT
#define API_OVPHYSICS __declspec(dllexport)
#else
#define API_OVPHYSICS __declspec(dllimport)
#endif // OVPHYSICS_EXPORT