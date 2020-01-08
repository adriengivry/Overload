/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#ifdef _DEBUG

#include "OvGame/Debug/FrameInfo.h"

OvGame::Debug::FrameInfo::FrameInfo(OvRendering::Core::Renderer& p_renderer, OvWindowing::Window& p_window) :
	m_renderer(p_renderer),
	m_window(p_window)
{
	m_defaultHorizontalAlignment = OvUI::Settings::EHorizontalAlignment::LEFT;
	m_defaultVerticalAlignment = OvUI::Settings::EVerticalAlignment::BOTTOM;
	m_defaultPosition.x = static_cast<float>(p_window.GetSize().first) - 10.f;
	m_defaultPosition.y = static_cast<float>(p_window.GetSize().second) - 10.f;

	m_frameInfo[0] = &CreateWidget<OvUI::Widgets::Texts::TextColored>("", OvUI::Types::Color::Yellow);
	m_frameInfo[1] = &CreateWidget<OvUI::Widgets::Texts::TextColored>("", OvUI::Types::Color::Yellow);
	m_frameInfo[2] = &CreateWidget<OvUI::Widgets::Texts::TextColored>("", OvUI::Types::Color::Yellow);
}

void OvGame::Debug::FrameInfo::Update(float p_deltaTime)
{
	auto& frameInfo = m_renderer.GetFrameInfo();

	m_frameInfo[0]->content = "Triangles: " + std::to_string(frameInfo.polyCount);
	m_frameInfo[1]->content = "Batches: " + std::to_string(frameInfo.batchCount);
	m_frameInfo[2]->content = "Instances: " + std::to_string(frameInfo.instanceCount);

	SetPosition({ 10.0f , static_cast<float>(m_window.GetSize().second) - 10.f });
	SetAlignment(OvUI::Settings::EHorizontalAlignment::LEFT, OvUI::Settings::EVerticalAlignment::BOTTOM);
}

#endif