/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <OvCore/ECS/Components/CCamera.h>
#include <OvCore/ECS/Components/CPhysicalBox.h>
#include <OvCore/ECS/Components/CPhysicalSphere.h>
#include <OvCore/ECS/Components/CPhysicalCapsule.h>
#include <OvCore/ECS/Components/CMaterialRenderer.h>
#include <OvCore/ECS/Components/CPointLight.h>
#include <OvCore/ECS/Components/CDirectionalLight.h>
#include <OvCore/ECS/Components/CSpotLight.h>

#include <OvRendering/Features/DebugShapeRenderFeature.h>

#include <OvAnalytics/Profiling/ProfilerSpy.h>

#include <OvDebug/Assertion.h>

#include "OvEditor/Rendering/DebugSceneRenderer.h"
#include "OvEditor/Rendering/GridRenderFeature.h"
#include "OvEditor/Rendering/OutlineRenderFeature.h"
#include "OvEditor/Rendering/GizmoRenderFeature.h"
#include "OvEditor/Rendering/PickingRenderFeature.h"
#include "OvEditor/Core/EditorResources.h"
#include "OvEditor/Panels/AView.h"
#include "OvEditor/Panels/GameView.h"
#include "OvEditor/Settings/EditorSettings.h"

#include "OvEditor/Core/EditorActions.h"

using namespace OvMaths;
using namespace OvRendering::Resources;
using namespace OvCore::Resources;

const OvMaths::FVector3 DEBUG_BOUNDS_COLOR		= { 1.0f, 0.0f, 0.0f };
const OvMaths::FVector3 LIGHT_VOLUME_COLOR		= { 1.0f, 1.0f, 0.0f };
const OvMaths::FVector3 COLLIDER_COLOR			= { 0.0f, 1.0f, 0.0f };
const OvMaths::FVector3 FRUSTUM_COLOR			= { 1.0f, 1.0f, 1.0f };

const OvMaths::FVector4 kDefaultOutlineColor{ 1.0f, 0.7f, 0.0f, 1.0f };
const OvMaths::FVector4 kSelectedOutlineColor{ 1.0f, 1.0f, 0.0f, 1.0f };

constexpr float kDefaultOutlineWidth = 2.5f;
constexpr float kSelectedOutlineWidth = 5.0f;

OvEditor::Rendering::DebugSceneRenderer::DebugSceneRenderer(OvRendering::Context::Driver& p_driver) :
	OvCore::Rendering::SceneRenderer(p_driver),
	m_debugShapeFeature(AddFeature<OvRendering::Features::DebugShapeRenderFeature>())
{
	AddFeature<OvEditor::Rendering::GridRenderFeature>();
	AddFeature<OvEditor::Rendering::OutlineRenderFeature>();
	AddFeature<OvEditor::Rendering::GizmoRenderFeature>();
	AddFeature<OvEditor::Rendering::PickingRenderFeature>();

	// TODO: Do not use the driver directly heres
	m_driver.SetCapability(OvRendering::Settings::ERenderingCapability::STENCIL_TEST, true);
	m_driver.SetStencilOperations(OvRendering::Settings::EOperation::KEEP, OvRendering::Settings::EOperation::KEEP, OvRendering::Settings::EOperation::REPLACE);
	m_driver.SetStencilAlgorithm(OvRendering::Settings::EComparaisonAlgorithm::ALWAYS, 1, 0xFF);

	/* Default Material */
	//m_defaultMaterial.SetShader(EDITOR_CONTEXT(shaderManager)[":Shaders\\Standard.glsl"]);
	//m_defaultMaterial.Set("u_Diffuse", FVector4(1.f, 1.f, 1.f, 1.f));
	//m_defaultMaterial.Set("u_Shininess", 100.0f);
	//m_defaultMaterial.Set<OvRendering::Resources::Texture*>("u_DiffuseMap", nullptr);

	/* Empty Material */
	//m_emptyMaterial.SetShader(EDITOR_CONTEXT(shaderManager)[":Shaders\\Unlit.glsl"]);
	//m_emptyMaterial.Set("u_Diffuse", FVector4(1.f, 0.f, 1.f, 1.0f));
	//m_emptyMaterial.Set<OvRendering::Resources::Texture*>("u_DiffuseMap", nullptr);

	/* Camera Material */
	m_cameraMaterial.SetShader(EDITOR_CONTEXT(shaderManager)[":Shaders\\Lambert.glsl"]);
	m_cameraMaterial.Set("u_Diffuse", FVector4(0.0f, 0.3f, 0.7f, 1.0f));
	m_cameraMaterial.Set<OvRendering::Resources::Texture*>("u_DiffuseMap", nullptr);

	/* Light Material */
	m_lightMaterial.SetShader(EDITOR_CONTEXT(editorResources)->GetShader("Billboard"));
	m_lightMaterial.Set("u_Diffuse", FVector4(1.f, 1.f, 0.5f, 0.5f));
	m_lightMaterial.SetBackfaceCulling(false);
	m_lightMaterial.SetBlendable(true);
	m_lightMaterial.SetDepthTest(false);
}

