/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvEditor/Rendering/OutlineRenderFeature.h"
#include "OvEditor/Core/EditorActions.h"
#include "OvEditor/Settings/EditorSettings.h"

#include <OvCore/ECS/Components/CMaterialRenderer.h>

OvEditor::Rendering::OutlineRenderFeature::OutlineRenderFeature(OvCore::Rendering::SceneRenderer& p_renderer) :
	OvRendering::Features::ARenderFeature(p_renderer)
{
	/* Stencil Fill Material */
	m_stencilFillMaterial.SetShader(EDITOR_CONTEXT(shaderManager)[":Shaders\\Unlit.glsl"]);
	m_stencilFillMaterial.SetBackfaceCulling(true);
	m_stencilFillMaterial.SetDepthTest(false);
	m_stencilFillMaterial.SetColorWriting(false);
	m_stencilFillMaterial.Set<OvRendering::Resources::Texture*>("u_DiffuseMap", nullptr);

	/* Outline Material */
	m_outlineMaterial.SetShader(EDITOR_CONTEXT(shaderManager)[":Shaders\\Unlit.glsl"]);
	m_outlineMaterial.Set<OvRendering::Resources::Texture*>("u_DiffuseMap", nullptr);
	m_outlineMaterial.SetDepthTest(false);
}

void OvEditor::Rendering::OutlineRenderFeature::DrawOutline(OvCore::ECS::Actor& p_actor)
{
	DrawStencilPass(p_actor);
	DrawOutlinePass(p_actor);
}

void OvEditor::Rendering::OutlineRenderFeature::DrawStencilPass(OvCore::ECS::Actor& p_actor)
{
	constexpr bool kSelected = true;

	if (p_actor.IsActive())
	{
		/* Render static mesh outline and bounding spheres */
		if (auto modelRenderer = p_actor.GetComponent<OvCore::ECS::Components::CModelRenderer>(); modelRenderer && modelRenderer->GetModel())
		{
			DrawModelToStencil(p_actor.transform.GetWorldMatrix(), *modelRenderer->GetModel());
		}

		/* Render camera component outline */
		if (auto cameraComponent = p_actor.GetComponent<OvCore::ECS::Components::CCamera>(); cameraComponent)
		{
			auto translation = OvMaths::FMatrix4::Translation(p_actor.transform.GetWorldPosition());
			auto rotation = OvMaths::FQuaternion::ToMatrix4(p_actor.transform.GetWorldRotation());
			auto model = translation * rotation;

			DrawModelToStencil(model, *EDITOR_CONTEXT(editorResources)->GetModel("Camera"));
		}
	}
}

void OvEditor::Rendering::OutlineRenderFeature::DrawOutlinePass(OvCore::ECS::Actor& p_actor)
{
	constexpr bool kSelected = true;
	constexpr float kOutlineWidth = kSelected ? 5.0f : 2.5f;

	m_outlineMaterial.Set("u_Diffuse", kSelected ? OvMaths::FVector4(1.f, 0.7f, 0.f, 1.0f) : OvMaths::FVector4(1.f, 1.f, 0.f, 1.0f));

	if (p_actor.IsActive())
	{
		/* Render static mesh outline and bounding spheres */
		if (auto modelRenderer = p_actor.GetComponent<OvCore::ECS::Components::CModelRenderer>(); modelRenderer && modelRenderer->GetModel())
		{
			RenderModelOutline(p_actor.transform.GetWorldMatrix(), *modelRenderer->GetModel(), kOutlineWidth);
		}

		/* Render camera component outline */
		if (auto cameraComponent = p_actor.GetComponent<OvCore::ECS::Components::CCamera>(); cameraComponent)
		{
			auto translation = OvMaths::FMatrix4::Translation(p_actor.transform.GetWorldPosition());
			auto rotation = OvMaths::FQuaternion::ToMatrix4(p_actor.transform.GetWorldRotation());
			auto model = translation * rotation;

			RenderModelOutline(model, *EDITOR_CONTEXT(editorResources)->GetModel("Camera"), kOutlineWidth);
		}

		for (auto& child : p_actor.GetChildren())
		{
			DrawOutlinePass(*child);
		}
	}
}

void OvEditor::Rendering::OutlineRenderFeature::DrawModelToStencil(const OvMaths::FMatrix4& p_worldMatrix, OvRendering::Resources::Model& p_model)
{
	m_driver.SetStencilMask(0xFF);
	m_renderer.DrawModelWithSingleMaterial(p_model, m_stencilFillMaterial, p_worldMatrix);
	m_driver.SetStencilMask(0x00);
}

void OvEditor::Rendering::OutlineRenderFeature::RenderModelOutline(const OvMaths::FMatrix4& p_worldMatrix, OvRendering::Resources::Model& p_model, float p_width)
{
	m_driver.SetStencilAlgorithm(OvRendering::Settings::EComparaisonAlgorithm::NOTEQUAL, 1, 0xFF);
	m_driver.SetRasterizationMode(OvRendering::Settings::ERasterizationMode::LINE);
	m_driver.SetRasterizationLinesWidth(p_width);
	m_renderer.DrawModelWithSingleMaterial(p_model, m_outlineMaterial, p_worldMatrix);
	m_driver.SetRasterizationLinesWidth(1.f);
	m_driver.SetRasterizationMode(OvRendering::Settings::ERasterizationMode::FILL);
	m_driver.SetStencilAlgorithm(OvRendering::Settings::EComparaisonAlgorithm::ALWAYS, 1, 0xFF);
}
