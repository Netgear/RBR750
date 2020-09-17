include $(TOPDIR)/rules.mk

PKG_NAME:=remGenie
PKG_VERSION:=3.2.8
PKG_RELEASE:=1

#PKG_BUILD_DIR:=$(BUILD_DIR)/remGenie-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/remGenie
  SECTION:=arlo
  CATEGORY:=arlo
  TITLE:=Netgear Remote Genie
  DEPENDS:=+libpthread +curl +libopenssl +d2
endef

define Package/remGenie/description
  Netgear Remote Genie
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef

define Build/Compile
endef

define Package/remGenie/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) ./files/usr/bin/genie_handler $(1)/usr/bin
	
	$(INSTALL_DIR) $(1)/www/cgi-bin
	$(INSTALL_BIN) ./files/www/cgi-bin/genie.cgi $(1)/www/cgi-bin

	$(INSTALL_DIR) $(1)/${FS_DIR}/etc/init.d
	$(INSTALL_BIN) ./files/remGenie.init $(1)/etc/init.d/remGenie
endef

$(eval $(call BuildPackage,remGenie))


