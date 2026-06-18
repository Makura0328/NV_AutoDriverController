include("bin/build_config.lua")

-- 外部ソフトウェアのパス
EXTERNAL_PATH = "external"

--============================================================================
-- ソリューション
--============================================================================
workspace_define("NV_DisplayControl")

    startproject "NV_DisplayControl"

--============================================================================
--外部ソフトウェア--
--============================================================================
group "外部ソフトウェア"

----------------------------------------------------------------------
-- imgui
----------------------------------------------------------------------
project_define("imgui", "StaticLib")

    SOURCE_PATH = path.join(EXTERNAL_PATH, "imgui")

    -- プロジェクトに含めるソース
    files {
        path.join(SOURCE_PATH, "*.cpp"),
        path.join(SOURCE_PATH, "*.h"),
        path.join(SOURCE_PATH, "backends/imgui_impl_dx11.*"),
        path.join(SOURCE_PATH, "backends/imgui_impl_win32.*"),
    }

    -- 追加のインクルードパス ""
    includedirs {
        SOURCE_PATH,
    }

    -- プリプロセッサ #define
    defines {
    	"IMGUI_DEFINE_MATH_OPERATORS",
	}
	
    -- プリコンパイルヘッダー
    pchheader("imgui.h")
    pchsource(path.join(SOURCE_PATH, "imgui.cpp"))
    
    -- 仮想パス
    vpaths {
        ["インクルード ファイル Include/**"] = {
            path.join(SOURCE_PATH, "**.h"),
        },
        ["ソース ファイル Source/**"] = {
            path.join(SOURCE_PATH, "**.cpp"),
        },
    }

--============================================================================
--　NV_AutoDriverControllerプロジェクト
--============================================================================
group "NV_AutoDriverController"

project_define("NV_AutoDriverController", "WindowedApp")

    SOURCE_PATH = "source"

      files {
       path.join(SOURCE_PATH, "**.c"),
        path.join(SOURCE_PATH, "**.cpp"),
        path.join(SOURCE_PATH, "**.cxx"),

        path.join(SOURCE_PATH, "**.h"),
        path.join(SOURCE_PATH, "**.hpp"),
        path.join(SOURCE_PATH, "**.hxx"),
    }

    includedirs {
        SOURCE_PATH
    }

    externalincludedirs {
        path.join(EXTERNAL_PATH, "nvapi"),
        path.join(EXTERNAL_PATH, "json/include"),
        path.join(EXTERNAL_PATH, "imgui"),
    }

    libdirs {
        path.join(EXTERNAL_PATH, "nvapi", "amd64"),
    }

     links {
        "nvapi64",
        "user32",
        "shell32",
        "imgui",
    }

group ""