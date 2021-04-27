# Copyright (c) 2014-2019 Zano Project
# Copyright (c) 2014 The Cryptonote developers
# Distributed under the MIT/X11 software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

# Define CMake generator
system := $(shell uname)
ifneq (, $(findstring MINGW, $(system)))
  cmake_gen = -G 'MSYS Makefiles'
endif

cmake = cmake $(cmake_gen)

cmake_debug = $(cmake) -D CMAKE_BUILD_TYPE=Debug
cmake_release = $(cmake) -D CMAKE_BUILD_TYPE=Release

# Helper macro
define CMAKE
  mkdir -p $1 && cd $1 && $2 ../../
endef

build = build
dir_debug = $(build)/debug
dir_release = $(build)/release

all: release

release:
	$(eval command += $(cmake_release))
	$(call CMAKE,$(dir_release),$(command)) && $(MAKE)

debug:
	$(eval command += $(cmake_debug))
	$(call CMAKE,$(dir_debug),$(command)) && $(MAKE)

static: static-release
static-release:
	$(eval command += $(cmake_release) $(cmake_static))
	$(call CMAKE,$(dir_release),$(command)) && $(MAKE)

clean:
	rm -rf build

tags:
	ctags -R --sort=1 --c++-kinds=+p --fields=+iaS --extra=+q --language-force=C++ src

.PHONY: all release debug static static-release clean tags
