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
	class LuaScriptContext : public IScriptContext
	{
	public:
		LuaScriptContext(sol::table p_table);
		virtual bool IsValid() const override;

	public:
		sol::table table;
	};
}
