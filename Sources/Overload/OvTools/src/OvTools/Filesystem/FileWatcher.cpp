/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvTools/FileSystem/FileWatcher.h"

OvTools::Filesystem::FileWatcher::FileWatcher(std::string p_pathToWatch, std::chrono::duration<int, std::milli> p_frequency) : m_pathToWatch(p_pathToWatch), m_frequency(p_frequency)
{
	for (auto& file : std::filesystem::recursive_directory_iterator(p_pathToWatch))
	{
		m_paths[file.path().string()] = std::filesystem::last_write_time(file);
	}
}

OvTools::Filesystem::FileWatcher::~FileWatcher()
{
	if (m_running)
	{
		Stop();
	}
}

void OvTools::Filesystem::FileWatcher::Start()
{
	if (!m_running)
	{
		m_running = true;
		m_worker = std::make_unique<std::thread>(std::bind(&FileWatcher::ConcurrentTask, this));
	}
}

void OvTools::Filesystem::FileWatcher::Stop()
{
	m_running = false;

	if (m_worker)
	{
		m_worker->join();
	}
}

void OvTools::Filesystem::FileWatcher::ConcurrentTask()
{
	while (m_running)
	{
		std::this_thread::sleep_for(m_frequency);

		for (auto it = m_paths.begin(); it != m_paths.end();)
		{
			const auto filePath = it->first;

			if (!std::filesystem::exists(filePath))
			{
				FileDeletedEvent.Invoke(filePath);
				it = m_paths.erase(it);
			}
			else
			{
				++it;
			}
		}

		// Check if a file was created or modified
		for (auto& file : std::filesystem::recursive_directory_iterator(m_pathToWatch))
		{
			const auto current_file_last_write_time = std::filesystem::last_write_time(file);
			const auto filePath = file.path().string();

			if (!Contains(filePath))
			{
				m_paths[filePath] = current_file_last_write_time;
				FileAddedEvent.Invoke(filePath);
			}
			else
			{
				if (m_paths[filePath] != current_file_last_write_time)
				{
					m_paths[filePath] = current_file_last_write_time;
					FileChangedEvent.Invoke(filePath);
				}
			}
		}
	}
}

bool OvTools::Filesystem::FileWatcher::Contains(const std::string& p_key)
{
	auto el = m_paths.find(p_key);
	return el != m_paths.end();
}
