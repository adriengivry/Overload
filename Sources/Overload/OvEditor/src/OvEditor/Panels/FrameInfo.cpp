/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvEditor/Panels/FrameInfo.h"

#include <OvDebug/Logger.h>
#include <OvUI/Widgets/Visual/Separator.h>

#include "OvEditor/Core/EditorActions.h"

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
}

void OvEditor::Panels::FrameInfo::Update(float p_deltaTime)
{
	m_batchCountText->content = "Batches: " + std::to_string(EDITOR_CONTEXT(renderer)->GetFrameInfo().batchCount);
	m_instanceCountText->content = "Instances: " + std::to_string(EDITOR_CONTEXT(renderer)->GetFrameInfo().instanceCount);
	m_polyCountText->content = "Polygons: " + std::to_string(EDITOR_CONTEXT(renderer)->GetFrameInfo().polyCount);
}
