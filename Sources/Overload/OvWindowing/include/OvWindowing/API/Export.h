/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#pragma warning(disable : 4251)
#pragma warning(disable : 4275)

#ifdef OVWINDOWING_EXPORT
#define API_OVWINDOWING __declspec(dllexport)
#else
#define API_OVWINDOWING __declspec(dllimport)
#endif // OVWINDOWING_EXPORT