void OvEditor::Rendering::DebugSceneRenderer::DrawPass(OvRendering::Settings::ERenderPass p_pass)
{
	OvCore::Rendering::SceneRenderer::DrawPass(p_pass);

	if (p_pass == OvRendering::Settings::ERenderPass::POST_TRANSPARENT)
	{
		OVASSERT(HasDescriptor<SceneDescriptor>(), "Missing SceneDescriptor!");
		OVASSERT(HasDescriptor<DebugSceneDescriptor>(), "Missing DebugSceneDescriptor!");

		auto& sceneDescriptor = GetDescriptor<SceneDescriptor>();
		auto& debugSceneDescriptor = GetDescriptor<DebugSceneDescriptor>();

		RenderCameras(sceneDescriptor.scene);
		RenderLights(sceneDescriptor.scene);

		if (debugSceneDescriptor.selectedActor)
		{
			auto& selectedActor = debugSceneDescriptor.selectedActor.value();
			DrawActorDebugElements(selectedActor);
			GetFeature<OutlineRenderFeature>().DrawOutline(selectedActor, kSelectedOutlineColor, kSelectedOutlineWidth);
			GetFeature<GizmoRenderFeature>().DrawGizmo(
				selectedActor.transform.GetWorldPosition(),
				selectedActor.transform.GetWorldRotation(),
				debugSceneDescriptor.gizmoOperation,
				false,
				debugSceneDescriptor.highlightedGizmoDirection
			);
		}
	}
}

OvMaths::FMatrix4 CalculateCameraModelMatrix(OvCore::ECS::Actor& p_actor)
{
	auto translation = FMatrix4::Translation(p_actor.transform.GetWorldPosition());
	auto rotation = FQuaternion::ToMatrix4(p_actor.transform.GetWorldRotation());

	return translation * rotation;
}

void OvEditor::Rendering::DebugSceneRenderer::RenderCameras(OvCore::SceneSystem::Scene& p_scene)
{
	using namespace OvMaths;

	for (auto camera : p_scene.GetFastAccessComponents().cameras)
	{
		auto& actor = camera->owner;

		if (actor.IsActive())
		{
			auto& model = *EDITOR_CONTEXT(editorResources)->GetModel("Camera");
			auto modelMatrix = CalculateCameraModelMatrix(actor);
			DrawModelWithSingleMaterial(model, m_cameraMaterial, modelMatrix);
		}
	}
}

std::optional<std::string> GetLightTypeTextureName(OvRendering::Entities::Light::Type type)
{
	using namespace OvRendering::Entities;

	switch (type)
	{
	case Light::Type::POINT: return "Bill_Point_Light";
	case Light::Type::SPOT: return "Bill_Spot_Light";
	case Light::Type::DIRECTIONAL: return "Bill_Directional_Light";
	case Light::Type::AMBIENT_BOX: return "Bill_Ambient_Box_Light";
	case Light::Type::AMBIENT_SPHERE: return "Bill_Ambient_Sphere_Light";
	}

	return std::nullopt;
}

void OvEditor::Rendering::DebugSceneRenderer::RenderLights(OvCore::SceneSystem::Scene& p_scene)
{
	m_lightMaterial.SetDepthTest(false);
	m_lightMaterial.Set<float>("u_Scale", Settings::EditorSettings::LightBillboardScale * 0.1f);

	for (auto light : p_scene.GetFastAccessComponents().lights)
	{
		auto& actor = light->owner;

		if (actor.IsActive())
		{
			auto& model = *EDITOR_CONTEXT(editorResources)->GetModel("Vertical_Plane");
			auto modelMatrix = OvMaths::FMatrix4::Translation(actor.transform.GetWorldPosition());

			auto lightType = static_cast<OvRendering::Entities::Light::Type>(static_cast<int>(light->GetData().type));
			auto lightTypeTextureName = GetLightTypeTextureName(lightType);

			auto lightTexture =
				lightTypeTextureName ?
				EDITOR_CONTEXT(editorResources)->GetTexture(lightTypeTextureName.value()) :
				nullptr;

			const auto& lightColor = light->GetColor();
			m_lightMaterial.Set<OvRendering::Resources::Texture*>("u_DiffuseMap", lightTexture);
			m_lightMaterial.Set<OvMaths::FVector4>("u_Diffuse", OvMaths::FVector4(lightColor.x, lightColor.y, lightColor.z, 0.75f));
			DrawModelWithSingleMaterial(model, m_lightMaterial, modelMatrix);
		}
	}
}

void OvEditor::Rendering::DebugSceneRenderer::DrawActorDebugElements(OvCore::ECS::Actor& p_actor)
{
	if (p_actor.IsActive())
	{
		/* Render static mesh outline and bounding spheres */
		if (auto modelRenderer = p_actor.GetComponent<OvCore::ECS::Components::CModelRenderer>(); modelRenderer && modelRenderer->GetModel())
		{
			RenderBoundingSpheres(*modelRenderer);
		}

		/* Render camera component outline */
		if (auto cameraComponent = p_actor.GetComponent<OvCore::ECS::Components::CCamera>(); cameraComponent)
		{
			auto model = CalculateCameraModelMatrix(p_actor);
			RenderCameraFrustum(*cameraComponent);
		}
		
		/* Render the actor collider */
		if (p_actor.GetComponent<OvCore::ECS::Components::CPhysicalObject>())
		{
			RenderActorCollider(p_actor);
		}

		/* Render the actor ambient light */
		if (auto ambientBoxComp = p_actor.GetComponent<OvCore::ECS::Components::CAmbientBoxLight>())
		{
			RenderAmbientBoxVolume(*ambientBoxComp);
		}

		if (auto ambientSphereComp = p_actor.GetComponent<OvCore::ECS::Components::CAmbientSphereLight>())
		{
			RenderAmbientSphereVolume(*ambientSphereComp);
		}

		if (OvEditor::Settings::EditorSettings::ShowLightBounds)
		{
			if (auto light = p_actor.GetComponent<OvCore::ECS::Components::CLight>())
			{
				RenderLightBounds(*light);
			}
		}

		for (auto& child : p_actor.GetChildren())
		{
			DrawActorDebugElements(*child);
		}
	}
}

