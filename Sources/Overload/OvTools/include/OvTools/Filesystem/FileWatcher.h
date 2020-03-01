/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <filesystem>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <string>
#include <functional>

#include "OvTools/API/Export.h"
#include "OvTools/Eventing/Event.h"

namespace OvTools::Filesystem
{
	/**
	* The FileWatcher class represents allow its user to track a given directory of any file event (Modification, rename, deletion...)
	*/
	class API_OVTOOLS FileWatcher
	{
	public:
		/**
		* Create a FileWatcher
		* @param p_pathToWatch (The directory to watch for)
		* @param p_frequency (The delay between checks. Stopping the FileWatcher won't skip the delay)
		*/
		FileWatcher(std::string p_pathToWatch, std::chrono::duration<int, std::milli> p_frequency);

		/**
		* Destructor of the FileWatcher.
		* If the FileWatcher is running, Stop() will be called.
		*/
		~FileWatcher();

		/**
		* Start watching the directory
		*/
		void Start();

		/**
		* Stop watching the directory (Delay can occur if the FileWatcher thread is waiting the given delay)
		*/
		void Stop();

	private:
		void ConcurrentTask();
		bool Contains(const std::string& p_path);

	public:
		OvTools::Eventing::Event<std::string> FileAddedEvent;
		OvTools::Eventing::Event<std::string> FileChangedEvent;
		OvTools::Eventing::Event<std::string> FileDeletedEvent;

	private:
		std::unique_ptr<std::thread> m_worker;
		std::string m_pathToWatch;
		std::chrono::duration<int, std::milli> m_frequency;

		std::unordered_map<std::string, std::filesystem::file_time_type> m_paths;
		bool m_running = false;
	};
}