/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#include "OvUI/API/Export.h"

namespace OvUI::Settings
{
	/**
	* Data structure to send to the panel window constructor to define its settings
	*/
	struct API_OVUI PanelWindowSettings
	{
		bool closable					= false;
		bool resizable					= true;
		bool movable					= true;
		bool dockable					= false;
		bool hideBackground				= false;
		bool forceHorizontalScrollbar	= false;
		bool forceVerticalScrollbar		= false;
		bool allowHorizontalScrollbar	= false;
		bool bringToFrontOnFocus		= true;
		bool collapsable				= false;
		bool allowInputs				= true;
		bool autoSize					= false;
	};
}