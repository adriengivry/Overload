/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <OvUI/Panels/PanelWindow.h>
#include <OvUI/Widgets/Texts/TextColored.h>

#include <OvRendering/Data/FrameInfo.h>

namespace OvEditor::Panels
{
	class FrameInfo : public OvUI::Panels::PanelWindow
	{
	public:
		/**
		* Constructor
		* @param p_title
		* @param p_opened
		* @param p_windowSettings
		*/
		FrameInfo
		(
			const std::string& p_title,
			bool p_opened,
			const OvUI::Settings::PanelWindowSettings& p_windowSettings
		);

		/**
		* Update frame info information
		* @param p_frameInfo
		*/
		void Update(const OvRendering::Data::FrameInfo& p_frameInfo);

	private:
		OvUI::Widgets::Texts::TextColored* m_batchCountText;
		OvUI::Widgets::Texts::TextColored* m_instanceCountText;
		OvUI::Widgets::Texts::TextColored* m_polyCountText;
		OvUI::Widgets::Texts::TextColored* m_vertexCountText;
	};
}