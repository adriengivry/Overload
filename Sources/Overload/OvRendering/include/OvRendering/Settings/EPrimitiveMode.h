/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once



namespace OvRendering::Settings
{
	/**
	* OpenGL primitive mode enum wrapper
	*/
	enum class EPrimitiveMode
	{
		POINTS						= 0x0000,
		LINES						= 0x0001,
		LINE_LOOP					= 0x0002,
		LINE_STRIP					= 0x0003,
		TRIANGLES					= 0x0004,
		TRIANGLE_STRIP				= 0x0005,
		TRIANGLE_FAN				= 0x0006,
		LINES_ADJACENCY				= 0x000A,
		LINE_STRIP_ADJACENCY		= 0x000B,
		TRIANGLES_ADJACENCY			= 0x000C,
		TRIANGLE_STRIP_ADJACENCY	= 0x000D,
		PATCHES						= 0xE,
	};
}