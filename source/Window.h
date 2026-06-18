// ウィンドウの基底クラスです。ウィンドウの作成、表示、メッセージ処理などの基本的な機能を提供します。
// 派生クラスでカスタマイズ可能なウィンドウプロシージャを提供します。
#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <Windows.h>

#include <string>

class Window
{
public:
	// コンストラクタ
	Window();

	// デストラクタ
	virtual ~Window() = default;

	// ウィンドウを作成。classNameは他とかぶらないようにする必要あり
	bool Create(int width, int height, const std::wstring& windowTitle, const std::wstring& className);

	// ウィンドウを表示します。以下のオーバーロード版に、自動でSH_SHOWを渡して実行します
	void Show();

	// Win32APIのShowWindow関数に渡すパラメーターを指定して、ウィンドウの表示を行います
	void Show(int nCmdShow);

	// メッセージを処理します
	bool ProcessMessage();

	// ウィンドウハンドルを取得します
	HWND GetWindowHandle() const;

	// 派生クラスでカスタマイズ可能なこのウィンドウに対するウィンドウプロシージャ
	virtual LRESULT LocalWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	HWND m_hWnd;
	HINSTANCE m_hInstance;

private:
	static LRESULT CALLBACK StaticWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void SetPointer(HWND hWnd);
};