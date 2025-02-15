/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#define LUA_SCRIPTING

#if defined(LUA_SCRIPTING)
#include <OvCore/Scripting/LuaScriptEngine.h>
#else
#include <OvCore/Scripting/NullScriptEngine.h>
#endif

namespace OvCore::Scripting
{
#if defined(LUA_SCRIPTING)
	using ScriptEngine = LuaScriptEngine;
	using Script = LuaScript;
#else
	using ScriptEngine = NullScriptEngine;
	using Script = NullScript;
#endif
}