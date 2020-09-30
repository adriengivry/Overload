/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <OvCore/ECS/Components/CCamera.h>

#include "OvEditor/Core/EditorRenderer.h"
#include "OvEditor/Panels/GameView.h"
#include "OvEditor/Core/EditorActions.h"
#include "OvEditor/Settings/EditorSettings.h"

OvEditor::Panels::GameView::GameView
(
	const std::string & p_title,
	bool p_opened,
	const OvUI::Settings::PanelWindowSettings & p_windowSettings
) : AView(p_title, p_opened, p_windowSettings), m_sceneManager(EDITOR_CONTEXT(sceneManager))
{
}

void OvEditor::Panels::GameView::Update(float p_deltaTime)
{
	AView::Update(p_deltaTime);

	auto currentScene = EDITOR_CONTEXT(sceneManager).GetCurrentScene();

	if (currentScene)
	{
		auto cameraComponent = EDITOR_CONTEXT(renderer)->FindMainCamera(*currentScene);
		if (cameraComponent)
		{
			m_camera = cameraComponent->GetCamera();
			m_cameraPosition = cameraComponent->owner.transform.GetWorldPosition();
			m_cameraRotation = cameraComponent->owner.transform.GetWorldRotation();
			m_hasCamera = true;
			PrepareCamera();
		}
		else
		{
			m_camera.SetClearColor({ 0.f, 0.f, 0.f });
			m_hasCamera = false;
		}
	}
}

void OvEditor::Panels::GameView::_Render_Impl()
{
	auto& baseRenderer = *EDITOR_CONTEXT(renderer).get();
	auto& currentScene = *m_sceneManager.GetCurrentScene();

	m_fbo.Bind();

	baseRenderer.Clear(m_camera);

	uint8_t glState = baseRenderer.FetchGLState();
	baseRenderer.ApplyStateMask(glState);

	if (m_hasCamera)
	{
		if (m_camera.HasFrustumLightCulling())
		{
			m_editorRenderer.UpdateLightsInFrustum(currentScene, m_camera.GetFrustum());
		}
		else
		{
			m_editorRenderer.UpdateLights(currentScene);
		}

		m_editorRenderer.RenderScene(m_cameraPosition, m_camera);
	}

	baseRenderer.ApplyStateMask(glState);

	m_fbo.Unbind();
}

bool OvEditor::Panels::GameView::HasCamera() const
{
	return m_hasCamera;
}

std::optional<OvRendering::Data::Frustum> OvEditor::Panels::GameView::GetActiveFrustum() const
{
	return m_hasCamera ? m_camera.GetFrustum() : std::optional<OvRendering::Data::Frustum>{};
}
