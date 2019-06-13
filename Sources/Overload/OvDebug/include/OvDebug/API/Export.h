/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#pragma warning(disable : 4251)
#pragma warning(disable : 4275)

#ifdef OVDEBUG_EXPORT
#define API_OVDEBUG __declspec(dllexport)
#else
#define API_OVDEBUG __declspec(dllimport)
#endif // OVDEBUG_EXPORT
