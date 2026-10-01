# GoldenGames RetroHub Pro

Native PS5 retro gaming frontend by GoldenGames, with 3D cover flow, automatic cover downloads, ROM scanning, synthesized menu sounds, and RetroArch integration for 19 systems.

## Download

[v0.1.0 — Professional UI update](https://github.com/robin2kk/Goldengames-RetroHub-Pro-PS5/releases/tag/v0.1.0)

**This release updates an existing working RetroHub Pro installation. It is not a standalone installer.** RetroArch, cores, native runtime, and return helpers must already be installed. See [INSTALL.md](INSTALL.md).

Download `Goldengames-RetroHub-Pro-0.1.0-Update.zip` to update the application. Download `Goldengames-RetroHub-Pro-0.1.0-Source.zip` for the complete source/build bundle, including the native build-tool source and artwork. The repository exposes the frontend and helper source for browsing; use the complete source ZIP to build.

## Features

- Horizontal 3D cover flow and original cover colors.
- Automatic cover downloads and scanning, including N64, PlayStation and Saturn.
- Light-blue interface with white header/footer, larger colored headings and PlayStation button symbols.
- Console name beneath the title; game and emulator names centered at the bottom.
- Synthesized menu sounds and controller navigation.
- RetroHub logo launch artwork.

## Compatibility and requirements

Tested by the author on **PS5 firmware 5.10**. Other firmware versions are untested by the author.

RetroArch is installed separately at `/data/homebrew/RetroArch/`; game launching requires the Homebrew Launcher/websrv service. Existing return-watchdog and return-chord helpers are required for the return flow.

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

See [LICENSE](LICENSE), [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md), and [BUILDING.md](BUILDING.md). The tested executable was supplied by the author from the working console; a bit-for-bit source/binary match has not been verified here.

No RetroArch executable, cores, ROMs, BIOS files, saves, downloaded covers, or personal configurations are included.
