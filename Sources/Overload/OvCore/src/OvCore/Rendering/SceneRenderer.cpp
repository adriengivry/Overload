/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <OvAnalytics/Profiling/ProfilerSpy.h>

#include <OvRendering/Data/Frustum.h>
#include <OvRendering/Features/LightingRenderFeature.h>

#include "OvCore/Rendering/SceneRenderer.h"
#include "OvCore/Rendering/EngineBufferRenderFeature.h"
#include "OvCore/ECS/Components/CModelRenderer.h"
#include "OvCore/ECS/Components/CMaterialRenderer.h"

OvCore::Rendering::SceneRenderer::SceneRenderer(OvRendering::Context::Driver& p_driver)
	: OvRendering::Core::CompositeRenderer(p_driver)
{
	AddFeature<EngineBufferRenderFeature>();
	AddFeature<OvRendering::Features::LightingRenderFeature>();
}

OvRendering::Features::LightingRenderFeature::LightSet FindActiveLights(const OvCore::SceneSystem::Scene& p_scene)
{
	OvRendering::Features::LightingRenderFeature::LightSet lights;

	const auto& facs = p_scene.GetFastAccessComponents();

	for (auto light : facs.lights)
	{
		if (light->owner.IsActive())
		{
			lights.push_back(std::ref(light->GetData()));
		}
	}

	return lights;
}

void OvCore::Rendering::SceneRenderer::BeginFrame(const OvRendering::Data::FrameDescriptor& p_frameDescriptor)
{
	OVASSERT(HasDescriptor<SceneDescriptor>(), "Cannot find SceneDescriptor attached to this renderer");

	auto& sceneDescriptor = GetDescriptor<SceneDescriptor>();

	sceneDescriptor.camera.CacheMatrices(p_frameDescriptor.renderWidth, p_frameDescriptor.renderHeight);

	AddDescriptor<OvCore::Rendering::EngineBufferRenderFeature::EngineBufferDescriptor>({
		sceneDescriptor.camera
	});

	AddDescriptor<OvRendering::Features::LightingRenderFeature::LightingDescriptor>({
		FindActiveLights(sceneDescriptor.scene),
		sceneDescriptor.camera.HasFrustumLightCulling() ? std::optional(sceneDescriptor.camera.GetFrustum()) : std::nullopt
	});

	ParseScene();

	OvRendering::Core::CompositeRenderer::BeginFrame(p_frameDescriptor);
}

void OvCore::Rendering::SceneRenderer::DrawPass(OvRendering::Settings::ERenderPass p_pass)
{
	if (p_pass == OvRendering::Settings::ERenderPass::OPAQUE)
	{
		for (const auto& [distance, drawable] : m_opaqueDrawables)
		{
			DrawEntity(drawable);
		}
	}
	else if (p_pass == OvRendering::Settings::ERenderPass::TRANSPARENT)
	{
		for (const auto& [distance, drawable] : m_transparentDrawables)
		{
			DrawEntity(drawable);
		}
	}
}

void OvCore::Rendering::SceneRenderer::ParseScene()
{
	using namespace OvCore::ECS::Components;

	m_opaqueDrawables.clear();
	m_transparentDrawables.clear();

	auto& sceneDescriptor = GetDescriptor<SceneDescriptor>();
	auto& scene = sceneDescriptor.scene;
	auto& camera = sceneDescriptor.camera;
	auto& materialOverride = sceneDescriptor.materialOverride;
	std::optional<OvRendering::Data::Frustum> frustum = std::nullopt;

	if (camera.HasFrustumGeometryCulling())
	{
		auto& frustumOverride = sceneDescriptor.frustumOverride;
		frustum = frustumOverride ? frustumOverride : camera.GetFrustum();
	}

	for (CModelRenderer* modelRenderer : scene.GetFastAccessComponents().modelRenderers)
	{
		auto& owner = modelRenderer->owner;

		if (owner.IsActive())
		{
			if (auto model = modelRenderer->GetModel())
			{
				if (auto materialRenderer = modelRenderer->owner.GetComponent<CMaterialRenderer>())
				{
					auto& transform = owner.transform.GetFTransform();

					auto cullingOptions = OvRendering::Settings::ECullingOptions::NONE;

					if (modelRenderer->GetFrustumBehaviour() != CModelRenderer::EFrustumBehaviour::DISABLED)
					{
						cullingOptions |= OvRendering::Settings::ECullingOptions::FRUSTUM_PER_MODEL;
					}

					if (modelRenderer->GetFrustumBehaviour() == CModelRenderer::EFrustumBehaviour::CULL_MESHES)
					{
						cullingOptions |= OvRendering::Settings::ECullingOptions::FRUSTUM_PER_MESH;
					}

					const auto& modelBoundingSphere = modelRenderer->GetFrustumBehaviour() == CModelRenderer::EFrustumBehaviour::CULL_CUSTOM ? modelRenderer->GetCustomBoundingSphere() : model->GetBoundingSphere();

					std::vector<OvRendering::Resources::Mesh*> meshes;

					if (frustum)
					{
						PROFILER_SPY("Frustum Culling");
						meshes = frustum.value().GetMeshesInFrustum(*model, modelBoundingSphere, transform, cullingOptions);
					}
					else
					{
						meshes = model->GetMeshes();
					}

					if (!meshes.empty())
					{
						float distanceToActor = OvMaths::FVector3::Distance(transform.GetWorldPosition(), camera.GetPosition());
						const OvCore::ECS::Components::CMaterialRenderer::MaterialList& materials = materialRenderer->GetMaterials();

						for (const auto& mesh : meshes)
						{
							OvCore::Resources::Material* material = nullptr;

							if (mesh->GetMaterialIndex() < MAX_MATERIAL_COUNT)
							{
								material = materials.at(mesh->GetMaterialIndex());
								if (!material || !material->GetShader())
									material = materialOverride ? &materialOverride.value().get() : nullptr;
							}

							if (material)
							{
								SceneDrawable drawable;
								drawable.modelMatrix = transform.GetWorldMatrix();
								drawable.mesh = *mesh;
								drawable.material = *material;
								drawable.stateMask = material->GenerateStateMask();
								drawable.userMatrix = materialRenderer->GetUserMatrix();
								drawable.actor = owner;

								if (material->IsBlendable())
								{
									m_transparentDrawables.emplace(distanceToActor, drawable);
								}
								else
								{
									m_opaqueDrawables.emplace(distanceToActor, drawable);
								}
							}
						}
					}
				}
			}
		}
	}
}
