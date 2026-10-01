# GoldenGames RetroHub Pro v0.1.0 — Professional UI update

This is an UPDATE for an existing working GoldenGames RetroHub Pro installation, not a standalone installer.
Tested by the author on PS5 firmware 5.10. Other firmware versions are untested.

## Install the update
1. Close RetroHub Pro.
2. Back up /data/homebrew/PPSA99202/eboot.bin and existing presentation assets.
3. Copy PPSA99202/eboot.bin from this ZIP to /data/homebrew/PPSA99202/eboot.bin.
4. Copy the supplied sce_sys/icon0.png and sce_sys/pic1.dds into the existing application's sce_sys directory. For the registered tile tested by the author, presentation files were updated in /user/app/PPSA99202/sce_sys/. Keep the existing param.json and mount.lnk.
5. Launch RetroHub Pro with etaHEN and the Homebrew Launcher/websrv service enabled.

Keep your existing sce_module, return-watchdog.elf, return-chord.elf, retrohub-return-session.cfg, ROM folders, and RetroArch installation. This update does not replace them.

## Required existing RetroArch layout
Executable: /data/homebrew/RetroArch/retroarch.elf
Configuration: /data/homebrew/RetroArch/retroarch.cfg
Cores: /data/homebrew/RetroArch/.config/retroarch/cores/
ROM root: /data/homebrew/RetroArch/roms/
Use the system folders configured in the included rom_scanner.c and main.cpp.

## Controls
Left/right: choose game. L1/R1 or up/down: choose system. X: launch. Circle: rescan selected system.
Hold L1 + R1 + touchpad during a game to return, using the existing return helpers. The PS5 menu may appear briefly.

## Changes
Professional larger colored headings, PlayStation button symbols, console name beneath the title, selected game and emulator centered at the bottom, cover-download status at the lower right, light-blue frontend with white bars, original cover colors, and RetroHub logo launch artwork.

## Known limitation
The selection background on the PS5 home menu did not update in the author's test. The launch artwork is supplied separately as pic1.dds. No fix for the selection background is claimed.

## Source and licenses
Source is supplied in Goldengames-RetroHub-Pro-0.1.0-Source.zip and in this repository. See LICENSE and THIRD_PARTY_NOTICES.md. The tested eboot.bin was supplied by the author from the console; a bit-for-bit reproducible match has not been verified here.
No RetroArch executable, cores, ROMs, BIOS files, saves, downloaded cover art, or personal configuration is included.
