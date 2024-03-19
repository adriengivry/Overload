/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

namespace OvRendering::Settings
{
	/**
	* Wraps from OpenGL types enum
	*/
	enum class EDataType
	{
		BYTE			= 0x1400,
		UNISGNED_BYTE	= 0x1401,
		SHORT			= 0x1402,
		UNSIGNED_SHORT	= 0x1403,
		INT				= 0x1404,
		UNSIGNED_INT	= 0x1405,
		FLOAT			= 0x1406,
		DOUBLE			= 0x140A
	};
}