/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#pragma warning(disable : 4251)
#pragma warning(disable : 4275)

#ifdef OVANALYTICS_EXPORT
#define API_OVANALYTICS __declspec(dllexport)
#else
#define API_OVANALYTICS __declspec(dllimport)
#endif // OVANALYTICS_EXPORT