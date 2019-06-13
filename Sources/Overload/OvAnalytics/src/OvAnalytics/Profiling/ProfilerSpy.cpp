/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
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