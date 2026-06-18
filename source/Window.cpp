#include "Window.h"

Window::Window()
	:m_hInstance(GetModuleHandle(NULL)),
	m_hWnd(nullptr)
{
}

bool Window::Create(
	int width, int height, const std::wstring& windowTitle, const std::wstring& className)
{
	WNDCLASSEX windowClass =
	{
		sizeof(WNDCLASSEX),
		CS_HREDRAW | CS_VREDRAW,
		StaticWindowProc,
		0,
		0,
		m_hInstance,
		LoadIcon(NULL,IDI_APPLICATION),
		LoadCursor(NULL,IDC_ARROW),
		(HBRUSH)GetStockObject(BLACK_BRUSH),
		NULL,
		className.c_str(),
		NULL
	};

	if (RegisterClassEx(&windowClass) == 0)
	{
		return false;
	}

	m_hWnd = CreateWindowEx(
		NULL,
		className.c_str(),
		windowTitle.c_str(),
		(WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX ^ WS_MINIMIZEBOX),
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		width,
		height,
		NULL,
		NULL,
		m_hInstance,
		this);	// ウィンドウプロシージャをメンバ関数に持つためのトリック

	if (m_hWnd == nullptr)
	{
		return false;
	}

	RECT windowRect, clientRect;

	if (GetWindowRect(m_hWnd, &windowRect) == false)
	{
		return false;
	}

	if (GetClientRect(m_hWnd, &clientRect) == false)
	{
		return false;
	}

	// クライアント領域を指定サイズに修正
	int frameSizeX = (windowRect.right - windowRect.left) - (clientRect.right - clientRect.left);
	int frameSizeY = (windowRect.bottom - windowRect.top) - (clientRect.bottom - clientRect.top);

	int resizeWidth = frameSizeX + width;
	int resizeHeight = frameSizeY + height;

	SetWindowPos(
		m_hWnd,
		NULL,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		resizeWidth,
		resizeHeight,
		SWP_NOMOVE
	);

	return true;
}

void Window::Show()
{
	this->Show(SW_SHOW);
}

void Window::Show(int _nCmdShow)
{
	ShowWindow(m_hWnd, _nCmdShow);
}

bool Window::ProcessMessage()
{
	MSG msg = {};

	while (PeekMessage(&msg, nullptr, NULL, NULL, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return false;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}

HWND Window::GetWindowHandle() const
{
	return m_hWnd;
}

void Window::SetPointer(HWND hWnd)
{
	// WndProcをメンバ関数として持ち、オーバーライド可能にするためにHWNDとWindowクラスのオブジェクトを紐づける
	SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	this->m_hWnd = hWnd;
}

LRESULT CALLBACK Window::StaticWindowProc(
	HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// hWndに紐づいているWindowクラスのオブジェクトを探す
	Window* window = (Window*)(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	// 初回のメッセージならまだHWNDとオブジェクトが紐づけられていない
	if (window == nullptr)
	{
		//おそらくWM_CREATEの最中なので
		if (msg == WM_NCCREATE)
		{
			// CreateWindowEXのパラメーターから取得する
			window = (Window*)(((LPCREATESTRUCT)lParam)->lpCreateParams);
		}

		// 見つかったらHWNDとオブジェクトを紐づける
		if (window != nullptr)
		{
			window->SetPointer(hWnd);
		}
	}

	// HWNDとWindowクラスのオブジェクトが紐づいてるならそのオブジェクトのWndProcを実行
	if (window != nullptr)
	{
		return window->LocalWindowProc(hWnd, msg, wParam, lParam);
	}
	else
	{
		// 例外を投げてもよいかもしれない
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}

LRESULT Window::LocalWindowProc(
	HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_QUIT:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
		break;
	}

	return 0;
}