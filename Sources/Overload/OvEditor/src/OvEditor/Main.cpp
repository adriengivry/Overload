/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#include <filesystem>

#include <OvTools/Utils/String.h>

#include "OvEditor/Core/ProjectHub.h"
#include "OvEditor/Core/Application.h"

#undef APIENTRY
#include "Windows.h"

/**
* When Overload is launched from a project file, we should consider the executable path as
* the current working directory
* @param p_executablePath
*/
void UpdateWorkingDirectory(const std::string& p_executablePath)
{
	if (!IsDebuggerPresent())
	{
		std::filesystem::current_path(OvTools::Utils::PathParser::GetContainingFolder(p_executablePath));
	}
}

int main(int argc, char** argv);

#ifndef _DEBUG
INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
	main(__argc, __argv);
}
#endif

int main(int argc, char** argv)
{
	UpdateWorkingDirectory(argv[0]);

	bool ready = false;
	std::string projectPath;
	std::string projectName;

	{
		OvEditor::Core::ProjectHub hub;

		if (argc < 2)
		{
			// No project file given as argument ==> Open the ProjectHub
			std::tie(ready, projectPath, projectName) = hub.Run();
		}
		else
		{
			// Project file given as argument ==> Open the project
			std::string projectFile = argv[1];

			if (OvTools::Utils::PathParser::GetExtension(projectFile) == "ovproject")
			{
				ready = true;
				projectPath = OvTools::Utils::PathParser::GetContainingFolder(projectFile);
				projectName = OvTools::Utils::PathParser::GetElementName(projectFile);
				OvTools::Utils::String::Replace(projectName, ".ovproject", "");
			}
		}
	}

	if (ready)
	{
		OvEditor::Core::Application app(projectPath, projectName);
		app.Run();
	}

	return EXIT_SUCCESS;
}