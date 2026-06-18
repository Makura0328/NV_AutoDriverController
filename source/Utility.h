// 文字列変換などののユーティリティ関数を提供するヘッダーファイルです。
#pragma once

#include <string>

namespace Utility {

// w_charをcharに変換
std::string WStringToUTF8(const std::wstring& wstr);

} // namespace Utility