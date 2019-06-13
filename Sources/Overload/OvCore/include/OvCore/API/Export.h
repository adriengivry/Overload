/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#pragma warning(disable : 4251)
#pragma warning(disable : 4275)

#ifdef OVCORE_EXPORT
#define API_OVCORE __declspec(dllexport)
#else
#define API_OVCORE __declspec(dllimport)
#endif // OVCORE_EXPORT