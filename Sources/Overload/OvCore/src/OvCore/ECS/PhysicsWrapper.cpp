/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#include "OvCore/ECS/PhysicsWrapper.h"

#include "OvCore/Global/ServiceLocator.h"

#include <OvPhysics/Core/PhysicsEngine.h>

std::optional<OvCore::ECS::PhysicsWrapper::RaycastHit> OvCore::ECS::PhysicsWrapper::Raycast(OvMaths::FVector3 p_origin, OvMaths::FVector3 p_direction, float p_distance)
{
	if (auto result = OVSERVICE(OvPhysics::Core::PhysicsEngine).Raycast(p_origin, p_direction, p_distance))
	{
		RaycastHit finalResult;

		finalResult.FirstResultObject = std::addressof(result.value().FirstResultObject->GetUserData<std::reference_wrapper<Components::CPhysicalObject>>().get());
		for (auto object : result.value().ResultObjects)
			finalResult.ResultObjects.push_back(std::addressof(object->GetUserData<std::reference_wrapper<Components::CPhysicalObject>>().get()));

		return finalResult;
	}
	else
		return {};
}
