/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#include <OvTools/Eventing/Event.h>

#include "OvUI/Widgets/DataWidget.h"
#include "OvUI/Types/Color.h"

namespace OvUI::Widgets::Selection
{
	/**
	* Widget that can open a color picker on click
	*/
	class API_OVUI ColorEdit : public DataWidget<Types::Color>
	{
	public:
		/**
		* Constructor
		* @param p_enableAlpha
		* @param p_defaultColor
		*/
		ColorEdit(bool p_enableAlpha = false, const Types::Color& p_defaultColor = {});

	protected:
		void _Draw_Impl() override;

	public:
		bool enableAlpha;
		Types::Color color;
		OvTools::Eventing::Event<Types::Color&> ColorChangedEvent;
	};
}