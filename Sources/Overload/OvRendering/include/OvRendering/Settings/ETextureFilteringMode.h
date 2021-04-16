/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once



namespace OvRendering::Settings
{
	/**
	* OpenGL texture filtering mode enum wrapper
	*/
	enum class ETextureFilteringMode
	{
		NEAREST					= 0x2600,
		LINEAR					= 0x2601,
		NEAREST_MIPMAP_NEAREST	= 0x2700,
		LINEAR_MIPMAP_LINEAR	= 0x2703,
		LINEAR_MIPMAP_NEAREST	= 0x2701,
		NEAREST_MIPMAP_LINEAR	= 0x2702
	};
}