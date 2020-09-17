include $(TOPDIR)/rules.mk
#include $(INCLUDE_DIR)/nls.mk
#include $(INCLUDE_DIR)/target.mk

PKG_NAME:=dil
PKG_VERSION:=0.1.0
PKG_RELEASE:=1

#PKG_BUILD_DIR:=./files/

include $(INCLUDE_DIR)/package.mk

define Package/dil
  SECTION:=dil
  CATEGORY:=Netgear
  TITLE:=Dil
  DEPENDS:=+libpthread +librt +libc +d2 +libcrypt0 +libssl +libcurl
endef


define Package/dil/description
 Netgear Data Abstraction Layer 2
endef

define Build/Prepare
endef

define Build/Compile
endef

define Package/dil/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) ./files/bin/fing_dil $(1)/usr/bin 
	$(INSTALL_BIN) ./files/bin/diltest $(1)/usr/bin
	$(INSTALL_BIN) ./files/bin/jsontest $(1)/usr/bin
	
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_BIN) ./files/lib/libd2dil.so $(1)/usr/lib
	$(INSTALL_BIN) ./files/lib/libdalcjson.so $(1)/usr/lib
	
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/dil.init $(1)/etc/init.d/dil
	$(INSTALL_BIN) ./files/dil_boot.sh $(1)/etc/init.d/dil_boot.sh

	$(INSTALL_DIR) $(1)/etc/dil
	$(INSTALL_BIN) ./files/etc/file1.txt $(1)/etc/dil
	$(INSTALL_BIN) ./files/etc/file2.txt $(1)/etc/dil
	
endef


$(eval $(call BuildPackage,dil))
