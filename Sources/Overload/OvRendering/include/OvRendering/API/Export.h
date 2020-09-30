/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#pragma warning(disable : 4251)
#pragma warning(disable : 4275)

#ifdef OVRENDERING_EXPORT
#define API_OVRENDERING __declspec(dllexport)
#else
#define API_OVRENDERING __declspec(dllimport)
#endif // OVRENDERING_EXPORT