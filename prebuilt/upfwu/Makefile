include $(TOPDIR)/rules.mk
#include $(INCLUDE_DIR)/nls.mk
#include $(INCLUDE_DIR)/target.mk

PKG_NAME:=fwcheck
PKG_VERSION:=1.0.0
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

PKG_BUILD_PARALLEL:=1

define Package/fwcheck
  SECTION:=fwcheck
  CATEGORY:=Netgear
  TITLE:=FwCheck
  DEPENDS:=+libpthread +libc +d2 +libcrypt0 +libssl +libcurl
endef


define Package/fwcheck/description
 Controlled FW rollout
endef


define Build/Prepare
endef

define Build/Compile
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(INSTALL_DATA) ./files/include/* $(1)/usr/include

	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_BIN) ./files/lib/*.so $(1)/usr/lib
	$(INSTALL_BIN) ./files/lib/*.a $(1)/usr/lib
	
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) ./files/bin/caller $(1)/usr/bin
endef

define Package/fwcheck/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_BIN) ./files/lib/*.so $(1)/usr/lib
	$(INSTALL_BIN) ./files/lib/*.a $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/include
	$(INSTALL_DATA) ./files/include/* $(1)/usr/include
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) ./files/bin/caller $(1)/usr/bin
endef


$(eval $(call BuildPackage,fwcheck))
