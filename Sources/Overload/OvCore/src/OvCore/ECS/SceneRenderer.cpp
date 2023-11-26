/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <OvAnalytics/Profiling/ProfilerSpy.h>

#include <OvRendering/Resources/Loaders/TextureLoader.h>
#include <OvRendering/Data/Frustum.h>
#include <OvRendering/Features/LightingRenderFeature.h>

#include "OvCore/ECS/SceneRenderer.h"
#include "OvCore/ECS/Components/CModelRenderer.h"
#include "OvCore/ECS/Components/CMaterialRenderer.h"

#include <chrono>

class EngineBufferRenderFeature : public OvRendering::Features::ARenderFeature
{
public:
	EngineBufferRenderFeature(OvRendering::Core::CompositeRenderer& p_renderer) : ARenderFeature(p_renderer)
	{
		m_engineBuffer = std::make_unique<OvRendering::Buffers::UniformBuffer>(
			/* UBO Data Layout */
			sizeof(OvMaths::FMatrix4) +
			sizeof(OvMaths::FMatrix4) +
			sizeof(OvMaths::FMatrix4) +
			sizeof(OvMaths::FVector3) +
			sizeof(float) +
			sizeof(OvMaths::FMatrix4),
			0, 0,
			OvRendering::Buffers::EAccessSpecifier::STREAM_DRAW
		);

		m_startTime = std::chrono::high_resolution_clock::now();
	}

	void UploadCameraData(const OvRendering::Entities::Camera& p_camera, const OvMaths::FVector3& p_cameraPosition)
	{
		// We skip the model matrix (Which is a special case, modified every draw calls)
		size_t offset = sizeof(OvMaths::FMatrix4);
		m_engineBuffer->SetSubData(OvMaths::FMatrix4::Transpose(p_camera.GetViewMatrix()), std::ref(offset));
		m_engineBuffer->SetSubData(OvMaths::FMatrix4::Transpose(p_camera.GetProjectionMatrix()), std::ref(offset));
		m_engineBuffer->SetSubData(p_cameraPosition, std::ref(offset));
	}

	void UploadEntityData(const OvRendering::Entities::Drawable& p_drawable)
	{
		m_engineBuffer->SetSubData(OvMaths::FMatrix4::Transpose(p_drawable.modelMatrix), 0);
		m_engineBuffer->SetSubData
		(
			p_drawable.userMatrix,

			// UBO layout offset
			sizeof(OvMaths::FMatrix4) +
			sizeof(OvMaths::FMatrix4) +
			sizeof(OvMaths::FMatrix4) +
			sizeof(OvMaths::FVector3) +
			sizeof(float)
		);
	}

	virtual void OnBeforeDraw(const OvRendering::Entities::Drawable& p_drawable) override
	{
		UploadEntityData(p_drawable);
	}

	virtual void OnBeginFrame(std::optional<OvRendering::Data::RenderOutputDesc>& p_outputDesc) override
	{
		m_engineBuffer->Bind();
		auto currentTime = std::chrono::high_resolution_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - m_startTime);
		m_engineBuffer->SetSubData(elapsedTime.count(), 3 * sizeof(OvMaths::FMatrix4) + sizeof(OvMaths::FVector3));
	}

	virtual void OnEndFrame() override
	{
		m_engineBuffer->Unbind();
	}

	std::chrono::high_resolution_clock::time_point m_startTime;
	std::unique_ptr<OvRendering::Buffers::UniformBuffer> m_engineBuffer;
};

OvCore::ECS::SceneRenderer::SceneRenderer(OvRendering::Context::Driver& p_driver)
	: OvRendering::Core::CompositeRenderer(p_driver)
{
	AddFeature<EngineBufferRenderFeature>();
	AddFeature<OvRendering::Features::LightingRenderFeature>();
}

OvCore::ECS::Components::CCamera* OvCore::ECS::SceneRenderer::FindMainCamera(const OvCore::SceneSystem::Scene& p_scene)
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

