# RetroHub Pro source

The frontend sources in src/ correspond to the professional UI project used for this update.

Build on Linux/WSL with LLVM 18, llvm-config-18, clang-18, clang++-18, make, ninja, Python 3, and zlib development headers. Set LLVM_CONFIG and PS5_CLANG for LLVM 18, then run bash BUILD.sh. The script downloads pinned public dependencies and builds the native app and return-chord helper. See THIRD_PARTY_NOTICES.md and the toolchain source archive for attribution and build tooling. Existing return-watchdog is a runtime prerequisite and is not supplied by this update.

The native-toolchain-source.tar.gz archive contains the complete native build tool source. It is not an SDK binary bundle. No proprietary runtime is supplied in this source package. The existing installation supplies its runtime.

The author supplied the tested console executable separately; binary reproducibility was not checked.

The author-confirmed executable is 109444 bytes. Supplied return-watchdog source is not included: it was not recovered here. The native runtime and ELF files were supplied separately by the author; this source bundle is not asserted to reproduce them bit-for-bit.