void OvEditor::Rendering::DebugSceneRenderer::DrawFrustumLines(
	const OvMaths::FVector3& pos,
	const OvMaths::FVector3& forward,
	float near,
	const float far,
	const OvMaths::FVector3& a,
	const OvMaths::FVector3& b,
	const OvMaths::FVector3& c,
	const OvMaths::FVector3& d,
	const OvMaths::FVector3& e,
	const OvMaths::FVector3& f,
	const OvMaths::FVector3& g,
	const OvMaths::FVector3& h
)
{
		// Convenient lambda to draw a frustum line
	auto draw = [&](const FVector3& p_start, const FVector3& p_end, const float planeDistance)
	{
		auto offset = pos + forward * planeDistance;
		auto start = offset + p_start;
		auto end = offset + p_end;
		m_debugShapeFeature.DrawLine(start, end, FRUSTUM_COLOR);
	};

	// Draw near plane
	draw(a, b, near);
	draw(b, d, near);
	draw(d, c, near);
	draw(c, a, near);

	// Draw far plane
	draw(e, f, far);
	draw(f, h, far);
	draw(h, g, far);
	draw(g, e, far);

	// Draw lines between near and far planes
	draw(a + forward * near, e + forward * far, 0);
	draw(b + forward * near, f + forward * far, 0);
	draw(c + forward * near, g + forward * far, 0);
	draw(d + forward * near, h + forward * far, 0);
}

void OvEditor::Rendering::DebugSceneRenderer::RenderCameraPerspectiveFrustum(std::pair<uint16_t, uint16_t>& p_size, OvCore::ECS::Components::CCamera& p_camera)
{
	const auto& owner = p_camera.owner;
	auto& camera = p_camera.GetCamera();

	const auto& cameraPos = owner.transform.GetWorldPosition();
	const auto& cameraRotation = owner.transform.GetWorldRotation();
	const auto& cameraForward = owner.transform.GetWorldForward();

	camera.CacheMatrices(p_size.first, p_size.second); // TODO: We shouldn't cache matrices mid air, we could use another funciton to get the matrices/calculate
	const auto proj = FMatrix4::Transpose(camera.GetProjectionMatrix());
	const auto near = camera.GetNear();
	const auto far = camera.GetFar();

	const auto nLeft = near * (proj.data[2] - 1.0f) / proj.data[0];
	const auto nRight = near * (1.0f + proj.data[2]) / proj.data[0];
	const auto nTop = near * (1.0f + proj.data[6]) / proj.data[5];
	const auto nBottom = near * (proj.data[6] - 1.0f) / proj.data[5];

	const auto fLeft = far * (proj.data[2] - 1.0f) / proj.data[0];
	const auto fRight = far * (1.0f + proj.data[2]) / proj.data[0];
	const auto fTop = far * (1.0f + proj.data[6]) / proj.data[5];
	const auto fBottom = far * (proj.data[6] - 1.0f) / proj.data[5];

	auto a = cameraRotation * FVector3{ nLeft, nTop, 0 };
	auto b = cameraRotation * FVector3{ nRight, nTop, 0 };
	auto c = cameraRotation * FVector3{ nLeft, nBottom, 0 };
	auto d = cameraRotation * FVector3{ nRight, nBottom, 0 };
	auto e = cameraRotation * FVector3{ fLeft, fTop, 0 };
	auto f = cameraRotation * FVector3{ fRight, fTop, 0 };
	auto g = cameraRotation * FVector3{ fLeft, fBottom, 0 };
	auto h = cameraRotation * FVector3{ fRight, fBottom, 0 };

	DrawFrustumLines(cameraPos, cameraForward, near, far, a, b, c, d, e, f, g, h);
}

void OvEditor::Rendering::DebugSceneRenderer::RenderCameraOrthographicFrustum(std::pair<uint16_t, uint16_t>& p_size, OvCore::ECS::Components::CCamera& p_camera)
{
	auto& owner = p_camera.owner;
	auto& camera = p_camera.GetCamera();
	const auto ratio = p_size.first / static_cast<float>(p_size.second);

	const auto& cameraPos = owner.transform.GetWorldPosition();
	const auto& cameraRotation = owner.transform.GetWorldRotation();
	const auto& cameraForward = owner.transform.GetWorldForward();

	const auto near = camera.GetNear();
	const auto far = camera.GetFar();
	const auto size = p_camera.GetSize();

	const auto right = ratio * size;
	const auto left = -right;
	const auto top = size;
	const auto bottom = -top;

	const auto a = cameraRotation * FVector3{ left, top, 0 };
	const auto b = cameraRotation * FVector3{ right, top, 0 };
	const auto c = cameraRotation * FVector3{ left, bottom, 0 };
	const auto d = cameraRotation * FVector3{ right, bottom, 0 };

	DrawFrustumLines(cameraPos, cameraForward, near, far, a, b, c, d, a, b, c, d);
}

void OvEditor::Rendering::DebugSceneRenderer::RenderCameraFrustum(OvCore::ECS::Components::CCamera& p_camera)
{
	auto& gameView = EDITOR_PANEL(Panels::GameView, "Game View");
	auto gameViewSize = gameView.GetSafeSize();

	if (gameViewSize.first == 0 || gameViewSize.second == 0)
	{
		gameViewSize = { 16, 9 };
	}

	switch (p_camera.GetProjectionMode())
	{
	case OvRendering::Settings::EProjectionMode::ORTHOGRAPHIC:
		RenderCameraOrthographicFrustum(gameViewSize, p_camera);
		break;
	
	case OvRendering::Settings::EProjectionMode::PERSPECTIVE:
		RenderCameraPerspectiveFrustum(gameViewSize, p_camera);
		break;
	}
}

