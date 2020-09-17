include $(TOPDIR)/rules.mk
#include $(INCLUDE_DIR)/nls.mk
#include $(INCLUDE_DIR)/target.mk

PKG_NAME:=bst
PKG_VERSION:=0.1.0
PKG_RELEASE:=1

#PKG_BUILD_DIR:=./files/

include $(INCLUDE_DIR)/package.mk

define Package/bst
  SECTION:=bst
  CATEGORY:=Netgear
  TITLE:=BST
  DEPENDS:=+libpthread +librt +libc +d2 +libcrypt0 +libssl +libcurl
endef


define Package/bst/description
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
	$(CP) -rp ./files/usr/lib/*.so $(1)/usr/lib
	
endef

define Package/bst/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) ./files/usr/bin/bst_daemon $(1)/usr/bin 
	$(INSTALL_BIN) ./files/usr/bin/bsttest $(1)/usr/bin
	
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_BIN) ./files/usr/lib/libd2bst.so $(1)/usr/lib
	$(INSTALL_BIN) ./files/usr/lib/libd2bst.a $(1)/usr/lib
	
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/bst.init $(1)/etc/init.d/bst

	$(INSTALL_DIR) $(1)/etc/bst
endef


$(eval $(call BuildPackage,bst))
