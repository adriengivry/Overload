/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#include <OvUI/Internal/WidgetContainer.h>

#include "OvCore/API/ISerializable.h"
#include "OvCore/Helpers/GUIDrawer.h"

namespace OvCore::API
{
	/**
	* IInspectorItem is an interface to any class that is serializable and drawable on inspector
	*/
	class API_OVCORE IInspectorItem : public ISerializable
	{
	public:
		/**
		* Called when the inspector needs to create the UI elements for the given IInspectorItem
		* @param p_root
		*/
		virtual void OnInspector(OvUI::Internal::WidgetContainer& p_root) = 0;
	};
}