void OvEditor::Rendering::DebugSceneRenderer::RenderActorCollider(OvCore::ECS::Actor & p_actor)
{
	using namespace OvCore::ECS::Components;
	using namespace OvPhysics::Entities;

	bool depthTestBackup = m_driver.GetCapability(OvRendering::Settings::ERenderingCapability::DEPTH_TEST);
	m_driver.SetCapability(OvRendering::Settings::ERenderingCapability::DEPTH_TEST, false);

	/* Draw the box collider if any */
	if (auto boxColliderComponent = p_actor.GetComponent<OvCore::ECS::Components::CPhysicalBox>(); boxColliderComponent)
	{
		OvMaths::FQuaternion rotation = p_actor.transform.GetWorldRotation();
		OvMaths::FVector3 center = p_actor.transform.GetWorldPosition();
		OvMaths::FVector3 colliderSize = boxColliderComponent->GetSize();
		OvMaths::FVector3 actorScale = p_actor.transform.GetWorldScale();
		OvMaths::FVector3 halfSize = { colliderSize.x * actorScale.x, colliderSize.y * actorScale.y, colliderSize.z * actorScale.z };
		OvMaths::FVector3 size = halfSize * 2.f;
		
		m_debugShapeFeature.DrawLine(center + rotation * OvMaths::FVector3{ -halfSize.x, -halfSize.y, -halfSize.z }, center	+ rotation * OvMaths::FVector3{ -halfSize.x, -halfSize.y, +halfSize.z }, OvMaths::FVector3{ 0.f, 1.f, 0.f }, 1.f);
		m_debugShapeFeature.DrawLine(center + rotation * OvMaths::FVector3{ -halfSize.x, halfSize.y, -halfSize.z }, center	+ rotation * OvMaths::FVector3{ -halfSize.x, +halfSize.y, +halfSize.z }, OvMaths::FVector3{ 0.f, 1.f, 0.f }, 1.f);
		m_debugShapeFeature.DrawLine(center + rotation * OvMaths::FVector3{ -halfSize.x, -halfSize.y, -halfSize.z }, center	+ rotation * OvMaths::FVector3{ -halfSize.x, +halfSize.y, -halfSize.z }, OvMaths::FVector3{ 0.f, 1.f, 0.f }, 1.f);
		m_debugShapeFeature.DrawLine(center + rotation * OvMaths::FVector3{ -halfSize.x, -halfSize.y, +halfSize.z }, center	+ rotation * OvMaths::FVector3{ -halfSize.x, +halfSize.y, +halfSize.z }, OvMaths::FVector3{ 0.f, 1.f, 0.f }, 1.f);
		m_debugShapeFeature.DrawLine(center + rotation * OvMaths::FVector3{ +halfSize.x, -halfSize.y, -halfSize.z }, center	+ rotation * OvMaths::FVector3{ +halfSize.x, -halfSize.y, +halfSize.z }, OvMaths::FVector3{ 0.f, 1.f, 0.f }, 1.f);
		m_debugShapeFeature.DrawLine(center + rotation * OvMaths::FVector3{ +halfSize.x, halfSize.y, -halfSize.z }, center	+ rotation * OvMaths::FVector3{ +halfSize.x, +halfSize.y, +halfSize.z }, OvMaths::FVector3{ 0.f, 1.f, 0.f }, 1.f);
		m_debugShapeFeature.DrawLine(center + rotation * OvMaths::FVector3{ +halfSize.x, -halfSize.y, -halfSize.z }, center	+ rotation * OvMaths::FVector3{ +halfSize.x, +halfSize.y, -halfSize.z }, OvMaths::FVector3{ 0.f, 1.f, 0.f }, 1.f);
		m_debugShapeFeature.DrawLine(center + rotation * OvMaths::FVector3{ +halfSize.x, -halfSize.y, +halfSize.z }, center	+ rotation * OvMaths::FVector3{ +halfSize.x, +halfSize.y, +halfSize.z }, OvMaths::FVector3{ 0.f, 1.f, 0.f }, 1.f);
		m_debugShapeFeature.DrawLine(center + rotation * OvMaths::FVector3{ -halfSize.x, -halfSize.y, -halfSize.z }, center	+ rotation * OvMaths::FVector3{ +halfSize.x, -halfSize.y, -halfSize.z }, OvMaths::FVector3{ 0.f, 1.f, 0.f }, 1.f);
		m_debugShapeFeature.DrawLine(center + rotation * OvMaths::FVector3{ -halfSize.x, +halfSize.y, -halfSize.z }, center	+ rotation * OvMaths::FVector3{ +halfSize.x, +halfSize.y, -halfSize.z }, OvMaths::FVector3{ 0.f, 1.f, 0.f }, 1.f);
		m_debugShapeFeature.DrawLine(center + rotation * OvMaths::FVector3{ -halfSize.x, -halfSize.y, +halfSize.z }, center	+ rotation * OvMaths::FVector3{ +halfSize.x, -halfSize.y, +halfSize.z }, OvMaths::FVector3{ 0.f, 1.f, 0.f }, 1.f);
		m_debugShapeFeature.DrawLine(center + rotation * OvMaths::FVector3{ -halfSize.x, +halfSize.y, +halfSize.z }, center	+ rotation * OvMaths::FVector3{ +halfSize.x, +halfSize.y, +halfSize.z }, OvMaths::FVector3{ 0.f, 1.f, 0.f }, 1.f);
	}

	/* Draw the sphere collider if any */
	if (auto sphereColliderComponent = p_actor.GetComponent<OvCore::ECS::Components::CPhysicalSphere>(); sphereColliderComponent)
	{
		FVector3 actorScale = p_actor.transform.GetWorldScale();
		OvMaths::FQuaternion rotation = p_actor.transform.GetWorldRotation();
		OvMaths::FVector3 center = p_actor.transform.GetWorldPosition();
		float radius = sphereColliderComponent->GetRadius() * std::max(std::max(std::max(actorScale.x, actorScale.y), actorScale.z), 0.0f);

		for (float i = 0; i <= 360.0f; i += 10.0f)
		{
			m_debugShapeFeature.DrawLine(center + rotation * (OvMaths::FVector3{ cos(i * (3.14f / 180.0f)), sin(i * (3.14f / 180.0f)), 0.f } * radius), center + rotation * (OvMaths::FVector3{ cos((i + 10.0f) * (3.14f / 180.0f)), sin((i + 10.0f) * (3.14f / 180.0f)), 0.f } * radius), OvMaths::FVector3{ 0.f, 1.f, 0.f }, 1.f);
			m_debugShapeFeature.DrawLine(center + rotation * (OvMaths::FVector3{ 0.f, sin(i * (3.14f / 180.0f)), cos(i * (3.14f / 180.0f)) } * radius), center + rotation * (OvMaths::FVector3{ 0.f, sin((i + 10.0f) * (3.14f / 180.0f)), cos((i + 10.0f) * (3.14f / 180.0f)) } * radius), OvMaths::FVector3{ 0.f, 1.f, 0.f }, 1.f);
			m_debugShapeFeature.DrawLine(center + rotation * (OvMaths::FVector3{ cos(i * (3.14f / 180.0f)), 0.f, sin(i * (3.14f / 180.0f)) } * radius), center + rotation * (OvMaths::FVector3{ cos((i + 10.0f) * (3.14f / 180.0f)), 0.f, sin((i + 10.0f) * (3.14f / 180.0f)) } * radius), OvMaths::FVector3{ 0.f, 1.f, 0.f }, 1.f);
		}
	}

	/* Draw the capsule collider if any */
	if (auto capsuleColliderComponent = p_actor.GetComponent<OvCore::ECS::Components::CPhysicalCapsule>(); capsuleColliderComponent)
	{
		float radius = abs(capsuleColliderComponent->GetRadius() * std::max(std::max(p_actor.transform.GetWorldScale().x, p_actor.transform.GetWorldScale().z), 0.f));
		float height = abs(capsuleColliderComponent->GetHeight() * p_actor.transform.GetWorldScale().y);
		float halfHeight = height / 2;

		FVector3 actorScale = p_actor.transform.GetWorldScale();
		OvMaths::FQuaternion rotation = p_actor.transform.GetWorldRotation();
		OvMaths::FVector3 center = p_actor.transform.GetWorldPosition();

		OvMaths::FVector3 hVec = { 0.0f, halfHeight, 0.0f };
		for (float i = 0; i < 360.0f; i += 10.0f)
		{
			m_debugShapeFeature.DrawLine(center + rotation * (hVec + OvMaths::FVector3{ cos(i * (3.14f / 180.0f)), 0.f, sin(i * (3.14f / 180.0f)) } *radius), center + rotation * (hVec + OvMaths::FVector3{ cos((i + 10.0f) * (3.14f / 180.0f)), 0.f, sin((i + 10.0f) * (3.14f / 180.0f)) } *radius), OvMaths::FVector3{ 0.f, 1.f, 0.f }, 1.f);
			m_debugShapeFeature.DrawLine(center + rotation * (-hVec + OvMaths::FVector3{ cos(i * (3.14f / 180.0f)), 0.f, sin(i * (3.14f / 180.0f)) } *radius), center + rotation * (-hVec + OvMaths::FVector3{ cos((i + 10.0f) * (3.14f / 180.0f)), 0.f, sin((i + 10.0f) * (3.14f / 180.0f)) } *radius), OvMaths::FVector3{ 0.f, 1.f, 0.f }, 1.f);

			if (i < 180.f)
			{
				m_debugShapeFeature.DrawLine(center + rotation * (hVec + OvMaths::FVector3{ cos(i * (3.14f / 180.0f)), sin(i * (3.14f / 180.0f)), 0.f } * radius), center + rotation * (hVec + OvMaths::FVector3{ cos((i + 10.0f) * (3.14f / 180.0f)), sin((i + 10.0f) * (3.14f / 180.0f)), 0.f } * radius), OvMaths::FVector3{ 0.f, 1.f, 0.f }, 1.f);
				m_debugShapeFeature.DrawLine(center + rotation * (hVec + OvMaths::FVector3{ 0.f, sin(i * (3.14f / 180.0f)), cos(i * (3.14f / 180.0f)) } * radius), center + rotation * (hVec + OvMaths::FVector3{ 0.f, sin((i + 10.0f) * (3.14f / 180.0f)), cos((i + 10.0f) * (3.14f / 180.0f)) } * radius), OvMaths::FVector3{ 0.f, 1.f, 0.f }, 1.f);
			}
			else
			{
				m_debugShapeFeature.DrawLine(center + rotation * (-hVec + OvMaths::FVector3{ cos(i * (3.14f / 180.0f)), sin(i * (3.14f / 180.0f)), 0.f } * radius), center + rotation * (-hVec + OvMaths::FVector3{ cos((i + 10.0f) * (3.14f / 180.0f)), sin((i + 10.0f) * (3.14f / 180.0f)), 0.f } * radius), OvMaths::FVector3{ 0.f, 1.f, 0.f }, 1.f);
				m_debugShapeFeature.DrawLine(center + rotation * (-hVec + OvMaths::FVector3{ 0.f, sin(i * (3.14f / 180.0f)), cos(i * (3.14f / 180.0f)) } * radius), center + rotation * (-hVec + OvMaths::FVector3{ 0.f, sin((i + 10.0f) * (3.14f / 180.0f)), cos((i + 10.0f) * (3.14f / 180.0f)) } * radius), OvMaths::FVector3{ 0.f, 1.f, 0.f }, 1.f);
			}
		}

		m_debugShapeFeature.DrawLine(center + rotation * (OvMaths::FVector3{ -radius, -halfHeight, 0.f }),	center + rotation * (OvMaths::FVector3{ -radius, +halfHeight, 0.f }), OvMaths::FVector3{ 0.f, 1.f, 0.f }, 1.f);
		m_debugShapeFeature.DrawLine(center + rotation * (OvMaths::FVector3{ radius, -halfHeight, 0.f }),	center + rotation * (OvMaths::FVector3{ radius, +halfHeight, 0.f }), OvMaths::FVector3{ 0.f, 1.f, 0.f }, 1.f);
		m_debugShapeFeature.DrawLine(center + rotation * (OvMaths::FVector3{ 0.f, -halfHeight, -radius }),	center + rotation * (OvMaths::FVector3{ 0.f, +halfHeight, -radius }), OvMaths::FVector3{ 0.f, 1.f, 0.f }, 1.f);
		m_debugShapeFeature.DrawLine(center + rotation * (OvMaths::FVector3{ 0.f, -halfHeight, radius }),	center + rotation * (OvMaths::FVector3{ 0.f, +halfHeight, radius }), OvMaths::FVector3{ 0.f, 1.f, 0.f }, 1.f);
	}

	m_driver.SetCapability(OvRendering::Settings::ERenderingCapability::DEPTH_TEST, depthTestBackup);
	m_driver.SetRasterizationLinesWidth(1.0f);
}

