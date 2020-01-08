/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvEditor/Panels/HardwareInfo.h"

#include <OvAnalytics/Hardware/HardwareInfo.h>

using namespace OvUI::Panels;
using namespace OvUI::Widgets;

OvEditor::Panels::HardwareInfo::HardwareInfo
(
	const std::string& p_title,
	bool p_opened,
	const OvUI::Settings::PanelWindowSettings& p_windowSettings,
	float p_logFrequency,
	size_t p_maxElements
) :
	PanelWindow(p_title, p_opened, p_windowSettings),
	m_logFrequency(p_logFrequency),
	m_maxElements(p_maxElements),
	m_hardwareInfo(new OvAnalytics::Hardware::HardwareInfo(m_logFrequency))
{
	m_cpuUsage = &CreateWidget<Plots::PlotLines>();
	m_gpuUsage = &CreateWidget<Plots::PlotLines>();
	m_ramUsage = &CreateWidget<Plots::PlotLines>();
	
	m_cpuUsage->minScale = 0.0f;
	m_cpuUsage->maxScale = 100.0f;
	m_cpuUsage->size.y = 75.0f;
	m_cpuUsage->data.resize(m_maxElements, 0);
	m_cpuUsage->overlay = "CPU Usage (%)";

	m_gpuUsage->minScale = 0.0f;
	m_gpuUsage->maxScale = 100.0f;
	m_gpuUsage->size.y = 75.0f;
	m_gpuUsage->data.resize(m_maxElements, 0);
	m_gpuUsage->overlay = "GPU Usage (%)";

	m_ramUsage->minScale = 0.0f;
	m_ramUsage->maxScale = 100.0f;
	m_ramUsage->size.y = 75.0f;
	m_ramUsage->data.resize(m_maxElements, 0);
	m_ramUsage->overlay = "RAM Usage (%)";
}

OvEditor::Panels::HardwareInfo::~HardwareInfo()
{
	delete m_hardwareInfo;
}

void OvEditor::Panels::HardwareInfo::Update(float p_deltaTime)
{
	m_hardwareInfo->Tick();

	p_updateTimer += p_deltaTime;

	while (p_updateTimer >= m_logFrequency)
	{
		OvAnalytics::Hardware::HardwareReport report = m_hardwareInfo->GenerateReport();

		m_cpuUsage->data.push_back(report.CPULoad);
		m_gpuUsage->data.push_back(report.GPULoad);
		m_ramUsage->data.push_back((report.RAMUsed / report.RAMMax) * 100.0f);

		if (m_cpuUsage->data.size() > m_maxElements)
			m_cpuUsage->data.erase(m_cpuUsage->data.begin());

		if (m_gpuUsage->data.size() > m_maxElements)
			m_gpuUsage->data.erase(m_gpuUsage->data.begin());

		if (m_ramUsage->data.size() > m_maxElements)
			m_ramUsage->data.erase(m_ramUsage->data.begin());

		p_updateTimer -= m_logFrequency;
	}
}
