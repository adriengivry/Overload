/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#include "OvUI/Plugins/IPlugin.h"
#include "OvUI/Internal/WidgetContainer.h"
#include "OvUI/Widgets/Menu/MenuList.h"
#include "OvUI/Widgets/Menu/MenuItem.h"

namespace OvUI::Plugins
{
	/**
	* A simple plugin that will show a contextual menu on right click
	* You can add widgets to a contextual menu
	*/
	class API_OVUI ContextualMenu : public IPlugin, public Internal::WidgetContainer
	{
	public:
		/**
		* Execute the plugin
		*/
		void Execute();

		/**
		* Force close the contextual menu
		*/
		void Close();
	};
}