void OvEditor::Rendering::DebugSceneRenderer::RenderLightBounds(OvCore::ECS::Components::CLight& p_light)
{
	bool depthTestBackup = m_driver.GetCapability(OvRendering::Settings::ERenderingCapability::DEPTH_TEST);
	m_driver.SetCapability(OvRendering::Settings::ERenderingCapability::DEPTH_TEST, false);

	auto& data = p_light.GetData();

	OvMaths::FQuaternion rotation = data.GetTransform().GetWorldRotation();
	OvMaths::FVector3 center = data.GetTransform().GetWorldPosition();
	float radius = data.GetEffectRange();

	if (!std::isinf(radius))
	{
		for (float i = 0; i <= 360.0f; i += 10.0f)
		{
			m_debugShapeFeature.DrawLine(center + rotation * (OvMaths::FVector3{ cos(i * (3.14f / 180.0f)), sin(i * (3.14f / 180.0f)), 0.f } *radius), center + rotation * (OvMaths::FVector3{ cos((i + 10.0f) * (3.14f / 180.0f)), sin((i + 10.0f) * (3.14f / 180.0f)), 0.f } *radius), DEBUG_BOUNDS_COLOR, 1.f);
			m_debugShapeFeature.DrawLine(center + rotation * (OvMaths::FVector3{ 0.f, sin(i * (3.14f / 180.0f)), cos(i * (3.14f / 180.0f)) } *radius), center + rotation * (OvMaths::FVector3{ 0.f, sin((i + 10.0f) * (3.14f / 180.0f)), cos((i + 10.0f) * (3.14f / 180.0f)) } *radius), DEBUG_BOUNDS_COLOR, 1.f);
			m_debugShapeFeature.DrawLine(center + rotation * (OvMaths::FVector3{ cos(i * (3.14f / 180.0f)), 0.f, sin(i * (3.14f / 180.0f)) } *radius), center + rotation * (OvMaths::FVector3{ cos((i + 10.0f) * (3.14f / 180.0f)), 0.f, sin((i + 10.0f) * (3.14f / 180.0f)) } *radius), DEBUG_BOUNDS_COLOR, 1.f);
		}
	}

	m_driver.SetCapability(OvRendering::Settings::ERenderingCapability::DEPTH_TEST, depthTestBackup);
}

