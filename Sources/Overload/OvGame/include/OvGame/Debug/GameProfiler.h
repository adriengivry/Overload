/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#ifdef _DEBUG

#include <OvRendering/Core/Renderer.h>
#include <OvWindowing/Window.h>

#include <OvAnalytics/Profiling/Profiler.h>
#include <OvUI/Panels/PanelUndecorated.h>
#include <OvUI/Widgets/Texts/TextColored.h>
#include <OvUI/Widgets/Layout/Group.h>
#include <OvUI/Widgets/Buttons/Button.h>

namespace OvGame::Debug
{
	/**
	* Panel that display profiling information
	*/
	class GameProfiler : public OvUI::Panels::PanelUndecorated
	{
	public:
		/**
		* Constructor
		* @param p_window
		* @param p_frequency
		*/
		GameProfiler(OvWindowing::Window& p_window, float p_frequency);

		/**
		* Update the data
		* @param p_deltaTime
		*/
		void Update(float p_deltaTime);
	

	private:
		OvUI::Types::Color CalculateActionColor(double p_percentage) const;
		std::string GenerateActionString(OvAnalytics::Profiling::ProfilerReport::Action& p_action);

	private:

		float m_frequency;
		float m_timer = 0.f;

		OvAnalytics::Profiling::Profiler m_profiler;

		OvWindowing::Window& m_window;
		OvUI::Widgets::AWidget* m_separator;
		OvUI::Widgets::Texts::TextColored* m_elapsedFramesText;
		OvUI::Widgets::Texts::TextColored* m_elapsedTimeText;
		OvUI::Widgets::Layout::Group* m_actionList;
	};
}

#endif