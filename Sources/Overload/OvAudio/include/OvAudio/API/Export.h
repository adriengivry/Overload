/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#pragma warning(disable : 4251)
#pragma warning(disable : 4275)

#ifdef OVAUDIO_EXPORT
#define API_OVAUDIO __declspec(dllexport)
#else
#define API_OVAUDIO __declspec(dllimport)
#endif // OVAUDIO_EXPORT