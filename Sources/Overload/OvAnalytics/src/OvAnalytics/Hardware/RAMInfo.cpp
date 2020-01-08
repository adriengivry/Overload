/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvAnalytics/Hardware/RAMInfo.h"

void OvAnalytics::Hardware::RAMInfo::Update()
{
	m_statex.dwLength = sizeof(m_statex);
	GlobalMemoryStatusEx(&m_statex);
}

float OvAnalytics::Hardware::RAMInfo::GetUsedRAM()
{
	return GetMaxRAM() - m_statex.ullAvailPhys / 1048576.0f;
}

float OvAnalytics::Hardware::RAMInfo::GetFreeRAM()
{
	return m_statex.ullAvailPhys / 1048576.0f;
}

float OvAnalytics::Hardware::RAMInfo::GetMaxRAM()
{
	return m_statex.ullTotalPhys / 1048576.0f;
}