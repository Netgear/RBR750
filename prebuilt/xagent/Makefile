
include $(TOPDIR)/rules.mk

PKG_NAME:=xagent
PKG_VERSION:=3.2.7

PKG_BUILD_DIR:=$(BUILD_DIR)/xagent-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/xagent
  SECTION:=arlo
  CATEGORY:=arlo
  TITLE:=Netgear xagent
  DEPENDS:=+libpthread +curl +libopenssl +d2
endef

define Package/xagent/description
 Netgear arlo xagent
endef

define Build/Prepare
endef

define Build/Compile
endef

define Package/xagent/install
	$(INSTALL_DIR) $(1)/opt
	$(CP) ./files/opt/* $(1)/opt
	
	$(INSTALL_DIR) $(1)/usr
	$(CP) ./files/usr/* $(1)/usr
	
	$(INSTALL_DIR) $(1)/www
	$(CP) ./files/www/* $(1)/www

	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/XAgent.init $(1)/etc/init.d/XAgent

	$(INSTALL_DIR) $(1)/etc/xagent
	$(CP) ./files/envswitch/* $(1)/etc/xagent

endef

$(eval $(call BuildPackage,xagent))
