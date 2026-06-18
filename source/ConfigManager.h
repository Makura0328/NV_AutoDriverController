// 設定を定義するjsonの書き込みや読み込みを行うクラス
#pragma once

#include "Setting.h"

#include <string>

class ConfigManager
{
public:
	// 現在の設定を読み込む
	Setting LoadConfig(const std::string& monitorId) const;

	// 設定を保存する
	bool SaveConfig(const Setting& setting, const std::string& monitorId) const;
};