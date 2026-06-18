// ディスプレイ名などの情報を取得するクラス
#pragma once

#include <string>
#include <vector>

struct DisplayInfo
{
	std::wstring name;
	std::wstring uniqueId;
	bool isPrimary = false;
};

class DisplayInfoManager
{
public:
	// 接続されているディスプレイの情報を列挙
	std::vector<DisplayInfo> EnumDisplayInfo() const;
};