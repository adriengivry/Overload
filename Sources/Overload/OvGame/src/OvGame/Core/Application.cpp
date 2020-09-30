/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <OvTools/Time/Clock.h>

#include "OvGame/Core/Application.h"

OvGame::Core::Application::Application() :
	m_game(m_context)
{

}

OvGame::Core::Application::~Application()
{
}

void OvGame::Core::Application::Run()
{
	OvTools::Time::Clock clock;

	while (IsRunning())
	{
		m_game.PreUpdate();
		m_game.Update(clock.GetDeltaTime());
		m_game.PostUpdate();

		clock.Update();
	}
}

bool OvGame::Core::Application::IsRunning() const
{
	return !m_context.window->ShouldClose();
}
