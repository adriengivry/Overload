/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <unordered_map>
#include <chrono>
#include <mutex>


#include "OvAnalytics/Profiling/ProfilerReport.h"

namespace OvAnalytics::Profiling
{
	/* Forward declaration of the profiler spy structure */
	struct ProfilerSpy;

	/**
	* The profiler collect data about the running program
	*/
	class Profiler final
	{
	public:
		/**
		* Create the profiler
		*/
		Profiler();

		/**
		* Generate a report containing data about the last profiling session
		*/
		ProfilerReport GenerateReport();

		/**
		* Clear any collected data
		*/
		void ClearHistory();

		/**
		* Update the profiler
		* @param p_deltaTime
		*/
		void Update(float p_deltaTime);

		/**
		* Save the given spy collected data to the profiler history
		* @param p_spy (Spy to collect data from)
		*/
		static void Save(ProfilerSpy& p_spy);

		/**
		* Verify if the profiler is currently enabled
		*/
		static bool IsEnabled();

		/**
		* Enable the profiler if it is currently disabled, and vice-versa
		*/
		static void ToggleEnable();

		/**
		* Enable the profiler
		*/
		static void Enable();

		/**
		* Disable the profiler
		*/
		static void Disable();

	private:
		/* Time relatives */
		std::chrono::steady_clock::time_point m_lastTime;

		/* Profiler settings */
		static bool __ENABLED;

		/* Collected data */
		static std::mutex									__SAVE_MUTEX;
		static std::unordered_map<std::string, double>		__ELPASED_HISTORY;
		static std::unordered_map<std::string, uint64_t>	__CALLS_COUNTER;
		static std::vector<std::thread::id>					__WORKING_THREADS;
		static uint32_t										__ELAPSED_FRAMES;
	};
}