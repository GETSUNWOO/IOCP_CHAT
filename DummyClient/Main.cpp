#include "pch.h"
#include "Main.h"
#include "Chat.h"
#include "Test.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	GameDesc desc;
	desc.appName = L"Chat";
	desc.hInstance = hInstance;
	desc.vsync = false;
	desc.hWnd = NULL;
	desc.width = GWinSizeX;
	desc.height = GWinSizeY;
	desc.app = MakeShared<Chat>();

	GAME->Run(desc);

	return 0;
}