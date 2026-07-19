API ?= 35

NDK_ROOT ?= $(or $(ANDROID_NDK_HOME),$(ANDROID_NDK_ROOT))

ifeq ($(OS),Windows_NT)
HOST_TAG ?= windows-x86_64
CLANG_EXT ?= .cmd
else
UNAME_S := $(shell uname -s 2>/dev/null)
ifeq ($(UNAME_S),Darwin)
HOST_TAG ?= darwin-x86_64
else
HOST_TAG ?= linux-x86_64
endif
CLANG_EXT ?=
endif

NDK_CC := $(NDK_ROOT)/toolchains/llvm/prebuilt/$(HOST_TAG)/bin/aarch64-linux-android$(API)-clang$(CLANG_EXT)

SRCS := \
  src/core/main.c \
  src/core/util.c \
  src/core/slide.c \
  src/core/fops.c \
  src/core/pipe.c \
  src/core/root.c \
  src/core/miniadb.c

CFLAGS := -O2 -Wall -Wno-unused-parameter -Wno-sign-compare -Wno-unused-function \
  -Isrc/core -Isrc/devices
LDFLAGS := -fPIE -pie -pthread

.PHONY: all clean

all: ghostlock

ghostlock: $(SRCS)
	$(NDK_CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

clean:
	rm -f ghostlock
