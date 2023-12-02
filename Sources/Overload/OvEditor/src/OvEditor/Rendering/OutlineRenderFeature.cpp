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
	m_renderer.pso.stencilMask = 0xFF; // Enable writing to the stencil buffer
	DrawActorToStencil(p_actor);
	m_renderer.pso.stencilMask = 0x00; // Disable writing to the stencil buffer
}

void OvEditor::Rendering::OutlineRenderFeature::DrawOutlinePass(OvCore::ECS::Actor& p_actor, const OvMaths::FVector4& p_color, float p_thickness)
{
	// Setup PSO
	m_renderer.pso.stencilTest = true;
	m_renderer.pso.stencilFailOp = OvRendering::Settings::EOperation::KEEP;
	m_renderer.pso.depthFailOp = OvRendering::Settings::EOperation::KEEP;
	m_renderer.pso.bothPassOp = OvRendering::Settings::EOperation::REPLACE;
	m_renderer.pso.stencilAlgorithm = OvRendering::Settings::EComparaisonAlgorithm::NOTEQUAL;
	m_renderer.pso.stencilAlgorithmReference = 1;
	m_renderer.pso.stencilAlgorithmMask = 0xFF;
	m_renderer.pso.rasterizationMode = OvRendering::Settings::ERasterizationMode::LINE;
	m_renderer.pso.rasterizationLinesWidth = p_thickness;

	// Prepare the outline material
	m_outlineMaterial.Set("u_Diffuse", p_color);

	DrawActorOutline(p_actor);
}

void OvEditor::Rendering::OutlineRenderFeature::DrawActorToStencil(OvCore::ECS::Actor& p_actor)
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

void OvEditor::Rendering::OutlineRenderFeature::DrawActorOutline(OvCore::ECS::Actor& p_actor)
{
	if (p_actor.IsActive())
	{
		if (auto modelRenderer = p_actor.GetComponent<OvCore::ECS::Components::CModelRenderer>(); modelRenderer && modelRenderer->GetModel())
		{
			DrawModelOutline(p_actor.transform.GetWorldMatrix(), *modelRenderer->GetModel());
		}

		if (auto cameraComponent = p_actor.GetComponent<OvCore::ECS::Components::CCamera>(); cameraComponent)
		{
			auto translation = OvMaths::FMatrix4::Translation(p_actor.transform.GetWorldPosition());
			auto rotation = OvMaths::FQuaternion::ToMatrix4(p_actor.transform.GetWorldRotation());
			auto model = translation * rotation;
			DrawModelOutline(model, *EDITOR_CONTEXT(editorResources)->GetModel("Camera"));
		}

		for (auto& child : p_actor.GetChildren())
		{
			DrawActorOutline(*child);
		}
	}
}

void OvEditor::Rendering::OutlineRenderFeature::DrawModelToStencil(const OvMaths::FMatrix4& p_worldMatrix, OvRendering::Resources::Model& p_model)
{
	m_renderer.DrawModelWithSingleMaterial(p_model, m_stencilFillMaterial, p_worldMatrix);
}

void OvEditor::Rendering::OutlineRenderFeature::DrawModelOutline(const OvMaths::FMatrix4& p_worldMatrix, OvRendering::Resources::Model& p_model)
{
	m_renderer.DrawModelWithSingleMaterial(p_model, m_outlineMaterial, p_worldMatrix);
}
