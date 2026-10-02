# GoldenGames RetroHub Pro

Native PS5 retro gaming frontend by GoldenGames, with 3D cover flow, automatic cover downloads, ROM scanning, synthesized menu sounds, and RetroArch integration for 19 systems.

## Download

[v0.1.0 — Native SELF fix](https://github.com/robin2kk/Goldengames-RetroHub-Pro-PS5/releases/tag/v0.1.0)

**Use the downloads marked `Native-Fixed`.** The author tested the recovered SELF executable and runtime on October 1, 2026. Earlier Complete-Corrected and Update-Corrected packages contained raw ELF files and are superseded.

- `Goldengames-RetroHub-Pro-0.1.0-Complete-Native-Fixed.zip`: application folder with both tested SELF files, return helpers and artwork.
- `Goldengames-RetroHub-Pro-0.1.0-Update-Native-Fixed.zip`: both tested SELF files and artwork for existing installations. Replace **both** eboot.bin and sce_module/libc.prx.
- Source archives marked `Superseded` are retained for history. Consult [BUILDING.md](BUILDING.md) on the main branch for corrected deployment guidance; a bit-for-bit rebuild is not verified.
- `Goldengames-RetroHub-Pro-Console-Overlays.zip`: optional console borders/presets.
- `SHA256SUMS-Native-Fixed.txt`: checksums for the new Complete and Update ZIPs.

The tested eboot.bin is **50,730 bytes**, SHA-256 `59cea01199b22570c734057aef26d2c6168e888ffe8508f4513b9c82c045ba5a`; libc.prx is **1,284,674 bytes**, SHA-256 `8a29784545983fffd7428446912711ec4e269672673e2d281612d3dbf4c39e88`. Size alone does not identify a build.

See [INSTALL.md](INSTALL.md). The complete ZIP is a deployable application folder, not a PKG or an automatic ELF installer. Homebrew Launcher/websrv and RetroArch/cores must be installed separately.

## Features

- Horizontal 3D cover flow and original cover colors.
- Automatic cover downloads and scanning, including N64, PlayStation and Saturn.
- Light-blue interface with white header/footer, larger colored headings and PlayStation button symbols.
- Console name beneath the title; game and emulator names centered at the bottom.
- Synthesized menu sounds and controller navigation.
- RetroHub logo launch artwork.

## Compatibility and requirements

Tested by the author on **PS5 firmware 5.10 with etaHEN 2.6B**. Other firmware versions are untested by the author.

**Homebrew Launcher with websrv enabled on port 8080 is required to launch games.** RetroArch and its PS5-compatible cores are installed separately at `/data/homebrew/RetroArch/`. Use a compatible native folder loader/registration workflow, such as the author's ShadowMountPlus setup, for the PS5 application tile. Both return helpers are included in the complete package. A clean first-time installation of the assembled archive has not been hardware-tested here.

## Controls

| Control | Action |
| --- | --- |
| Left / right | Select game |
| L1 / R1 or up / down | Select system |
| X | Launch game |
| Circle | Rescan selected system |
| Hold L1 + R1 + touchpad in game | Return using the existing helpers |

The PS5 menu may appear briefly before RetroHub returns. The PS5 selection background remains unresolved; this release does not claim to fix it. Vulkan rendering and 6x scaling are not features of this frontend release.

## Source and credits

See [LICENSE](LICENSE), [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md), and [BUILDING.md](BUILDING.md). The tested executable was supplied by the author from the working console; a bit-for-bit source/binary match has not been verified here. The custom source for the supplied return-watchdog binary was not recovered in this session and is not included.

No RetroArch executable, cores, ROMs, BIOS files, saves, downloaded covers, or personal configurations are included.
