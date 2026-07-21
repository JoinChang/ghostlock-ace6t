# GhostLock — OnePlus Locked Bootloader Jailbreak

Kernel exploit for OnePlus devices with locked bootloader. Achieves root + KernelSU installation without unlocking bootloader or modifying boot image. Runtime auto-detection of kernel version with multi-device offset table.

<p align="center">
  <img src="assets/screenshot.jpg" width="300" alt="GhostLock running on OnePlus Ace 6T with KernelSU (LKM, Jailbreak mode)">
</p>

## Vulnerability

**CVE-2026-43499** — Futex PI (Priority Inheritance) Use-After-Free

Affects Linux kernel 2.6.39 ~ 7.1. Fixed in mainline 7.1 (commit `3bfdc63936dd`). The tested OnePlus Android 16 kernels remain vulnerable.

The `pselect6` syscall copies `fd_set` data onto the kernel stack. When combined with the futex PI waiter mechanism, a freed stack frame can be reclaimed as an `rt_mutex_waiter` structure. The rb-tree rebalance during PI chain walk then writes controlled values to arbitrary kernel addresses.

## Tested Devices

| Device | SoC | OS | Kernel | Status |
|--------|-----|-----|--------|--------|
| OnePlus Ace 6T | SM8845 (Snapdragon 8s Elite) | Android 16, ColorOS 16.0.2.403 | `6.12.38-...-ab14275539` | **Verified** |
| OnePlus Ace 6T | SM8845 (Snapdragon 8s Elite) | Android 16, ColorOS 16.0.8.301 | `6.12.38-...-ab14552068` | **Verified** |
| OnePlus Pad 3 | SM8750 (Snapdragon 8 Elite) | Android 16, OxygenOS 16.0.9.400 | `6.6.118-...-ab15114928` | **Verified** |

### Untested (offsets extracted, not verified on device)

| Device | Kernel | Notes |
|--------|--------|-------|
| OnePlus 15 | `6.12.23-...-ab14541642` | Offsets extracted from OTA boot.img |

Other OnePlus kernel 6.6.x or 6.12.x builds can be added with matching offsets.

## Exploit Flow

```
Write 1 (mode=1)  →  SELinux enforcing = 0
                      (low byte of kernel ptr = 0x00)

Write 2 (mode=2)  →  task->cred = init_cred
                      (uid=0, all capabilities)

Root shell         →  ksud late-load (KernelSU LKM)
                   →  su -c load_policy (fix SELinux policycap)
                   →  dynamic manager registration
```

### Bootstrap Mode (phone standalone)

```
App (seccomp)  →  Write 1 (no perf needed)
               →  mini-adb connect TCP 5555 (RSA auth)
               →  adb shell: full exploit (perf works, no seccomp)
               →  root → KSU → network fix
```

## Kernel Notes

### Runtime Kernel Matching

Offsets are stored in `offsets.h` as a lookup table keyed by `uname -r`. The exploit auto-selects at startup and refuses to run on unknown kernels.

```c
static const struct kernel_offsets known_offsets[] = {
  OFFSETS_ENTRY("6.12.38-android16-5-g8c67d4274c0a-ab14275539-4k", ...),
  OFFSETS_ENTRY("6.12.38-android16-5-g844001fb8721-ab14552068-4k", ...),
  OFFSETS_ENTRY("6.12.23-android16-5-gb2a876903b49-ab14541642-4k", ...),
  OFFSETS_ENTRY("6.6.89-android15-8-g97a9aaefab9a-ab14519050-4k", ...),
  { .uname_r = NULL }
};
```

### Version Differences

- Kernel 6.12 uses the Rust ashmem symbol layout present in the original Ace 6T target.
- Kernel 6.6 uses C ashmem symbols; `ashmem_misc` is a `struct miscdevice`, so the fops pointer is `ashmem_misc + 0x10`.
- Struct offsets, pselect waiter words, owner-chain behavior, and route timeout are selected from the per-kernel profile in `src/devices/offsets.h`.
- `perf_find_task` on ARM64 uses register mask bits 0-31 only.

### Mini ADB Client

Built-in ADB protocol implementation (`miniadb.c`) for bootstrap mode:
- TCP connect to localhost:5555
- RSA authentication via `dlopen(libcrypto.so)`
- Shell command execution without app seccomp restrictions

### SELinux Network Fix

Write 1 corrupts `selinux_state` bytes beyond `enforcing`, breaking `netif egress` permissions. Fixed by running `load_policy` from KSU's `su` context after exploit completes.

## Build

```bash
ANDROID_NDK_HOME=/path/to/android-ndk make
```

On Windows, the Makefile auto-selects the NDK `windows-x86_64` toolchain. Override with `HOST_TAG=...` if needed.

## Prerequisites

### ksud (required for KSU installation)

GhostLock only provides root. KernelSU installation depends on **ksud** — a binary that contains embedded `kernelsu.ko` modules for each KMI version. The root script finds ksud on device and calls `ksud late-load --kmi` using the running kernel's KMI, for example `android16-6.12` or `android15-6.6`.

| Method | Steps |
|--------|-------|
| **ReSukiSU APK** | Install [ReSukiSU](https://github.com/ReSukiSU/ReSukiSU). Official release bundles `libksud.so`. |
| **CI release** | Download `ksud-aarch64-linux-android.zip` from [ReSukiSU CI](https://github.com/cctv18/ReSukiSU_CI/releases) |

> Without ksud, the exploit achieves root (uid=0) but KSU won't be installed and `su` won't persist.

## Setup (one-time)

```bash
adb tcpip 5555
adb push ~/.android/adbkey /data/local/tmp/a/adbkey
adb push ghostlock /data/local/tmp/a/e
adb shell chmod 755 /data/local/tmp/a/e
```

## Usage

```bash
/data/local/tmp/a/e              # Full exploit (adb shell)
/data/local/tmp/a/e --bootstrap  # Phone standalone (app context)
/data/local/tmp/a/e --write1     # SELinux disable only
```

## Adding New Devices / Kernel Versions

Use device `kallsyms` for global symbols and BTF for struct fields.

### Extract offsets

```bash
# 1. Extract kernel
python -c "import struct; d=open('boot.img','rb').read(); open('kernel','wb').write(d[4096:4096+struct.unpack_from('<I',d,8)[0]])"

# 2. Global symbols
python tools/extract_target.py    # 28 offsets, auto-validated

# 3. Struct fields (BTF)
python tools/extract_btf.py kernel  # 57 offsets, auto-validated

# 4. Add to offsets.h, rebuild
```

## Files

| File | Description |
|------|-------------|
| `src/core/main.c` | Exploit entry, Write 1/2, bootstrap, root script |
| `src/core/fops.c` | pselect route, PI write mechanism |
| `src/core/util.c` | Heap spray, kernelsnitch, slab drain |
| `src/core/miniadb.c` | Mini ADB client (TCP + RSA auth) |
| `src/core/common.h` | Timing parameters, macros |
| `src/core/target.h` | Memory layout, struct field constants |
| `src/devices/offsets.h` | Aggregates all device offset tables |
| `src/devices/<device>/offsets.h` | Per-device kernel offset entries |
| `src/core/slide.c` | SLIDE kernel address leak |
| `src/core/pipe.c` | Pipe buffer manipulation |
| `src/core/root.c` | Root shell setup |
| `tools/extract_target.py` | Offset extraction from kallsyms |
| `tools/extract_btf.py` | Struct offset extraction from BTF |

## License

For authorized security research and educational purposes only.
