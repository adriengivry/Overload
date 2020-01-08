/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvEditor/Panels/HelpWindow.h"

#include <OvUI/Widgets/Layout/GroupCollapsable.h>
#include <OvUI/Widgets/Texts/Text.h>
#include <OvUI/Widgets/Visual/Bullet.h>
#include <OvUI/Widgets/Layout/Columns.h>

OvEditor::Panels::HelpWindow::HelpWindow
(
	const std::string& p_title,
	bool p_opened,
	const OvUI::Settings::PanelWindowSettings& p_windowSettings
) : PanelWindow(p_title, p_opened, p_windowSettings)
{
	auto& controls = CreateWidget<OvUI::Widgets::Layout::GroupCollapsable>("Controls");

	float columnWidth = 100;

	auto& columns = controls.CreateWidget<OvUI::Widgets::Layout::Columns<2>>();
	columns.widths[0] = 75;

	columns.CreateWidget<OvUI::Widgets::Texts::Text>("Slide:");
	columns.CreateWidget<OvUI::Widgets::Texts::Text>("[MB_MIDDLE]");

	columns.CreateWidget<OvUI::Widgets::Texts::Text>("Rotate:");
	columns.CreateWidget<OvUI::Widgets::Texts::Text>("[MB_RIGHT]");

	columns.CreateWidget<OvUI::Widgets::Texts::Text>("Forward:");
	columns.CreateWidget<OvUI::Widgets::Texts::Text>("[MB_RIGHT] + [W]");

	columns.CreateWidget<OvUI::Widgets::Texts::Text>("Backward:");
	columns.CreateWidget<OvUI::Widgets::Texts::Text>("[MB_RIGHT] + [S]");

	columns.CreateWidget<OvUI::Widgets::Texts::Text>("Left:");
	columns.CreateWidget<OvUI::Widgets::Texts::Text>("[MB_RIGHT] + [A]");

	columns.CreateWidget<OvUI::Widgets::Texts::Text>("Right:");
	columns.CreateWidget<OvUI::Widgets::Texts::Text>("[MB_RIGHT] + [D]");

	columns.CreateWidget<OvUI::Widgets::Texts::Text>("Up:");
	columns.CreateWidget<OvUI::Widgets::Texts::Text>("[MB_RIGHT] + [E]");

	columns.CreateWidget<OvUI::Widgets::Texts::Text>("Down:");
	columns.CreateWidget<OvUI::Widgets::Texts::Text>("[MB_RIGHT] + [Q]");
}