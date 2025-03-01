#include "pch.h"
#include "ImguiManager.h"

ImguiManager::~ImguiManager()
{
}

void ImguiManager::Init(HWND hwnd)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    // DPI ����
    float dpi = GetDpiForWindow(hwnd);
    float dpiScale = dpi / 96.0f;

    ImGuiIO& io = ImGui::GetIO();
    io.DisplayFramebufferScale = ImVec2(dpiScale, dpiScale);

    // �ʱ� ���÷��� ũ�� ����
    RECT windowRect;
    GetClientRect(hwnd, &windowRect);
    io.DisplaySize = ImVec2(
        static_cast<float>(windowRect.right - windowRect.left),
        static_cast<float>(windowRect.bottom - windowRect.top)
    );

    ImGui_ImplWin32_Init(hwnd);
    ASSERT_CRASH(ImGui_ImplDX11_Init(DEVICE.Get(), CONTEXT.Get()));

    // ��Ʈ ����
    io.Fonts->Clear();
    ImFont* font = io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/NanumGothic.ttf", 16.0f * dpiScale, NULL, io.Fonts->GetGlyphRangesKorean());
    io.Fonts->Build();
}

void ImguiManager::Close()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void ImguiManager::Update()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImguiManager::Render()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}