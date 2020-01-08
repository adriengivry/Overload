/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <OvAnalytics/Profiling/Profiler.h>

#include <OvUI/Panels/PanelWindow.h>
#include <OvUI/Widgets/Texts/TextColored.h>
#include <OvUI/Widgets/Selection/CheckBox.h>
#include <OvUI/Widgets/Layout/Group.h>
#include <OvUI/Widgets/Layout/Columns.h>
#include <OvUI/Widgets/Buttons/Button.h>

namespace OvEditor::Panels
{
	class Profiler : public OvUI::Panels::PanelWindow
	{
	public:
		/**
		* Constructor
		* @param p_title
		* @param p_opened
		* @param p_windowSettings
		* @param p_frequency
		*/
		Profiler
		(
			const std::string& p_title,
			bool p_opened,
			const OvUI::Settings::PanelWindowSettings& p_windowSettings,
			float p_frequency
		);

		/**
		* Update profiling information
		* @param p_deltaTime
		*/
		void Update(float p_deltaTime);

		/**
		* Enable or disable the profiler
		* @param p_value
		* @param p_disableLog
		*/
		void Enable(bool p_value, bool p_disableLog = false);

	private:
		OvUI::Types::Color CalculateActionColor(double p_percentage) const;
		std::string GenerateActionString(OvAnalytics::Profiling::ProfilerReport::Action& p_action);

	private:
		enum class EProfilingMode
		{
			DEFAULT,
			CAPTURE
		};

		float m_frequency;
		float m_timer = 0.f;
		float m_fpsTimer = 0.f;
		EProfilingMode m_profilingMode = EProfilingMode::DEFAULT;

		OvAnalytics::Profiling::Profiler m_profiler;

		OvUI::Widgets::AWidget* m_separator;
		OvUI::Widgets::Buttons::Button* m_captureResumeButton;
		OvUI::Widgets::Texts::TextColored* m_fpsText;
		OvUI::Widgets::Texts::TextColored* m_elapsedFramesText;
		OvUI::Widgets::Texts::TextColored* m_elapsedTimeText;
		OvUI::Widgets::Layout::Columns<5>* m_actionList;
	};
}