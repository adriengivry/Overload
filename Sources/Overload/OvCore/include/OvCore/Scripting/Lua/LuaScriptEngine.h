/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <vector>
#include <memory>

#include <OvCore/Scripting/IScriptEngine.h>

namespace sol
{
	class state;
}

namespace OvCore::Scripting
{
	/**
	* Lua scripting backend implementation.
	* This class provides the implementation of the IScriptEngine interface using Lua as the scripting language.
	*/
	class LuaScriptEngine : public IScriptEngine
	{
	public:
		/**
		* Constructor for the LuaScriptEngine.
		* @param p_scriptRootFolder The root folder where Lua scripts are located.
		*/
		LuaScriptEngine(const std::string& p_scriptRootFolder);

		/**
		* Destructor for the LuaScriptEngine.
		*/
		~LuaScriptEngine();

		/**
		* Adds a behaviour to the scripting engine.
		* @param p_toAdd The behaviour to add.
		*/
		void AddBehaviour(OvCore::ECS::Components::Behaviour& p_toAdd) override;

		/**
		* Removes a behaviour from the scripting engine.
		* @param p_toRemove The behaviour to remove.
		*/
		void RemoveBehaviour(OvCore::ECS::Components::Behaviour& p_toRemove) override;

		/**
		* Reloads the scripting engine.
		*/
		void Reload() override;

		/**
		* Checks if the scripting engine is in a good state.
		* @return True if the engine is okay, false otherwise.
		*/
		bool IsOk() const override;

		/**
		* Called when a behaviour is awakened.
		* @param p_target The target behaviour.
		*/
		void OnAwake(OvCore::ECS::Components::Behaviour& p_target) override;

		/**
		* Called when a behaviour starts.
		* @param p_target The target behaviour.
		*/
		void OnStart(OvCore::ECS::Components::Behaviour& p_target) override;

		/**
		* Called when a behaviour is enabled.
		* @param p_target The target behaviour.
		*/
		void OnEnable(OvCore::ECS::Components::Behaviour& p_target) override;

		/**
		* Called when a behaviour is disabled.
		* @param p_target The target behaviour.
		*/
		void OnDisable(OvCore::ECS::Components::Behaviour& p_target) override;

		/**
		* Called when a behaviour is destroyed.
		* @param p_target The target behaviour.
		*/
		void OnDestroy(OvCore::ECS::Components::Behaviour& p_target) override;

		/**
		* Called every frame to update a behaviour.
		* @param p_target The target behaviour.
		* @param p_deltaTime The time elapsed since the last frame.
		*/
		void OnUpdate(OvCore::ECS::Components::Behaviour& p_target, float p_deltaTime) override;

		/**
		* Called every physics frame to update a behaviour.
		* @param p_target The target behaviour.
		* @param p_deltaTime The time elapsed since the last physics frame.
		*/
		void OnFixedUpdate(OvCore::ECS::Components::Behaviour& p_target, float p_deltaTime) override;

		/**
		* Called every frame after OnUpdate to perform late updates on a behaviour.
		* @param p_target The target behaviour.
		* @param p_deltaTime The time elapsed since the last frame.
		*/
		void OnLateUpdate(OvCore::ECS::Components::Behaviour& p_target, float p_deltaTime) override;

		/**
		* Called when a behaviour's owner enters a collision with another physical object.
		* @param p_target The target behaviour.
		* @param p_otherObject The other physical object involved in the collision.
		*/
		void OnCollisionEnter(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject) override;

		/**
		* Called when a behaviour's owner stays in collision with another physical object.
		* @param p_target The target behaviour.
		* @param p_otherObject The other physical object involved in the collision.
		*/
		void OnCollisionStay(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject) override;

		/**
		* Called when a behaviour's owner exits a collision with another physical object.
		* @param p_target The target behaviour.
		* @param p_otherObject The other physical object involved in the collision.
		*/
		void OnCollisionExit(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject) override;

		/**
		* Called when a behaviour's owner enters a trigger with another physical object.
		* @param p_target The target behaviour.
		* @param p_otherObject The other physical object involved in the trigger.
		*/
		void OnTriggerEnter(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject) override;

		/**
		* Called when a behaviour's owner stays in a trigger with another physical object.
		* @param p_target The target behaviour.
		* @param p_otherObject The other physical object involved in the trigger.
		*/
		void OnTriggerStay(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject) override;

		/**
		* Called when a behaviour's owner exits a trigger with another physical object.
		* @param p_target The target behaviour.
		* @param p_otherObject The other physical object involved in the trigger.
		*/
		void OnTriggerExit(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject) override;

	private:
		void CreateContext();
		void DestroyContext();

	private:
		std::unique_ptr<sol::state> m_luaState;
		std::string m_scriptRootFolder;
		std::vector<std::reference_wrapper<OvCore::ECS::Components::Behaviour>> m_behaviours;
		bool m_isOk;
	};
}
