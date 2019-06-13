/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#include "OvEditor/Core/ProjectHub.h"
#include "OvEditor/Core/Application.h"

#ifdef _DEBUG
	int main()
#else
	#undef APIENTRY
	#include "Windows.h"
	INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
#endif
{
	OvEditor::Core::ProjectHub* hub = new OvEditor::Core::ProjectHub();
	auto[ready, projectPath, projectName] = hub->Run();
	delete hub;

	if (ready)
	{
		OvEditor::Core::Application app(projectPath, projectName);
		app.Run();
	}

	return EXIT_SUCCESS;
}