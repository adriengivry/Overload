/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvEditor/Rendering/OutlineRenderFeature.h"
#include "OvEditor/Core/EditorActions.h"
#include "OvEditor/Settings/EditorSettings.h"

#include <OvCore/ECS/Components/CMaterialRenderer.h>

constexpr uint32_t kStencilMask = 0xFF;
constexpr int32_t kStencilReference = 1;

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
	auto pso = m_renderer.CreatePipelineState();

	pso.stencilTest = true;
	pso.stencilMask = kStencilMask;
	pso.stencilAlgorithmReference = kStencilReference;
	pso.stencilAlgorithmMask = kStencilMask;
	pso.stencilFailOp = OvRendering::Settings::EOperation::REPLACE;
	pso.depthFailOp = OvRendering::Settings::EOperation::REPLACE;
	pso.bothPassOp = OvRendering::Settings::EOperation::REPLACE;
	pso.colorWriting = false;

	DrawActorToStencil(pso, p_actor);
}

void OvEditor::Rendering::OutlineRenderFeature::DrawOutlinePass(OvCore::ECS::Actor& p_actor, const OvMaths::FVector4& p_color, float p_thickness)
{
	auto pso = m_renderer.CreatePipelineState();

	pso.stencilTest = true;
	pso.stencilFailOp = OvRendering::Settings::EOperation::KEEP;
	pso.depthFailOp = OvRendering::Settings::EOperation::KEEP;
	pso.bothPassOp = OvRendering::Settings::EOperation::REPLACE;
	pso.stencilAlgorithm = OvRendering::Settings::EComparaisonAlgorithm::NOTEQUAL;
	pso.stencilAlgorithmReference = kStencilReference;
	pso.stencilAlgorithmMask = kStencilMask;
	pso.rasterizationMode = OvRendering::Settings::ERasterizationMode::LINE;
	pso.rasterizationLinesWidth = p_thickness;

	// Prepare the outline material
	m_outlineMaterial.Set("u_Diffuse", p_color);

	DrawActorOutline(pso, p_actor);
}

void OvEditor::Rendering::OutlineRenderFeature::DrawActorToStencil(OvRendering::Data::PipelineState p_pso, OvCore::ECS::Actor& p_actor)
{
	if (p_actor.IsActive())
	{
		/* Render static mesh outline and bounding spheres */
		if (auto modelRenderer = p_actor.GetComponent<OvCore::ECS::Components::CModelRenderer>(); modelRenderer && modelRenderer->GetModel())
		{
			DrawModelToStencil(p_pso, p_actor.transform.GetWorldMatrix(), *modelRenderer->GetModel());
		}

		/* Render camera component outline */
		if (auto cameraComponent = p_actor.GetComponent<OvCore::ECS::Components::CCamera>(); cameraComponent)
		{
			auto translation = OvMaths::FMatrix4::Translation(p_actor.transform.GetWorldPosition());
			auto rotation = OvMaths::FQuaternion::ToMatrix4(p_actor.transform.GetWorldRotation());
			auto model = translation * rotation;
			DrawModelToStencil(p_pso, model, *EDITOR_CONTEXT(editorResources)->GetModel("Camera"));
		}
	}
}

void OvEditor::Rendering::OutlineRenderFeature::DrawActorOutline(OvRendering::Data::PipelineState p_pso, OvCore::ECS::Actor& p_actor)
{
	if (p_actor.IsActive())
	{
		if (auto modelRenderer = p_actor.GetComponent<OvCore::ECS::Components::CModelRenderer>(); modelRenderer && modelRenderer->GetModel())
		{
			DrawModelOutline(p_pso, p_actor.transform.GetWorldMatrix(), *modelRenderer->GetModel());
		}

		if (auto cameraComponent = p_actor.GetComponent<OvCore::ECS::Components::CCamera>(); cameraComponent)
		{
			auto translation = OvMaths::FMatrix4::Translation(p_actor.transform.GetWorldPosition());
			auto rotation = OvMaths::FQuaternion::ToMatrix4(p_actor.transform.GetWorldRotation());
			auto model = translation * rotation;
			DrawModelOutline(p_pso, model, *EDITOR_CONTEXT(editorResources)->GetModel("Camera"));
		}

		for (auto& child : p_actor.GetChildren())
		{
			DrawActorOutline(p_pso, *child);
		}
	}
}

void OvEditor::Rendering::OutlineRenderFeature::DrawModelToStencil(
	OvRendering::Data::PipelineState p_pso,
	const OvMaths::FMatrix4& p_worldMatrix,
	OvRendering::Resources::Model& p_model
)
{
	m_renderer.DrawModelWithSingleMaterial(p_pso, p_model, m_stencilFillMaterial, p_worldMatrix);
}

void OvEditor::Rendering::OutlineRenderFeature::DrawModelOutline(
	OvRendering::Data::PipelineState p_pso,
	const OvMaths::FMatrix4& p_worldMatrix,
	OvRendering::Resources::Model& p_model
)
{
	m_renderer.DrawModelWithSingleMaterial(p_pso, p_model, m_outlineMaterial, p_worldMatrix);
}
