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
	enum class ECullFace : uint8_t
	{
		FRONT,
		BACK,
		FRONT_AND_BACK
	};
}