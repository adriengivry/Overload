/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#include <string>

#include "OvDebug/API/Export.h"

#define OVASSERT(condition, message) OvDebug::Assertion::Assert(condition, message)

namespace OvDebug
{
	/**
	* Wrapper for C++ assert
	*/
	class API_OVDEBUG Assertion
	{
	public:

		/**
		* Disabled constructor
		*/
		Assertion() = delete;

		/**
		* C++ assertion wrapped call
		* @param p_condition
		* @param p_message
		*/
		static void Assert(bool p_condition, const std::string& p_message = "");
	};
}