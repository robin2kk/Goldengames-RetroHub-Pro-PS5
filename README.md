# GoldenGames RetroHub Pro

Native PS5 retro gaming frontend by GoldenGames, with 3D cover flow, automatic cover downloads, ROM scanning, synthesized menu sounds, and RetroArch integration for 19 systems.

## Download

[v0.1.0 — Complete package and corrected update](https://github.com/robin2kk/Goldengames-RetroHub-Pro-PS5/releases/tag/v0.1.0)

**Use the downloads marked `Corrected`.** The author confirmed the working executable is **109,444 bytes**; the earlier 50,730-byte update is superseded.

- `Goldengames-RetroHub-Pro-0.1.0-Complete-Corrected.zip`: application folder with native runtime, return helpers and logo artwork for first-time RetroHub deployment.
- `Goldengames-RetroHub-Pro-0.1.0-Update-Corrected.zip`: executable and artwork for existing installations.
- `Goldengames-RetroHub-Pro-0.1.0-Source-Corrected.zip`: frontend, native build-tool/runtime-builder source, return-chord source and artwork. Use this source bundle to build; repository archives alone omit the toolchain archive/artwork.
- `Goldengames-RetroHub-Pro-Console-Overlays.zip`: optional console borders/presets for an overlay-enabled RetroArch build.
- `SHA256SUMS-Corrected.txt`: archive checksums.

See [INSTALL.md](INSTALL.md). The complete ZIP is a deployable application folder, not a PKG or an automatic ELF installer. Homebrew Launcher/websrv and RetroArch/cores must be installed separately.

## Features

- Horizontal 3D cover flow and original cover colors.
- Automatic cover downloads and scanning, including N64, PlayStation and Saturn.
- Light-blue interface with white header/footer, larger colored headings and PlayStation button symbols.
- Console name beneath the title; game and emulator names centered at the bottom.
- Synthesized menu sounds and controller navigation.
- RetroHub logo launch artwork.

## Compatibility and requirements

Tested by the author on **PS5 firmware 5.10**. Other firmware versions are untested by the author.

**Homebrew Launcher with websrv enabled on port 8080 is required to launch games.** RetroArch and its PS5-compatible cores are installed separately at `/data/homebrew/RetroArch/`. Both return helpers are included in the complete package. A clean first-time installation of the assembled archive has not been hardware-tested here.

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
