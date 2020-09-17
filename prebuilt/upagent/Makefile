include $(TOPDIR)/rules.mk
#include $(INCLUDE_DIR)/nls.mk
#include $(INCLUDE_DIR)/target.mk

PKG_NAME:=upagent
PKG_VERSION:=1.0.0
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

#PKG_BUILD_PARALLEL:=1

define Package/upagent
  SECTION:=upagent
  CATEGORY:=Netgear
  TITLE:=UpAgent
  DEPENDS:=+libpthread +libc +d2 +libcrypt0 +libssl +libcurl 
endef

define Package/upagent/description
 UpAgent for Orbi
endef

define Build/Prepare
endef

define Build/Compile
endef

define Package/upagent/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) ./files/usr/bin/upagent $(1)/usr/bin

	$(INSTALL_DIR) $(1)/${FS_DIR}/etc/init.d
	$(INSTALL_BIN) ./files/UPAgent.init $(1)/etc/init.d/UPAgent

endef

$(eval $(call BuildPackage,upagent))

