/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#include <vector>

#include "OvAnalytics/API/Export.h"

namespace OvAnalytics::Profiling
{
	/**
	* Contains profiling data about a running period
	*/
	struct API_OVANALYTICS ProfilerReport final
	{
		/**
		* Data about an action (Called method)
		*/
		struct Action final
		{
			std::string name;
			double duration;
			double percentage;
			uint64_t calls;
		};

		double				elaspedTime		= 0.0;
		uint16_t			workingThreads	= 0u;
		uint32_t			elapsedFrames	= 0u;
		std::vector<Action> actions;
	};
}