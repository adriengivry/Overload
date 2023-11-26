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
) :
	AView(p_title, p_opened, p_windowSettings),
	m_sceneManager(EDITOR_CONTEXT(sceneManager)),
	m_sceneRenderer(*
		EDITOR_CONTEXT(driver),
		*EDITOR_CONTEXT(engineUBO),
		*EDITOR_CONTEXT(lightSSBO)
	)
{
}

void OvEditor::Panels::GameView::Update(float p_deltaTime)
{
	AView::Update(p_deltaTime);

	auto currentScene = EDITOR_CONTEXT(sceneManager).GetCurrentScene();

	if (currentScene)
	{
		auto cameraComponent = m_sceneRenderer.FindMainCamera(*currentScene);

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
	if (auto currentScene = m_sceneManager.GetCurrentScene())
	{
		m_fbo.Bind();

		m_sceneRenderer.RenderScene(
			*currentScene,
			static_cast<uint16_t>(m_size.x),
			static_cast<uint16_t>(m_size.y),
			nullptr // TODO: Evaluate if we want to use a default material here
		); 

		m_fbo.Unbind();
	}
}

bool OvEditor::Panels::GameView::HasCamera() const
{
	return m_hasCamera;
}

std::optional<OvRendering::Data::Frustum> OvEditor::Panels::GameView::GetActiveFrustum() const
{
	return m_hasCamera ? m_camera.GetFrustum() : std::optional<OvRendering::Data::Frustum>{};
}
