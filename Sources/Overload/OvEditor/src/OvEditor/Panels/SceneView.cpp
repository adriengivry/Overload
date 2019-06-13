/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#include <OvUI/Plugins/DDTarget.h>

#include "OvEditor/Core/EditorRenderer.h"
#include "OvEditor/Core/EditorActions.h"
#include "OvEditor/Panels/SceneView.h"

OvEditor::Panels::SceneView::SceneView
(
	const std::string& p_title,
	bool p_opened,
	const OvUI::Settings::PanelWindowSettings& p_windowSettings
) : AViewControllable(p_title, p_opened, p_windowSettings, true),
	m_sceneManager(EDITOR_CONTEXT(sceneManager))
{
	m_camera.SetClearColor({ 0.278f, 0.278f, 0.278f });

	m_image->AddPlugin<OvUI::Plugins::DDTarget<std::pair<std::string, OvUI::Widgets::Layout::Group*>>>("File").DataReceivedEvent += [this](auto p_data)
	{
		std::string path = p_data.first;

		switch (OvTools::Utils::PathParser::GetFileType(path))
		{
		case OvTools::Utils::PathParser::EFileType::SCENE:	EDITOR_EXEC(LoadSceneFromDisk(path));			break;
		case OvTools::Utils::PathParser::EFileType::MODEL:	EDITOR_EXEC(CreateActorWithModel(path, true));	break;
		}
	};
}

void OvEditor::Panels::SceneView::Update(float p_deltaTime)
{
	AViewControllable::Update(p_deltaTime);

	if (IsHovered() && EDITOR_CONTEXT(inputManager)->IsMouseButtonPressed(OvWindowing::Inputs::EMouseButton::MOUSE_BUTTON_LEFT))
	{
		/* Prevent losing focus on actor while resizing a window */
		if (auto cursor = ImGui::GetMouseCursor();
			cursor != ImGuiMouseCursor_ResizeEW &&
			cursor != ImGuiMouseCursor_ResizeNS &&
			cursor != ImGuiMouseCursor_ResizeNWSE &&
			cursor != ImGuiMouseCursor_ResizeNESW &&
			cursor != ImGuiMouseCursor_ResizeAll)
		EDITOR_EXEC(UnselectActor());
	}
}

void OvEditor::Panels::SceneView::_Render_Impl()
{
	EDITOR_CONTEXT(renderer)->SetStencilMask(0xFF);
	EDITOR_CONTEXT(renderer)->Clear(m_camera);
	EDITOR_CONTEXT(renderer)->SetStencilMask(0x00);

	uint8_t glState = EDITOR_CONTEXT(renderer)->FetchGLState();
	EDITOR_CONTEXT(renderer)->ApplyStateMask(glState);

	m_editorRenderer.RenderGrid(m_cameraPosition, m_gridColor);
	m_editorRenderer.RenderCameras();
	m_editorRenderer.RenderScene(m_cameraPosition);

	if (EDITOR_EXEC(IsAnyActorSelected()))
	{
		auto& selectedActor = EDITOR_EXEC(GetSelectedActor());

		if (selectedActor.IsActive())
		{
			m_editorRenderer.RenderActorAsSelected(selectedActor, true);
			EDITOR_CONTEXT(renderer)->ApplyStateMask(glState);
			m_editorRenderer.RenderActorAsSelected(selectedActor, false);
		}

		EDITOR_CONTEXT(renderer)->ApplyStateMask(glState);
		EDITOR_CONTEXT(renderer)->Clear(false, true, false);
		m_editorRenderer.RenderGuizmo(selectedActor.transform.GetWorldPosition(), selectedActor.transform.GetWorldRotation());
	}

	EDITOR_CONTEXT(renderer)->ApplyStateMask(glState);
}