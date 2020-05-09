/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "OvRendering/API/Export.h"

#define FORCE_DEDICATED_GPU \
extern "C"\
{\
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;\
}