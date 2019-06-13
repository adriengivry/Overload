/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#include <string>

#include <OvTools/Eventing/Event.h>

#include "OvUI/Widgets/AWidget.h"

namespace OvUI::Widgets::Buttons
{
	/**
	* Base class for any button widget
	*/
	class API_OVUI AButton : public AWidget
	{
	protected:
		void _Draw_Impl() override = 0;

	public:
		OvTools::Eventing::Event<> ClickedEvent;
	};
}