# GoldenGames RetroHub Pro v0.1.0

## Which download?
- Complete-Native-Fixed.zip: RetroHub application folder, native runtime, return helpers and launch logo. Suitable for first-time RetroHub deployment when the prerequisites below are already installed.
- Update-Native-Fixed.zip: the same author-confirmed executable and launch artwork for existing RetroHub installations. Replace both eboot.bin and sce_module/libc.prx; preserve existing helpers and personal configuration.
- Console-Overlays.zip: optional console-border artwork and presets for a RetroArch build with working overlay support.
- Source.zip: frontend, native build-tool source, return-chord source, build script and artwork. It does not contain the custom source of the supplied return-watchdog binary.

## Required before installation
1. A jailbroken PS5 with etaHEN and a working Homebrew Launcher installation.
2. Homebrew Launcher/websrv must be enabled and listening on port 8080. Without it, games will not launch.
3. Install RetroArch and its PS5-compatible cores separately. RetroArch and cores are NOT included.

Tested by the author only on firmware 5.10 with etaHEN 2.6B. The author confirmed the recovered executable/runtime pair works on October 1, 2026. This archive was assembled and checked on a computer; a clean first-time installation has not been tested on a PS5 here.

## Install the complete folder
1. Extract Complete-Native-Fixed.zip on your computer.
2. Upload the entire PPSA99202 folder to /data/homebrew/ with FileZilla. Final path: /data/homebrew/PPSA99202/. Do not create /data/homebrew/PPSA99202/PPSA99202/.
3. Register/open the native application folder using a compatible native folder loader, such as the author's ShadowMountPlus setup. Homebrew Launcher/websrv is required for launching RetroArch games; it is not the native folder registration step. This ZIP is not a PKG or an automatic ELF installer.
4. Keep Homebrew Launcher/websrv enabled on port 8080 when using RetroHub.

## Existing installation update
Close RetroHub and back up the complete working folder to your computer. Copy BOTH files from Update-Native-Fixed.zip:
- PPSA99202/eboot.bin -> /data/homebrew/PPSA99202/eboot.bin
- PPSA99202/sce_module/libc.prx -> /data/homebrew/PPSA99202/sce_module/libc.prx

Copy the supplied sce_sys artwork if wanted. Preserve param.json, helpers, session configuration, ROMs, BIOS files, saves and personal RetroArch settings. Keep the folder structure exactly as supplied. The pic1.dds is launch artwork; the PS5 selection background remains unresolved.

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
The selection background on the PS5 menu remains unresolved. Vulkan rendering and 6x scaling are not provided by this frontend. The previous Corrected binary packages contained raw ELF files. Native deployment requires SELF containers for both eboot.bin and libc.prx. The working pair is:
- eboot.bin: 50730 bytes; SHA-256 59cea01199b22570c734057aef26d2c6168e888ffe8508f4513b9c82c045ba5a
- sce_module/libc.prx: 1284674 bytes; SHA-256 8a29784545983fffd7428446912711ec4e269672673e2d281612d3dbf4c39e88

The 109444-byte executable and 1335962-byte runtime are internal raw ELF payloads, not the files to deploy. Older files with the same SELF size may differ; verify hashes.

See LICENSE and THIRD_PARTY_NOTICES.md for attribution and source coverage.
