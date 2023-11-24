/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvAnalytics/Hardware/RAMInfo.h"

#include <Windows.h>
#include <psapi.h>

void OvAnalytics::Hardware::RAMInfo::Update()
{
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);

	m_maxRAM = statex.ullTotalPhys / 1048576.0f;
	m_freeRAM = statex.ullAvailPhys / 1048576.0f;
	m_usedRAM = m_maxRAM - statex.ullAvailPhys / 1048576.0f;
}

float OvAnalytics::Hardware::RAMInfo::GetUsedRAM()
{
	return m_usedRAM;
}

float OvAnalytics::Hardware::RAMInfo::GetFreeRAM()
{
	return m_freeRAM;
}

float OvAnalytics::Hardware::RAMInfo::GetMaxRAM()
{
	return m_maxRAM;
}