void OvCore::ECS::SceneRenderer::RenderScene(OvCore::SceneSystem::Scene& p_scene, uint16_t p_viewportWidth, uint16_t p_viewportHeight, OvCore::Resources::Material* p_defaultMaterial)
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

		const auto& cameraPosition = mainCameraComponent->owner.transform.GetWorldPosition();
		const auto& cameraRotation = mainCameraComponent->owner.transform.GetWorldRotation();
		auto& camera = mainCameraComponent->GetCamera();

		camera.CacheMatrices(p_viewportWidth, p_viewportHeight, cameraPosition, cameraRotation);

		GetFeature<EngineBufferRenderFeature>().UploadCameraData(camera, cameraPosition);

		OvMaths::FVector3 clearColor = camera.GetClearColor();
		m_driver.SetClearColor(clearColor.x, clearColor.y, clearColor.z);
		m_driver.Clear(true, true, false);

		m_driver.UpdateStateMask();
		OvRendering::Data::StateMask state = m_driver.GetStateMask();
		RenderScene(p_scene, cameraPosition, camera, nullptr, p_defaultMaterial);
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

void OvCore::ECS::SceneRenderer::UpdateLights(OvCore::SceneSystem::Scene& p_scene)
{
	PROFILER_SPY("Light SSBO Update");
	auto activeLights = FindActiveLights(p_scene);
	GetFeature<OvRendering::Features::LightingRenderFeature>().UploadLightingData(activeLights, std::nullopt);
}

void OvCore::ECS::SceneRenderer::UpdateLightsInFrustum(OvCore::SceneSystem::Scene& p_scene, const OvRendering::Data::Frustum& p_frustum)
{
	PROFILER_SPY("Light SSBO Update (Frustum culled)");
	auto activeLights = FindActiveLights(p_scene);
	GetFeature<OvRendering::Features::LightingRenderFeature>().UploadLightingData(activeLights, p_frustum);
}

