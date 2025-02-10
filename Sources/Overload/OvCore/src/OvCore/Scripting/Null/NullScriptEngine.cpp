/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <OvCore/Scripting/Null/NullScriptEngine.h>

void OvCore::Scripting::NullScriptEngine::AddBehaviour(OvCore::ECS::Components::Behaviour& p_toAdd)
{
}

void OvCore::Scripting::NullScriptEngine::RemoveBehaviour(OvCore::ECS::Components::Behaviour& p_toRemove)
{
}

void OvCore::Scripting::NullScriptEngine::Reload()
{
}

bool OvCore::Scripting::NullScriptEngine::IsOk() const
{
	return false;
}

void OvCore::Scripting::NullScriptEngine::OnAwake(OvCore::ECS::Components::Behaviour& p_target)
{
}

void OvCore::Scripting::NullScriptEngine::OnStart(OvCore::ECS::Components::Behaviour& p_target)
{
}

void OvCore::Scripting::NullScriptEngine::OnEnable(OvCore::ECS::Components::Behaviour& p_target)
{
}

void OvCore::Scripting::NullScriptEngine::OnDisable(OvCore::ECS::Components::Behaviour& p_target)
{
}

void OvCore::Scripting::NullScriptEngine::OnDestroy(OvCore::ECS::Components::Behaviour& p_target)
{
}

void OvCore::Scripting::NullScriptEngine::OnUpdate(OvCore::ECS::Components::Behaviour& p_target, float p_deltaTime)
{
}

void OvCore::Scripting::NullScriptEngine::OnFixedUpdate(OvCore::ECS::Components::Behaviour& p_target, float p_deltaTime)
{
}

void OvCore::Scripting::NullScriptEngine::OnLateUpdate(OvCore::ECS::Components::Behaviour& p_target, float p_deltaTime)
{
}

void OvCore::Scripting::NullScriptEngine::OnCollisionEnter(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject)
{
}

void OvCore::Scripting::NullScriptEngine::OnCollisionStay(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject)
{
}

void OvCore::Scripting::NullScriptEngine::OnCollisionExit(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject)
{
}

void OvCore::Scripting::NullScriptEngine::OnTriggerEnter(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject)
{
}

void OvCore::Scripting::NullScriptEngine::OnTriggerStay(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject)
{
}

void OvCore::Scripting::NullScriptEngine::OnTriggerExit(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject)
{
}
