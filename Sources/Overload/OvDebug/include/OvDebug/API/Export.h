/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#pragma warning(disable : 4251)
#pragma warning(disable : 4275)

#ifdef OVDEBUG_EXPORT
#define API_OVDEBUG __declspec(dllexport)
#else
#define API_OVDEBUG __declspec(dllimport)
#endif // OVDEBUG_EXPORT
