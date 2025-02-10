/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <OvDebug/Logger.h>
#include <OvDebug/Assertion.h>

#include <OvCore/Scripting/Lua/LuaScriptingBackend.h>
#include <OvCore/Scripting/Lua/LuaScriptContext.h>
#include <OvCore/ECS/Components/Behaviour.h>
#include <OvCore/ECS/Actor.h>

#include <sol.hpp>

void BindLuaActor(sol::state& p_state);
void BindLuaComponents(sol::state& p_state);
void BindLuaGlobal(sol::state& p_state);
void BindLuaMath(sol::state& p_state);

constexpr auto luaBindings = std::array{
	BindLuaActor,
	BindLuaComponents,
	BindLuaGlobal,
	BindLuaMath
};

template<typename... Args>
void ExecuteLuaFunction(OvCore::ECS::Components::Behaviour& p_behaviour, const std::string& p_functionName, Args&& ...p_args)
{
	auto context = p_behaviour.GetScriptContext();

	OVASSERT(context.has_value(), "The given context is null");
	OVASSERT(context->IsValid(), "The given context is invalid");

	const sol::table& table = static_cast<OvCore::Scripting::LuaScriptContext&>(context.value()).table;

	if (table[p_functionName].valid())
	{
		sol::protected_function pfr = table[p_functionName];
		auto pfrResult = pfr.call(table, std::forward<Args>(p_args)...);
		if (!pfrResult.valid())
		{
			sol::error err = pfrResult;
			OVLOG_ERROR(err.what());
		}
	}
}

OvCore::Scripting::LuaScriptingBackend::LuaScriptingBackend(const std::string& p_scriptRootFolder) :
	m_scriptRootFolder(p_scriptRootFolder)
{
	CreateContext();
}

OvCore::Scripting::LuaScriptingBackend::~LuaScriptingBackend()
{
	DestroyContext();
}

struct ScriptRegistrationResult
{
	bool success;
	std::optional<sol::table> table;
};

ScriptRegistrationResult RegisterScript(sol::state& p_luaState, const std::string& p_scriptName)
{
	using namespace OvCore::Scripting;

	const auto result = p_luaState.safe_script_file(p_scriptName, &sol::script_pass_on_error);

	if (!result.valid())
	{
		sol::error err = result; // TODO: is there a better way to do that?
		OVLOG_ERROR(err.what());
		return { false };
	}
	else
	{
		if (result.return_count() == 1 && result[0].is<sol::table>())
		{
			return { true, result[0] };
		}
		else
		{
			OVLOG_ERROR("'" + p_scriptName + "' missing return expression");
			return { false };
		}
	}
}

bool RegisterBehaviour(sol::state& p_luaState, OvCore::ECS::Components::Behaviour& p_behaviour, const std::string& p_scriptName)
{
	auto result = RegisterScript(p_luaState, p_scriptName);

	if (result.success && result.table.has_value())
	{
		result.table.value()["owner"] = &p_behaviour.owner;
		auto scriptContext = std::make_unique<OvCore::Scripting::LuaScriptContext>(result.table.value());
		p_behaviour.SetScriptContext(std::move(scriptContext));
		return true;
	}

	return false;
}

void OvCore::Scripting::LuaScriptingBackend::OnTriggerExit(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject)
{
}

void OvCore::Scripting::LuaScriptingBackend::CreateContext()
{
	// TODO: Asset if already has a lua state
	if (!m_luaState)
	{
		m_luaState = std::make_unique<sol::state>();
		m_luaState->open_libraries(sol::lib::base, sol::lib::math);

		for (auto& callback : luaBindings)
		{
			callback(*m_luaState);
		}

		m_isOk = true;

		std::for_each(m_behaviours.begin(), m_behaviours.end(),
			[this](std::reference_wrapper<OvCore::ECS::Components::Behaviour> behaviour)
			{
				if (!RegisterBehaviour(*m_luaState, behaviour.get(), m_scriptRootFolder + behaviour.get().name + ".lua"))
				{
					m_isOk = false;
				}
			}
		);

		if (!m_isOk)
		{
			OVLOG_ERROR("Script interpreter failed to register scripts. Check your lua scripts");
		}
	}
}

