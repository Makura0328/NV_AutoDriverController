--============================================================================
--　ソリューションの設定
--============================================================================
function workspace_define(workspace_name)
    workspace (workspace_name)
    location  ".build"
    
    platforms {"x64"}

    configurations
    {
        "Debug",
        "Release",
        "ReleaseLTCG",
    }

    debugdir (path.join( "%{wks.location}", ".."))
end

--============================================================================
-- プロジェクト設定
--============================================================================
function project_define(project_name, kind_type)
    project     (project_name)
    location    (path.join(".build", "%{prj.name}"))                              -- プロジェクトファイルの場所
    targetdir   (path.join(".build", "%{prj.name}/%{cfg.buildcfg}"))              -- 出力ディレクトリ
    objdir      (path.join(".build", "%{prj.name}/%{cfg.buildcfg}/intermediate")) -- 中間ディレクトリ

    language    "C++"           -- 言語
    kind        (kind_type)     -- 種類（libやexeなど）

    -- 各種オプション設定
    cppdialect        "C++latest"   -- C++最新準拠
    rtti              "Off"			-- ランタイム方情報を有効にする	
    exceptionhandling "Off"			-- 例外を有効にする
    floatingpoint     "Fast"		-- fastmath
    characterset      "Unicode"	
    intrinsics        "On"			-- 組み込み関数を使用する
    staticruntime     "On"			-- ランタイムにDLLを使用しない

    toolset "v145"

    flags {
        "MultiProcessorCompile",	-- 複数プロセッサによるコンパイル
        "NoRuntimeChecks",			-- 基本ランタイムチェック無効
    }

    -- リンカーの追加オプション
    linkoptions {
        "/IGNORE:4099"
    }

    -- 追加のインクルードパス ""
    includedirs {
    }

    -- 追加のシステムインクルードパス <>
    externalincludedirs {
    }

    -- 除去するファイルSs
    removefiles {
    }

    -- 追加のライブラリディレクトリ
    libdirs
    {
    }

    -- プリプロセッサ #define
    defines {
        -- "_CRT_SECURE_NO_WARNINGS",
        "WIN64",
        "WIN32",
        "_WIN32",
        "_WINDOWS",
    }

    filter { "kind:StaticLib" }
        defines { "_LIB" }
    filter { "kind:SharedLib" }
        defines { "_DLL" }
    filter { "kind:WindowedApp" }
    filter { "kind:ConsoleApp" }
        defines { "_CONSOLE" }
    filter {}

    -- 特定の警告の無効化
    disablewarnings {
        "4324",		-- warning C4324: アラインメント指定子のために構造体がパッドされました
        "4201",		-- warning C4201: 非標準の拡張機能が使用されています: 無名の構造体または共用体です。
        "5054",		-- warning C5054: 演算子 '|': 異なる型の列挙間では非推奨です (Cerealで使用されています)
    }

    --============================================================================
    -- デバッグ(Debug) ビルド
    --============================================================================
    filter "configurations:Debug"
        defines {
            "_DEBUG",
            "DEBUG",
            "_WINDOWS"
        }

        symbols           "FastLink"	-- 高速リンク
        optimize             "Off"
        omitframepointer     "Off"
        inlining             "Explicit"
        functionlevellinking "On"
        editandcontinue      "Off"

        buildoptions {
            "/Zo",	-- 最適化されたデバッグ機能の強化
            "/Zc:char8_t-", -- char8_tは使用しない
        }

    --============================================================================
    -- リリース(Release) ビルド
    --============================================================================
    filter "configurations:Release*"
        defines {
            "NDEBUG",
        }
        buildoptions {
            "/GT",	-- ファイバー保護の最適化
        }

    -- Release
    filter "configurations:Release"
        optimize             "Speed"
        omitframepointer     "Off"
        inlining             "Explicit"
        functionlevellinking "Off"

        defines {
            "PROFILE",
        }
        buildoptions {
            "/Zo",	-- 最適化されたデバッグ機能の強化
            "/Zc:char8_t-", -- char8_tは使用しない
        }

    -- ReleaseLTCG
    filter "configurations:ReleaseLTCG"
        optimize             "Size"
        omitframepointer     "On"
        inlining             "Auto"
        functionlevellinking "On"
        linktimeoptimization "On"

        defines {
            "LTCG",
        }
        buildoptions {
            "/Zc:char8_t-", -- char8_tは使用しない
        }
        linkoptions{
            "/OPT:REF",     -- 参照されていないデータと関数を削除
        }

    filter {}	-- 元に戻す
end
