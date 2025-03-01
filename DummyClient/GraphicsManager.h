#pragma once
class GraphicsManager
{
	DECLARE_SINGLE(GraphicsManager);

public:
	void Init(HWND hwnd);
	void Close();
	~GraphicsManager();

	void RenderBegin();
	void RenderEnd();

	Microsoft::WRL::ComPtr<ID3D11Device> GetDevice() { return _device; }
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetDeviceContext() { return _deviceContext; }

private:
	void CreateDeviceAndSwapChain();
	void CreateRenderTargetView();

public:
	void SetViewport();

private:
	HWND _hwnd = {};

	// Device & SwapChain
	Microsoft::WRL::ComPtr<ID3D11Device> _device = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> _deviceContext = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain> _swapChain = nullptr;

	// RTV
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _renderTargetView;

	// Misc
	D3D11_VIEWPORT _viewport = { 0 };
	float _clearColor[4] = { 0.5f, 0.5f, 0.5f, 0.5f };
};

