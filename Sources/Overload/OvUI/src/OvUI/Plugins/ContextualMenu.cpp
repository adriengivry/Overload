/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#include "OvUI/Plugins/ContextualMenu.h"

void OvUI::Plugins::ContextualMenu::Execute()
{
	if (ImGui::BeginPopupContextItem())
	{
		DrawWidgets();
		ImGui::EndPopup();
	}
}

void OvUI::Plugins::ContextualMenu::Close()
{
	ImGui::CloseCurrentPopup();
}
