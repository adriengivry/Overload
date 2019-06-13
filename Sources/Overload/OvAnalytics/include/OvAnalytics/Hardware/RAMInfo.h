/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#include <Windows.h>
#include <psapi.h>

namespace OvAnalytics::Hardware
{
	/**
	* The GPUInfo class will gather informations about the GPU usage
	*/
	class RAMInfo final
	{
	public:
		/**
		* Update the RAMInfo
		*/
		void Update();

		/**
		* Return the total physical memory reserved (MB)
		*/
		float GetUsedRAM();

		/**
		* Return the total physical memory free (MB)
		*/
		float GetFreeRAM();

		/**
		* Return the maximum available memory on the system (MB)
		*/
		float GetMaxRAM();

	private:
		MEMORYSTATUSEX m_statex;
	};
}