/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvEditor/Rendering/PickingRenderFeature.h"
#include "OvEditor/Core/EditorActions.h"
#include "OvEditor/Settings/EditorSettings.h"

#include <OvCore/ECS/Components/CMaterialRenderer.h>

OvEditor::Rendering::PickingRenderFeature::PickingRenderFeature(OvCore::Rendering::SceneRenderer& p_renderer) :
	OvRendering::Features::ARenderFeature(p_renderer)
{
	/* Light Material */
	m_lightMaterial.SetShader(EDITOR_CONTEXT(editorResources)->GetShader("Billboard"));
	m_lightMaterial.SetDepthTest(true);
}

OvRendering::Settings::ERenderPassMask OvEditor::Rendering::PickingRenderFeature::GetRenderPassMask() const
{
	return OvRendering::Settings::ERenderPassMask::PRE_PROCESSING;
}

void OvEditor::Rendering::PickingRenderFeature::DrawPass(OvRendering::Settings::ERenderPass p_renderPass)
{
	using namespace OvCore::Rendering;

	OVASSERT(m_renderer.HasDescriptor<SceneRenderer::SceneDescriptor>(), "Cannot find SceneDescriptor attached to this renderer");

	auto& sceneDescriptor = m_renderer.GetDescriptor<SceneRenderer::SceneDescriptor>();
	auto& scene = sceneDescriptor.scene;

	m_actorPickingFramebuffer.Bind();

	DrawPickableModels(scene);
	DrawPickableCameras(scene);
	DrawPickableLights(scene);

	m_actorPickingFramebuffer.Unbind();

	if (auto output = m_renderer.GetFrameDescriptor().outputBuffer)
	{
		output->Bind();
	}
}

void PreparePickingMaterial(OvCore::ECS::Actor& p_actor, OvCore::Resources::Material& p_material)
{
	uint32_t actorID = static_cast<uint32_t>(p_actor.GetID());

	auto bytes = reinterpret_cast<uint8_t*>(&actorID);
	auto color = OvMaths::FVector4{ bytes[0] / 255.0f, bytes[1] / 255.0f, bytes[2] / 255.0f, 1.0f };

	p_material.Set("u_Diffuse", color);
}

void OvEditor::Rendering::PickingRenderFeature::DrawPickableModels(OvCore::SceneSystem::Scene& p_scene)
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

					PreparePickingMaterial(actor, m_actorPickingMaterial);

					for (auto mesh : model->GetMeshes())
					{
						if (mesh->GetMaterialIndex() < MAX_MATERIAL_COUNT)
						{
							if (auto material = materials.at(mesh->GetMaterialIndex()))
							{
								if (material && material->GetShader())
								{
									OvRendering::Entities::Drawable drawable;
									drawable.modelMatrix = modelMatrix;
									drawable.mesh = *mesh;
									drawable.material = m_actorPickingMaterial;
									drawable.stateMask = material->GenerateStateMask();

									m_renderer.DrawEntity(drawable);
								}
							}
						}
					}
				}
			}
		}
	}
}

void OvEditor::Rendering::PickingRenderFeature::DrawPickableCameras(OvCore::SceneSystem::Scene& p_scene)
{
	for (auto camera : p_scene.GetFastAccessComponents().cameras)
	{
		auto& actor = camera->owner;

		if (actor.IsActive())
		{
			PreparePickingMaterial(actor, m_actorPickingMaterial);
			auto& cameraModel = *EDITOR_CONTEXT(editorResources)->GetModel("Camera");
			auto translation = OvMaths::FMatrix4::Translation(actor.transform.GetWorldPosition());
			auto rotation = OvMaths::FQuaternion::ToMatrix4(actor.transform.GetWorldRotation());
			auto modelMatrix = translation * rotation;
			m_renderer.DrawModelWithSingleMaterial(cameraModel, m_actorPickingMaterial, modelMatrix);
		}
	}
}

void OvEditor::Rendering::PickingRenderFeature::DrawPickableLights(OvCore::SceneSystem::Scene& p_scene)
{
	if (Settings::EditorSettings::LightBillboardScale > 0.001f)
	{
		m_driver.Clear(false, true, false);

		m_lightMaterial.Set<float>("u_Scale", Settings::EditorSettings::LightBillboardScale * 0.1f);

		for (auto light : p_scene.GetFastAccessComponents().lights)
		{
			auto& actor = light->owner;

			if (actor.IsActive())
			{
				PreparePickingMaterial(actor, m_lightMaterial);
				auto& lightModel = *EDITOR_CONTEXT(editorResources)->GetModel("Vertical_Plane");
				auto modelMatrix = OvMaths::FMatrix4::Translation(actor.transform.GetWorldPosition());
				m_renderer.DrawModelWithSingleMaterial(lightModel, m_lightMaterial, modelMatrix);
			}
		}
	}
}
