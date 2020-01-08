/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvAnalytics/Hardware/GPUInfo.h"

OvAnalytics::Hardware::GPUInfo::GPUInfo()
{
	#if _WIN32 || _WIN64
		#if _WIN64
			#define ENVIRONMENT64
		#else
			#define ENVIRONMENT32
		#endif
	#endif

	#ifdef ENVIRONMENT64
		HMODULE hmod = LoadLibraryA("nvapi64.dll");
	#elif ENVIRONMENT32
		HMODULE hmod = LoadLibraryA("nvapi.dll");
	#else
		HMODULE hmod = nullptr;
	#endif

	/* Handle "Couldn't find nvapi.dll" */
	if (hmod)
	{
		m_NvAPI_QueryInterface = (NvAPI_QueryInterface_t)GetProcAddress(hmod, "nvapi_QueryInterface");
		m_NvAPI_Initialize = (NvAPI_Initialize_t)(*m_NvAPI_QueryInterface)(0x0150E828);
		m_NvAPI_EnumPhysicalGPUs = (NvAPI_EnumPhysicalGPUs_t)(*m_NvAPI_QueryInterface)(0xE5AC921F);
		m_NvAPI_GPU_GetUsages = (NvAPI_GPU_GetUsages_t)(*m_NvAPI_QueryInterface)(0x189A1FDF);

		if (m_NvAPI_Initialize && m_NvAPI_EnumPhysicalGPUs && m_NvAPI_EnumPhysicalGPUs && m_NvAPI_GPU_GetUsages)
		{
			(*m_NvAPI_Initialize)();

			m_gpuUsages[0] = (136) | 0x10000;

			(*m_NvAPI_EnumPhysicalGPUs)(m_gpuHandles, &m_gpuCount);

			m_NvAPIReady = true;
		}
	}
}

float OvAnalytics::Hardware::GPUInfo::CalculateGPULoad()
{
	if (!m_NvAPIReady)
		return -100.0f;

	(*m_NvAPI_GPU_GetUsages)(m_gpuHandles[0], m_gpuUsages);
	return static_cast<float>(m_gpuUsages[3]);
}
