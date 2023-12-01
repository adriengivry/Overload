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
	m_driver.SetStencilMask(0xFF); // Enable writing to the stencil buffer
	DrawActorToStencil(p_actor);
	m_driver.SetStencilMask(0x00); // Disable writing to the stencil buffer
}

void OvEditor::Rendering::OutlineRenderFeature::DrawOutlinePass(OvCore::ECS::Actor& p_actor, const OvMaths::FVector4& p_color, float p_thickness)
{
	// Prepare the driver for stencil test
	m_driver.SetCapability(OvRendering::Settings::ERenderingCapability::STENCIL_TEST, true);
	m_driver.SetStencilOperations(OvRendering::Settings::EOperation::KEEP, OvRendering::Settings::EOperation::KEEP, OvRendering::Settings::EOperation::REPLACE);
	m_driver.SetStencilAlgorithm(OvRendering::Settings::EComparaisonAlgorithm::NOTEQUAL, 1, 0xFF);
	m_driver.SetRasterizationMode(OvRendering::Settings::ERasterizationMode::LINE);
	m_driver.SetRasterizationLinesWidth(p_thickness);

	// Prepare the outline material
	m_outlineMaterial.Set("u_Diffuse", p_color);

	DrawActorOutline(p_actor);

	// Reset driver settings (Could be skipped if the driver state mask was properly implemented)
	m_driver.SetRasterizationLinesWidth(1.f);
	m_driver.SetRasterizationMode(OvRendering::Settings::ERasterizationMode::FILL);
	m_driver.SetStencilAlgorithm(OvRendering::Settings::EComparaisonAlgorithm::ALWAYS, 1, 0xFF);
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
