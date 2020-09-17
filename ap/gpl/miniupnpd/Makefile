include ../config.in
include ../config.mk

PACKAGE_VERSION := 1.9.20150609
PACKAGE_DIR := miniupnpd-$(PACKAGE_VERSION)



CFLAGS="-Os -w -pipe -fno-caller-saves -fhonour-copts -Wno-error=unused-but-set-variable -Wno-error=unused-result \
	-I$(STAGING_DIR)/usr/include -I$(STAGING_DIR)/include -I$(TOOLCHAIN_DIR)/usr/include -I$(TOOLCHAIN_DIR)/include \
	-I$(ACOSTOPDIR)/shared -DLINUX26"

ifeq ($(PLATFORM),arm-openwrt)
CFLAGS += -march=armv7-a -mtune=cortex-a7 -mfloat-abi=soft
endif

#export PKG_CONFIG_PATH="$(STAGING_DIR)/usr/lib/pkgconfig:$(STAGING_DIR)/usr/share/pkgconfig"

LDFLAGS="-L$(STAGING_DIR)/usr/lib -L$(STAGING_DIR)/lib -L$(TOOLCHAIN_DIR)/usr/lib  -L$(TOOLCHAIN_DIR)/lib \
	-L$(ACOSTOPDIR)/shared  -lacos_shared \
	-luci -L$(ACOSTOPDIR)/nvram -lnvram"


# Add environment for pkg-config
HOST_DIR=$(STAGING_DIR)/../host
PATH:=$(STAGING_DIR)/host/bin:$(HOST_DIR)/bin:$(PATH)
export PATH

export STAGING_PREFIX=$(STAGING_DIR)/usr

export PKG_CONFIG=$(STAGING_DIR_HOST)/bin/pkg-config

export PKG_CONFIG_PATH=$(STAGING_DIR)/usr/lib/pkgconfig:$(STAGING_DIR)/usr/share/pkgconfig
export PKG_CONFIG_LIBDIR=$(STAGING_DIR)/usr/lib/pkgconfig:$(STAGING_DIR)/usr/share/pkgconfig



MAKE_FLAGS += \
        TEST=0 \
        LIBS="" \
	CC="$(CROSS_COMPILE)gcc -DIPTABLES_143 -lip4tc -lip6tc" \
	CFLAGS=$(CFLAGS) \
	LDFLAGS=$(LDFLAGS) \
	CONFIG_OPTIONS="--ipv6 --leasefile --vendorcfg --portinuse --strict" \
        -f Makefile.linux \
        miniupnpd

all: build


#$(PACKAGE_DIR)/config.in:
#	cd $(PACKAGE_DIR); ./genconfig.sh --ipv6 --igd2 --leasefile

build:
	cd $(PACKAGE_DIR); make $(MAKE_FLAGS)


#	cd $(PACKAGE_DIR); make -j1 -f Makefile.linux $(CC) miniupnpd

clean:
	cd $(PACKAGE_DIR); make clean -f Makefile.linux


install:
	make -j1 -C $(PACKAGE_DIR) -f Makefile.linux PREFIX=$(TARGETDIR) install
