/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#include "OvGame/Core/GameRenderer.h"

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
		UpdateLights(*currentScene);

		if (OvCore::ECS::Components::CCamera* mainCamera = m_context.renderer->FindMainCamera(*currentScene))
		{
			UpdateEngineUBO(*mainCamera);

			m_context.renderer->Clear(mainCamera->GetCamera(), true, true, false);

			uint8_t glState = m_context.renderer->FetchGLState();
			m_context.renderer->ApplyStateMask(glState);
			m_context.renderer->RenderScene(*currentScene, mainCamera->owner.transform.GetWorldPosition(), &m_emptyMaterial);
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
	auto [winWidth, winHeight] = m_context.window->GetSize();

	size_t offset = sizeof(OvMaths::FMatrix4); // We skip the model matrix (Which is a special case, modified every draw calls)

	m_context.engineUBO->SetSubData(OvMaths::FMatrix4::Transpose(p_mainCamera.GetViewMatrix()), std::ref(offset));
	m_context.engineUBO->SetSubData(OvMaths::FMatrix4::Transpose(p_mainCamera.GetProjectionMatrix(winWidth, winHeight)), std::ref(offset));
	m_context.engineUBO->SetSubData(p_mainCamera.owner.transform.GetWorldPosition(), std::ref(offset));
}

void OvGame::Core::GameRenderer::UpdateLights(OvCore::SceneSystem::Scene& p_scene)
{
	std::vector<FMatrix4> lightMatrices;
	m_context.renderer->FindLightMatrices(p_scene, lightMatrices);
	m_context.lightSSBO->SendBlocks<FMatrix4>(lightMatrices.data(), lightMatrices.size() * sizeof(FMatrix4));
}