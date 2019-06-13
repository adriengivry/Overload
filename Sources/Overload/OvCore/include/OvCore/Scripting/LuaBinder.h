/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#include <sol.hpp>

namespace OvCore::Scripting
{
	/**
	* Handle lua binding
	*/
	class LuaBinder
	{
	public:
		/**
		* Bind engine features to lua
		*/
		static void CallBinders(sol::state& p_luaState);
	};
}