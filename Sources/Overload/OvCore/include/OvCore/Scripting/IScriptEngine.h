/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <string>

namespace OvCore::ECS::Components
{
	class Behaviour;
	class CPhysicalObject;
}

namespace OvCore::Scripting
{
	class IScriptEngine
	{
	public:
		virtual void AddBehaviour(OvCore::ECS::Components::Behaviour& p_toAdd) = 0;
		virtual void RemoveBehaviour(OvCore::ECS::Components::Behaviour& p_toRemove) = 0;
		virtual void Reload() = 0;
		virtual bool IsOk() const = 0;

		virtual void OnAwake(OvCore::ECS::Components::Behaviour& p_target) = 0;
		virtual void OnStart(OvCore::ECS::Components::Behaviour& p_target) = 0;
		virtual void OnEnable(OvCore::ECS::Components::Behaviour& p_target) = 0;
		virtual void OnDisable(OvCore::ECS::Components::Behaviour& p_target) = 0;
		virtual void OnDestroy(OvCore::ECS::Components::Behaviour& p_target) = 0;
		virtual void OnUpdate(OvCore::ECS::Components::Behaviour& p_target, float p_deltaTime) = 0;
		virtual void OnFixedUpdate(OvCore::ECS::Components::Behaviour& p_target, float p_deltaTime) = 0;
		virtual void OnLateUpdate(OvCore::ECS::Components::Behaviour& p_target, float p_deltaTime) = 0;
		virtual void OnCollisionEnter(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject) = 0;
		virtual void OnCollisionStay(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject) = 0;
		virtual void OnCollisionExit(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject) = 0;
		virtual void OnTriggerEnter(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject) = 0;
		virtual void OnTriggerStay(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject) = 0;
		virtual void OnTriggerExit(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject) = 0;
	};
}
