/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvEditor/Panels/Profiler.h"

#include <OvDebug/Logger.h>
#include <OvUI/Widgets/Visual/Separator.h>

using namespace OvUI::Panels;
using namespace OvUI::Widgets;
using namespace OvUI::Types;

OvEditor::Panels::Profiler::Profiler
(
	const std::string& p_title,
	bool p_opened,
	const OvUI::Settings::PanelWindowSettings& p_windowSettings,
	float p_frequency
) :
	PanelWindow(p_title, p_opened, p_windowSettings),
	m_frequency(p_frequency)
{
	allowHorizontalScrollbar = true;

	CreateWidget<Texts::Text>("Profiler state: ").lineBreak = false;
	CreateWidget<Selection::CheckBox>(false, "").ValueChangedEvent += std::bind(&Profiler::Enable, this, std::placeholders::_1, false);

	m_fpsText = &CreateWidget<Texts::TextColored>("");
	m_captureResumeButton = &CreateWidget<Buttons::Button>("Capture");
	m_captureResumeButton->idleBackgroundColor = { 0.7f, 0.5f, 0.f };
	m_captureResumeButton->ClickedEvent += [this]
	{
		m_profilingMode					= m_profilingMode == EProfilingMode::CAPTURE ? EProfilingMode::DEFAULT : EProfilingMode::CAPTURE;
		m_captureResumeButton->label	= m_profilingMode == EProfilingMode::CAPTURE ? "Resume" : "Capture";
	};
	m_elapsedFramesText = &CreateWidget<Texts::TextColored>("", Color(1.f, 0.8f, 0.01f, 1));
	m_elapsedTimeText = &CreateWidget<Texts::TextColored>("", Color(1.f, 0.8f, 0.01f, 1));
	m_separator = &CreateWidget<OvUI::Widgets::Visual::Separator>();
	m_actionList = &CreateWidget<Layout::Columns<5>>();
	m_actionList->widths = { 300.f, 100.f, 100.f, 100.f, 200.f };

	Enable(false, true);
}

void OvEditor::Panels::Profiler::Update(float p_deltaTime)
{
	m_timer += p_deltaTime;
	m_fpsTimer += p_deltaTime;

	while (m_fpsTimer >= 0.07f)
	{
		m_fpsText->content = "FPS: " + std::to_string(static_cast<int>(1.0f / p_deltaTime));
		m_fpsTimer -= 0.07f;
	}

	if (m_profiler.IsEnabled())
	{
		m_profiler.Update(p_deltaTime);

		while (m_timer >= m_frequency)
		{
			if (m_profilingMode == EProfilingMode::DEFAULT)
			{
				OvAnalytics::Profiling::ProfilerReport report = m_profiler.GenerateReport();
				m_profiler.ClearHistory();
				m_actionList->RemoveAllWidgets();

				m_elapsedFramesText->content = "Elapsed frames: " + std::to_string(report.elapsedFrames);
				m_elapsedTimeText->content = "Elapsed time: " + std::to_string(report.elaspedTime);

				m_actionList->CreateWidget<Texts::Text>("Action");
				m_actionList->CreateWidget<Texts::Text>("Total duration");
				m_actionList->CreateWidget<Texts::Text>("Frame duration");
				m_actionList->CreateWidget<Texts::Text>("Frame load");
				m_actionList->CreateWidget<Texts::Text>("Total calls");

				for (auto& action : report.actions)
				{
					auto color = CalculateActionColor(action.percentage);
					m_actionList->CreateWidget<Texts::TextColored>(action.name, color);
					m_actionList->CreateWidget<Texts::TextColored>(std::to_string(action.duration) + "s", color);
					m_actionList->CreateWidget<Texts::TextColored>(std::to_string(action.duration / action.calls) + "s", color);
					m_actionList->CreateWidget<Texts::TextColored>(std::to_string(action.percentage) + "%%", color);
					m_actionList->CreateWidget<Texts::TextColored>(std::to_string(action.calls) + " calls", color);
				}
			}

			m_timer -= m_frequency;
		}
	}
}

void OvEditor::Panels::Profiler::Enable(bool p_value, bool p_disableLog)
{
	if (p_value)
	{
		if (!p_disableLog)
			OVLOG_INFO("Profiling started!");
		m_profiler.Enable();
	}
	else
	{
		if (!p_disableLog)
			OVLOG_INFO("Profiling stoped!");
		m_profiler.Disable();
		m_profiler.ClearHistory();
		m_actionList->RemoveAllWidgets();
	}

	m_captureResumeButton->enabled = p_value;
	m_elapsedFramesText->enabled = p_value;
	m_elapsedTimeText->enabled = p_value;
	m_separator->enabled = p_value;
}

OvUI::Types::Color OvEditor::Panels::Profiler::CalculateActionColor(double p_percentage) const
{
	if (p_percentage <= 25.0f)		return { 0.0f, 1.0f, 0.0f, 1.0f };
	else if (p_percentage <= 50.0f) return { 1.0f, 1.0f, 0.0f, 1.0f };
	else if (p_percentage <= 75.0f) return { 1.0f, 0.6f, 0.0f, 1.0f };
	else							return { 1.0f, 0.0f, 0.0f, 1.0f };
}

std::string OvEditor::Panels::Profiler::GenerateActionString(OvAnalytics::Profiling::ProfilerReport::Action & p_action)
{
	std::string result;

	result += "[" + p_action.name + "]";
	result += std::to_string(p_action.duration) + "s (total) | ";
	result += std::to_string(p_action.duration / p_action.calls) + "s (per call) | ";
	result += std::to_string(p_action.percentage) + "%% | ";
	result += std::to_string(p_action.calls) + " calls";

	return result;
}
