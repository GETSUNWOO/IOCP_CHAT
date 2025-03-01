#pragma once
class ImguiManager
{
	DECLARE_SINGLE(ImguiManager);

public:

	~ImguiManager();

public:
	void Init(HWND hwnd);
	void Close();

public:
	void Update();
	void Render();
};

