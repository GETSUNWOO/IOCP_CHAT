#pragma once

#define WIN32_LEAN_AND_MEAN // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

#ifdef _DEBUG
#pragma comment(lib, "ServerCore\\Debug\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Debug\\libprotobufd.lib")
#else
#pragma comment(lib, "ServerCore\\Release\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Release\\libprotobuf.lib")
#endif

//Header
#include "CorePch.h"
#include "Define.h"
#include "Types.h"

// WIN
#include <assert.h>
#include <optional>

//imgui
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

//dx
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <directXMath.h>
using namespace DirectX;
using namespace Microsoft::WRL;

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

//Managers
#include "GameManager.h"
#include "GraphicsManager.h"
#include "ImguiManager.h"
#include "ServerManager.h"
#include "MessageManager.h"
#include "IExecute.h"

using namespace Client;