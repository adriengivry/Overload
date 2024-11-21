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
	m_data.type = OvRendering::Settings::ELightType::DIRECTIONAL;
}

std::string OvCore::ECS::Components::CDirectionalLight::GetName()
{
	return "Directional Light";
}

void OvCore::ECS::Components::CDirectionalLight::SetCastShadows(bool p_enabled)
{
	m_data.castShadows = p_enabled;
}

bool OvCore::ECS::Components::CDirectionalLight::GetCastShadows() const
{
	return m_data.castShadows;
}

void OvCore::ECS::Components::CDirectionalLight::OnSerialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	CLight::OnSerialize(p_doc, p_node);
	OvCore::Helpers::Serializer::SerializeBoolean(p_doc, p_node, "cast_shadows", m_data.castShadows);
	OvCore::Helpers::Serializer::SerializeFloat(p_doc, p_node, "shadow_area_size", m_data.shadowAreaSize);
}

void OvCore::ECS::Components::CDirectionalLight::OnDeserialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	CLight::OnDeserialize(p_doc, p_node);
	m_data.castShadows = OvCore::Helpers::Serializer::DeserializeBoolean(p_doc, p_node, "cast_shadows");
	m_data.shadowAreaSize = OvCore::Helpers::Serializer::DeserializeFloat(p_doc, p_node, "shadow_area_size");
}

void OvCore::ECS::Components::CDirectionalLight::OnInspector(OvUI::Internal::WidgetContainer& p_root)
{
	CLight::OnInspector(p_root);
	OvCore::Helpers::GUIDrawer::DrawBoolean(p_root, "Cast Shadows", m_data.castShadows);
	OvCore::Helpers::GUIDrawer::DrawScalar(p_root, "Shadow Area Size", m_data.shadowAreaSize);
}
