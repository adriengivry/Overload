/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvGame/Core/GameRenderer.h"

#include <OvAnalytics/Profiling/ProfilerSpy.h>

#include <OvCore/ECS/Components/CMaterialRenderer.h>
#include <OvCore/ECS/Components/CModelRenderer.h>
#include <OvCore/ECS/Components/CPointLight.h>
#include <OvCore/ECS/Components/CDirectionalLight.h>
#include <OvCore/ECS/Components/CSpotLight.h>
#include <OvCore/ECS/Components/CAmbientBoxLight.h>
#include <OvCore/ECS/Components/CAmbientSphereLight.h>

using namespace OvMaths;
using namespace OvRendering::Resources;
using namespace OvCore::Resources;

OvGame::Core::GameRenderer::GameRenderer(Context & p_context) :
	m_context(p_context)
{
	/* Empty Material */
	m_emptyMaterial.SetShader(m_context.shaderManager[":Shaders\\Unlit.glsl"]);
	m_emptyMaterial.Set("u_Diffuse", FVector3(1.f, 0.f, 1.f));
	m_emptyMaterial.Set<OvRendering::Resources::Texture*>("u_DiffuseMap", nullptr);

	m_context.renderer->RegisterModelMatrixSender([this](const OvMaths::FMatrix4 & p_modelMatrix)
	{
		m_context.engineUBO->SetSubData(OvMaths::FMatrix4::Transpose(p_modelMatrix), 0);
	});

	m_context.renderer->RegisterUserMatrixSender([this](const OvMaths::FMatrix4 & p_userMatrix)
	{
		m_context.engineUBO->SetSubData
		(
			p_userMatrix,

			// UBO layout offset
			sizeof(OvMaths::FMatrix4) +
			sizeof(OvMaths::FMatrix4) +
			sizeof(OvMaths::FMatrix4) +
			sizeof(OvMaths::FVector3) +
			sizeof(float)
		);
	});
}

void OvGame::Core::GameRenderer::RenderScene()
{
	if (auto currentScene = m_context.sceneManager.GetCurrentScene())
	{
		if (OvCore::ECS::Components::CCamera* mainCameraComponent = m_context.renderer->FindMainCamera(*currentScene))
		{
			if (mainCameraComponent->HasFrustumLightCulling())
			{
				UpdateLightsInFrustum(*currentScene, mainCameraComponent->GetCamera().GetFrustum());
			}
			else
			{
				UpdateLights(*currentScene);
			}

			auto [winWidth, winHeight] = m_context.window->GetSize();
			const auto& cameraPosition = mainCameraComponent->owner.transform.GetWorldPosition();
			const auto& cameraRotation = mainCameraComponent->owner.transform.GetWorldRotation();
			auto& camera = mainCameraComponent->GetCamera();

			camera.CacheMatrices(winWidth, winHeight, cameraPosition, cameraRotation);

			UpdateEngineUBO(*mainCameraComponent);

			m_context.renderer->Clear(camera, true, true, false);

			uint8_t glState = m_context.renderer->FetchGLState();
			m_context.renderer->ApplyStateMask(glState);
			m_context.renderer->RenderScene(*currentScene, cameraPosition, camera, nullptr, &m_emptyMaterial);
			m_context.renderer->ApplyStateMask(glState);
		}
		else
		{
			m_context.renderer->SetClearColor(0.0f, 0.0f, 0.0f);
			m_context.renderer->Clear(true, true, false);
		}
	}
}

void OvGame::Core::GameRenderer::UpdateEngineUBO(OvCore::ECS::Components::CCamera& p_mainCamera)
{
	size_t offset = sizeof(OvMaths::FMatrix4); // We skip the model matrix (Which is a special case, modified every draw calls)
	auto& camera = p_mainCamera.GetCamera();

	m_context.engineUBO->SetSubData(OvMaths::FMatrix4::Transpose(camera.GetViewMatrix()), std::ref(offset));
	m_context.engineUBO->SetSubData(OvMaths::FMatrix4::Transpose(camera.GetProjectionMatrix()), std::ref(offset));
	m_context.engineUBO->SetSubData(p_mainCamera.owner.transform.GetWorldPosition(), std::ref(offset));
}

void OvGame::Core::GameRenderer::UpdateLights(OvCore::SceneSystem::Scene& p_scene)
{
	PROFILER_SPY("Light SSBO Update");
	auto lightMatrices = m_context.renderer->FindLightMatrices(p_scene);
	m_context.lightSSBO->SendBlocks<FMatrix4>(lightMatrices.data(), lightMatrices.size() * sizeof(FMatrix4));
}

void OvGame::Core::GameRenderer::UpdateLightsInFrustum(OvCore::SceneSystem::Scene& p_scene, const OvRendering::Data::Frustum& p_frustum)
{
	PROFILER_SPY("Light SSBO Update (Frustum culled)");
	auto lightMatrices = m_context.renderer->FindLightMatricesInFrustum(p_scene, p_frustum);
	m_context.lightSSBO->SendBlocks<FMatrix4>(lightMatrices.data(), lightMatrices.size() * sizeof(FMatrix4));
}
