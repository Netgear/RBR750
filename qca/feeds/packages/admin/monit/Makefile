#
# Copyright (C) 2006-2014 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=monit
PKG_VERSION:=5.14
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://mmonit.com/monit/dist
PKG_MD5SUM:=1b3ae1eb08a0914402a8764e5689c1c5

PKG_LICENSE:=AGPL-3.0
PKG_LICENSE_FILES:=COPYING

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(BUILD_VARIANT)/$(PKG_NAME)-$(PKG_VERSION)
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk

define Package/monit/Default
  SECTION:=admin
  CATEGORY:=Administration
  DEPENDS:= +libpthread
  TITLE:=System services monitoring utility
  URL:=http://mmonit.com/monit/
  MAINTAINER:=Etienne CHAMPETIER <champetier.etienne@gmail.com>
endef

define Package/monit/Default/description
	An utility for monitoring services on a Unix system
endef

define Package/monit
$(call Package/monit/Default)
  DEPENDS+= +libopenssl
  TITLE+= (with SSL support)
  VARIANT:=ssl
endef

define Package/monit/description
$(call Package/monit/Default/description)
	This package is built with SSL support.
endef

define Package/monit-nossl
$(call Package/monit/Default)
  TITLE+= (without SSL support)
  VARIANT:=nossl
endef

define Package/monit-nossl/description
$(call Package/monit/Default/description)
	This package is built without SSL support.
endef

CONFIGURE_ARGS += \
	--without-pam \
	libmonit_cv_setjmp_available=yes \
	libmonit_cv_vsnprintf_c99_conformant=yes

ifeq ($(BUILD_VARIANT),ssl)
	CONFIGURE_ARGS += \
		--with-ssl \
		--with-ssl-dir="$(STAGING_DIR)/usr"
endif

ifeq ($(BUILD_VARIANT),nossl)
	CONFIGURE_ARGS += \
		--without-ssl
endif

define Package/monit/conffiles
/etc/monitrc
endef

define Package/monit/install
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_CONF) ./files/monitrc_router $(1)/etc/
	$(INSTALL_CONF) ./files/monitrc_satellite $(1)/etc/
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/monit.init $(1)/etc/init.d/monit
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) ./files/error_recovery.sh $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/monit $(1)/usr/bin/
endef

Package/monit-nossl/conffiles = $(Package/monit/conffiles)
Package/monit-nossl/install = $(Package/monit/install)

$(eval $(call BuildPackage,monit))
$(eval $(call BuildPackage,monit-nossl))
