/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvAnalytics/Profiling/ProfilerSpy.h"
#include "OvAnalytics/Profiling/Profiler.h"

OvAnalytics::Profiling::ProfilerSpy::ProfilerSpy(const std::string & p_name) :
	name(p_name),
	start(std::chrono::steady_clock::now())
{

}

OvAnalytics::Profiling::ProfilerSpy::~ProfilerSpy()
{
	end = std::chrono::steady_clock::now();
	OvAnalytics::Profiling::Profiler::Save(*this);
}