// ドライバ情報の書き込みや読み込みを行うクラス
#pragma once

#include <optional>

class DriverInfoManager
{
public:
	bool Initialize();
	bool SetGlobalDriverInfo(const struct Setting& setting);
	~DriverInfoManager();
};