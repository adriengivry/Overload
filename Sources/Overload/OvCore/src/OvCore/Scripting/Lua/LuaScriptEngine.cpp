/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <OvDebug/Logger.h>
#include <OvDebug/Assertion.h>

#include <OvCore/Scripting/Lua/LuaScriptEngine.h>
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

sol::table RegisterScript(sol::state& p_luaState, const std::string& p_scriptName)
{
	using namespace OvCore::Scripting;

	const auto result = p_luaState.safe_script_file(p_scriptName, &sol::script_pass_on_error);

	if (!result.valid())
	{
		sol::error err = result; // TODO: is there a better way to do that?
		OVLOG_ERROR(err.what());
		return {};
	}
	else
	{
		if (result.return_count() == 1 && result[0].is<sol::table>())
		{
			return result[0];
		}
		else
		{
			OVLOG_ERROR("'" + p_scriptName + "' missing return expression");
			return {};
		}
	}
}

bool RegisterBehaviour(sol::state& p_luaState, OvCore::ECS::Components::Behaviour& p_behaviour, const std::string& p_scriptName)
{
	p_behaviour.SetScriptContext(
		std::make_unique<OvCore::Scripting::LuaScriptContext>(
			RegisterScript(p_luaState, p_scriptName)
		)
	);

	// Update the script context to add the owner reference
	if (auto context = p_behaviour.GetScriptContext(); context.has_value() && context->IsValid())
	{
		auto& luaScriptContext = static_cast<OvCore::Scripting::LuaScriptContext&>(context.value());
		luaScriptContext.table["owner"] = &p_behaviour.owner;
		return true;
	}

	return false;
}

OvCore::Scripting::LuaScriptEngine::LuaScriptEngine(const std::string& p_scriptRootFolder) : m_scriptRootFolder(p_scriptRootFolder)
{
	CreateContext();
}

OvCore::Scripting::LuaScriptEngine::~LuaScriptEngine()
{
	DestroyContext();
}

std::vector<std::string> OvCore::Scripting::LuaScriptEngine::GetValidExtensions()
{
	return { ".lua" };
}

std::string OvCore::Scripting::LuaScriptEngine::GetDefaultScriptContent(const std::string& p_name)
{
	return "local " + p_name + " =\n{\n}\n\nfunction " + p_name + ":OnStart()\nend\n\nfunction " + p_name + ":OnUpdate(deltaTime)\nend\n\nreturn " + p_name;
}

std::string OvCore::Scripting::LuaScriptEngine::GetDefaultExtension()
{
	return ".lua";
}

void OvCore::Scripting::LuaScriptEngine::CreateContext()
{
	OVASSERT(m_luaState == nullptr, "A Lua context already exists!");

	m_luaState = std::make_unique<sol::state>();
	m_luaState->open_libraries(sol::lib::base, sol::lib::math);

	for (auto& callback : luaBindings)
	{
		callback(*m_luaState);
	}

	m_errorCount = 0;

	std::for_each(m_behaviours.begin(), m_behaviours.end(),
		[this](std::reference_wrapper<OvCore::ECS::Components::Behaviour> behaviour)
		{
			if (!RegisterBehaviour(*m_luaState, behaviour.get(), m_scriptRootFolder + behaviour.get().name + GetDefaultExtension()))
			{
				++m_errorCount;
			}
		}
	);

	if (m_errorCount > 0)
	{
		const std::string message = std::to_string(m_errorCount) + " script(s) failed to register";
		OVLOG_ERROR(message);
	}
}

void OvCore::Scripting::LuaScriptEngine::DestroyContext()
{
	OVASSERT(m_luaState != nullptr, "No valid Lua context");

	std::for_each(m_behaviours.begin(), m_behaviours.end(),
		[this](std::reference_wrapper<OvCore::ECS::Components::Behaviour> behaviour)
		{
			behaviour.get().ResetScriptContext();
		}
	);

	m_luaState.reset();
}