void OvCore::Scripting::LuaScriptingBackend::DestroyContext()
{
	// TODO: Assert if no lua state
	if (m_luaState)
	{
		std::for_each(m_behaviours.begin(), m_behaviours.end(),
			[this](std::reference_wrapper<OvCore::ECS::Components::Behaviour> behaviour)
			{
				behaviour.get().ResetScriptContext();
			}
		);

		m_luaState.reset();
		m_isOk = false;
	}
}

void OvCore::Scripting::LuaScriptingBackend::AddBehaviour(OvCore::ECS::Components::Behaviour& p_toAdd)
{
	// TODO: Assert if no lua state
	if (m_luaState)
	{
		m_behaviours.push_back(std::ref(p_toAdd));

		if (!RegisterBehaviour(*m_luaState, p_toAdd, m_scriptRootFolder + p_toAdd.name + ".lua"))
		{
			m_isOk = false;
		}
	}
}

void OvCore::Scripting::LuaScriptingBackend::RemoveBehaviour(OvCore::ECS::Components::Behaviour& p_toRemove)
{
	if (m_luaState)
		p_toRemove.ResetScriptContext();

	m_behaviours.erase(
		std::remove_if(m_behaviours.begin(), m_behaviours.end(),
			[&p_toRemove](std::reference_wrapper< OvCore::ECS::Components::Behaviour> behaviour) {
				return &p_toRemove == &behaviour.get();
			}
		)
	);

	// Unconsidering a script is impossible with Lua, we have to reparse every behaviours
	// @note this might be constly, we should look into it more seriously.
	Reload(); 
}

void OvCore::Scripting::LuaScriptingBackend::Reload()
{
	DestroyContext();
	CreateContext();
}

bool OvCore::Scripting::LuaScriptingBackend::IsOk() const
{
	return m_isOk;
}

void OvCore::Scripting::LuaScriptingBackend::OnAwake(OvCore::ECS::Components::Behaviour& p_target)
{
	ExecuteLuaFunction(p_target, "OnAwake");
}

void OvCore::Scripting::LuaScriptingBackend::OnStart(OvCore::ECS::Components::Behaviour& p_target)
{
	ExecuteLuaFunction(p_target, "OnStart");
}

void OvCore::Scripting::LuaScriptingBackend::OnEnable(OvCore::ECS::Components::Behaviour& p_target)
{
	ExecuteLuaFunction(p_target, "OnEnable");
}

void OvCore::Scripting::LuaScriptingBackend::OnDisable(OvCore::ECS::Components::Behaviour& p_target)
{
	ExecuteLuaFunction(p_target, "OnDisable");
}

void OvCore::Scripting::LuaScriptingBackend::OnDestroy(OvCore::ECS::Components::Behaviour& p_target)
{
	ExecuteLuaFunction(p_target, "OnDestroy");
}

void OvCore::Scripting::LuaScriptingBackend::OnUpdate(OvCore::ECS::Components::Behaviour& p_target, float p_deltaTime)
{
	ExecuteLuaFunction(p_target, "OnUpdate", p_deltaTime);
}

void OvCore::Scripting::LuaScriptingBackend::OnFixedUpdate(OvCore::ECS::Components::Behaviour& p_target, float p_deltaTime)
{
	ExecuteLuaFunction(p_target, "OnFixedUpdate", p_deltaTime);
}

void OvCore::Scripting::LuaScriptingBackend::OnLateUpdate(OvCore::ECS::Components::Behaviour& p_target, float p_deltaTime)
{
	ExecuteLuaFunction(p_target, "OnLateUpdate", p_deltaTime);
}

void OvCore::Scripting::LuaScriptingBackend::OnCollisionEnter(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject)
{
	ExecuteLuaFunction(p_target, "OnCollisionEnter", p_otherObject);
}

void OvCore::Scripting::LuaScriptingBackend::OnCollisionStay(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject)
{
	ExecuteLuaFunction(p_target, "OnCollisionStay", p_otherObject);
}

void OvCore::Scripting::LuaScriptingBackend::OnCollisionExit(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject)
{
	ExecuteLuaFunction(p_target, "OnCollisionExit", p_otherObject);
}

void OvCore::Scripting::LuaScriptingBackend::OnTriggerEnter(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject)
{
	ExecuteLuaFunction(p_target, "OnTriggerEnter", p_otherObject);
}

void OvCore::Scripting::LuaScriptingBackend::OnTriggerStay(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject)
{
	ExecuteLuaFunction(p_target, "OnTriggerStay", p_otherObject);
}
