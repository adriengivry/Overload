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
	OvRendering::Core::ARenderPass(p_renderer),
	m_shadowFramebuffer(kShadowMapSize, kShadowMapSize, true)
{
	m_opaqueMaterial.SetShader(EDITOR_CONTEXT(shaderManager)[":Shaders\\Shadow.ovfx"]);
	m_opaqueMaterial.SetFrontfaceCulling(true);
	m_opaqueMaterial.SetBackfaceCulling(false);
}

OvRendering::Resources::TextureHandle OvEditor::Rendering::ShadowRenderPass::GetDepthMap()
{
	return m_shadowFramebuffer.GetTexture();
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
	m_shadowFramebuffer.Resize(kShadowMapSize, kShadowMapSize);

	m_shadowFramebuffer.Bind();

	auto pso = m_renderer.CreatePipelineState();

	m_renderer.Clear(true, true, true);

	std::optional<OvRendering::Entities::Light> directionalLight;
	for (auto& light : lightingDescriptor.lights)
	{
		if (light.get().type == OvRendering::Settings::ELightType::DIRECTIONAL)
		{
			directionalLight = light;
			break;
		}
	}

	if (directionalLight)
	{
		DrawOpaques(pso, scene);
	}

	m_shadowFramebuffer.Unbind();

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

						// Override the state mask to use the material state mask (if this one is valid)
						if (auto material = materials.at(mesh->GetMaterialIndex()); material && material->IsValid())
						{
							stateMask = material->GenerateStateMask();
						}

						const float near_plane = 1.0f, far_plane = 7.5f;
						OvRendering::Entities::Camera lightCamera;
						const auto pos = OvMaths::FVector3{ 1.0f, 10.0f, 1.0f };
						lightCamera.SetPosition(pos);
						lightCamera.SetRotation(OvMaths::FQuaternion::LookAt(OvMaths::FVector3::Zero - pos, OvMaths::FVector3::Up));
						lightCamera.CacheMatrices(kShadowMapSize, kShadowMapSize);
						const auto lightSpaceMatrix = lightCamera.GetProjectionMatrix() * lightCamera.GetViewMatrix();

						m_opaqueMaterial.Set("u_LightSpaceMatrix", lightSpaceMatrix);

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
