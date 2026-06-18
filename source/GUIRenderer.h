// D3D11でimguiを描画
#pragma once

#include <wrl/client.h>
#include <d3d11.h>

using Microsoft::WRL::ComPtr;

class GUIRenderer
{
public:
	bool Initialize(int width, int height, HWND hWnd);
	void Begin();
	void End();
	~GUIRenderer();

private:
	ComPtr<ID3D11Device>			m_device;
	ComPtr<ID3D11DeviceContext>		m_context;
	ComPtr<IDXGISwapChain>			m_swapChain;
	ComPtr<ID3D11RenderTargetView>  m_renderTargetView;
};