#include "GUIWindow.h"

#include <backends/imgui_impl_win32.h>
#include <shellapi.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static constexpr UINT WM_TRAYICON = WM_APP + 1;		// トレイアイコンのクリックを検知するためのカスタムメッセージ
static constexpr UINT TRAY_ICON_ID = 1;				// タスクトレイアイコンのID

LRESULT GUIWindow::LocalWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//----------------------------------------------------------
	// imguiのウィンドウプロシージャ
	//----------------------------------------------------------
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
	{
		return true;
	}

	//----------------------------------------------------------
	// このプロジェクト独自のウィンドウプロシージャ
	//----------------------------------------------------------
	switch (msg)
	{
		// ディスプレイの変更を検知したら、少し遅延させてからコールバックを呼ぶ
	case WM_DISPLAYCHANGE:
		SetTimer(hWnd, 1, 800, nullptr);
		return 0;

	case WM_TIMER:
		if (wParam == 1)
		{
			KillTimer(hWnd, 1);
			m_displayChangeCallback();
		}

		return 0;

	case WM_CLOSE:
		MinimizeToTray();
		return 0;

	case WM_TRAYICON:
		if (lParam == WM_LBUTTONUP)
		{
			RestoreFromTray();
		}
		else if (lParam == WM_RBUTTONUP)
		{
			ShowTrayMenu();
		}

		return 0;
	}

	return Window::LocalWindowProc(hWnd, msg, wParam, lParam);
}

void GUIWindow::SetDisplayChangeCallBack(std::function<void()> func)
{
	m_displayChangeCallback = func;
}

bool GUIWindow::IsInTray() const
{
	return m_isInTray;
}

int GUIWindow::RunMessageLoop()
{
	//----------------------------------------------------------
	// タスクトレイ状態のメッセージループ
	//----------------------------------------------------------
	MSG msg{};

	while (IsInTray())
	{
		BOOL ret = GetMessage(&msg, nullptr, 0, 0);

		// エラー
		if (ret == -1)
		{
			return -1;
		}

		// WM_QUIT
		if (ret == 0)
		{
			// 外側ループにも伝える
			PostQuitMessage((int)msg.wParam);
			return (int)msg.wParam;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

void GUIWindow::MinimizeToTray()
{
	// すでにタスクトレイ状態なら何もしない
	if (m_isInTray)
	{
		return;
	}

	m_isInTray = true;

	//----------------------------------------------------------
	// タスクトレイにアイコンを追加
	//----------------------------------------------------------
	NOTIFYICONDATAW trayIconData{};
	trayIconData.cbSize			  = sizeof(NOTIFYICONDATAW);
	trayIconData.hWnd			  = m_hWnd;
	trayIconData.uID			  = TRAY_ICON_ID;
	trayIconData.uFlags			  = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	trayIconData.uCallbackMessage = WM_TRAYICON;
	trayIconData.hIcon			  = LoadIcon(nullptr, IDI_APPLICATION);

	// ツールチップの文字列を設定
	wcscpy_s(trayIconData.szTip, L"NV AutoDriverController");

	Shell_NotifyIconW(NIM_ADD, &trayIconData);
	ShowWindow(m_hWnd, SW_HIDE);
}

void GUIWindow::RestoreFromTray()
{
	// タスクトレイ状態でなければ何もしない
	if (!m_isInTray)
	{
		return;
	}

	m_isInTray = false;

	//----------------------------------------------------------
	// トレイアイコンを削除
	//----------------------------------------------------------
	NOTIFYICONDATAW trayIconData{};
	trayIconData.cbSize = sizeof(trayIconData);
	trayIconData.hWnd = m_hWnd;
	trayIconData.uID = TRAY_ICON_ID;
	Shell_NotifyIconW(NIM_DELETE, &trayIconData);

	// ウィンドウを表示して前面に持ってくる
	ShowWindow(m_hWnd, SW_SHOW);
	SetForegroundWindow(m_hWnd);
}

void GUIWindow::ShowTrayMenu()
{
	//----------------------------------------------------------
	// トレイメニューを作成
	//----------------------------------------------------------
	HMENU hMenu = CreatePopupMenu();
	AppendMenuW(hMenu, MF_STRING, 1, L"Restore");
	AppendMenuW(hMenu, MF_STRING, 2, L"Exit");

	// これが無いと、メニュー外をクリックしても閉じないことがある
	SetForegroundWindow(m_hWnd);

	POINT cursorPos;
	GetCursorPos(&cursorPos);

	int cmd = TrackPopupMenu(hMenu, TPM_RETURNCMD | TPM_NONOTIFY | TPM_LEFTALIGN | TPM_TOPALIGN,
		cursorPos.x, cursorPos.y, 0, m_hWnd, nullptr);

	DestroyMenu(hMenu);

	switch (cmd)
	{
	case 1:
		RestoreFromTray();
		break;

	case 2:
	{
		// 終了前にアイコンを消す（消さないとゴーストが残る）
		NOTIFYICONDATAW trayIconData{};
		trayIconData.cbSize = sizeof(trayIconData);
		trayIconData.hWnd = m_hWnd;
		trayIconData.uID = TRAY_ICON_ID;
		Shell_NotifyIconW(NIM_DELETE, &trayIconData);
	}

	PostQuitMessage(0);
	break;
	}
}