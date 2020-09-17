include $(TOPDIR)/rules.mk
#include $(INCLUDE_DIR)/nls.mk
#include $(INCLUDE_DIR)/target.mk

PKG_NAME:=pushntf
PKG_VERSION:=1.0.0
PKG_RELEASE:=1

#PKG_BUILD_DIR:=./files/

include $(INCLUDE_DIR)/package.mk

define Package/pushntf
  SECTION:=pushntf
  CATEGORY:=Netgear
  TITLE:=PNTF
  DEPENDS:=+libpthread +librt +libc +d2 +dil
endef


define Package/pushntf/description
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
	
endef

define Package/pushntf/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) ./files/usr/bin/dalh $(1)/usr/bin 
	$(INSTALL_BIN) ./files/usr/bin/pushntftest $(1)/usr/bin
	
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/DALH.init $(1)/etc/init.d/DALH
	
endef


$(eval $(call BuildPackage,pushntf))
