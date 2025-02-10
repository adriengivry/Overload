/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <OvCore/Scripting/Null/NullScriptingBackend.h>

void OvCore::Scripting::NullScriptingBackend::AddBehaviour(OvCore::ECS::Components::Behaviour& p_toAdd)
{
}

void OvCore::Scripting::NullScriptingBackend::RemoveBehaviour(OvCore::ECS::Components::Behaviour& p_toRemove)
{
}

void OvCore::Scripting::NullScriptingBackend::Reload()
{
}

bool OvCore::Scripting::NullScriptingBackend::IsOk() const
{
	return false;
}

void OvCore::Scripting::NullScriptingBackend::OnAwake(OvCore::ECS::Components::Behaviour& p_target)
{
}

void OvCore::Scripting::NullScriptingBackend::OnStart(OvCore::ECS::Components::Behaviour& p_target)
{
}

void OvCore::Scripting::NullScriptingBackend::OnEnable(OvCore::ECS::Components::Behaviour& p_target)
{
}

void OvCore::Scripting::NullScriptingBackend::OnDisable(OvCore::ECS::Components::Behaviour& p_target)
{
}

void OvCore::Scripting::NullScriptingBackend::OnDestroy(OvCore::ECS::Components::Behaviour& p_target)
{
}

void OvCore::Scripting::NullScriptingBackend::OnUpdate(OvCore::ECS::Components::Behaviour& p_target, float p_deltaTime)
{
}

void OvCore::Scripting::NullScriptingBackend::OnFixedUpdate(OvCore::ECS::Components::Behaviour& p_target, float p_deltaTime)
{
}

void OvCore::Scripting::NullScriptingBackend::OnLateUpdate(OvCore::ECS::Components::Behaviour& p_target, float p_deltaTime)
{
}

void OvCore::Scripting::NullScriptingBackend::OnCollisionEnter(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject)
{
}

void OvCore::Scripting::NullScriptingBackend::OnCollisionStay(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject)
{
}

void OvCore::Scripting::NullScriptingBackend::OnCollisionExit(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject)
{
}

void OvCore::Scripting::NullScriptingBackend::OnTriggerEnter(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject)
{
}

void OvCore::Scripting::NullScriptingBackend::OnTriggerStay(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject)
{
}

void OvCore::Scripting::NullScriptingBackend::OnTriggerExit(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject)
{
}
