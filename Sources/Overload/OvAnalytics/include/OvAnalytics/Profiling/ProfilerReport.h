/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <vector>


namespace OvAnalytics::Profiling
{
	/**
	* Contains profiling data about a running period
	*/
	struct ProfilerReport final
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