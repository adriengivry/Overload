/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvUI/Widgets/Buttons/Button.h"

OvUI::Widgets::Buttons::Button::Button(const std::string& p_label, const OvMaths::FVector2& p_size, bool p_disabled) :
	label(p_label), size(p_size), disabled(p_disabled)
{
	auto& style = ImGui::GetStyle();

	idleBackgroundColor		= style.Colors[ImGuiCol_Button];
	hoveredBackgroundColor	= style.Colors[ImGuiCol_ButtonHovered];
	clickedBackgroundColor	= style.Colors[ImGuiCol_ButtonActive];
	textColor				= style.Colors[ImGuiCol_Text];
}

void OvUI::Widgets::Buttons::Button::_Draw_Impl()
{
	auto& style = ImGui::GetStyle();

	auto defaultIdleColor		= style.Colors[ImGuiCol_Button];
	auto defaultHoveredColor	= style.Colors[ImGuiCol_ButtonHovered];
	auto defaultClickedColor	= style.Colors[ImGuiCol_ButtonActive];
	auto defaultTextColor		= style.Colors[ImGuiCol_Text];

	style.Colors[ImGuiCol_Button]			= idleBackgroundColor.ToVector4();
	style.Colors[ImGuiCol_ButtonHovered]	= hoveredBackgroundColor.ToVector4();
	style.Colors[ImGuiCol_ButtonActive]		= clickedBackgroundColor.ToVector4();
	style.Colors[ImGuiCol_Text]				= textColor.ToVector4();

	if (disabled)
		ImGui::BeginDisabled();

	if (ImGui::Button((label + m_widgetID).c_str(), size))
		ClickedEvent.Invoke();

	if (disabled)
		ImGui::EndDisabled();

	style.Colors[ImGuiCol_Button]			= defaultIdleColor;
	style.Colors[ImGuiCol_ButtonHovered]	= defaultHoveredColor;
	style.Colors[ImGuiCol_ButtonActive]		= defaultClickedColor;
	style.Colors[ImGuiCol_Text]				= defaultTextColor;
}
