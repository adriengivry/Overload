/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvEditor/Rendering/OutlineRenderFeature.h"
#include "OvEditor/Core/EditorActions.h"
#include "OvEditor/Settings/EditorSettings.h"

#include <OvCore/ECS/Components/CMaterialRenderer.h>

OvEditor::Rendering::OutlineRenderFeature::OutlineRenderFeature(OvRendering::Core::CompositeRenderer& p_renderer) :
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

void OvEditor::Rendering::OutlineRenderFeature::DrawOutline(
	OvCore::ECS::Actor& p_actor,
	const OvMaths::FVector4& p_color,
	float p_thickness
)
{
	DrawStencilPass(p_actor);
	DrawOutlinePass(p_actor, p_color, p_thickness);
}

void OvEditor::Rendering::OutlineRenderFeature::DrawStencilPass(OvCore::ECS::Actor& p_actor)
{
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

void OvEditor::Rendering::OutlineRenderFeature::DrawOutlinePass(
	OvCore::ECS::Actor& p_actor,
	const OvMaths::FVector4& p_color,
	float p_thickness
)
{
	m_outlineMaterial.Set("u_Diffuse", p_color);

	if (p_actor.IsActive())
	{
		/* Render static mesh outline and bounding spheres */
		if (auto modelRenderer = p_actor.GetComponent<OvCore::ECS::Components::CModelRenderer>(); modelRenderer && modelRenderer->GetModel())
		{
			RenderModelOutline(p_actor.transform.GetWorldMatrix(), *modelRenderer->GetModel(), p_thickness);
		}

		/* Render camera component outline */
		if (auto cameraComponent = p_actor.GetComponent<OvCore::ECS::Components::CCamera>(); cameraComponent)
		{
			auto translation = OvMaths::FMatrix4::Translation(p_actor.transform.GetWorldPosition());
			auto rotation = OvMaths::FQuaternion::ToMatrix4(p_actor.transform.GetWorldRotation());
			auto model = translation * rotation;

			RenderModelOutline(model, *EDITOR_CONTEXT(editorResources)->GetModel("Camera"), p_thickness);
		}

		for (auto& child : p_actor.GetChildren())
		{
			DrawOutlinePass(*child, p_color, p_thickness);
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
