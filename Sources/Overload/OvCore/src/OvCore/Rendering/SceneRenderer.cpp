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

using OpaqueDrawables = std::multimap<float, OvRendering::Entities::Drawable, std::less<float>>;
using TransparentDrawables = std::multimap<float, OvRendering::Entities::Drawable, std::greater<float>>;
using AllDrawables = std::pair<OpaqueDrawables, TransparentDrawables>;

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

	OvRendering::Core::CompositeRenderer::BeginFrame(p_frameDescriptor);
}

AllDrawables FindAndSortDrawables(
	const OvCore::SceneSystem::Scene& p_scene,
	const OvMaths::FVector3& p_cameraPosition,
	std::optional<OvRendering::Data::Frustum> p_frustum,
	std::optional<std::reference_wrapper<OvCore::Resources::Material>> p_materialOverride
)
{
	using namespace OvCore::ECS::Components;

	OpaqueDrawables opaqueDrawables;
	TransparentDrawables transparentDrawables;

	for (CModelRenderer* modelRenderer : p_scene.GetFastAccessComponents().modelRenderers)
	{
		auto& owner = modelRenderer->owner;

		if (owner.IsActive())
		{
			if (auto model = modelRenderer->GetModel())
			{
				if (auto materialRenderer = modelRenderer->owner.GetComponent<CMaterialRenderer>())
				{
					auto& transform = owner.transform.GetFTransform();

					OvRendering::Settings::ECullingOptions cullingOptions = OvRendering::Settings::ECullingOptions::NONE;

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

					if (p_frustum)
					{
						PROFILER_SPY("Frustum Culling");
						meshes = p_frustum.value().GetMeshesInFrustum(*model, modelBoundingSphere, transform, cullingOptions);
					}
					else
					{
						meshes = model->GetMeshes();
					}

					if (!meshes.empty())
					{
						float distanceToActor = OvMaths::FVector3::Distance(transform.GetWorldPosition(), p_cameraPosition);
						const OvCore::ECS::Components::CMaterialRenderer::MaterialList& materials = materialRenderer->GetMaterials();

						for (const auto& mesh : meshes)
						{
							OvCore::Resources::Material* material = nullptr;

							if (mesh->GetMaterialIndex() < MAX_MATERIAL_COUNT)
							{
								material = materials.at(mesh->GetMaterialIndex());
								if (!material || !material->GetShader())
									material = p_materialOverride ? &p_materialOverride.value().get() : nullptr;
							}

							if (material)
							{
								OvRendering::Entities::Drawable element{
									transform.GetWorldMatrix(),
									*mesh,
									*material,
									material->GenerateStateMask(),
									materialRenderer->GetUserMatrix()
								};

								if (material->IsBlendable())
								{
									transparentDrawables.emplace(distanceToActor, element);
								}
								else
								{
									opaqueDrawables.emplace(distanceToActor, element);
								}
							}
						}
					}
				}
			}
		}
	}

	return { opaqueDrawables, transparentDrawables };
}

void OvCore::Rendering::SceneRenderer::Draw()
{
	auto& sceneDescriptor = GetDescriptor<SceneDescriptor>();
	auto& scene = sceneDescriptor.scene;
	auto& camera = sceneDescriptor.camera;
	auto& frustumOverride = sceneDescriptor.frustumOverride;
	auto& materialOverride = sceneDescriptor.materialOverride;

	OpaqueDrawables	opaqueMeshes;
	TransparentDrawables transparentMeshes;

	if (camera.HasFrustumGeometryCulling())
	{
		const auto& frustum = frustumOverride ? frustumOverride.value() : camera.GetFrustum();
		std::tie(opaqueMeshes, transparentMeshes) = FindAndSortDrawables(scene, camera.GetPosition(), frustum, materialOverride);
	}
	else
	{
		// TODO: Consider auto[x, y] instead of std::tie (not sure if it's exactly equivalent)
		std::tie(opaqueMeshes, transparentMeshes) = FindAndSortDrawables(scene, camera.GetPosition(), std::nullopt, materialOverride);
	}

	for (const auto& [distance, drawable] : opaqueMeshes)
	{
		DrawEntity(drawable);
	}

	for (const auto& [distance, drawable] : transparentMeshes)
	{
		DrawEntity(drawable);
	}

}
