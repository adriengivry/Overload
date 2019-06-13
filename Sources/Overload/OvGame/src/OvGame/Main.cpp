/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#include "OvGame/Core/Application.h"

#ifdef _DEBUG
int main()
#else
#undef APIENTRY
#include "Windows.h"
INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
#endif
{
	OvGame::Core::Application app;
	app.Run();

	return EXIT_SUCCESS;
}