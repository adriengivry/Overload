/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvEditor/Panels/FrameInfo.h"

#include <OvDebug/Logger.h>
#include <OvUI/Widgets/Visual/Separator.h>

#include "OvEditor/Core/EditorActions.h"
#include "OvTools/Utils/Format.h"

using namespace OvUI::Panels;
using namespace OvUI::Widgets;
using namespace OvUI::Types;

OvEditor::Panels::FrameInfo::FrameInfo
(
	const std::string& p_title,
	bool p_opened,
	const OvUI::Settings::PanelWindowSettings& p_windowSettings
) : PanelWindow(p_title, p_opened, p_windowSettings)
{
	m_batchCountText = &CreateWidget<Texts::TextColored>("");
	m_instanceCountText = &CreateWidget<Texts::TextColored>("");
	m_polyCountText = &CreateWidget<Texts::TextColored>("");
	m_polyCountText->lineBreak = false;
	m_vertexCountText = &CreateWidget<Texts::TextColored>("");
}

void OvEditor::Panels::FrameInfo::Update(const OvRendering::Data::FrameInfo& p_frameInfo)
{
	m_batchCountText->content = "Batches: " + OvTools::Utils::Format::ReadableNumber(p_frameInfo.batchCount);
	m_instanceCountText->content = "Instances: " + OvTools::Utils::Format::ReadableNumber(p_frameInfo.instanceCount);
	m_polyCountText->content = "Polygons: " + OvTools::Utils::Format::ReadableNumber(p_frameInfo.polyCount);
	m_vertexCountText->content = "Vertices: " + OvTools::Utils::Format::ReadableNumber(p_frameInfo.vertexCount);
}
