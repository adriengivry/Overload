/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#include "OvCore/ECS/Components/AComponent.h"
#include "OvCore/ECS/Actor.h"

OvCore::ECS::Components::AComponent::AComponent(ECS::Actor& p_owner) : owner(p_owner)
{
}

OvCore::ECS::Components::AComponent::~AComponent()
{
	if (owner.IsActive())
	{
		OnDisable();
		OnDestroy();
	}
}
