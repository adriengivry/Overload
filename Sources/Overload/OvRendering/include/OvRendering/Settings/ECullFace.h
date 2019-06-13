/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#include "OvRendering/API/Export.h"

namespace OvRendering::Settings
{
	/**
	* OpenGL cullface enum wrapper
	*/
	enum class ECullFace
	{
		FRONT			= 0x0404,
		BACK			= 0x0405,
		FRONT_AND_BACK	= 0x0408
	};
}