/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#pragma warning(disable : 4251)
#pragma warning(disable : 4275)

#ifdef OVTOOLS_EXPORT
#define API_OVTOOLS __declspec(dllexport)
#else
#define API_OVTOOLS __declspec(dllimport)
#endif // OVTOOLS_EXPORT