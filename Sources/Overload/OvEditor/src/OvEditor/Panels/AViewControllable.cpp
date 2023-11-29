/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvEditor/Panels/AViewControllable.h"

OvEditor::Panels::AViewControllable::AViewControllable
(
	const std::string& p_title,
	bool p_opened,
	const OvUI::Settings::PanelWindowSettings& p_windowSettings,
	bool p_enableFocusInputs
) : AView(p_title, p_opened, p_windowSettings), m_cameraController(*this, m_camera, p_enableFocusInputs)
{
	ResetCameraTransform();
}

void OvEditor::Panels::AViewControllable::Update(float p_deltaTime)
{
	m_cameraController.HandleInputs(p_deltaTime);
	AView::Update(p_deltaTime);
}

void OvEditor::Panels::AViewControllable::ResetCameraTransform()
{
	auto& transform = m_camera.GetTransform();
	transform.SetWorldPosition({ -10.0f, 3.0f, 10.0f });
	transform.SetWorldScale({ 0.0f, 135.0f, 0.0f });
}

OvEditor::Core::CameraController& OvEditor::Panels::AViewControllable::GetCameraController()
{
	return m_cameraController;
}

OvRendering::Entities::Camera* OvEditor::Panels::AViewControllable::GetCamera()
{
	return &m_camera;
}
