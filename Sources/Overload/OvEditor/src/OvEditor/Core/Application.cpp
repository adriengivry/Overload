/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#include <OvTools/Time/Clock.h>

#include "OvEditor/Core/Application.h"

OvEditor::Core::Application::Application(const std::string& p_projectPath, const std::string& p_projectName) :
	m_context(p_projectPath, p_projectName),
	m_editor(m_context)
{

}

OvEditor::Core::Application::~Application()
{
}

void OvEditor::Core::Application::Run()
{
	OvTools::Time::Clock clock;

	while (IsRunning())
	{
		m_editor.PreUpdate();
		m_editor.Update(clock.GetDeltaTime());
		m_editor.PostUpdate();

		clock.Update();
	}
}

bool OvEditor::Core::Application::IsRunning() const
{
	return !m_context.window->ShouldClose();
}
