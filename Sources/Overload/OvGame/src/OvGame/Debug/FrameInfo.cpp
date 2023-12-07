/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#ifdef _DEBUG

#include <OvTools/Utils/Format.h>

#include "OvGame/Debug/FrameInfo.h"

OvGame::Debug::FrameInfo::FrameInfo(OvWindowing::Window& p_window) :
	m_window(p_window),
	m_batchText(CreateWidget<OvUI::Widgets::Texts::TextColored>("", OvUI::Types::Color::Yellow)),
	m_instanceText(CreateWidget<OvUI::Widgets::Texts::TextColored>("", OvUI::Types::Color::Yellow)),
	m_polyText(CreateWidget<OvUI::Widgets::Texts::TextColored>("", OvUI::Types::Color::Yellow)),
	m_vertexText(CreateWidget<OvUI::Widgets::Texts::TextColored>("", OvUI::Types::Color::Yellow))
{
	m_defaultHorizontalAlignment = OvUI::Settings::EHorizontalAlignment::LEFT;
	m_defaultVerticalAlignment = OvUI::Settings::EVerticalAlignment::BOTTOM;
	m_defaultPosition.x = static_cast<float>(p_window.GetSize().first) - 10.f;
	m_defaultPosition.y = static_cast<float>(p_window.GetSize().second) - 10.f;
}

void OvGame::Debug::FrameInfo::Update(const OvRendering::Data::FrameInfo& p_frameInfo)
{
	m_batchText.content = "Batches: " + OvTools::Utils::Format::ReadableNumber(p_frameInfo.batchCount);
	m_instanceText.content = "Instances: " + OvTools::Utils::Format::ReadableNumber(p_frameInfo.instanceCount);
	m_polyText.content = "Polygons: " + OvTools::Utils::Format::ReadableNumber(p_frameInfo.polyCount);
	m_vertexText.content = "Vertices: " + OvTools::Utils::Format::ReadableNumber(p_frameInfo.vertexCount);

	SetPosition({ 10.0f , static_cast<float>(m_window.GetSize().second) - 10.f });
	SetAlignment(OvUI::Settings::EHorizontalAlignment::LEFT, OvUI::Settings::EVerticalAlignment::BOTTOM);
}

#endif