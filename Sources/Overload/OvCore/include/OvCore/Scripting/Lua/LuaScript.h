/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <memory>

#include <OvCore/Scripting/Common/TScript.h>

#include <sol.hpp>

namespace OvCore::Scripting
{
	/**
	* Lua script context
	*/
	struct LuaScriptContext
	{
		sol::table table;
	};

	using LuaScriptBase = TScript<EScriptingLanguage::LUA, LuaScriptContext>;

	/**
	* Lua script implementation
	*/
	class LuaScript : public LuaScriptBase
	{
	public:
		LuaScript(sol::table p_table);
	};
}
