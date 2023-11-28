/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <OvTools/Utils/PathParser.h>
#include <OvUI/Plugins/DDTarget.h>
#include <OvCore/Rendering/EngineBufferRenderFeature.h>
#include <OvRendering/Features/DebugShapeRenderFeature.h>
#include <OvRendering/Features/LightingRenderFeature.h>

#include "OvEditor/Rendering/EditorViewRenderFeature.h"
#include "OvEditor/Core/EditorActions.h"
#include "OvEditor/Panels/AssetView.h"

/*
void OvEditor::Rendering::AssetViewRenderFeature::RenderModelAsset(OvRendering::Resources::Model& p_model)
{
	FMatrix4 model = OvMaths::FMatrix4::Scaling({ 3.f, 3.f, 3.f });
	DrawModelWithSingleMaterial(p_model, m_defaultMaterial, &model);
}

void OvEditor::Rendering::AssetViewRenderFeature::RenderTextureAsset(OvRendering::Resources::Texture & p_texture)
{
	FMatrix4 model = FMatrix4::RotateOnAxisX(FMatrix4::Scaling({ 5.f, 5.f, 5.f }), 90.f * 0.0174f);

	m_textureMaterial.Set<Texture*>("u_DiffuseMap", &p_texture);
	DrawModelWithSingleMaterial(*m_context.editorResources->GetModel("Plane"), m_textureMaterial, &model);
}

void OvEditor::Rendering::AssetViewRenderFeature::RenderMaterialAsset(OvCore::Resources::Material & p_material)
{
	FMatrix4 model = OvMaths::FMatrix4::Scaling({ 3.f, 3.f, 3.f });
	DrawModelWithSingleMaterial(*m_context.editorResources->GetModel("Sphere"), p_material, &model, &m_emptyMaterial);
}
*/

OvEditor::Panels::AssetView::AssetView
(
	const std::string& p_title,
	bool p_opened,
	const OvUI::Settings::PanelWindowSettings& p_windowSettings
) : AViewControllable(p_title, p_opened, p_windowSettings)
{
	m_renderer = std::make_unique<OvRendering::Core::CompositeRenderer>(*EDITOR_CONTEXT(driver));
	auto& renderer = GetRendererAs<OvRendering::Core::CompositeRenderer>();
	auto& lightingFeature = renderer.AddFeature<OvRendering::Features::LightingRenderFeature>();
	// TODO: Reenable, for something reason having multiple UBOs break the game view right now
	// renderer.AddFeature<OvCore::Rendering::EngineBufferRenderFeature>();
	renderer.AddFeature<OvEditor::Rendering::EditorViewRenderFeature>();

	OvMaths::FTransform simulatedLightTransform;
	simulatedLightTransform.SetLocalRotation(OvMaths::FQuaternion({ 45.f, 180.f, 10.f }));

	OvRendering::Entities::Light simulatedDirectionalLight(simulatedLightTransform, OvRendering::Entities::Light::Type::DIRECTIONAL);
	simulatedDirectionalLight.color = { 1.f, 1.f, 1.f };
	simulatedDirectionalLight.intensity = 1.f;

	OvRendering::Entities::Light simulatedAmbientLight(simulatedLightTransform, OvRendering::Entities::Light::Type::AMBIENT_SPHERE);
	simulatedAmbientLight.color = { 0.07f, 0.07f, 0.07f };
	simulatedAmbientLight.intensity = 1.f;
	simulatedAmbientLight.constant = 1000.0f;

	lightingFeature.UploadLightingData({
		std::ref(simulatedDirectionalLight),
		std::ref(simulatedAmbientLight)
	}, std::nullopt);

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
	auto& renderer = GetRendererAs<OvRendering::Core::CompositeRenderer>();
	auto& engineBufferFeature = renderer.GetFeature<OvCore::Rendering::EngineBufferRenderFeature>();
	auto& editorViewFeature = renderer.GetFeature<OvEditor::Rendering::EditorViewRenderFeature>();

	auto [winWidth, winHeight] = GetSafeSize();

	engineBufferFeature.UploadViewData(m_camera, winWidth, winHeight);

	auto& driver = *EDITOR_CONTEXT(driver).get();

	driver.SetStencilMask(0xFF);
	
	OvMaths::FVector3 clearColor = m_camera.GetClearColor();
	driver.SetClearColor(clearColor.x, clearColor.y, clearColor.z);
	driver.Clear(true, true, false);

	driver.SetStencilMask(0x00);

	driver.UpdateStateMask();
	OvRendering::Data::StateMask stateMask = driver.GetStateMask();

	editorViewFeature.RenderGrid(m_camera.GetPosition(), m_gridColor);

	if (auto pval = std::get_if<OvRendering::Resources::Model*>(&m_resource); pval && *pval)
		editorViewFeature.RenderModelAsset(**pval);
	
	if (auto pval = std::get_if<OvRendering::Resources::Texture*>(&m_resource); pval && *pval)
		editorViewFeature.RenderTextureAsset(**pval);
	
	if (auto pval = std::get_if<OvCore::Resources::Material*>(&m_resource); pval && *pval)
		editorViewFeature.RenderMaterialAsset(**pval);

	driver.ApplyStateMask(stateMask);
}

void OvEditor::Panels::AssetView::SetResource(ViewableResource p_resource)
{
	m_resource = p_resource;
}

OvEditor::Panels::AssetView::ViewableResource OvEditor::Panels::AssetView::GetResource() const
{
	return m_resource;
}
