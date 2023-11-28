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

#include <chrono>

OvCore::Rendering::SceneRenderer::SceneRenderer(OvRendering::Context::Driver& p_driver)
	: OvRendering::Core::CompositeRenderer(p_driver)
{
	AddFeature<EngineBufferRenderFeature>();
	AddFeature<OvRendering::Features::LightingRenderFeature>();
}

OvCore::ECS::Components::CCamera* OvCore::Rendering::SceneRenderer::FindMainCamera(const OvCore::SceneSystem::Scene& p_scene)
{
	for (OvCore::ECS::Components::CCamera* camera : p_scene.GetFastAccessComponents().cameras)
	{
		if (camera->owner.IsActive())
		{
			return camera;
		}
	}

	return nullptr;
}

void OvCore::Rendering::SceneRenderer::RenderScene(
	OvCore::SceneSystem::Scene& p_scene,
	uint16_t p_viewportWidth,
	uint16_t p_viewportHeight,
	std::optional<std::reference_wrapper<OvCore::Resources::Material>> p_materialOverride
)
{
	if (OvCore::ECS::Components::CCamera* mainCameraComponent = FindMainCamera(p_scene))
	{
		if (mainCameraComponent->HasFrustumLightCulling())
		{
			UpdateLightsInFrustum(p_scene, mainCameraComponent->GetCamera().GetFrustum());
		}
		else
		{
			UpdateLights(p_scene);
		}

		auto& camera = mainCameraComponent->GetCamera();

		GetFeature<EngineBufferRenderFeature>().UploadViewData(
			camera,
			p_viewportWidth,
			p_viewportHeight
		);

		OvMaths::FVector3 clearColor = camera.GetClearColor();
		m_driver.SetClearColor(clearColor.x, clearColor.y, clearColor.z);
		m_driver.Clear(true, true, false);

		m_driver.UpdateStateMask();
		OvRendering::Data::StateMask state = m_driver.GetStateMask();
		RenderScene(p_scene, camera, std::nullopt, p_materialOverride);
		m_driver.ApplyStateMask(state);
	}
	else
	{
		m_driver.SetClearColor(0.0f, 0.0f, 0.0f);
		m_driver.Clear(true, true, false);
	}
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

void OvCore::Rendering::SceneRenderer::UpdateLights(OvCore::SceneSystem::Scene& p_scene)
{
	PROFILER_SPY("Light SSBO Update");
	auto activeLights = FindActiveLights(p_scene);
	GetFeature<OvRendering::Features::LightingRenderFeature>().UploadLightingData(activeLights, std::nullopt);
}

void OvCore::Rendering::SceneRenderer::UpdateLightsInFrustum(OvCore::SceneSystem::Scene& p_scene, const OvRendering::Data::Frustum& p_frustum)
{
	PROFILER_SPY("Light SSBO Update (Frustum culled)");
	auto activeLights = FindActiveLights(p_scene);
	GetFeature<OvRendering::Features::LightingRenderFeature>().UploadLightingData(activeLights, p_frustum);
}

void OvCore::Rendering::SceneRenderer::RenderScene(
	OvCore::SceneSystem::Scene& p_scene,
	const OvRendering::Entities::Camera& p_camera,
	std::optional<OvRendering::Data::Frustum> p_frustumOverride,
	std::optional<std::reference_wrapper<OvCore::Resources::Material>> p_materialOverride
)
{
	OpaqueDrawables	opaqueMeshes;
	TransparentDrawables transparentMeshes;

	if (p_camera.HasFrustumGeometryCulling())
	{
		const auto& frustum = p_frustumOverride ? p_frustumOverride.value() : p_camera.GetFrustum();
		std::tie(opaqueMeshes, transparentMeshes) = FindAndSortDrawables(p_scene, p_camera.GetPosition(), frustum, p_materialOverride);
	}
	else
	{
		std::tie(opaqueMeshes, transparentMeshes) = FindAndSortDrawables(p_scene, p_camera.GetPosition(), std::nullopt, p_materialOverride);
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

std::pair<OvCore::Rendering::SceneRenderer::OpaqueDrawables, OvCore::Rendering::SceneRenderer::TransparentDrawables> OvCore::Rendering::SceneRenderer::FindAndSortDrawables(
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

void OvCore::Rendering::SceneRenderer::DrawModelWithSingleMaterial(
	OvRendering::Resources::Model& p_model,
	OvCore::Resources::Material& p_material,
	OvMaths::FMatrix4 const* p_modelMatrix,
	std::optional<std::reference_wrapper<OvCore::Resources::Material>> p_materialOverride
)
{
	std::optional<std::reference_wrapper<OvCore::Resources::Material>> targetMaterial =
		p_material.GetShader() ?
		std::ref(p_material) :
		p_materialOverride;
	
	if (p_materialOverride)
	{
		OvCore::Resources::Material& material = targetMaterial.value().get();

		OvRendering::Data::StateMask stateMask = material.GenerateStateMask();
		OvMaths::FMatrix4 userMatrix = OvMaths::FMatrix4::Identity;

		for (auto mesh : p_model.GetMeshes())
		{
			OvRendering::Entities::Drawable element{
				*p_modelMatrix,
				*mesh,
				material,
				stateMask,
				userMatrix
			};

			DrawEntity(element);
		}
	}
}
