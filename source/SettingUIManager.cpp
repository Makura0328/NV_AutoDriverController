#include "SettingUIManager.h"
#include "Utility.h"

#include <imgui.h>

#include <string>

using namespace Utility;

bool SettingUIManager::Initialize()
{
	if (!m_driverInfoManager.Initialize())
	{
		return false;
	}

	// ディスプレイ情報を列挙
	m_displayInfos = m_displayInfoManager.EnumDisplayInfo();
	m_selectedDisplayId = WStringToUTF8(m_displayInfos[m_selectedDisplayIndex].uniqueId);

	// 選択したディスプレイの現在の設定があれば読み込む
	auto setting = m_configManager.LoadConfig(m_selectedDisplayId);
	m_vsync = setting.isVsync;
	m_max = setting.maxFPS;

	return true;
}

void SettingUIManager::Update()
{
	//----------------------------------------------------------
	// ビューポート全体を覆うベースウィンドウ
	//----------------------------------------------------------
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);

	ImGuiWindowFlags windowFlags =
		ImGuiWindowFlags_NoDecoration |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoBringToFrontOnFocus;

	// 端までぴったり使うため余白を消す
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::Begin("MainWindow", nullptr, windowFlags);
	ImGui::PopStyleVar();

	//----------------------------------------------------------
	// ディスプレイ選択
	//----------------------------------------------------------
	ImGui::BeginChild("DisplayPane", ImVec2(200, 0), true);

	ImGui::Text("Display");
	ImGui::Separator();

	// ディスプレイ列挙
	auto displayInfos = m_displayInfoManager.EnumDisplayInfo();
	for (int i = 0; i < displayInfos.size(); ++i)
	{
		std::string displayName = WStringToUTF8(displayInfos[i].name);

		if (ImGui::Selectable(displayName.c_str(), m_selectedDisplayIndex == i))
		{
			// ディスプレイ選択
			m_selectedDisplayId = WStringToUTF8(displayInfos[i].uniqueId);
			m_selectedDisplayIndex = i;

			// 選択したディスプレイの現在の設定があれば読み込む
			auto setting = m_configManager.LoadConfig(m_selectedDisplayId);
			m_vsync = setting.isVsync;
			m_max = setting.maxFPS;
		}
	}

	ImGui::EndChild();

	ImGui::SameLine();

	//----------------------------------------------------------
	// グローバル設定
	//----------------------------------------------------------
	ImGui::BeginChild("SettingsPane", ImVec2(0, 0), true);

	ImGui::Text("Global Settings");
	ImGui::Separator();

	ImGui::Checkbox("Enable VSync", &m_vsync);
	ImGui::Separator();
	ImGui::SetNextItemWidth(200.0f);
	ImGui::SliderInt("Max FPS", &m_max, 0, 1000, "%d");

	if (ImGui::Button("Save"))
	{
		m_configManager.SaveConfig(Setting{ m_vsync, m_max }, m_selectedDisplayId);
	}

	ImGui::EndChild();

	ImGui::End();
}

void SettingUIManager::OnDisplayChanged()
{
	// メインディスプレイ探す
	std::string primaryDisplayId;
	auto displayInfos = m_displayInfoManager.EnumDisplayInfo();
	for (auto& info : displayInfos)
	{
		if (info.isPrimary)
		{
			primaryDisplayId = WStringToUTF8(info.uniqueId);
			break;
		}
	}

	// メインディスプレイの設定を読み込み、ドライバ設定を更新
	auto setting = m_configManager.LoadConfig(primaryDisplayId);
	m_driverInfoManager.SetGlobalDriverInfo(setting);
}