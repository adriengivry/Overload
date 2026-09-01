/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <OvCore/ECS/Actor.h>
#include <OvCore/ECS/Components/UI/UIInspectorUtils.h>
#include <OvCore/ECS/Components/UI/UITransformResolver.h>
#include <OvUI/Widgets/Texts/TextColored.h>

void OvCore::ECS::Components::UI::UIInspectorUtils::DrawCanvasRequirement(
	OvUI::Internal::WidgetContainer& p_root,
	const ECS::Actor& p_owner
)
{
	if (OvCore::ECS::Components::UI::UITransformResolver::HasActiveUIData(p_owner))
	{
		return;
	}

	p_root.CreateWidget<OvUI::Widgets::Texts::TextColored>(
		"Requires a Canvas component on one of its parents to be rendered",
		OVUI_STYLE(Warning)
	);
}
