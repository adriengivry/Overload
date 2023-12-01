/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvEditor/Rendering/PickingRenderFeature.h"
#include "OvEditor/Core/EditorActions.h"
#include "OvEditor/Settings/EditorSettings.h"
#include "OvEditor/Rendering/DebugSceneRenderer.h"

#include <OvCore/ECS/Components/CMaterialRenderer.h>

OvEditor::Rendering::PickingRenderFeature::PickingRenderFeature(OvRendering::Core::CompositeRenderer& p_renderer) :
	OvRendering::Features::ARenderFeature(p_renderer)
{
	/* Light Material */
	m_lightMaterial.SetShader(EDITOR_CONTEXT(editorResources)->GetShader("Billboard"));
	m_lightMaterial.SetDepthTest(true);

	/* Gizmo Pickable Material */
	m_gizmoPickingMaterial.SetShader(EDITOR_CONTEXT(editorResources)->GetShader("Gizmo"));
	m_gizmoPickingMaterial.SetGPUInstances(3);
	m_gizmoPickingMaterial.Set("u_IsBall", false);
	m_gizmoPickingMaterial.Set("u_IsPickable", true);

	/* Picking Material */
	m_actorPickingMaterial.SetShader(EDITOR_CONTEXT(shaderManager)[":Shaders\\Unlit.glsl"]);
	m_actorPickingMaterial.Set("u_Diffuse", OvMaths::FVector4(1.f, 1.f, 1.f, 1.0f));
	m_actorPickingMaterial.Set<OvRendering::Resources::Texture*>("u_DiffuseMap", nullptr);
	m_actorPickingMaterial.SetFrontfaceCulling(false);
	m_actorPickingMaterial.SetBackfaceCulling(false);
}

OvRendering::Settings::ERenderPassMask OvEditor::Rendering::PickingRenderFeature::GetRenderPassMask() const
{
	return OvRendering::Settings::ERenderPassMask::PRE_PROCESSING;
}

void OvEditor::Rendering::PickingRenderFeature::DrawPass(OvRendering::Settings::ERenderPass p_renderPass)
{
	// TODO: Make sure we only renderer when the view is hovered and not being resized

	using namespace OvCore::Rendering;

	OVASSERT(m_renderer.HasDescriptor<SceneRenderer::SceneDescriptor>(), "Cannot find SceneDescriptor attached to this renderer");
	OVASSERT(m_renderer.HasDescriptor<DebugSceneRenderer::DebugSceneDescriptor>(), "Cannot find DebugSceneDescriptor attached to this renderer");

	auto& sceneDescriptor = m_renderer.GetDescriptor<SceneRenderer::SceneDescriptor>();
	auto& debugSceneDescriptor = m_renderer.GetDescriptor<DebugSceneRenderer::DebugSceneDescriptor>();
	auto& frameDescriptor = m_renderer.GetFrameDescriptor();
	auto& scene = sceneDescriptor.scene;

	m_actorPickingFramebuffer.Resize(frameDescriptor.renderWidth, frameDescriptor.renderHeight);

	m_actorPickingFramebuffer.Bind();

	m_driver.SetClearColor(0.0f, 0.0f, 0.0f);
	m_driver.Clear();

	DrawPickableModels(scene);
	DrawPickableCameras(scene);
	DrawPickableLights(scene);

	if (debugSceneDescriptor.selectedActor)
	{
		auto& selectedActor = debugSceneDescriptor.selectedActor.get();

		DrawPickableGizmo(
			selectedActor.transform.GetWorldPosition(),
			selectedActor.transform.GetWorldRotation(),
			debugSceneDescriptor.gizmoOperation
		);
	}

	m_actorPickingFramebuffer.Unbind();

	if (auto output = frameDescriptor.outputBuffer)
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

OvEditor::Rendering::PickingRenderFeature::PickingResult OvEditor::Rendering::PickingRenderFeature::ReadbackPickingResult(
	const OvCore::SceneSystem::Scene& p_scene,
	uint32_t p_x,
	uint32_t p_y
)
{
	uint8_t pixel[3];

	m_actorPickingFramebuffer.Bind();

	m_driver.ReadPixels(
		p_x, p_y, 1, 1,
		OvRendering::Settings::EPixelDataFormat::RGB,
		OvRendering::Settings::EPixelDataType::UNSIGNED_BYTE,
		pixel
	);

	m_actorPickingFramebuffer.Unbind();

	uint32_t actorID = (0 << 24) | (pixel[2] << 16) | (pixel[1] << 8) | (pixel[0] << 0);
	auto actorUnderMouse = p_scene.FindActorByID(actorID);

	if (actorUnderMouse)
	{
		return OvTools::Utils::OptRef(*actorUnderMouse);
	}
	else if (
		pixel[0] == 255 &&
		pixel[1] == 255 &&
		pixel[2] >= 252 &&
		pixel[2] <= 254
		)
	{
		return static_cast<OvEditor::Core::GizmoBehaviour::EDirection>(pixel[2] - 252);
	}

	return std::nullopt;
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

void OvEditor::Rendering::PickingRenderFeature::DrawPickableGizmo(const OvMaths::FVector3& p_position, const OvMaths::FQuaternion& p_rotation, OvEditor::Core::EGizmoOperation p_operation)
{
	auto modelMatrix =
		OvMaths::FMatrix4::Translation(p_position) *
		OvMaths::FQuaternion::ToMatrix4(OvMaths::FQuaternion::Normalize(p_rotation));

	auto arrowModel = EDITOR_CONTEXT(editorResources)->GetModel("Arrow_Picking");
	m_renderer.DrawModelWithSingleMaterial(*arrowModel, m_gizmoPickingMaterial, modelMatrix);
}
