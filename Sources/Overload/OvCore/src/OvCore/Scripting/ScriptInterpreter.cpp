/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <OvDebug/Logger.h>

#include "OvCore/Scripting/LuaBinder.h"
#include "OvCore/Scripting/ScriptInterpreter.h"

OvCore::Scripting::ScriptInterpreter::ScriptInterpreter(const std::string& p_scriptRootFolder) :
	m_scriptRootFolder(p_scriptRootFolder)
{
	CreateLuaContextAndBindGlobals();

	/* Listen to behaviours */
	OvCore::ECS::Components::Behaviour::CreatedEvent	+= std::bind(&ScriptInterpreter::Consider, this, std::placeholders::_1);
	OvCore::ECS::Components::Behaviour::DestroyedEvent	+= std::bind(&ScriptInterpreter::Unconsider, this, std::placeholders::_1);
}

OvCore::Scripting::ScriptInterpreter::~ScriptInterpreter()
{
	DestroyLuaContext();
}

void OvCore::Scripting::ScriptInterpreter::CreateLuaContextAndBindGlobals()
{
	if (!m_luaState)
	{
		m_luaState = std::make_unique<sol::state>();
		m_luaState->open_libraries(sol::lib::base, sol::lib::math);
		OvCore::Scripting::LuaBinder::CallBinders(*m_luaState);
		m_isOk = true;

		std::for_each(m_behaviours.begin(), m_behaviours.end(), [this](ECS::Components::Behaviour* behaviour)
		{
			if (!behaviour->RegisterToLuaContext(*m_luaState, m_scriptRootFolder))
				m_isOk = false;
		});

		if (!m_isOk)
			OVLOG_ERROR("Script interpreter failed to register scripts. Check your lua scripts");
	}
}

void OvCore::Scripting::ScriptInterpreter::DestroyLuaContext()
{
	if (m_luaState)
	{
		std::for_each(m_behaviours.begin(), m_behaviours.end(), [this](ECS::Components::Behaviour* behaviour)
		{
			behaviour->UnregisterFromLuaContext();
		});

		m_luaState.reset();
		m_isOk = false;
	}
}

void OvCore::Scripting::ScriptInterpreter::Consider(OvCore::ECS::Components::Behaviour* p_toConsider)
{
	if (m_luaState)
	{
		m_behaviours.push_back(p_toConsider);

		if (!p_toConsider->RegisterToLuaContext(*m_luaState, m_scriptRootFolder))
			m_isOk = false;
	}
}

void OvCore::Scripting::ScriptInterpreter::Unconsider(OvCore::ECS::Components::Behaviour* p_toUnconsider)
{
	if (m_luaState)
		p_toUnconsider->UnregisterFromLuaContext();

	m_behaviours.erase(std::remove_if(m_behaviours.begin(), m_behaviours.end(), [p_toUnconsider](ECS::Components::Behaviour* behaviour)
	{
		return p_toUnconsider == behaviour;
	}));

	RefreshAll(); // Unconsidering a script is impossible with Lua, we have to reparse every behaviours
}

void OvCore::Scripting::ScriptInterpreter::RefreshAll()
{
	DestroyLuaContext();
	CreateLuaContextAndBindGlobals();
}

bool OvCore::Scripting::ScriptInterpreter::IsOk() const
{
	return m_isOk;
}
