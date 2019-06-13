/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#include <map>

#include <OvTools/Eventing/Event.h>

#include "OvUI/Widgets/DataWidget.h"

namespace OvUI::Widgets::Selection
{
	/**
	* Widget that can display a list of values that the user can select
	*/
	class API_OVUI ComboBox : public DataWidget<int>
	{
	public:
		/**
		* Constructor
		* @param p_currentChoice
		*/
		ComboBox(int p_currentChoice = 0);

	protected:
		void _Draw_Impl() override;

	public:
		std::map<int, std::string> choices;
		int currentChoice;

	public:
		OvTools::Eventing::Event<int> ValueChangedEvent;
	};
}