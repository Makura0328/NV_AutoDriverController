@echo off

:: UTF-8
chcp 65001 > nul

:: 変数の展開を適切に行う
setlocal enabledelayedexpansion

cd /d "%~dp0"

::pushd "%~dp0"
::call "@cleanup.cmd"
::popd

::---------------------------------------------------------------
:: .buildフォルダを作成
:: 環境変数BUILD_PATHを設定した場合は作成先を移動可能。
::---------------------------------------------------------------
pushd "%~dp0"

    :: テンポラリフォルダ用にGUIDを取得
    for /f %%a in ('powershell -command "$([guid]::NewGuid().ToString())"') do ( set GUID=%%a )

    :: .buildがなかったら通常作成するためスキップ
    if not exist .build goto build_cleanup_skip

    :: すでに.buildがアクセスできたらスキップ。dirがエラーになったらジャンクションが途切れている
    :: dirでアクセス可能か試す
    dir .build /b > nul 2>&1
    if %errorlevel%  equ 0 (
        goto build_cleanup_skip
    )

    :: 途切れて壊れているジャンクションを一旦解消
    echo .build ジャンクションを再構築します.
    rd /S /Q .build

:build_cleanup_skip

    :: すでにフォルダが存在していたら作成をスキップ
    if exist .build goto skip

    :: カスタムビルドパスの環境変数が存在しなかった場合はスキップ
    if not defined BUILD_PATH goto skip

    echo 環境変数BUILD_PATHが設定されています。%BUILD_PATH%
    echo BUILD_PATH=%BUILD_PATH%

    :: シンボリックリンク接続先のフォルダ生成
    if not exist "%BUILD_PATH%\.build\%GUID%" (
        md "%BUILD_PATH%\.build\%GUID%"
    )

    :: ジャンクション作成 (シンボリックリンクは管理者権限が必要だがジャンクションは不要)
    mklink /j .build "%BUILD_PATH%\.build\%GUID%"

:skip
	popd


::---------------------------------------------------------------
:: プロジェクトファイルを生成
::---------------------------------------------------------------

:: WindowsのTemp内にジャンクション作成 (日本語パス対策)
:: Tempフォルダ内から dp0 を見えるようにする
mklink /j "%TEMP%\%GUID%" "%~dp0" > nul

:: Temp内からプロジェクト生成
pushd "%TEMP%\%GUID%"
    bin\premake5.exe vs2022
popd

if %ERRORLEVEL% neq 0 (
    echo.
    echo エラーが発生しました。Luaスクリプトを確認してください。
    echo.
    echo ==========
    echo "～～/premake5.lua: No such file or directory" と赤い文字で表示される場合はフォルダパスに全角文字が含まれていないか
    echo 配置場所を確認してください。
    echo.
    echo [現在の配置場所]
    cd
    echo.
    echo 全角文字（日本語など）が含まれています。
    echo.
    echo ==========
    echo.
    pause
)

endlocal
