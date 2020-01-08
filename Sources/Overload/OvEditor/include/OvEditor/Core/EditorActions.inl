/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "OvEditor/Core/EditorActions.h"

namespace OvEditor::Core
{
	template<typename T>
	inline OvCore::ECS::Actor & EditorActions::CreateMonoComponentActor(bool p_focusOnCreation, OvCore::ECS::Actor* p_parent)
	{
		auto& instance = CreateEmptyActor(false, p_parent);

		instance.AddComponent<T>();

		if (p_focusOnCreation)
			SelectActor(instance);

		return instance;
	}
}