/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <OvCore/Scripting/IScriptEngine.h>

namespace OvCore::Scripting
{
	/**
	* Dummy scripting backend that does nothing.
	* This class provides a null implementation of the IScriptEngine interface.
	* All methods are implemented but do not perform any operations.
	*/
	class NullScriptEngine : public IScriptEngine
	{
	public:
		/**
		* Adds a behaviour to the scripting engine.
		* This implementation does nothing.
		* @param p_toAdd The behaviour to add.
		*/
		void AddBehaviour(OvCore::ECS::Components::Behaviour& p_toAdd) override;

		/**
		* Removes a behaviour from the scripting engine.
		* This implementation does nothing.
		* @param p_toRemove The behaviour to remove.
		*/
		void RemoveBehaviour(OvCore::ECS::Components::Behaviour& p_toRemove) override;

		/**
		* Reloads the scripting engine.
		* This implementation does nothing.
		*/
		void Reload() override;

		/**
		* Checks if the scripting engine is in a good state.
		* This implementation always returns true.
		* @return True if the engine is okay, false otherwise.
		*/
		bool IsOk() const override;

		/**
		* Called when a behaviour is awakened.
		* This implementation does nothing.
		* @param p_target The target behaviour.
		*/
		void OnAwake(OvCore::ECS::Components::Behaviour& p_target) override;

		/**
		* Called when a behaviour starts.
		* This implementation does nothing.
		* @param p_target The target behaviour.
		*/
		void OnStart(OvCore::ECS::Components::Behaviour& p_target) override;

		/**
		* Called when a behaviour is enabled.
		* This implementation does nothing.
		* @param p_target The target behaviour.
		*/
		void OnEnable(OvCore::ECS::Components::Behaviour& p_target) override;

		/**
		* Called when a behaviour is disabled.
		* This implementation does nothing.
		* @param p_target The target behaviour.
		*/
		void OnDisable(OvCore::ECS::Components::Behaviour& p_target) override;

		/**
		* Called when a behaviour is destroyed.
		* This implementation does nothing.
		* @param p_target The target behaviour.
		*/
		void OnDestroy(OvCore::ECS::Components::Behaviour& p_target) override;

		/**
		* Called every frame to update a behaviour.
		* This implementation does nothing.
		* @param p_target The target behaviour.
		* @param p_deltaTime The time elapsed since the last frame.
		*/
		void OnUpdate(OvCore::ECS::Components::Behaviour& p_target, float p_deltaTime) override;

		/**
		* Called every physics frame to update a behaviour.
		* This implementation does nothing.
		* @param p_target The target behaviour.
		* @param p_deltaTime The time elapsed since the last physics frame.
		*/
		void OnFixedUpdate(OvCore::ECS::Components::Behaviour& p_target, float p_deltaTime) override;

		/**
		* Called every frame after OnUpdate to perform late updates on a behaviour.
		* This implementation does nothing.
		* @param p_target The target behaviour.
		* @param p_deltaTime The time elapsed since the last frame.
		*/
		void OnLateUpdate(OvCore::ECS::Components::Behaviour& p_target, float p_deltaTime) override;

		/**
		* Called when a behaviour's owner enters a collision with another physical object.
		* This implementation does nothing.
		* @param p_target The target behaviour.
		* @param p_otherObject The other physical object involved in the collision.
		*/
		void OnCollisionEnter(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject) override;

		/**
		* Called when a behaviour's owner stays in collision with another physical object.
		* This implementation does nothing.
		* @param p_target The target behaviour.
		* @param p_otherObject The other physical object involved in the collision.
		*/
		void OnCollisionStay(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject) override;

		/**
		* Called when a behaviour's owner exits a collision with another physical object.
		* This implementation does nothing.
		* @param p_target The target behaviour.
		* @param p_otherObject The other physical object involved in the collision.
		*/
		void OnCollisionExit(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject) override;

		/**
		* Called when a behaviour's owner enters a trigger with another physical object.
		* This implementation does nothing.
		* @param p_target The target behaviour.
		* @param p_otherObject The other physical object involved in the trigger.
		*/
		void OnTriggerEnter(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject) override;

		/**
		* Called when a behaviour's owner stays in a trigger with another physical object.
		* This implementation does nothing.
		* @param p_target The target behaviour.
		* @param p_otherObject The other physical object involved in the trigger.
		*/
		void OnTriggerStay(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject) override;

		/**
		* Called when a behaviour's owner exits a trigger with another physical object.
		* This implementation does nothing.
		* @param p_target The target behaviour.
		* @param p_otherObject The other physical object involved in the trigger.
		*/
		void OnTriggerExit(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject) override;
	};
}
