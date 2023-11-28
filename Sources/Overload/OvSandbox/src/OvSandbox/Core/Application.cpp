/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <OvTools/Time/Clock.h>

#include "OvSandbox/Core/Application.h"

OvSandbox::Core::Application::Application() :
	m_game(m_context)
{

}

OvSandbox::Core::Application::~Application()
{
}

void OvSandbox::Core::Application::Run()
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

bool OvSandbox::Core::Application::IsRunning() const
{
	return !m_context.window->ShouldClose();
}