void OvCore::ECS::SceneRenderer::RenderScene(
	OvCore::SceneSystem::Scene& p_scene,
	const OvMaths::FVector3& p_cameraPosition,
	const OvRendering::Entities::Camera& p_camera,
	const OvRendering::Data::Frustum* p_customFrustum,
	OvCore::Resources::Material* p_defaultMaterial
)
{
	OpaqueDrawables	opaqueMeshes;
	TransparentDrawables transparentMeshes;

	if (p_camera.HasFrustumGeometryCulling())
	{
		const auto& frustum = p_customFrustum ? *p_customFrustum : p_camera.GetFrustum();
		std::tie(opaqueMeshes, transparentMeshes) = FindAndSortFrustumCulledDrawables(p_scene, p_cameraPosition, frustum, p_defaultMaterial);
	}
	else
	{
		std::tie(opaqueMeshes, transparentMeshes) = FindAndSortDrawables(p_scene, p_cameraPosition, p_defaultMaterial);
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

void FindAndSortDrawables(
	OvCore::ECS::SceneRenderer::OpaqueDrawables& p_opaques,
	OvCore::ECS::SceneRenderer::TransparentDrawables& p_transparents,
	const OvCore::SceneSystem::Scene& p_scene,
	const OvMaths::FVector3& p_cameraPosition,
	OvCore::Resources::Material* p_defaultMaterial
)
{
	for (OvCore::ECS::Components::CModelRenderer* modelRenderer : p_scene.GetFastAccessComponents().modelRenderers)
	{
		if (modelRenderer->owner.IsActive())
		{
			if (auto model = modelRenderer->GetModel())
			{
				float distanceToActor = OvMaths::FVector3::Distance(modelRenderer->owner.transform.GetWorldPosition(), p_cameraPosition);

				if (auto materialRenderer = modelRenderer->owner.GetComponent<OvCore::ECS::Components::CMaterialRenderer>())
				{
					const auto& transform = modelRenderer->owner.transform.GetFTransform();

					const OvCore::ECS::Components::CMaterialRenderer::MaterialList& materials = materialRenderer->GetMaterials();

					for (auto mesh : model->GetMeshes())
					{
						OvCore::Resources::Material* material = nullptr;

						if (mesh->GetMaterialIndex() < MAX_MATERIAL_COUNT)
						{
							material = materials.at(mesh->GetMaterialIndex());
							if (!material || !material->GetShader())
								material = p_defaultMaterial;
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
								p_transparents.emplace(distanceToActor, element);
							}
							else
							{
								p_opaques.emplace(distanceToActor, element);
							}
						}
					}
				}
			}
		}
	}
}

std::pair<OvCore::ECS::SceneRenderer::OpaqueDrawables, OvCore::ECS::SceneRenderer::TransparentDrawables> OvCore::ECS::SceneRenderer::FindAndSortFrustumCulledDrawables(
	const OvCore::SceneSystem::Scene& p_scene,
	const OvMaths::FVector3& p_cameraPosition,
	const OvRendering::Data::Frustum& p_frustum,
	OvCore::Resources::Material* p_defaultMaterial
)
{
	using namespace OvCore::ECS::Components;

	OvCore::ECS::SceneRenderer::OpaqueDrawables opaqueDrawables;
	OvCore::ECS::SceneRenderer::TransparentDrawables transparentDrawables;

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

					std::vector<std::reference_wrapper<OvRendering::Resources::Mesh>> meshes;

					{
						PROFILER_SPY("Frustum Culling");
						meshes = p_frustum.GetMeshesInFrustum(*model, modelBoundingSphere, transform, cullingOptions);
					}

					if (!meshes.empty())
					{
						float distanceToActor = OvMaths::FVector3::Distance(transform.GetWorldPosition(), p_cameraPosition);
						const OvCore::ECS::Components::CMaterialRenderer::MaterialList& materials = materialRenderer->GetMaterials();

						for (const auto& mesh : meshes)
						{
							OvCore::Resources::Material* material = nullptr;

							if (mesh.get().GetMaterialIndex() < MAX_MATERIAL_COUNT)
							{
								material = materials.at(mesh.get().GetMaterialIndex());
								if (!material || !material->GetShader())
									material = p_defaultMaterial;
							}

							if (material)
							{
								OvRendering::Entities::Drawable element{
									transform.GetWorldMatrix(),
									mesh.get(),
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

std::pair<OvCore::ECS::SceneRenderer::OpaqueDrawables, OvCore::ECS::SceneRenderer::TransparentDrawables> OvCore::ECS::SceneRenderer::FindAndSortDrawables(
	const OvCore::SceneSystem::Scene& p_scene,
	const OvMaths::FVector3& p_cameraPosition,
	OvCore::Resources::Material* p_defaultMaterial
)
{
	OvCore::ECS::SceneRenderer::OpaqueDrawables opaqueDrawables;
	OvCore::ECS::SceneRenderer::TransparentDrawables transparentDrawables;

	for (OvCore::ECS::Components::CModelRenderer* modelRenderer : p_scene.GetFastAccessComponents().modelRenderers)
	{
		if (modelRenderer->owner.IsActive())
		{
			if (auto model = modelRenderer->GetModel())
			{
				float distanceToActor = OvMaths::FVector3::Distance(modelRenderer->owner.transform.GetWorldPosition(), p_cameraPosition);

				if (auto materialRenderer = modelRenderer->owner.GetComponent<OvCore::ECS::Components::CMaterialRenderer>())
				{
					const auto& transform = modelRenderer->owner.transform.GetFTransform();

					const OvCore::ECS::Components::CMaterialRenderer::MaterialList& materials = materialRenderer->GetMaterials();

					for (auto mesh : model->GetMeshes())
					{
						OvCore::Resources::Material* material = nullptr;

						if (mesh->GetMaterialIndex() < MAX_MATERIAL_COUNT)
						{
							material = materials.at(mesh->GetMaterialIndex());
							if (!material || !material->GetShader())
								material = p_defaultMaterial;
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

	return { opaqueDrawables, transparentDrawables };
}

void OvCore::ECS::SceneRenderer::DrawModelWithSingleMaterial(
	OvRendering::Resources::Model& p_model,
	OvCore::Resources::Material& p_material,
	OvMaths::FMatrix4 const* p_modelMatrix,
	OvCore::Resources::Material* p_defaultMaterial)
{
	OvCore::Resources::Material* material = p_material.GetShader() ? &p_material : p_defaultMaterial;
	
	if (material)
	{
		OvRendering::Data::StateMask stateMask = material->GenerateStateMask();
		OvMaths::FMatrix4 userMatrix = OvMaths::FMatrix4::Identity;

		for (auto mesh : p_model.GetMeshes())
		{
			OvRendering::Entities::Drawable element{
				*p_modelMatrix,
				*mesh,
				*material,
				stateMask,
				userMatrix
			};

			DrawEntity(element);
		}
	}
}
