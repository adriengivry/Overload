/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once



namespace OvRendering::Settings
{
	/**
	* OpenGL comparaison algorithm enum wrapper
	*/
	enum class EComparaisonAlgorithm : uint8_t
	{
		NEVER,
		LESS,
		EQUAL,
		LESS_EQUAL,
		GREATER,
		NOTEQUAL,
		GREATER_EQUAL,
		ALWAYS
	};
}
