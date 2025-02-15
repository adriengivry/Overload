/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <vector>
#include <memory>

#include <OvCore/Scripting/EScriptingLanguage.h>
#include <OvCore/Scripting/TScriptEngine.h>
#include <OvCore/Scripting/TScript.h>

#include <sol.hpp>

namespace OvCore::ECS::Components
{
	class Behaviour;
}

namespace OvCore::Scripting
{
	/**
	* Lua scripting backend implementation.
	* This class provides the implementation of the IScriptEngine interface using Lua as the scripting language.
	*/
	struct LuaScriptEngineContext
	{
		std::unique_ptr<sol::state> luaState;
		std::string scriptRootFolder;
		std::vector<std::reference_wrapper<OvCore::ECS::Components::Behaviour>> behaviours;
		uint32_t errorCount;
	};

	struct LuaScriptContext
	{
		std::unique_ptr<sol::table> table;
	};

	using LuaScriptEngineBase = TScriptEngine<EScriptingLanguage::LUA, LuaScriptEngineContext>;
	using LuaScriptBase = TScript<EScriptingLanguage::LUA, LuaScriptContext>;

	class LuaScriptEngine : public LuaScriptEngineBase
	{
	public:
		LuaScriptEngine();
		~LuaScriptEngine();

		void CreateContext();
		void DestroyContext();
	};

	class LuaScript : public LuaScriptBase
	{
	public:
		LuaScript(sol::table p_table);
	};
}
