/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#pragma warning(disable : 4251)
#pragma warning(disable : 4275)

#ifdef OVCORE_EXPORT
#define API_OVCORE __declspec(dllexport)
#else
#define API_OVCORE __declspec(dllimport)
#endif // OVCORE_EXPORT