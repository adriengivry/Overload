/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#include "OvUI/API/Export.h"

namespace OvUI::Plugins
{
	/**
	* Interface to any plugin of OvUI.
	* A plugin is basically a behaviour that you can plug to a widget
	*/
	class API_OVUI IPlugin
	{
	public:
		/**
		* Execute the plugin behaviour
		*/
		virtual void Execute() = 0;

		/* Feel free to store any data you want here */
		void* userData = nullptr;
	};
}