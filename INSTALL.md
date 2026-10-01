# GoldenGames RetroHub Pro v0.1.0

## Which download?
- Complete-Corrected.zip: RetroHub application folder, native runtime, return helpers and launch logo. Suitable for first-time RetroHub deployment when the prerequisites below are already installed.
- Update-Corrected.zip: the same author-confirmed executable and launch artwork for existing RetroHub installations. Keep existing helpers/runtime.
- Console-Overlays.zip: optional console-border artwork and presets for a RetroArch build with working overlay support.
- Source.zip: frontend, native build-tool source, return-chord source, build script and artwork. It does not contain the custom source of the supplied return-watchdog binary.

## Required before installation
1. A jailbroken PS5 with etaHEN and a working Homebrew Launcher installation.
2. Homebrew Launcher/websrv must be enabled and listening on port 8080. Without it, games will not launch.
3. Install RetroArch and its PS5-compatible cores separately. RetroArch and cores are NOT included.

Tested by the author only on firmware 5.10. This archive was assembled and checked on a computer; a clean first-time installation has not been tested on a PS5 here.

## Install the complete folder
1. Extract Complete-Corrected.zip on your computer.
2. Upload the entire PPSA99202 folder to /data/homebrew/ with FileZilla. Final path: /data/homebrew/PPSA99202/. Do not create /data/homebrew/PPSA99202/PPSA99202/.
3. Start/refresh Homebrew Launcher with websrv enabled and open GoldenGames RetroHub Pro. If your installation uses a registered native-title tile, register this application folder using the same folder-registration workflow your launcher uses. This ZIP is an application folder, not a PKG or an automatic ELF installer.
4. Keep websrv enabled when using RetroHub.

## Existing installation update
Close RetroHub and back up its executable and presentation files. Copy PPSA99202/eboot.bin from Update-Corrected.zip to /data/homebrew/PPSA99202/eboot.bin. Copy the supplied sce_sys artwork into the application folder. Do not replace existing param.json, runtime, helpers or personal RetroArch configuration with unrelated files. Registered tiles may also use /user/app/PPSA99202/sce_sys/ for presentation; the new pic1.dds is the launch logo, not the selection background.

## Required RetroArch paths
- Executable: /data/homebrew/RetroArch/retroarch.elf
- Main configuration: /data/homebrew/RetroArch/retroarch.cfg
- Cores: /data/homebrew/RetroArch/.config/retroarch/cores/
- ROM root: /data/homebrew/RetroArch/roms/

System folder -> default core:
nes -> fceumm_libretro.so
snes -> snes9x_libretro.so
n64 -> parallel_n64_libretro.so
gb, gbc -> gambatte_libretro.so
gba -> mgba_libretro.so
genesis -> genesis_plus_gx_libretro.so
segacd, x32 -> picodrive_libretro.so
saturn -> yabause_libretro.so
psx -> pcsx_rearmed_libretro.so
atari2600 -> stella2023_libretro.so
atari7800 -> prosystem_libretro.so
lynx -> handy_libretro.so
jaguar -> virtualjaguar_libretro.so
pce -> mednafen_pce_fast_libretro.so
arcade -> fbneo_libretro.so
amiga -> puae_libretro.so
c64 -> vice_x64_libretro.so

Place your own games in the corresponding system folder. BIOS files belong in the actual System/BIOS directory selected by your RetroArch installation. No ROMs, BIOS, saves or personal game lists are included.

## Controls
Left/right: select game. L1/R1 or up/down: select system. X: launch. Circle: rescan selected system. Hold L1 + R1 + touchpad in game to return using the supplied helpers. The PS5 menu may appear briefly before RetroHub returns.

## Optional console overlays
Extract the optional overlays folder into /data/homebrew/PPSA99202/. In RetroArch use the appropriate overlays/<system>.cfg preset and save an override for that system after adjusting the viewport. Requires overlay support in your RetroArch build. Do not append all session presets globally; Game Boy/GBC/GBA viewports need individual adjustment. See overlays/LICENSE.txt and overlays/SOURCES.json.

## Known limitations and correction
The selection background on the PS5 menu remains unresolved. Vulkan rendering and 6x scaling are not provided by this frontend. On 2026-09-30 the author confirmed that the working executable is 109444 bytes, SHA-256 8c3bec7e175b1de260a5d2d36b4e1318b75b5bd62f6ef36b80e51c6fce332e01. This supersedes the earlier 50730-byte update executable.

See LICENSE and THIRD_PARTY_NOTICES.md for attribution and source coverage.