void OvEditor::Rendering::DebugSceneRenderer::RenderAmbientBoxVolume(OvCore::ECS::Components::CAmbientBoxLight & p_ambientBoxLight)
{
	bool depthTestBackup = m_driver.GetCapability(OvRendering::Settings::ERenderingCapability::DEPTH_TEST);
	m_driver.SetCapability(OvRendering::Settings::ERenderingCapability::DEPTH_TEST, false);

	auto& data = p_ambientBoxLight.GetData();

	FMatrix4 model =
		FMatrix4::Translation(p_ambientBoxLight.owner.transform.GetWorldPosition()) *
		FMatrix4::Scaling({ data.constant * 2.f, data.linear * 2.f, data.quadratic * 2.f });

	OvMaths::FVector3 center = p_ambientBoxLight.owner.transform.GetWorldPosition();
	OvMaths::FVector3 size = { data.constant * 2.f, data.linear * 2.f, data.quadratic * 2.f };
	OvMaths::FVector3 actorScale = p_ambientBoxLight.owner.transform.GetWorldScale();
	OvMaths::FVector3 halfSize = size / 2.f;

	m_debugShapeFeature.DrawLine(center + OvMaths::FVector3{ -halfSize.x, -halfSize.y, -halfSize.z }, center + OvMaths::FVector3{ -halfSize.x, -halfSize.y, +halfSize.z }, LIGHT_VOLUME_COLOR, 1.f);
	m_debugShapeFeature.DrawLine(center + OvMaths::FVector3{ -halfSize.x, halfSize.y, -halfSize.z }, center + OvMaths::FVector3{ -halfSize.x, +halfSize.y, +halfSize.z }, LIGHT_VOLUME_COLOR, 1.f);
	m_debugShapeFeature.DrawLine(center + OvMaths::FVector3{ -halfSize.x, -halfSize.y, -halfSize.z }, center + OvMaths::FVector3{ -halfSize.x, +halfSize.y, -halfSize.z }, LIGHT_VOLUME_COLOR, 1.f);
	m_debugShapeFeature.DrawLine(center + OvMaths::FVector3{ -halfSize.x, -halfSize.y, +halfSize.z }, center + OvMaths::FVector3{ -halfSize.x, +halfSize.y, +halfSize.z }, LIGHT_VOLUME_COLOR, 1.f);
	m_debugShapeFeature.DrawLine(center + OvMaths::FVector3{ +halfSize.x, -halfSize.y, -halfSize.z }, center + OvMaths::FVector3{ +halfSize.x, -halfSize.y, +halfSize.z }, LIGHT_VOLUME_COLOR, 1.f);
	m_debugShapeFeature.DrawLine(center + OvMaths::FVector3{ +halfSize.x, halfSize.y, -halfSize.z }, center + OvMaths::FVector3{ +halfSize.x, +halfSize.y, +halfSize.z }, LIGHT_VOLUME_COLOR, 1.f);
	m_debugShapeFeature.DrawLine(center + OvMaths::FVector3{ +halfSize.x, -halfSize.y, -halfSize.z }, center + OvMaths::FVector3{ +halfSize.x, +halfSize.y, -halfSize.z }, LIGHT_VOLUME_COLOR, 1.f);
	m_debugShapeFeature.DrawLine(center + OvMaths::FVector3{ +halfSize.x, -halfSize.y, +halfSize.z }, center + OvMaths::FVector3{ +halfSize.x, +halfSize.y, +halfSize.z }, LIGHT_VOLUME_COLOR, 1.f);
	m_debugShapeFeature.DrawLine(center + OvMaths::FVector3{ -halfSize.x, -halfSize.y, -halfSize.z }, center + OvMaths::FVector3{ +halfSize.x, -halfSize.y, -halfSize.z }, LIGHT_VOLUME_COLOR, 1.f);
	m_debugShapeFeature.DrawLine(center + OvMaths::FVector3{ -halfSize.x, +halfSize.y, -halfSize.z }, center + OvMaths::FVector3{ +halfSize.x, +halfSize.y, -halfSize.z }, LIGHT_VOLUME_COLOR, 1.f);
	m_debugShapeFeature.DrawLine(center + OvMaths::FVector3{ -halfSize.x, -halfSize.y, +halfSize.z }, center + OvMaths::FVector3{ +halfSize.x, -halfSize.y, +halfSize.z }, LIGHT_VOLUME_COLOR, 1.f);
	m_debugShapeFeature.DrawLine(center + OvMaths::FVector3{ -halfSize.x, +halfSize.y, +halfSize.z }, center + OvMaths::FVector3{ +halfSize.x, +halfSize.y, +halfSize.z }, LIGHT_VOLUME_COLOR, 1.f);

	m_driver.SetCapability(OvRendering::Settings::ERenderingCapability::DEPTH_TEST, depthTestBackup);
}

