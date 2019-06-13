/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#include <algorithm>

#include "OvUI/Panels/APanel.h"

uint64_t OvUI::Panels::APanel::__PANEL_ID_INCREMENT = 0;

OvUI::Panels::APanel::APanel()
{
	m_panelID = "##" + std::to_string(__PANEL_ID_INCREMENT++);
}

void OvUI::Panels::APanel::Draw()
{
	if (enabled)
		_Draw_Impl();
}

const std::string & OvUI::Panels::APanel::GetPanelID() const
{
	return m_panelID;
}
