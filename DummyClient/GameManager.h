#pragma once

struct GameDesc
{
	shared_ptr<class IExecute> app = nullptr;
	wstring appName = L"Chat";
	HINSTANCE hInstance = 0;
	HWND hWnd = 0;
	float width = 300;
	float height = 600;
	bool vsync = false;
	bool windowed = true;
	float clearColor[4] = { 0.5f, 0.5f, 0.5f, 0.5f };
};

class GameManager
{
	DECLARE_SINGLE(GameManager);
public:
	~GameManager();

	WPARAM Run(GameDesc& desc);

	GameDesc& GetGameDesc() { return _desc; }

private:
	ATOM MyRegisterClass();
	BOOL InitInstance(int cmdShow);

	void Update();

	static LRESULT CALLBACK WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

private:
	GameDesc _desc;
};

