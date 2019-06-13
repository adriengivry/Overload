/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#include <OvTools/Utils/PathParser.h>
#include <OvUI/Plugins/DDTarget.h>

#include "OvEditor/Core/EditorRenderer.h"
#include "OvEditor/Core/EditorActions.h"
#include "OvEditor/Panels/AssetView.h"


OvEditor::Panels::AssetView::AssetView
(
	const std::string& p_title,
	bool p_opened,
	const OvUI::Settings::PanelWindowSettings& p_windowSettings
) : AViewControllable(p_title, p_opened, p_windowSettings)
{
	m_camera.SetClearColor({ 0.278f, 0.278f, 0.278f });

	m_resource = static_cast<OvRendering::Resources::Model*>(nullptr);
	m_image->AddPlugin<OvUI::Plugins::DDTarget<std::pair<std::string, OvUI::Widgets::Layout::Group*>>>("File").DataReceivedEvent += [this](auto p_data)
	{
		std::string path = p_data.first;

		switch (OvTools::Utils::PathParser::GetFileType(path))
		{
		case OvTools::Utils::PathParser::EFileType::MODEL:
			if (auto resource = OvCore::Global::ServiceLocator::Get<OvCore::ResourceManagement::ModelManager>().GetResource(path); resource)
				m_resource = resource;
			break;
		case OvTools::Utils::PathParser::EFileType::TEXTURE:
			if (auto resource = OvCore::Global::ServiceLocator::Get<OvCore::ResourceManagement::TextureManager>().GetResource(path); resource)
				m_resource = resource;
			break;

		case OvTools::Utils::PathParser::EFileType::MATERIAL:
			if (auto resource = OvCore::Global::ServiceLocator::Get<OvCore::ResourceManagement::MaterialManager>().GetResource(path); resource)
				m_resource = resource;
			break;
		}
	};
}

void OvEditor::Panels::AssetView::_Render_Impl()
{
	EDITOR_CONTEXT(renderer)->SetStencilMask(0xFF);
	EDITOR_CONTEXT(renderer)->Clear(m_camera);
	EDITOR_CONTEXT(renderer)->SetStencilMask(0x00);

	uint8_t glState = EDITOR_CONTEXT(renderer)->FetchGLState();
	EDITOR_CONTEXT(renderer)->ApplyStateMask(glState);

	m_editorRenderer.RenderGrid(m_cameraPosition, m_gridColor);

	if (auto pval = std::get_if<OvRendering::Resources::Model*>(&m_resource); pval && *pval)
		m_editorRenderer.RenderModelAsset(**pval);
	
	if (auto pval = std::get_if<OvRendering::Resources::Texture*>(&m_resource); pval && *pval)
		m_editorRenderer.RenderTextureAsset(**pval);
	
	if (auto pval = std::get_if<OvCore::Resources::Material*>(&m_resource); pval && *pval)
		m_editorRenderer.RenderMaterialAsset(**pval);

	EDITOR_CONTEXT(renderer)->ApplyStateMask(glState);
}

void OvEditor::Panels::AssetView::SetResource(ViewableResource p_resource)
{
	m_resource = p_resource;
}

OvEditor::Panels::AssetView::ViewableResource OvEditor::Panels::AssetView::GetResource() const
{
	return m_resource;
}
