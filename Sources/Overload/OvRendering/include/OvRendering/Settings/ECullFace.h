/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once



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