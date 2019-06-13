/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#pragma warning(disable : 4251)
#pragma warning(disable : 4275)

#ifdef OVANALYTICS_EXPORT
#define API_OVANALYTICS __declspec(dllexport)
#else
#define API_OVANALYTICS __declspec(dllimport)
#endif // OVANALYTICS_EXPORT