/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvEditor/Rendering/DebugModelRenderFeature.h"
#include "OvEditor/Rendering/ShadowRenderPass.h"
#include "OvEditor/Core/EditorActions.h"
#include "OvEditor/Settings/EditorSettings.h"
#include "OvEditor/Rendering/DebugSceneRenderer.h"

#include <OvCore/ECS/Components/CMaterialRenderer.h>
#include <OvCore/Rendering/EngineDrawableDescriptor.h>
#include <OvCore/Rendering/EngineBufferRenderFeature.h>
#include <OvRendering/Features/LightingRenderFeature.h>

constexpr uint16_t kShadowMapSize = 4096;

OvEditor::Rendering::ShadowRenderPass::ShadowRenderPass(OvRendering::Core::CompositeRenderer& p_renderer) :
	OvRendering::Core::ARenderPass(p_renderer)
{
	m_opaqueMaterial.SetShader(EDITOR_CONTEXT(shaderManager)[":Shaders\\Shadow.ovfx"]);
	m_opaqueMaterial.SetFrontfaceCulling(true);
	m_opaqueMaterial.SetBackfaceCulling(false);
}

void OvEditor::Rendering::ShadowRenderPass::Draw(OvRendering::Data::PipelineState p_pso)
{
	using namespace OvCore::Rendering;

	OVASSERT(m_renderer.HasDescriptor<SceneRenderer::SceneDescriptor>(), "Cannot find SceneDescriptor attached to this renderer");
	OVASSERT(m_renderer.HasFeature<OvCore::Rendering::EngineBufferRenderFeature>(), "Cannot find EngineBufferRenderFeature attached to this renderer");
	OVASSERT(m_renderer.HasDescriptor<OvRendering::Features::LightingRenderFeature::LightingDescriptor>(), "Cannot find LightingDescriptor attached to this renderer");

	auto& engineBufferRenderFeature = m_renderer.GetFeature<OvCore::Rendering::EngineBufferRenderFeature>();
	auto& lightingDescriptor = m_renderer.GetDescriptor<OvRendering::Features::LightingRenderFeature::LightingDescriptor>();

	auto& sceneDescriptor = m_renderer.GetDescriptor<SceneRenderer::SceneDescriptor>();
	auto& frameDescriptor = m_renderer.GetFrameDescriptor();
	auto& scene = sceneDescriptor.scene;

	EDITOR_CONTEXT(driver)->SetViewport(0, 0, kShadowMapSize, kShadowMapSize);

	auto pso = m_renderer.CreatePipelineState();

	for (auto lightReference : lightingDescriptor.lights)
	{
		auto& light = lightReference.get();

		if (light.type == OvRendering::Settings::ELightType::DIRECTIONAL)
		{
			light.UpdateShadowData(kShadowMapSize);
			const auto& lightSpaceMatrix = light.GetLightSpaceMatrix();
			const auto& shadowBuffer = light.GetShadowBuffer();
			m_opaqueMaterial.Set("u_LightSpaceMatrix", lightSpaceMatrix);
			shadowBuffer.Bind();
			m_renderer.Clear(true, true, true);
			DrawOpaques(pso, scene);
			shadowBuffer.Unbind();
		}
	}

	if (auto output = frameDescriptor.outputBuffer)
	{
		output.value().Bind();
	}

	EDITOR_CONTEXT(driver)->SetViewport(0, 0, frameDescriptor.renderWidth, frameDescriptor.renderHeight);
}

void OvEditor::Rendering::ShadowRenderPass::DrawOpaques(
	OvRendering::Data::PipelineState p_pso,
	OvCore::SceneSystem::Scene& p_scene
)
{
	for (auto modelRenderer : p_scene.GetFastAccessComponents().modelRenderers)
	{
		auto& actor = modelRenderer->owner;

		if (actor.IsActive())
		{
			if (auto model = modelRenderer->GetModel())
			{
				if (auto materialRenderer = modelRenderer->owner.GetComponent<OvCore::ECS::Components::CMaterialRenderer>())
				{
					const auto& materials = materialRenderer->GetMaterials();
					const auto& modelMatrix = actor.transform.GetWorldMatrix();

					for (auto mesh : model->GetMeshes())
					{
						auto stateMask = m_opaqueMaterial.GenerateStateMask();

						// TODO: Don't override the state
						// Override the state mask to use the material state mask (if this one is valid)
						if (auto material = materials.at(mesh->GetMaterialIndex()); material && material->IsValid() && material->IsShadowCaster())
						{
							stateMask = material->GenerateStateMask();
							OvRendering::Entities::Drawable drawable;
							drawable.mesh = *mesh;
							drawable.material = m_opaqueMaterial;
							drawable.stateMask = stateMask;

							drawable.AddDescriptor<OvCore::Rendering::EngineDrawableDescriptor>({
								modelMatrix
							});

							m_renderer.DrawEntity(p_pso, drawable);
						}

					}
				}
			}
		}
	}
}
