// アプリケーションのエントリポイント
#include "SettingUIManager.h"
#include "GUIRenderer.h"
#include "GUIWindow.h"

constexpr int WINDOW_WIDTH  = 800;
constexpr int WINDOW_HEIGHT = 600;
constexpr wchar_t WINDOW_TITLE[] = L"NV AutoDriverController";

int APIENTRY wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int)
{
	// ウィンドウ作成
	GUIWindow window;
	window.Create(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, WINDOW_TITLE);
	window.Show();

	// GUIレンダラーの初期化
	GUIRenderer guiRenderer;
	if (!guiRenderer.Initialize(WINDOW_WIDTH, WINDOW_HEIGHT, window.GetWindowHandle()))
	{
		return -1;
	}

	// 設定UIマネージャーの初期化
	SettingUIManager settingUIManager;
	if (!settingUIManager.Initialize())
	{
		return -1;
	}

	// ディスプレイ変更時のコールバックを設定
	window.SetDisplayChangeCallBack([&]()
		{
			settingUIManager.OnDisplayChanged();
		});

	// メインループ
	while (window.ProcessMessage())
	{
		// ウィンドウがタスクトレイにある場合は描画しない
		if (window.IsInTray())
		{
			window.RunMessageLoop();
			continue;
		}

		guiRenderer.Begin();
		settingUIManager.Update();	// この中で設定構築を行う
		guiRenderer.End();

		Sleep(16);	// ドライバ設定で強制的に垂直同期オフにされている場合もあるから念のため
	}

	return 0;
}