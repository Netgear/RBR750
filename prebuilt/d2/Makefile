include $(TOPDIR)/rules.mk
#include $(INCLUDE_DIR)/nls.mk
#include $(INCLUDE_DIR)/target.mk

PKG_NAME:=d2
PKG_VERSION:=0.1.0
PKG_RELEASE:=1

#PKG_BUILD_DIR:=./files/

include $(INCLUDE_DIR)/package.mk

define Package/d2
  SECTION:=d2
  CATEGORY:=Netgear
  TITLE:=D2
  DEPENDS:=+libpthread +librt +libc
endef


define Package/d2/description
 Netgear Data Abstraction Layer 2
endef

define Build/Prepare
endef

define Build/Compile
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(INSTALL_DIR) $(1)/usr/lib
	
	$(CP) -rp ./files/include/* $(1)/usr/include
	$(CP) -rp ./files/lib/*.so $(1)/usr/lib
	$(CP) -rp ./files/lib/*.a $(1)/usr/lib
	
endef

define Package/d2/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) ./files/bin/d2d $(1)/usr/bin 
	$(INSTALL_BIN) ./files/bin/d2 $(1)/usr/bin
	
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_BIN) ./files/lib/libd2lib.so $(1)/usr/lib

	$(INSTALL_DIR) $(1)/usr/include
	$(CP) -rp ./files/include/* $(1)/usr/include
	 
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/d2.init $(1)/etc/init.d/d2
	$(INSTALL_BIN) ./files/d2_countryname.sh $(1)/etc/init.d/d2_countryname.sh
	
	$(INSTALL_DIR) $(1)/etc/d2
	$(CP) -rp ./files/beta/* $(1)/etc/d2
endef


$(eval $(call BuildPackage,d2))
