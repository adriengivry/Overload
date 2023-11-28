/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <OvRendering/Utils/Defines.h>

#include "OvSandbox/Core/Application.h"

#include <filesystem>

#include <OvTools/Utils/String.h>
#include <OvTools/Utils/PathParser.h>

#include <OvRendering/Utils/Defines.h>

#undef APIENTRY
#include "Windows.h"

FORCE_DEDICATED_GPU

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

	OvSandbox::Core::Application app;
	app.Run();

	return EXIT_SUCCESS;
}