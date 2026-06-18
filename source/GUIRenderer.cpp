#include "GUIRenderer.h"

#include <array>

#include <dxgi1_6.h>
#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_dx11.h>

#pragma comment(lib, "d3d11.lib")

bool GUIRenderer::Initialize(int width, int height, HWND hWnd)
{
	// DXGIとD3D11のデバッグレイヤーを有効化
	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	//----------------------------------------------------------
	// D3D11デバイスとスワップチェインの作成
	//----------------------------------------------------------
	constexpr std::array<D3D_FEATURE_LEVEL, 2> featureLevels
	{
	   D3D_FEATURE_LEVEL_11_1,
	   D3D_FEATURE_LEVEL_11_0,
	};

	// スワップチェインの設定
	DXGI_SWAP_CHAIN_DESC swapChainDesc{};
	swapChainDesc.BufferCount		 = 2;
	swapChainDesc.BufferDesc.Width	 = width;
	swapChainDesc.BufferDesc.Height  = height;
	swapChainDesc.BufferDesc.Format  = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage		 = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow		 = hWnd;
	swapChainDesc.SampleDesc.Count	 = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed			 = TRUE;
	swapChainDesc.SwapEffect		 = DXGI_SWAP_EFFECT_DISCARD;

	// D3D11デバイスとスワップチェインの作成
	HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr,
											   D3D_DRIVER_TYPE_HARDWARE,
											   nullptr,
											   createDeviceFlags,
											   featureLevels.data(),
											   static_cast<UINT>(featureLevels.size()),
											   D3D11_SDK_VERSION,
											   &swapChainDesc,
											   &m_swapChain,
											   &m_device,
											   nullptr,
											   &m_context
	);

	if (FAILED(hr))
	{
		return false;
	}

	// レンダーターゲットビューの作成
	ComPtr<ID3D11Texture2D> backBuffer;
	hr = m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	if (FAILED(hr))
	{
		return false;
	}

	// バックバッファからレンダーターゲットビューを作成
	hr = m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_renderTargetView);
	if (FAILED(hr))
	{
		return false;
	}

	// レンダーターゲットをセット
	m_context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), nullptr);

	// ビューポートを設定
	D3D11_VIEWPORT viewport{};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width	  = static_cast<FLOAT>(width);
	viewport.Height	  = static_cast<FLOAT>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	m_context->RSSetViewports(1, &viewport);

	//----------------------------------------------------------
	// imguiを初期化
	//----------------------------------------------------------
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = nullptr;
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(m_device.Get(), m_context.Get());

	return true;
}

void GUIRenderer::Begin()
{
	//----------------------------------------------------------
	// 画面をクリア
	//----------------------------------------------------------
	float clearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	m_context->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);
	
	//----------------------------------------------------------
	// imguiのフレーム開始
	//----------------------------------------------------------
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void GUIRenderer::End()
{
	//----------------------------------------------------------
	// imguiのフレーム終了
	//----------------------------------------------------------
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	//----------------------------------------------------------
	// スワップチェインの表示
	//----------------------------------------------------------
	m_swapChain->Present(1, 0);
}

GUIRenderer::~GUIRenderer()
{
	//----------------------------------------------------------
	// imguiのシャットダウン
	//----------------------------------------------------------
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}