/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
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
	m_camera.SetClearColor({ 0.098f, 0.098f, 0.098f });
	m_camera.SetFar(5000.0f);

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
	PrepareCamera();

	auto& driver = *EDITOR_CONTEXT(driver).get();

	m_fbo.Bind();

	driver.SetStencilMask(0xFF);
	
	OvMaths::FVector3 clearColor = m_camera.GetClearColor();
	driver.SetClearColor(clearColor.x, clearColor.y, clearColor.z);
	driver.Clear(true, true, false);

	driver.SetStencilMask(0x00);

	driver.UpdateStateMask();
	OvRendering::Data::StateMask stateMask = driver.GetStateMask();

	EDITOR_RENDERER().RenderGrid(m_cameraPosition, m_gridColor);

	if (auto pval = std::get_if<OvRendering::Resources::Model*>(&m_resource); pval && *pval)
		EDITOR_RENDERER().RenderModelAsset(**pval);
	
	if (auto pval = std::get_if<OvRendering::Resources::Texture*>(&m_resource); pval && *pval)
		EDITOR_RENDERER().RenderTextureAsset(**pval);
	
	if (auto pval = std::get_if<OvCore::Resources::Material*>(&m_resource); pval && *pval)
		EDITOR_RENDERER().RenderMaterialAsset(**pval);

	driver.ApplyStateMask(stateMask);

	m_fbo.Unbind();
}

void OvEditor::Panels::AssetView::SetResource(ViewableResource p_resource)
{
	m_resource = p_resource;
}

OvEditor::Panels::AssetView::ViewableResource OvEditor::Panels::AssetView::GetResource() const
{
	return m_resource;
}
