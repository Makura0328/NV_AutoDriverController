// このプロジェクト独自のウィンドウプロシージャの定義
#pragma once

#include "Window.h"

#include <functional>

class GUIWindow : public Window
{
public:
	void SetDisplayChangeCallBack(std::function<void()> func);
	int  RunMessageLoop();
	bool IsInTray() const;

	// このプロジェクト独自のウィンドウプロシージャの定義
	LRESULT LocalWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

private:
	void MinimizeToTray();
	void RestoreFromTray();
	void ShowTrayMenu();

	bool m_isInTray = false;
	std::function<void()> m_displayChangeCallback;
};