/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#include <string>
#include <queue>

#include "ILogHandler.h"

namespace OvDebug
{
	/*
	* Treat the log in a history queue
	*/
	class API_OVDEBUG HistoryHandler : public ILogHandler
	{
	public:

		/**
		* Log to the history
		*/
		void Log(const LogData& p_logData);

		/**
		* Return log queue
		*/
		static std::queue<LogData>& GetLogQueue();

	private:

		static std::queue<LogData> LOG_QUEUE;
	};
}