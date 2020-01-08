/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <sol.hpp>

namespace OvCore::Scripting
{
	/**
	* Handle lua global binding
	*/
	class LuaGlobalsBinder
	{
	public:
		/**
		* Bind engine global features to lua
		*/
		static void BindGlobals(sol::state& p_luaState);
	};
}