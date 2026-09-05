/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <OvCore/ECS/Actor.h>
#include <OvCore/ECS/Components/UI/UIInspectorUtils.h>
#include <OvCore/ECS/Components/UI/UITransformResolver.h>
#include <OvUI/Widgets/Texts/TextColored.h>
#include <OvUI/Widgets/Texts/TextWrapped.h>

void OvCore::ECS::Components::UI::UIInspectorUtils::DrawCanvasRequirement(
	OvUI::Internal::WidgetContainer& p_root,
	const ECS::Actor& p_owner
)
{
	if (OvCore::ECS::Components::UI::UITransformResolver::HasActiveUIData(p_owner))
	{
		return;
	}

	// The inspector lays components out in a two column grid, so both cells of the row must be filled
	p_root.CreateWidget<OvUI::Widgets::Texts::TextColored>("Canvas", OVUI_STYLE(Warning));
	p_root.CreateWidget<OvUI::Widgets::Texts::TextWrapped>("Required on a parent actor to render this component");
}
