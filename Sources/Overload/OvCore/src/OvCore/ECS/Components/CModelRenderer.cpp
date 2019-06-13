/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#include <OvUI/Widgets/Texts/Text.h>
#include <OvUI/Plugins/DDTarget.h>
#include <OvUI/Widgets/Visual/Image.h>

#include "OvCore/Global/ServiceLocator.h"
#include "OvCore/ResourceManagement/TextureManager.h"
#include "OvCore/ResourceManagement/ModelManager.h"
#include "OvCore/ResourceManagement/ShaderManager.h"
#include "OvCore/ECS/Components/CModelRenderer.h"
#include "OvCore/ECS/Components/CMaterialRenderer.h"
#include "OvCore/ECS/Actor.h"

OvCore::ECS::Components::CModelRenderer::CModelRenderer(ECS::Actor& p_owner) : AComponent(p_owner)
{
	m_modelChangedEvent += [this]
	{
		if (auto materialRenderer = owner.GetComponent<CMaterialRenderer>())
			materialRenderer->UpdateMaterialList();
	};
}

std::string OvCore::ECS::Components::CModelRenderer::GetName()
{
	return "Model Renderer";
}

void OvCore::ECS::Components::CModelRenderer::SetModel(OvRendering::Resources::Model* p_model)
{
	m_model = p_model;
	m_modelChangedEvent.Invoke();
}

OvRendering::Resources::Model * OvCore::ECS::Components::CModelRenderer::GetModel() const
{
	return m_model;
}

void OvCore::ECS::Components::CModelRenderer::OnSerialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	OvCore::Helpers::Serializer::SerializeModel(p_doc, p_node, "model", m_model);
}

void OvCore::ECS::Components::CModelRenderer::OnDeserialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode* p_node)
{
	OvCore::Helpers::Serializer::DeserializeModel(p_doc, p_node, "model", m_model);
}

void OvCore::ECS::Components::CModelRenderer::OnInspector(OvUI::Internal::WidgetContainer& p_root)
{
	OvCore::Helpers::GUIDrawer::DrawMesh(p_root, "Model", m_model, &m_modelChangedEvent);
}
