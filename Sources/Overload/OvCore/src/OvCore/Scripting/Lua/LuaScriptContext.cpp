/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <OvCore/Scripting/Lua/LuaScriptContext.h>

OvCore::Scripting::LuaScriptContext::LuaScriptContext(sol::table p_table) : table(p_table)
{
}

bool OvCore::Scripting::LuaScriptContext::LuaScriptContext::IsValid() const
{
	return table.valid();
}
