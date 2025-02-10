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
	* Lua scripting backend implementation
	*/
	class LuaScriptEngine : public IScriptEngine
	{
	public:
		LuaScriptEngine(const std::string& p_scriptRootFolder);
		~LuaScriptEngine();

		void AddBehaviour(OvCore::ECS::Components::Behaviour& p_toAdd);
		void RemoveBehaviour(OvCore::ECS::Components::Behaviour& p_toRemove);
		void Reload();
		bool IsOk() const;

		void OnAwake(OvCore::ECS::Components::Behaviour& p_target) override;
		void OnStart(OvCore::ECS::Components::Behaviour& p_target) override;
		void OnEnable(OvCore::ECS::Components::Behaviour& p_target) override;
		void OnDisable(OvCore::ECS::Components::Behaviour& p_target) override;
		void OnDestroy(OvCore::ECS::Components::Behaviour& p_target) override;
		void OnUpdate(OvCore::ECS::Components::Behaviour& p_target, float p_deltaTime) override;
		void OnFixedUpdate(OvCore::ECS::Components::Behaviour& p_target, float p_deltaTime) override;
		void OnLateUpdate(OvCore::ECS::Components::Behaviour& p_target, float p_deltaTime) override;
		void OnCollisionEnter(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject) override;
		void OnCollisionStay(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject) override;
		void OnCollisionExit(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject) override;
		void OnTriggerEnter(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject) override;
		void OnTriggerStay(OvCore::ECS::Components::Behaviour& p_target, OvCore::ECS::Components::CPhysicalObject& p_otherObject) override;
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
