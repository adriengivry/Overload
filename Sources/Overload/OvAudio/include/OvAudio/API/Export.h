/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <OvTools/Utils/Platform.h>

#pragma warning(disable : 4251)
#pragma warning(disable : 4275)

#ifdef PLATFORM_WINDOWS
#ifdef OVAUDIO_EXPORT
#define API_OVAUDIO __declspec(dllexport)
#else
#define API_OVAUDIO __declspec(dllimport)
#endif // OVAUDIO_EXPORT
#elif PLATFORM_LINUX
#define API_OVAUDIO
#endif // PLATFORM_WINDOWS