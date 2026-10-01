#!/usr/bin/env bash
set -euo pipefail
source_root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
build_root=${1:-"$source_root/build-environment"}
mkdir -p "$build_root"
tar -xzf "$source_root/native-toolchain-source.tar.gz" -C "$build_root"
cp -a "$source_root/src" "$build_root/retrohub-src"
cp "$source_root/param.json" "$build_root/retrohub-param.json"
cp "$source_root/sce_sys/"* "$build_root/sce_sys/"
cd "$build_root"
make deps USE_CCACHE=0 PS5_CLANG="${PS5_CLANG:-clang}"
sdk="$build_root/.deps/native/ps5-payload-sdk"
PS5_PAYLOAD_SDK="$sdk" PS5_CLANG="${PS5_CLANG:-clang}" USE_CCACHE=0 sh tooling/prospero-clang18 -std=c11 -O2 -fPIC -c "$source_root/tooling/pngdec_link_stub.c" -o "$build_root/pngdec_link_stub.o"
"$sdk/bin/prospero-lld" --shared -soname libScePngDec.prx -o "$sdk/target/lib/libScePngDec.so" "$build_root/pngdec_link_stub.o"
make app USE_CCACHE=0 PS5_CLANG="${PS5_CLANG:-clang}" APP_SOURCE_DIR=retrohub-src APP_PARAM=retrohub-param.json
make -C "$source_root/payload" PS5_PAYLOAD_SDK="$sdk"
cp "$source_root/payload/return-chord.elf" "$build_root/dist/PPSA99202/"
cp "$source_root/retrohub-return-session.cfg" "$build_root/dist/PPSA99202/"
