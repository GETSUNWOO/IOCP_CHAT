#include "pch.h"
#include "GameManager.h"
#include "IExecute.h"
#include "ThreadManager.h"
#include "ShutDownManager.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

GameManager::~GameManager()
{
}

WPARAM GameManager::Run(GameDesc& desc)
{
	_desc = desc;
	assert(_desc.app != nullptr);

	// 1) 윈도우 창 정보 등록
	MyRegisterClass();

	// 2) 윈도우 창 생성
	if (!InitInstance(SW_SHOWNORMAL))
		return FALSE;

	GRAPHICS->Init(_desc.hWnd);
	IMGUI->Init(_desc.hWnd);
	SERVER->Init();
	MESSAGE->Init();
	_desc.app->Init();

	MSG msg = { 0 };

	while (msg.message != WM_QUIT)
	{
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
		{
			Update();
		}
	}

	GRAPHICS->Close();
	IMGUI->Close();
	MESSAGE->Close();

	ShutDownManager::GetInstance()->Shutdown();

	return msg.wParam;
}

ATOM GameManager::MyRegisterClass()
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = _desc.hInstance;
	wcex.hIcon = ::LoadIcon(NULL, IDI_WINLOGO);
	wcex.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = _desc.appName.c_str();
	wcex.hIconSm = wcex.hIcon;

	return RegisterClassExW(&wcex);
}

BOOL GameManager::InitInstance(int cmdShow)
{
	RECT windowRect = { 0, 0, _desc.width, _desc.height };
	::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);

	_desc.hWnd = CreateWindowW(_desc.appName.c_str(), _desc.appName.c_str(), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, nullptr, nullptr, _desc.hInstance, nullptr);

	if (!_desc.hWnd)
		return FALSE;

	::ShowWindow(_desc.hWnd, cmdShow);
	::UpdateWindow(_desc.hWnd);

	return TRUE;
}

void GameManager::Update()
{
	GRAPHICS->RenderBegin();
	IMGUI->Update();
	_desc.app->Update();
	_desc.app->Render();
	IMGUI->Render();
	GRAPHICS->RenderEnd();
}

LRESULT GameManager::WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(handle, message, wParam, lParam))
		return true;

	switch (message)
	{
	case WM_SIZE:
		break;
	case WM_CLOSE:
		ShutDownManager::GetInstance()->Shutdown();
		PostQuitMessage(0);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return ::DefWindowProc(handle, message, wParam, lParam);
	}

	return 0;
}