void OvEditor::Rendering::DebugSceneRenderer::RenderAmbientSphereVolume(OvCore::ECS::Components::CAmbientSphereLight & p_ambientSphereLight)
{
	bool depthTestBackup = m_driver.GetCapability(OvRendering::Settings::ERenderingCapability::DEPTH_TEST);
	m_driver.SetCapability(OvRendering::Settings::ERenderingCapability::DEPTH_TEST, false);

	auto& data = p_ambientSphereLight.GetData();

	OvMaths::FQuaternion rotation = p_ambientSphereLight.owner.transform.GetWorldRotation();
	OvMaths::FVector3 center = p_ambientSphereLight.owner.transform.GetWorldPosition();
	float radius = data.constant;

	for (float i = 0; i <= 360.0f; i += 10.0f)
	{
		m_debugShapeFeature.DrawLine(center + rotation * (OvMaths::FVector3{ cos(i * (3.14f / 180.0f)), sin(i * (3.14f / 180.0f)), 0.f } *radius), center + rotation * (OvMaths::FVector3{ cos((i + 10.0f) * (3.14f / 180.0f)), sin((i + 10.0f) * (3.14f / 180.0f)), 0.f } *radius), LIGHT_VOLUME_COLOR, 1.f);
		m_debugShapeFeature.DrawLine(center + rotation * (OvMaths::FVector3{ 0.f, sin(i * (3.14f / 180.0f)), cos(i * (3.14f / 180.0f)) } *radius), center + rotation * (OvMaths::FVector3{ 0.f, sin((i + 10.0f) * (3.14f / 180.0f)), cos((i + 10.0f) * (3.14f / 180.0f)) } *radius), LIGHT_VOLUME_COLOR, 1.f);
		m_debugShapeFeature.DrawLine(center + rotation * (OvMaths::FVector3{ cos(i * (3.14f / 180.0f)), 0.f, sin(i * (3.14f / 180.0f)) } *radius), center + rotation * (OvMaths::FVector3{ cos((i + 10.0f) * (3.14f / 180.0f)), 0.f, sin((i + 10.0f) * (3.14f / 180.0f)) } *radius), LIGHT_VOLUME_COLOR, 1.f);
	}

	m_driver.SetCapability(OvRendering::Settings::ERenderingCapability::DEPTH_TEST, depthTestBackup);
}

