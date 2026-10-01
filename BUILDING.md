# RetroHub Pro source

The frontend sources in src/ correspond to the professional UI project used for this update.

Build on Linux/WSL with LLVM 18, llvm-config-18, clang-18, clang++-18, make, ninja, Python 3, and zlib development headers. Set LLVM_CONFIG and PS5_CLANG for LLVM 18, then run bash BUILD.sh. The script downloads pinned public dependencies and builds the native app and return-chord helper. See THIRD_PARTY_NOTICES.md and the toolchain source archive for attribution and build tooling. Existing return-watchdog is a runtime prerequisite and is not supplied by this update.

The native-toolchain-source.tar.gz archive contains the complete native build tool source. It is not an SDK binary bundle. No proprietary runtime is supplied in this source package. The existing installation supplies its runtime.

The author supplied the tested console executable separately; binary reproducibility was not checked.

Native deployment requires SELF containers, not raw ELF renamed to eboot.bin/libc.prx. The recovered raw payloads (109444-byte executable, 1335962-byte runtime) were wrapped with the project's native tool. The author tested the resulting 50730-byte eboot.bin and 1284674-byte libc.prx together on PS5 firmware 5.10 / etaHEN 2.6B. See INSTALL.md for hashes. Preserve the SELF wrapping steps in tools/build.sh and tools/rebuild-libc.sh when using the original toolchain. A matching filename or byte count alone is not proof of correct format.

Supplied return-watchdog source is not included: it was not recovered here. A bit-for-bit source/binary rebuild is not asserted.
