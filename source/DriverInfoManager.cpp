#include "DriverInfoManager.h"
#include "Setting.h"

#include <nvapi.h>
#include <NvApiDriverSettings.h>

bool DriverInfoManager::Initialize()
{
	//----------------------------------------------------------
	// nvapiの初期化
	//----------------------------------------------------------
	if (NvAPI_Initialize() != NVAPI_OK)
	{
		return false;
	}

	return true;
}

DriverInfoManager::~DriverInfoManager()
{
	NvAPI_Unload();
}

bool DriverInfoManager::SetGlobalDriverInfo(const Setting& setting)
{
	//----------------------------------------------------------
	// セッションを開き、プロファイルを取得
	//----------------------------------------------------------
	NvAPI_Status status;
	NvDRSSessionHandle hSession = nullptr;
	status = NvAPI_DRS_CreateSession(&hSession);
	if (status != NVAPI_OK)
	{
		return false;
	}

	// 現在のドライバ設定を読み込む
	status = NvAPI_DRS_LoadSettings(hSession);
	if (status != NVAPI_OK)
	{
		NvAPI_DRS_DestroySession(hSession);
		return false;
	}

	// グローバルプロファイルのハンドルを取得
	NvDRSProfileHandle hProfile = nullptr;
	status = NvAPI_DRS_GetBaseProfile(hSession, &hProfile);
	if (status != NVAPI_OK)
	{
		NvAPI_DRS_DestroySession(hSession);
		return false;
	}

	//----------------------------------------------------------
	// VSync設定を書き込み
	//----------------------------------------------------------
	{
		NVDRS_SETTING s{};
		s.version			= NVDRS_SETTING_VER;
		s.settingId			= VSYNCMODE_ID;
		s.settingType		= NVDRS_DWORD_TYPE;
		s.u32CurrentValue	= setting.isVsync ? VSYNCMODE_FORCEON : VSYNCMODE_FORCEOFF;

		NvAPI_DRS_SetSetting(hSession, hProfile, &s);
	}

	//----------------------------------------------------------
	// 最大フレームレート設定を書き込み
	//----------------------------------------------------------
	{
		NVDRS_SETTING s{};
		s.version			= NVDRS_SETTING_VER;
		s.settingId			= FRL_FPS_ID;
		s.settingType		= NVDRS_DWORD_TYPE;
		s.u32CurrentValue	= setting.maxFPS;

		NvAPI_DRS_SetSetting(hSession, hProfile, &s);
	}

	// 設定を保存してセッションを閉じる
	status = NvAPI_DRS_SaveSettings(hSession);
	NvAPI_DRS_DestroySession(hSession);

	return status == NVAPI_OK;
}