#include "DisplayInfoManager.h"

#include <Windows.h>

#include <vector>
#include <string>

std::vector<DisplayInfo> DisplayInfoManager::EnumDisplayInfo() const
{
    //----------------------------------------------------------
	// 接続されているディスプレイの情報を列挙する
    //----------------------------------------------------------
    std::vector<DisplayInfo> displays;
    UINT32 pathCount = 0;
    UINT32 modeCount = 0;

    if (GetDisplayConfigBufferSizes(QDC_ONLY_ACTIVE_PATHS, &pathCount, &modeCount) != ERROR_SUCCESS)
    {
        return displays;
    }

    std::vector<DISPLAYCONFIG_PATH_INFO> paths(pathCount);
    std::vector<DISPLAYCONFIG_MODE_INFO> modes(modeCount);

    if (QueryDisplayConfig(QDC_ONLY_ACTIVE_PATHS, &pathCount, paths.data(), &modeCount, modes.data(), nullptr) != ERROR_SUCCESS)
    {
        return displays;
    }

    for (std::size_t i = 0; i < pathCount; ++i)
    {
        DISPLAYCONFIG_TARGET_DEVICE_NAME targetName = {};
        targetName.header.type = DISPLAYCONFIG_DEVICE_INFO_GET_TARGET_NAME;
        targetName.header.size = sizeof(targetName);
        targetName.header.adapterId = paths[i].targetInfo.adapterId;
        targetName.header.id = paths[i].targetInfo.id;

        if (DisplayConfigGetDeviceInfo(&targetName.header) != ERROR_SUCCESS)
        {
            continue;
        }

        DisplayInfo info;

        if (targetName.flags.friendlyNameFromEdid)
        {
            info.name = targetName.monitorFriendlyDeviceName;
        }
        else
        {
            info.name = L"Unknown Monitor";
        }

        info.uniqueId = targetName.monitorDevicePath;

        //----------------------------------------------------------
        // メインディスプレイかどうか判定する
        //----------------------------------------------------------
        const LUID   srcAdapter = paths[i].sourceInfo.adapterId;
        const UINT32 srcId = paths[i].sourceInfo.id;

        for (std::size_t j = 0; j < modeCount; ++j)
        {
            if (modes[j].infoType == DISPLAYCONFIG_MODE_INFO_TYPE_SOURCE &&
                modes[j].adapterId.HighPart == srcAdapter.HighPart &&
                modes[j].adapterId.LowPart == srcAdapter.LowPart &&
                modes[j].id == srcId)
            {
                const POINTL pos = modes[j].sourceMode.position;
                info.isPrimary = (pos.x == 0 && pos.y == 0);
                break;
            }
        }

        displays.push_back(info);
    }

    return displays;
}