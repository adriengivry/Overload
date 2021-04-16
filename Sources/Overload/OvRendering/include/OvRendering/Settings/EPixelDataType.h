/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once



namespace OvRendering::Settings
{
	/**
	* OpenGL pixel data type enum wrapper
	*/
	enum class EPixelDataType
	{
		BYTE						= 0x1400,
		UNSIGNED_BYTE				= 0x1401,
		BITMAP						= 0x1A00,
		SHORT						= 0x1402,
		UNSIGNED_SHORT				= 0x1403,
		INT							= 0x1404,
		UNSIGNED_INT				= 0x1405,
		FLOAT						= 0x1406,
		UNSIGNED_BYTE_3_3_2			= 0x8032,
		UNSIGNED_BYTE_2_3_3_REV		= 0x8362,
		UNSIGNED_SHORT_5_6_5		= 0x8363,
		UNSIGNED_SHORT_5_6_5_REV	= 0x8364,
		UNSIGNED_SHORT_4_4_4_4		= 0x8033,
		UNSIGNED_SHORT_4_4_4_4_REV	= 0x8365,
		UNSIGNED_SHORT_5_5_5_1		= 0x8034,
		UNSIGNED_SHORT_1_5_5_5_REV	= 0x8366,
		UNSIGNED_INT_8_8_8_8		= 0x8035,
		UNSIGNED_INT_8_8_8_8_REV	= 0x8367,
		UNSIGNED_INT_10_10_10_2		= 0x8036,
		UNSIGNED_INT_2_10_10_10_REV	= 0x8368
	};
}