void OvEditor::Rendering::DebugSceneRenderer::RenderBoundingSpheres(OvCore::ECS::Components::CModelRenderer& p_modelRenderer)
{
	using namespace OvCore::ECS::Components;
	using namespace OvPhysics::Entities;

	bool depthTestBackup = m_driver.GetCapability(OvRendering::Settings::ERenderingCapability::DEPTH_TEST);
	m_driver.SetCapability(OvRendering::Settings::ERenderingCapability::DEPTH_TEST, false);

	/* Draw the sphere collider if any */
	if (auto model = p_modelRenderer.GetModel())
	{
		auto& actor = p_modelRenderer.owner;

		OvMaths::FVector3 actorScale = actor.transform.GetWorldScale();
		OvMaths::FQuaternion actorRotation = actor.transform.GetWorldRotation();
		OvMaths::FVector3 actorPosition = actor.transform.GetWorldPosition();

		const auto& modelBoundingsphere = 
			p_modelRenderer.GetFrustumBehaviour() == OvCore::ECS::Components::CModelRenderer::EFrustumBehaviour::CULL_CUSTOM ?
			p_modelRenderer.GetCustomBoundingSphere() :
			model->GetBoundingSphere();

		float radiusScale = std::max(std::max(std::max(actorScale.x, actorScale.y), actorScale.z), 0.0f);
		float scaledRadius = modelBoundingsphere.radius * radiusScale;
		auto sphereOffset = OvMaths::FQuaternion::RotatePoint(modelBoundingsphere.position, actorRotation) * radiusScale;

		OvMaths::FVector3 boundingSphereCenter = actorPosition + sphereOffset;

		for (float i = 0; i <= 360.0f; i += 10.0f)
		{
			m_debugShapeFeature.DrawLine(boundingSphereCenter + actorRotation * (OvMaths::FVector3{ cos(i * (3.14f / 180.0f)), sin(i * (3.14f / 180.0f)), 0.f } *scaledRadius), boundingSphereCenter + actorRotation * (OvMaths::FVector3{ cos((i + 10.0f) * (3.14f / 180.0f)), sin((i + 10.0f) * (3.14f / 180.0f)), 0.f } *scaledRadius), DEBUG_BOUNDS_COLOR, 1.f);
			m_debugShapeFeature.DrawLine(boundingSphereCenter + actorRotation * (OvMaths::FVector3{ 0.f, sin(i * (3.14f / 180.0f)), cos(i * (3.14f / 180.0f)) } *scaledRadius), boundingSphereCenter + actorRotation * (OvMaths::FVector3{ 0.f, sin((i + 10.0f) * (3.14f / 180.0f)), cos((i + 10.0f) * (3.14f / 180.0f)) } *scaledRadius), DEBUG_BOUNDS_COLOR, 1.f);
			m_debugShapeFeature.DrawLine(boundingSphereCenter + actorRotation * (OvMaths::FVector3{ cos(i * (3.14f / 180.0f)), 0.f, sin(i * (3.14f / 180.0f)) } *scaledRadius), boundingSphereCenter + actorRotation * (OvMaths::FVector3{ cos((i + 10.0f) * (3.14f / 180.0f)), 0.f, sin((i + 10.0f) * (3.14f / 180.0f)) } *scaledRadius), DEBUG_BOUNDS_COLOR, 1.f);
		}

		if (p_modelRenderer.GetFrustumBehaviour() == OvCore::ECS::Components::CModelRenderer::EFrustumBehaviour::CULL_MESHES)
		{
			const auto& meshes = model->GetMeshes();

			if (meshes.size() > 1) // One mesh would result into the same bounding sphere for mesh and model
			{
				for (auto mesh : meshes)
				{
					auto& meshBoundingSphere = mesh->GetBoundingSphere();
					float scaledRadius = meshBoundingSphere.radius * radiusScale;
					auto sphereOffset = OvMaths::FQuaternion::RotatePoint(meshBoundingSphere.position, actorRotation) * radiusScale;

					OvMaths::FVector3 boundingSphereCenter = actorPosition + sphereOffset;

					for (float i = 0; i <= 360.0f; i += 10.0f)
					{
						m_debugShapeFeature.DrawLine(boundingSphereCenter + actorRotation * (OvMaths::FVector3{ cos(i * (3.14f / 180.0f)), sin(i * (3.14f / 180.0f)), 0.f } *scaledRadius), boundingSphereCenter + actorRotation * (OvMaths::FVector3{ cos((i + 10.0f) * (3.14f / 180.0f)), sin((i + 10.0f) * (3.14f / 180.0f)), 0.f } *scaledRadius), DEBUG_BOUNDS_COLOR, 1.f);
						m_debugShapeFeature.DrawLine(boundingSphereCenter + actorRotation * (OvMaths::FVector3{ 0.f, sin(i * (3.14f / 180.0f)), cos(i * (3.14f / 180.0f)) } *scaledRadius), boundingSphereCenter + actorRotation * (OvMaths::FVector3{ 0.f, sin((i + 10.0f) * (3.14f / 180.0f)), cos((i + 10.0f) * (3.14f / 180.0f)) } *scaledRadius), DEBUG_BOUNDS_COLOR, 1.f);
						m_debugShapeFeature.DrawLine(boundingSphereCenter + actorRotation * (OvMaths::FVector3{ cos(i * (3.14f / 180.0f)), 0.f, sin(i * (3.14f / 180.0f)) } *scaledRadius), boundingSphereCenter + actorRotation * (OvMaths::FVector3{ cos((i + 10.0f) * (3.14f / 180.0f)), 0.f, sin((i + 10.0f) * (3.14f / 180.0f)) } *scaledRadius), DEBUG_BOUNDS_COLOR, 1.f);
					}
				}
			}
		}
	}

	m_driver.SetCapability(OvRendering::Settings::ERenderingCapability::DEPTH_TEST, depthTestBackup);
	m_driver.SetRasterizationLinesWidth(1.0f);
}

