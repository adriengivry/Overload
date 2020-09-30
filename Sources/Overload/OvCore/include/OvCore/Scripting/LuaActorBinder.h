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
	* Handle actor binding
	*/
	class LuaActorBinder
	{
	public:
		/**
		* Bind engine actor-related features to lua
		*/
		static void BindActor(sol::state& p_luaState);
	};
}