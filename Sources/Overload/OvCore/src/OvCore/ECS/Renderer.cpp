/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#include <OvRendering/Resources/Loaders/TextureLoader.h>
#include <OvRendering/Data/Frustum.h>

#include "OvCore/ECS/Renderer.h"
#include "OvCore/ECS/Components/CModelRenderer.h"
#include "OvCore/ECS/Components/CMaterialRenderer.h"

OvCore::ECS::Renderer::Renderer(OvRendering::Context::Driver& p_driver) :
	OvRendering::Core::Renderer(p_driver),
	m_emptyTexture(OvRendering::Resources::Loaders::TextureLoader::CreateColor
	(
		(255 << 24) | (255 << 16) | (255 << 8) | 255,
		OvRendering::Settings::ETextureFilteringMode::NEAREST,
		OvRendering::Settings::ETextureFilteringMode::NEAREST,
		false
	))
{
}

OvCore::ECS::Renderer::~Renderer()
{
	OvRendering::Resources::Loaders::TextureLoader::Destroy(m_emptyTexture);
}

OvCore::ECS::Components::CCamera* OvCore::ECS::Renderer::FindMainCamera(const OvCore::SceneSystem::Scene& p_scene)
{
	for (OvCore::ECS::Components::CCamera* camera : p_scene.GetFastAccessComponents().cameras)
		if (camera->owner.IsActive())
			return camera;

	return nullptr;
}

void OvCore::ECS::Renderer::FindLightMatrices(const OvCore::SceneSystem::Scene& p_scene, std::vector<OvMaths::FMatrix4>& p_out)
{
	const auto& facs = p_scene.GetFastAccessComponents();

	for (auto light : facs.lights)
		if (light->owner.IsActive())
			p_out.push_back(light->GetData().GenerateMatrix());
}

void OvCore::ECS::Renderer::RenderScene(OvCore::SceneSystem::Scene& p_scene, const OvMaths::FVector3& p_cameraPosition, OvRendering::Data::Frustum const* p_frustum, Resources::Material* p_defaultMaterial)
{
	OpaqueDrawables			opaqueMeshes;
	TransparentDrawables	transparentMeshes;

	FindAndSortDrawables(opaqueMeshes, transparentMeshes, p_scene, p_cameraPosition, p_frustum, p_defaultMaterial);

	for (const auto&[distance, drawable] : opaqueMeshes)
		DrawDrawable(drawable);

	for (const auto& [distance, drawable] : transparentMeshes)
		DrawDrawable(drawable);
}

void OvCore::ECS::Renderer::FindAndSortDrawables
(
	OpaqueDrawables& p_opaques,
	TransparentDrawables& p_transparents,
	const OvCore::SceneSystem::Scene& p_scene,
	const OvMaths::FVector3& p_cameraPosition,
	OvRendering::Data::Frustum const* p_frustum,
	Resources::Material* p_defaultMaterial
)
{
	auto camera = FindMainCamera(p_scene);

	for (OvCore::ECS::Components::CModelRenderer* modelRenderer : p_scene.GetFastAccessComponents().modelRenderers)
	{
		auto position = modelRenderer->owner.transform.GetWorldPosition();
		if (modelRenderer->owner.IsActive())
		{
			if (auto model = modelRenderer->GetModel())
			{
				if (!p_frustum || p_frustum->PointInFrustum(position.x, position.y, position.z))
				{
					float distanceToActor = OvMaths::FVector3::Distance(modelRenderer->owner.transform.GetWorldPosition(), p_cameraPosition);

					if (auto materialRenderer = modelRenderer->owner.GetComponent<OvCore::ECS::Components::CMaterialRenderer>())
					{
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
								Drawable element = { modelRenderer->owner.transform.GetWorldMatrix(), mesh, material, materialRenderer->GetUserMatrix() };

								if (material->IsBlendable())
									p_transparents.emplace(distanceToActor, element);
								else
									p_opaques.emplace(distanceToActor, element);
							}
						}
					}
				}
			}
		}
	}
}

void OvCore::ECS::Renderer::DrawDrawable(const Drawable& p_toDraw)
{
	m_userMatrixSender(std::get<3>(p_toDraw));
	DrawMesh(*std::get<1>(p_toDraw), *std::get<2>(p_toDraw), &std::get<0>(p_toDraw));
}

void OvCore::ECS::Renderer::DrawModelWithSingleMaterial(OvRendering::Resources::Model& p_model, OvCore::Resources::Material& p_material, OvMaths::FMatrix4 const* p_modelMatrix, OvCore::Resources::Material* p_defaultMaterial)
{
	if (p_modelMatrix)
		m_modelMatrixSender(*p_modelMatrix);

	for (auto mesh : p_model.GetMeshes())
	{
		OvCore::Resources::Material* material = p_material.GetShader() ? &p_material : p_defaultMaterial;

		if (material)
			DrawMesh(*mesh, *material, nullptr);
	}
}

void OvCore::ECS::Renderer::DrawModelWithMaterials(OvRendering::Resources::Model& p_model, std::vector<OvCore::Resources::Material*> p_materials, OvMaths::FMatrix4 const* p_modelMatrix, OvCore::Resources::Material* p_defaultMaterial)
{
	if (p_modelMatrix)
		m_modelMatrixSender(*p_modelMatrix);

	for (auto mesh : p_model.GetMeshes())
	{
		OvCore::Resources::Material* material = p_materials.size() > mesh->GetMaterialIndex() ? p_materials[mesh->GetMaterialIndex()] : p_defaultMaterial;
		if (material)
			DrawMesh(*mesh, *material, nullptr);
	}
}

void OvCore::ECS::Renderer::DrawMesh(OvRendering::Resources::Mesh& p_mesh, OvCore::Resources::Material& p_material, OvMaths::FMatrix4 const* p_modelMatrix)
{
	using namespace OvRendering::Settings;

	if (p_material.HasShader() && p_material.GetGPUInstances() > 0)
	{
		if (p_modelMatrix)
			m_modelMatrixSender(*p_modelMatrix);

		uint8_t stateMask = p_material.GenerateStateMask();
		ApplyStateMask(stateMask);
		
		/* Draw the mesh */
		p_material.Bind(m_emptyTexture);
		Draw(p_mesh, OvRendering::Settings::EPrimitiveMode::TRIANGLES, p_material.GetGPUInstances());
		p_material.UnBind();
	}
}

void OvCore::ECS::Renderer::RegisterModelMatrixSender(std::function<void(OvMaths::FMatrix4)> p_modelMatrixSender)
{
	m_modelMatrixSender = p_modelMatrixSender;
}

void OvCore::ECS::Renderer::RegisterUserMatrixSender(std::function<void(OvMaths::FMatrix4)> p_userMatrixSender)
{
	m_userMatrixSender = p_userMatrixSender;
}
