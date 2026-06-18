// 設定画面のUIや動作を定義します
#pragma once

#include "DisplayInfoManager.h"
#include "DriverInfoManager.h"
#include "ConfigManager.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <Windows.h>

class SettingUIManager
{
public:
	bool Initialize();
	void Update();

	// TODO: これは別のクラスに切り出すべきかもしれない
	void OnDisplayChanged();

private:
	int  m_selectedDisplayIndex = 0;
	bool m_vsync = false;
	int  m_max = 0;
	std::string m_selectedDisplayId;
	std::vector<DisplayInfo> m_displayInfos;

	// ディスプレイ情報管理クラス
	DisplayInfoManager m_displayInfoManager;
	DriverInfoManager  m_driverInfoManager;
	ConfigManager	   m_configManager;
};