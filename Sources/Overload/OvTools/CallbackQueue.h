#pragma once

#include <queue>
#include <functional>

namespace OvTools::Utils
{
	class CallbackQueue : public std::queue<std::function<void()>>
	{
	public:
		void Process()
		{
			while (!empty())
			{
				auto& callback = front();
				callback();
				pop();
			}
		}
	};
}
