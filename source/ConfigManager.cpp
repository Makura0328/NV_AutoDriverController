#include "ConfigManager.h"

#include <nlohmann/json.hpp>

#include <fstream>
#include <string>

using json = nlohmann::json;

namespace {

constexpr const char* CONFIG_PATH = "config.json";

// ファイル全体を読み込む。無い/壊れている場合は空オブジェクトを返す
json LoadJsonFile()
{
	// ファイルを開く
	std::ifstream ifs(CONFIG_PATH);
	if (!ifs)
	{
		return json::object();
	}

	// JSONをパースする。失敗した場合は空オブジェクトを返す
	json j = json::parse(ifs, nullptr, false);
	if (j.is_discarded())
	{
		return json::object();
	}

	return j;
}

} // namespace

Setting ConfigManager::LoadConfig(const std::string& id) const
{
	Setting setting;
	json root = LoadJsonFile();

	// displays に該当モニターのエントリがあれば読み込む
	if (root.contains("displays") && root["displays"].contains(id))
	{
		const json& e	= root["displays"][id];
		setting.isVsync = e.value("isVsync", setting.isVsync);
		setting.maxFPS  = e.value("maxFramerate", setting.maxFPS);
	}

	return setting;
}

bool ConfigManager::SaveConfig(const Setting& setting, const std::string& id) const
{
	// まず既存の内容を読み込む（他モニターの設定を消さないため）
	json root = LoadJsonFile();

	json entry;
	entry["isVsync"]	  = setting.isVsync;
	entry["maxFramerate"] = setting.maxFPS;
	root["displays"][id]  = entry;   // 該当モニターだけ追加/更新

	std::ofstream ofs(CONFIG_PATH);
	if (!ofs)
	{
		return false;
	}

	ofs << root.dump(4);

	return true;
}