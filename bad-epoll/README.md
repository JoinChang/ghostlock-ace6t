# Bad Epoll (CVE-2026-46242) — Find X9 Ultra

Kernel exploit for OPPO Find X9 Ultra (SM8750, kernel 6.12.58, ARM64 4K pages).

Unlike GhostLock (pselect stack overlay), Bad Epoll uses an epoll close-vs-close
race UAF with no stack layout dependency. Bypasses PAC + kCFI via data-only attack.

## Target

- Device: OPPO Find X9 Ultra
- Kernel: `6.12.58-android16-6-g7704a1ae279b-ab15213644-4k`
- Page size: 4096

## Build

```bash
NDK=/path/to/android-ndk
CC=$NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android35-clang
$CC -static -O2 -o bad_epoll exploit.c
```

## Run

```bash
adb push bad_epoll /data/local/tmp/
adb shell chmod 755 /data/local/tmp/bad_epoll
adb shell /data/local/tmp/bad_epoll 2>&1 | tee output.txt
```

**IMPORTANT**: Capture the full output and share it for debugging — even if it
fails, the output contains critical diagnostic information.

## Status

- Race trigger: verified in QEMU
- Cross-cache: untested on real hardware (expected to work under memory pressure)
- KASLR bypass: via ARM64 linear map (no info leak needed)
- Write primitives: write-zero + write-one (byte)

## Worst case

- Race timeout → clean exit
- Cross-cache miss → retry or clean exit
- Kernel panic → device reboots (no data loss, no brick risk)
