# NV_AutoDriverController
Automatically switches NVIDIA driver settings (V-Sync and Max Frame Rate) based on the display currently set as primary in Windows.

<img width="400" height="321" alt="スクリーンショット 2026-06-18 221544" src="https://github.com/user-attachments/assets/be5315af-4e57-412d-ab75-c00e471234f6" />

## Why
The NVIDIA Control Panel only stores a single set of global 3D settings plus per-application profiles. There is no built-in way to keep different driver settings for each monitor. This is inconvenient in setups where the primary display changes frequently—for example:
- A PC connected to both a gaming monitor and a TV (e.g. a 144Hz monitor with VRR support and a 4K60Hz TV without VRR)

This tool watches for primary display changes and automatically applies the V-Sync and Max Frame Rate settings you've saved for that display.

## Description
- Settings are stored in config.json.
- When you close the application, it keeps running in the system tray.

## Requirements
- Windows 10/11 (x64)
- Direct3D11
- Build environment: Visual Studio 2026

## Build
This project uses Git Submodules for its dependencies, so clone it with the following command:
```sh
git clone --recurse-submodules https://github.com/Makura0328/NV_AutoDriverController.git
```
Then run @open_project.cmd located in the repository.

## Usage
> Because this app uses NVAPI to change NVIDIA driver profile settings, **please run it as administrator.**

1. Launch the app and select the display whose settings you want to change.
2. Edit each setting. Hold the left Ctrl key while selecting a slider to enter a value directly.
3. Setting Max Frame Rate to 0 makes the frame rate unlimited.
4. Press "Save" to save the settings. Be sure to press Save for each display individually.
5. When you close the window, the app minimizes to the system tray and automatically applies the settings the next time the primary display changes.

## Download
Download the latest version from the [Releases page](https://github.com/Makura0328/NV_AutoDriverController/releases/latest).

## Dependencies
This project uses the following libraries, included as Git submodules:

- [Dear ImGui](https://github.com/ocornut/imgui) — GUI rendering (MIT License)
- [nlohmann/json](https://github.com/nlohmann/json) — Reading and writing config.json (MIT License)
- [NVAPI](https://developer.nvidia.com/nvidia-api) — NVIDIA driver profile control (subject to NVIDIA's license terms)

Each library is distributed under its own license. Please refer to the respective repositories for details.

## License
MIT License. See the LICENSE file for details.
