/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#ifdef _DEBUG

#include "OvGame/Debug/DriverInfo.h"

OvGame::Debug::DriverInfo::DriverInfo(OvRendering::Core::Renderer& p_renderer, OvWindowing::Window& p_window)
{
	m_defaultHorizontalAlignment = OvUI::Settings::EHorizontalAlignment::RIGHT;
	m_defaultVerticalAlignment = OvUI::Settings::EVerticalAlignment::BOTTOM;
	m_defaultPosition.x = static_cast<float>(p_window.GetSize().first) - 10.f;
	m_defaultPosition.y = static_cast<float>(p_window.GetSize().second) - 10.f;

	CreateWidget<OvUI::Widgets::Texts::TextColored>("Vendor: "	+ p_renderer.GetString(GL_VENDOR), OvUI::Types::Color::Yellow);
	CreateWidget<OvUI::Widgets::Texts::TextColored>("Hardware: " + p_renderer.GetString(GL_RENDERER), OvUI::Types::Color::Yellow);
	CreateWidget<OvUI::Widgets::Texts::TextColored>("OpenGL Version: " + p_renderer.GetString(GL_VERSION), OvUI::Types::Color::Yellow);
	CreateWidget<OvUI::Widgets::Texts::TextColored>("GLSL Version: " + p_renderer.GetString(GL_SHADING_LANGUAGE_VERSION), OvUI::Types::Color::Yellow);
}

#endif
