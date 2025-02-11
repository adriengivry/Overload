/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <string>
#include <vector>

namespace OvCore::ECS::Components
{
	class Behaviour;
	class CPhysicalObject;
}

namespace OvCore::Scripting
{
	/**
	* Interface for any scripting backend to implement.
	* This class defines the necessary methods that a scripting engine should provide
	* to interact with the engine's components and handle various lifecycle events.
	*/
	class IScriptEngine
	{
	public:
		/**
		* Returns a list of valid extensions for scripts.
		*/
		virtual std::vector<std::string> GetValidExtensions() = 0;

		/**
		* Returns the content for a default script
		* @param p_name Name of the class or object represented by this script
		*/
		virtual std::string GetDefaultScriptContent(const std::string& p_name) = 0;

		/**
		* Returns the extension for a default script
		*/
		virtual std::string GetDefaultExtension() = 0;

		/**
		* Adds a behaviour to the scripting engine.
		* @param p_toAdd The behaviour to add.
		*/
		virtual void AddBehaviour(OvCore::ECS::Components::Behaviour& p_toAdd) = 0;

		/**
		* Removes a behaviour from the scripting engine.
		* @param p_toRemove The behaviour to remove.
		*/
		virtual void RemoveBehaviour(OvCore::ECS::Components::Behaviour& p_toRemove) = 0;

		/**
		* Reloads the scripting engine.
		*/
		virtual void Reload() = 0;

		/**
		* Checks if the scripting engine is in a good state.
		* @return True if the engine is okay, false otherwise.
		*/
		virtual bool IsOk() const = 0;

		/**
		* Called when a behaviour is awakened.
		* @param p_target The target behaviour.
		*/
		virtual void OnAwake(OvCore::ECS::Components::Behaviour& p_target) = 0;

		/**
		* Called when a behaviour starts.
		* @param p_target The target behaviour.
		*/
		virtual void OnStart(OvCore::ECS::Components::Behaviour& p_target) = 0;

		/**
		* Called when a behaviour is enabled.
		* @param p_target The target behaviour.
		*/
		virtual void OnEnable(OvCore::ECS::Components::Behaviour& p_target) = 0;

		/**
		* Called when a behaviour is disabled.
		* @param p_target The target behaviour.
		*/
		virtual void OnDisable(OvCore::ECS::Components::Behaviour& p_target) = 0;

		/**
		* Called when a behaviour is destroyed.
		* @param p_target The target behaviour.
		*/
		virtual void OnDestroy(OvCore::ECS::Components::Behaviour& p_target) = 0;

		/**
		* Called every frame to update a behaviour.
		* @param p_target The target behaviour.
		* @param p_deltaTime The time elapsed since the last frame.
		*/
		virtual void OnUpdate(OvCore::ECS::Components::Behaviour& p_target, float p_deltaTime) = 0;

		/**
		* Called every physics frame to update a behaviour.
		* @param p_target The target behaviour.
		* @param p_deltaTime The time elapsed since the last physics frame.
		*/
		virtual void OnFixedUpdate(OvCore::ECS::Components::Behaviour& p_target, float p_deltaTime) = 0;

		/**
		* Called every frame after OnUpdate to perform late updates on a behaviour.
		* @param p_target The target behaviour.
		* @param p_deltaTime The time elapsed since the last frame.
		*/
		virtual void OnLateUpdate(OvCore::ECS::Components::Behaviour& p_target, float p_deltaTime) = 0;

		/**
		* Called when a behaviour's owner enters a collision with another physical object.
		* @param p_target The target behaviour.
		* @param p_otherObject The other physical object involved in the collision.
		*/
		virtual void OnCollisionEnter(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject) = 0;

		/**
		* Called when a behaviour's owner stays in collision with another physical object.
		* @param p_target The target behaviour.
		* @param p_otherObject The other physical object involved in the collision.
		*/
		virtual void OnCollisionStay(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject) = 0;

		/**
		* Called when a behaviour's owner exits a collision with another physical object.
		* @param p_target The target behaviour.
		* @param p_otherObject The other physical object involved in the collision.
		*/
		virtual void OnCollisionExit(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject) = 0;

		/**
		* Called when a behaviour's owner enters a trigger with another physical object.
		* @param p_target The target behaviour.
		* @param p_otherObject The other physical object involved in the trigger.
		*/
		virtual void OnTriggerEnter(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject) = 0;

		/**
		* Called when a behaviour's owner stays in a trigger with another physical object.
		* @param p_target The target behaviour.
		* @param p_otherObject The other physical object involved in the trigger.
		*/
		virtual void OnTriggerStay(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject) = 0;

		/**
		* Called when a behaviour's owner exits a trigger with another physical object.
		* @param p_target The target behaviour.
		* @param p_otherObject The other physical object involved in the trigger.
		*/
		virtual void OnTriggerExit(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject) = 0;
	};
}
