/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#ifdef _DEBUG

#include <OvRendering/Core/Renderer.h>
#include <OvWindowing/Window.h>

#include <OvUI/Panels/PanelUndecorated.h>
#include <OvUI/Widgets/Texts/TextColored.h>

namespace OvGame::Debug
{
	/**
	* Panel that display information about the driver (hardware and software)
	*/
	class DriverInfo : public OvUI::Panels::PanelUndecorated
	{
	public:
		/**
		* Constructor
		* @param p_renderer
		* @param p_window
		*/
		DriverInfo(OvRendering::Core::Renderer& p_renderer, OvWindowing::Window& p_window);
	};
}

#endif