/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <OvCore/ECS/Components/CCamera.h>
#include <OvCore/Rendering/SceneRenderer.h>

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
	m_sceneManager(EDITOR_CONTEXT(sceneManager))
{
	m_renderer = std::make_unique<OvCore::Rendering::SceneRenderer>(*EDITOR_CONTEXT(driver));
}

void OvEditor::Panels::GameView::Update(float p_deltaTime)
{
	AView::Update(p_deltaTime);

	/*
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
	*/
}

void OvEditor::Panels::GameView::_Render_Impl()
{
	if (auto currentScene = m_sceneManager.GetCurrentScene(); currentScene && m_size.x > 0 && m_size.y > 0)
	{
		GetRendererAs<OvCore::Rendering::SceneRenderer>().RenderScene(
			*currentScene,
			static_cast<uint16_t>(m_size.x),
			static_cast<uint16_t>(m_size.y),
			std::nullopt // TODO: Evaluate if we want to use a default material here
		);
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
