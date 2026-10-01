# Source and attribution

GPL-3.0-or-later. Corresponding frontend, helper and build tool source included.

- Native startup, module writer, clean-room runtime and VideoOut adapter:
  BlackBearReloaded/ps5-native-app-boilerplate,
  c795cdf80bc3d52e83cf8dae221ba525ada0308b.
- PNG declarations/link stub: BlackBearReloaded/ProsperoLight,
  282030911e1f4ae20edb2457d0c575c88b1090ea.
- Controller ABI checked against the public ProsperoLight/PS5_RetroArch
  declarations. New chord/lifecycle/command helper source is included.
- Helper toolchain: ps5-payload-dev/sdk v0.42, GPL-3.0-or-later. Its public
  headers, stubs, CRT and source are fetched by the included pinned setup.
  Process discovery follows its public samples/ps/main.c.
- HTTP declarations checked against OpenOrbis public headers; no OpenOrbis
  binary is included.
- RetroArch's public command.c documents GET_STATUS. The helper requests
  the standard QUIT command; no emulator binary or core is redistributed.
- Runtime cover artwork remains with its respective owners; no ROMs,
  BIOS files or game artwork are bundled.

Links:
https://github.com/blackbearreloaded/ps5-native-app-boilerplate
https://github.com/blackbearreloaded/ProsperoLight
https://github.com/mihawk-99/PS5_RetroArch
https://github.com/ps5-payload-dev/sdk
https://github.com/libretro/RetroArch
