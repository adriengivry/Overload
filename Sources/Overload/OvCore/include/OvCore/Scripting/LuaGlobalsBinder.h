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