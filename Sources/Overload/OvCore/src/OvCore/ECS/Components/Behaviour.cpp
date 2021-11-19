/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <OvUI/Widgets/Texts/TextColored.h>
#include <OvDebug/Logger.h>

#include "OvCore/ECS/Actor.h"
#include "OvCore/ECS/Components/Behaviour.h"
#include "OvCore/Scripting/LuaBinder.h"

OvTools::Eventing::Event<OvCore::ECS::Components::Behaviour*> OvCore::ECS::Components::Behaviour::CreatedEvent;
OvTools::Eventing::Event<OvCore::ECS::Components::Behaviour*> OvCore::ECS::Components::Behaviour::DestroyedEvent;

OvCore::ECS::Components::Behaviour::Behaviour(ECS::Actor& p_owner, const std::string& p_name) :
	name(p_name), AComponent(p_owner)
{
	CreatedEvent.Invoke(this);
}

OvCore::ECS::Components::Behaviour::~Behaviour()
{
	DestroyedEvent.Invoke(this);
}

std::string OvCore::ECS::Components::Behaviour::GetName()
{
	return "Behaviour";
}

bool OvCore::ECS::Components::Behaviour::RegisterToLuaContext(sol::state& p_luaState, const std::string& p_scriptFolder)
{
	using namespace OvCore::Scripting;

	auto result = p_luaState.safe_script_file(p_scriptFolder + name + ".lua", &sol::script_pass_on_error);

	if (!result.valid())
	{
		sol::error err = result;
		OVLOG_ERROR(err.what());
		return false;
	}
	else
	{
		if (result.return_count() == 1 && result[0].is<sol::table>())
		{
			m_object = result[0];
			m_object["owner"] = &owner;
			return true;
		}
		else
		{
			OVLOG_ERROR("'" + name + ".lua' missing return expression");
			return false;
		}
	}
}

void OvCore::ECS::Components::Behaviour::UnregisterFromLuaContext()
{
	m_object = sol::nil;
}

sol::table& OvCore::ECS::Components::Behaviour::GetTable()
{
	return m_object;
}

void OvCore::ECS::Components::Behaviour::OnAwake()
{
	LuaCall("OnAwake");
}

void OvCore::ECS::Components::Behaviour::OnStart()
{
	LuaCall("OnStart");
}

void OvCore::ECS::Components::Behaviour::OnEnable()
{
	LuaCall("OnEnable");
}

void OvCore::ECS::Components::Behaviour::OnDisable()
{
	LuaCall("OnDisable");
}

void OvCore::ECS::Components::Behaviour::OnDestroy()
{
	LuaCall("OnEnd"); // Retro-compatibility
	LuaCall("OnDestroy");
}

void OvCore::ECS::Components::Behaviour::OnUpdate(float p_deltaTime)
{
	LuaCall("OnUpdate", p_deltaTime);
}

void OvCore::ECS::Components::Behaviour::OnFixedUpdate(float p_deltaTime)
{
	LuaCall("OnFixedUpdate", p_deltaTime);
}

void OvCore::ECS::Components::Behaviour::OnLateUpdate(float p_deltaTime)
{
	LuaCall("OnLateUpdate", p_deltaTime);
}

void OvCore::ECS::Components::Behaviour::OnCollisionEnter(Components::CPhysicalObject& p_otherObject)
{
	LuaCall("OnCollisionStart", p_otherObject); // Retro-compatibility
	LuaCall("OnCollisionEnter", p_otherObject);
}

void OvCore::ECS::Components::Behaviour::OnCollisionStay(Components::CPhysicalObject& p_otherObject)
{
	LuaCall("OnCollisionStay", p_otherObject);
}

void OvCore::ECS::Components::Behaviour::OnCollisionExit(Components::CPhysicalObject& p_otherObject)
{
	LuaCall("OnCollisionStop", p_otherObject); // Retro-compatibility
	LuaCall("OnCollisionExit", p_otherObject);
}

void OvCore::ECS::Components::Behaviour::OnTriggerEnter(Components::CPhysicalObject& p_otherObject)
{
	LuaCall("OnTriggerStart", p_otherObject); // Retro-compatibility
	LuaCall("OnTriggerEnter", p_otherObject);
}

void OvCore::ECS::Components::Behaviour::OnTriggerStay(Components::CPhysicalObject& p_otherObject)
{
	LuaCall("OnTriggerStay", p_otherObject);
}

void OvCore::ECS::Components::Behaviour::OnTriggerExit(Components::CPhysicalObject& p_otherObject)
{
	LuaCall("OnTriggerStop", p_otherObject); // Retro-compatibility
	LuaCall("OnTriggerExit", p_otherObject);
}

void OvCore::ECS::Components::Behaviour::OnSerialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
}

void OvCore::ECS::Components::Behaviour::OnDeserialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
}

void OvCore::ECS::Components::Behaviour::OnInspector(OvUI::Internal::WidgetContainer & p_root)
{
	using namespace OvMaths;
	using namespace OvCore::Helpers;

	if (m_object.valid())
	{
		p_root.CreateWidget<OvUI::Widgets::Texts::TextColored>("Ready", OvUI::Types::Color::Green);
		p_root.CreateWidget<OvUI::Widgets::Texts::TextColored>("Your script gets interpreted by the engine with success", OvUI::Types::Color::White);
	}
	else
	{
		p_root.CreateWidget<OvUI::Widgets::Texts::TextColored>("Compilation failed!", OvUI::Types::Color::Red);
		p_root.CreateWidget<OvUI::Widgets::Texts::TextColored>("Check the console for more information", OvUI::Types::Color::White);
	}
}
