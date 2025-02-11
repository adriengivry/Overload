/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <OvCore/Scripting/IScriptContext.h>

#include <sol.hpp>

namespace OvCore::Scripting
{
	/**
	* Lua script context implementation.
	* This class provides the implementation of the IScriptContext interface using Lua as the scripting language.
	* It holds a Lua table that represents the script context.
	*/
	class LuaScriptContext : public IScriptContext
	{
	public:
		/**
		* Constructor for the LuaScriptContext.
		* @param p_table The Lua table representing the script context.
		*/
		LuaScriptContext(sol::table p_table);

		/**
		* Checks if the script context is valid.
		* @return True if the context is valid, false otherwise.
		*/
		virtual bool IsValid() const override;

	public:
		sol::table table;
	};
}
