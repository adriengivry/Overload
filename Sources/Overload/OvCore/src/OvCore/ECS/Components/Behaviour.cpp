/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <OvUI/Widgets/Texts/TextColored.h>
#include <OvDebug/Logger.h>

#include <OvCore/ECS/Actor.h>
#include <OvCore/ECS/Components/Behaviour.h>
#include <OvCore/Global/ServiceLocator.h>
#include <OvCore/Scripting/IScriptEngine.h>

OvCore::ECS::Components::Behaviour::Behaviour(ECS::Actor& p_owner, const std::string& p_name) :
	name(p_name), AComponent(p_owner)
{
	OVSERVICE(Scripting::IScriptEngine).AddBehaviour(*this);
}

OvCore::ECS::Components::Behaviour::~Behaviour()
{
	OVSERVICE(Scripting::IScriptEngine).RemoveBehaviour(*this);
}

std::string OvCore::ECS::Components::Behaviour::GetName()
{
	return "Behaviour";
}

void OvCore::ECS::Components::Behaviour::SetScriptContext(std::unique_ptr<Scripting::IScriptContext>&& p_scriptContext)
{
	m_scriptContext = std::move(p_scriptContext);
}

OvTools::Utils::OptRef<OvCore::Scripting::IScriptContext> OvCore::ECS::Components::Behaviour::GetScriptContext()
{
	if (m_scriptContext)
	{
		return { *m_scriptContext };
	}

	return std::nullopt;
}

void OvCore::ECS::Components::Behaviour::ResetScriptContext()
{
	m_scriptContext.reset();
}

void OvCore::ECS::Components::Behaviour::OnAwake()
{
	OVSERVICE(Scripting::IScriptEngine).OnAwake(*this);
}

void OvCore::ECS::Components::Behaviour::OnStart()
{
	OVSERVICE(Scripting::IScriptEngine).OnAwake(*this);
}

void OvCore::ECS::Components::Behaviour::OnEnable()
{
	OVSERVICE(Scripting::IScriptEngine).OnEnable(*this);
}

void OvCore::ECS::Components::Behaviour::OnDisable()
{
	OVSERVICE(Scripting::IScriptEngine).OnDisable(*this);
}

void OvCore::ECS::Components::Behaviour::OnDestroy()
{
	OVSERVICE(Scripting::IScriptEngine).OnDestroy(*this);
}

void OvCore::ECS::Components::Behaviour::OnUpdate(float p_deltaTime)
{
	OVSERVICE(Scripting::IScriptEngine).OnUpdate(*this, p_deltaTime);
}

void OvCore::ECS::Components::Behaviour::OnFixedUpdate(float p_deltaTime)
{
	OVSERVICE(Scripting::IScriptEngine).OnFixedUpdate(*this, p_deltaTime);
}

void OvCore::ECS::Components::Behaviour::OnLateUpdate(float p_deltaTime)
{
	OVSERVICE(Scripting::IScriptEngine).OnLateUpdate(*this, p_deltaTime);
}

void OvCore::ECS::Components::Behaviour::OnCollisionEnter(Components::CPhysicalObject& p_otherObject)
{
	OVSERVICE(Scripting::IScriptEngine).OnCollisionEnter(*this, p_otherObject);
}

void OvCore::ECS::Components::Behaviour::OnCollisionStay(Components::CPhysicalObject& p_otherObject)
{
	OVSERVICE(Scripting::IScriptEngine).OnCollisionStay(*this, p_otherObject);
}

void OvCore::ECS::Components::Behaviour::OnCollisionExit(Components::CPhysicalObject& p_otherObject)
{
	OVSERVICE(Scripting::IScriptEngine).OnCollisionExit(*this, p_otherObject);
}

void OvCore::ECS::Components::Behaviour::OnTriggerEnter(Components::CPhysicalObject& p_otherObject)
{
	OVSERVICE(Scripting::IScriptEngine).OnTriggerEnter(*this, p_otherObject);
}

void OvCore::ECS::Components::Behaviour::OnTriggerStay(Components::CPhysicalObject& p_otherObject)
{
	OVSERVICE(Scripting::IScriptEngine).OnTriggerStay(*this, p_otherObject);
}

void OvCore::ECS::Components::Behaviour::OnTriggerExit(Components::CPhysicalObject& p_otherObject)
{
	OVSERVICE(Scripting::IScriptEngine).OnTriggerExit(*this, p_otherObject);
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

	if (m_scriptContext && m_scriptContext->IsValid())
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