void OvCore::Scripting::LuaScriptEngine::AddBehaviour(OvCore::ECS::Components::Behaviour& p_toAdd)
{
	OVASSERT(m_luaState != nullptr, "No valid Lua context");

	m_behaviours.push_back(std::ref(p_toAdd));

	if (!RegisterBehaviour(*m_luaState, p_toAdd, m_scriptRootFolder + p_toAdd.name + GetDefaultExtension()))
	{
		++m_errorCount;
	}
}

void OvCore::Scripting::LuaScriptEngine::RemoveBehaviour(OvCore::ECS::Components::Behaviour& p_toRemove)
{
	if (m_luaState)
	{
		p_toRemove.ResetScriptContext();
	}

	m_behaviours.erase(
		std::remove_if(m_behaviours.begin(), m_behaviours.end(),
			[&p_toRemove](std::reference_wrapper< OvCore::ECS::Components::Behaviour> behaviour) {
				return &p_toRemove == &behaviour.get();
			}
		)
	);

	// Unconsidering a script is impossible with Lua, we have to reparse every behaviours
	// @note this might be costly, we should look into it more seriously.
	Reload(); 
}

void OvCore::Scripting::LuaScriptEngine::Reload()
{
	DestroyContext();
	CreateContext();
}

bool OvCore::Scripting::LuaScriptEngine::IsOk() const
{
	return m_luaState && m_errorCount == 0;
}

void OvCore::Scripting::LuaScriptEngine::OnAwake(OvCore::ECS::Components::Behaviour& p_target)
{
	ExecuteLuaFunction(p_target, "OnAwake");
}

void OvCore::Scripting::LuaScriptEngine::OnStart(OvCore::ECS::Components::Behaviour& p_target)
{
	ExecuteLuaFunction(p_target, "OnStart");
}

void OvCore::Scripting::LuaScriptEngine::OnEnable(OvCore::ECS::Components::Behaviour& p_target)
{
	ExecuteLuaFunction(p_target, "OnEnable");
}

void OvCore::Scripting::LuaScriptEngine::OnDisable(OvCore::ECS::Components::Behaviour& p_target)
{
	ExecuteLuaFunction(p_target, "OnDisable");
}

void OvCore::Scripting::LuaScriptEngine::OnDestroy(OvCore::ECS::Components::Behaviour& p_target)
{
	ExecuteLuaFunction(p_target, "OnDestroy");
}

void OvCore::Scripting::LuaScriptEngine::OnUpdate(OvCore::ECS::Components::Behaviour& p_target, float p_deltaTime)
{
	ExecuteLuaFunction(p_target, "OnUpdate", p_deltaTime);
}

void OvCore::Scripting::LuaScriptEngine::OnFixedUpdate(OvCore::ECS::Components::Behaviour& p_target, float p_deltaTime)
{
	ExecuteLuaFunction(p_target, "OnFixedUpdate", p_deltaTime);
}

void OvCore::Scripting::LuaScriptEngine::OnLateUpdate(OvCore::ECS::Components::Behaviour& p_target, float p_deltaTime)
{
	ExecuteLuaFunction(p_target, "OnLateUpdate", p_deltaTime);
}

void OvCore::Scripting::LuaScriptEngine::OnCollisionEnter(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject)
{
	ExecuteLuaFunction(p_target, "OnCollisionEnter", p_otherObject);
}

void OvCore::Scripting::LuaScriptEngine::OnCollisionStay(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject)
{
	ExecuteLuaFunction(p_target, "OnCollisionStay", p_otherObject);
}

void OvCore::Scripting::LuaScriptEngine::OnCollisionExit(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject)
{
	ExecuteLuaFunction(p_target, "OnCollisionExit", p_otherObject);
}

void OvCore::Scripting::LuaScriptEngine::OnTriggerEnter(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject)
{
	ExecuteLuaFunction(p_target, "OnTriggerEnter", p_otherObject);
}

void OvCore::Scripting::LuaScriptEngine::OnTriggerStay(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject)
{
	ExecuteLuaFunction(p_target, "OnTriggerStay", p_otherObject);
}

void OvCore::Scripting::LuaScriptEngine::OnTriggerExit(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject)
{
	ExecuteLuaFunction(p_target, "OnTriggerExit", p_otherObject);
}
