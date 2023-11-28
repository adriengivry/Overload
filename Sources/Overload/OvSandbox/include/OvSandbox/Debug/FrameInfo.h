/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#ifdef _DEBUG

#include <OvWindowing/Window.h>

#include <OvRendering/Data/FrameInfo.h>

#include <OvUI/Panels/PanelUndecorated.h>
#include <OvUI/Widgets/Texts/TextColored.h>

namespace OvSandbox::Debug
{
	/**
	* Panel that display information about the frame
	*/
	class FrameInfo : public OvUI::Panels::PanelUndecorated
	{
	public:
		/**
		* Constructor
		* @param p_window
		*/
		FrameInfo(OvWindowing::Window& p_window);

		/**
		* Update the data
		* @parma p_frameInfo
		*/
		void Update(const OvRendering::Data::FrameInfo& p_frameInfo);

	private:
		OvWindowing::Window& m_window;

		OvUI::Widgets::Texts::TextColored* m_frameInfo[3];
	};
}

#endif