/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <OvUI/Widgets/Texts/Text.h>
#include <OvUI/Widgets/Drags/DragFloat.h>
#include <OvUI/Widgets/Selection/ColorEdit.h>

#include "OvCore/ECS/Actor.h"

#include "OvCore/ECS/Components/CDirectionalLight.h"

OvCore::ECS::Components::CDirectionalLight::CDirectionalLight(ECS::Actor & p_owner) :
	CLight(p_owner)
{
	m_data.type = static_cast<float>(OvRendering::Entities::Light::Type::DIRECTIONAL);
}

std::string OvCore::ECS::Components::CDirectionalLight::GetName()
{
	return "Directional Light";
}

void OvCore::ECS::Components::CDirectionalLight::OnSerialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	CLight::OnSerialize(p_doc, p_node);
}

void OvCore::ECS::Components::CDirectionalLight::OnDeserialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	CLight::OnDeserialize(p_doc, p_node);
}

void OvCore::ECS::Components::CDirectionalLight::OnInspector(OvUI::Internal::WidgetContainer& p_root)
{
	CLight::OnInspector(p_root);
}
