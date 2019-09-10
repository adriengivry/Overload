/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#include <OvCore/ECS/Components/CCamera.h>

#include "OvEditor/Core/EditorRenderer.h"
#include "OvEditor/Panels/GameView.h"
#include "OvEditor/Core/EditorActions.h"

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
	
	m_fbo.Bind();

	baseRenderer.Clear(m_camera);

	uint8_t glState = baseRenderer.FetchGLState();
	baseRenderer.ApplyStateMask(glState);

	if (m_hasCamera)
	{
		m_editorRenderer.RenderScene(m_cameraPosition, &m_camera.GetFrustum());
	}

	baseRenderer.ApplyStateMask(glState);

	m_fbo.Unbind();
}

bool OvEditor::Panels::GameView::HasCamera() const
{
	return m_hasCamera;
}
