#***************************************************************************
#***
#***    Copyright 2005-2019  Hon Hai Precision Ind. Co. Ltd.
#***    All Rights Reserved.
#***    No portions of this material shall be reproduced in any form without the
#***    written permission of Hon Hai Precision Ind. Co. Ltd.
#***
#***    All information contained in this document is Hon Hai Precision Ind.  
#***    Co. Ltd. company private, proprietary, and trade secret property and 
#***    are protected by international intellectual property laws and treaties.
#***
#****************************************************************************

include ../config.mk
include ../config.in

WLAN_PROC_PIL_IMAGES:=$(TOPDIR)/../chipcode/WLAN.HK.2.0/wlan_proc/build/ms/bin/PIL_IMAGES
WLAN_PROC_HWV2_PIL_IMAGES:=$(TOPDIR)/../chipcode/WLAN.HK.2.0/wlan_proc_hwv2/build/ms/bin/PIL_IMAGES
BDF_QCA8074:=$(TOPDIR)/../chipcode/WLAN.HK.2.0/wlan_proc/wlan/phyrf_svc/tools/bdfUtil/device/bdf/qca8074
BDF_QCA8074_V2:=$(TOPDIR)/../chipcode/WLAN.HK.2.0/wlan_proc_hwv2/wlan/phyrf_svc/tools/bdfUtil/device/bdf/qca8074_v2

ifeq ($(CONFIG_CABLE_ORBI_HW),y)
HW_boardData:=HW20181016_CBR40
REGULATORY := FCC
endif


ifeq ($(CONFIG_ORBI_AX_HW),y)
ifeq ($(CONFIG_ORBI_AX_HW_HE_V2),y)
HW_boardData:=RBR850_HE_V2
else
HW_boardData:=RBR850_HE_V1.2
endif
ifeq ($(CONFIG_ORBI_AX_HW_OAK),y)
HW_boardData:=RBR750_OAK
endif
REGULATORY := FCC IC ETSI CTICK SRRC MKK SGP VNM IND
endif


all: install

install:
ifeq ($(CONFIG_CABLE_ORBI_HW),y)
	mkdir -p $(TARGETDIR)/lib/firmware/IPQ4019/hw.1
	mkdir -p $(TARGETDIR)/lib/firmware/QCA9888/hw.2
endif
ifeq ($(CONFIG_ORBI_AX_HW),y)
	mkdir -p $(TARGETDIR)/lib/firmware/IPQ8074
ifeq ($(CONFIG_ORBI_AX_HW_HE_V2),n)
	mkdir -p $(TARGETDIR)/lib/firmware/QCA9984/hw.1
endif
endif


ifeq ($(CONFIG_CABLE_ORBI_HW),y)
	###Dakota IPQ4019
	install -m 644 ./$(HW_boardData)/FCC/boardData_1_0_IPQ4019_DK04_2G.bin $(TARGETDIR)/lib/firmware/IPQ4019/hw.1/boarddata_0.bin
	install -m 644 ./$(HW_boardData)/FCC/boardData_1_0_IPQ4019_DK04_2G.bin $(TARGETDIR)/lib/firmware/IPQ4019/hw.1/boardData_1_0_IPQ4019_DK04_2G.bin
	install -m 644 ./$(HW_boardData)/FCC/boardData_1_0_IPQ4019_DK04_5G.bin $(TARGETDIR)/lib/firmware/IPQ4019/hw.1/boarddata_1.bin
	install -m 644 ./$(HW_boardData)/FCC/boardData_1_0_IPQ4019_DK04_5G.bin $(TARGETDIR)/lib/firmware/IPQ4019/hw.1/boardData_1_0_IPQ4019_DK04_5G.bin
	for i in ${REGULATORY} ; do \
	mkdir -p $(TARGETDIR)/lib/firmware/IPQ4019/hw.1/$$i ; \
	install -m 644 ./$(HW_boardData)/$$i/boardData_1_0_IPQ4019_DK04_2G.bin $(TARGETDIR)/lib/firmware/IPQ4019/hw.1/$$i/boarddata_0.bin ; \
	install -m 644 ./$(HW_boardData)/$$i/boardData_1_0_IPQ4019_DK04_2G.bin $(TARGETDIR)/lib/firmware/IPQ4019/hw.1/$$i/boardData_1_0_IPQ4019_DK04_2G.bin ; \
	install -m 644 ./$(HW_boardData)/$$i/boardData_1_0_IPQ4019_DK04_5G.bin $(TARGETDIR)/lib/firmware/IPQ4019/hw.1/$$i/boarddata_1.bin ; \
	install -m 644 ./$(HW_boardData)/$$i/boardData_1_0_IPQ4019_DK04_5G.bin $(TARGETDIR)/lib/firmware/IPQ4019/hw.1/$$i/boardData_1_0_IPQ4019_DK04_5G.bin ; \
	done


	###Bersa QCA9888
	rm -f $(TARGETDIR)/lib/firmware/QCA9888/hw.2/boarddata_0.bin
	rm -f $(TARGETDIR)/lib/firmware/QCA9888/hw.2/boarddata_2.bin
	install -m 644 ./$(HW_boardData)/FCC/boardData_2_0_QCA9888_5G_YA105.bin $(TARGETDIR)/lib/firmware/QCA9888/hw.2/boardData_2_0_QCA9888_5G_YA105.bin
	install -m 644 ./$(HW_boardData)/FCC/boardData_2_0_QCA9888_5G_YA105.bin $(TARGETDIR)/lib/firmware/QCA9888/hw.2/boarddata_0.bin
	install -m 644 ./$(HW_boardData)/FCC/boardData_2_0_QCA9888_5G_YA105.bin $(TARGETDIR)/lib/firmware/QCA9888/hw.2/boarddata_2.bin
   

	for i in ${REGULATORY} ; do \
	mkdir -p $(TARGETDIR)/lib/firmware/QCA9888/hw.2/$$i ; \
	install -m 644 ./$(HW_boardData)/$$i/boardData_2_0_QCA9888_5G_YA105.bin $(TARGETDIR)/lib/firmware/QCA9888/hw.2/$$i/boardData_2_0_QCA9888_5G_YA105.bin ; \
	install -m 644 ./$(HW_boardData)/$$i/boardData_2_0_QCA9888_5G_YA105.bin $(TARGETDIR)/lib/firmware/QCA9888/hw.2/$$i/boarddata_0.bin ; \
	install -m 644 ./$(HW_boardData)/$$i/boardData_2_0_QCA9888_5G_YA105.bin $(TARGETDIR)/lib/firmware/QCA9888/hw.2/$$i/boarddata_2.bin ; \
	done
endif


ifeq ($(CONFIG_ORBI_AX_HW),y)
ifeq ($(CONFIG_ORBI_AX_HW_HE_V2),y)
	###PIL_IMAGES
	install -m 644 -D $(WLAN_PROC_HWV2_PIL_IMAGES)/*.* $(TARGETDIR)/lib/firmware/IPQ8074

	###HAWKEYE IPQ8074
	install -m 644 ./$(HW_boardData)/FCC/bdwlan.b215 $(TARGETDIR)/lib/firmware/IPQ8074/bdwlan.b215
	install -m 644 ./$(HW_boardData)/FCC/bdwlan.b215 $(TARGETDIR)/lib/firmware/IPQ8074/bdwlan.bin

	for i in ${REGULATORY} ; do \
	mkdir -p $(TARGETDIR)/lib/firmware/IPQ8074/$$i ; \
	install -m 644 ./$(HW_boardData)/$$i/bdwlan.b215 $(TARGETDIR)/lib/firmware/IPQ8074/$$i/bdwlan.b215 ; \
	install -m 644 ./$(HW_boardData)/$$i/bdwlan.b215 $(TARGETDIR)/lib/firmware/IPQ8074/$$i/bdwlan.bin ; \
	done
else
ifeq ($(CONFIG_ORBI_AX_HW_OAK),y)
	###PIL_IMAGES
	install -m 644 -D $(WLAN_PROC_HWV2_PIL_IMAGES)/*.* $(TARGETDIR)/lib/firmware/IPQ8074
	###OAK
	install -m 644 ./$(HW_boardData)/FCC/bdwlan.b213 $(TARGETDIR)/lib/firmware/IPQ8074/bdwlan.b213
	install -m 644 ./$(HW_boardData)/FCC/bdwlan.b213 $(TARGETDIR)/lib/firmware/IPQ8074/bdwlan.bin
	for i in ${REGULATORY} ; do \
	mkdir -p $(TARGETDIR)/lib/firmware/IPQ8074/$$i ; \
	install -m 644 ./$(HW_boardData)/$$i/bdwlan.b213 $(TARGETDIR)/lib/firmware/IPQ8074/$$i/bdwlan.b213 ; \
	install -m 644 ./$(HW_boardData)/$$i/bdwlan.b213 $(TARGETDIR)/lib/firmware/IPQ8074/$$i/bdwlan.bin ; \
	done			
else
	###PIL_IMAGES
	install -m 644 -D $(WLAN_PROC_PIL_IMAGES)/*.* $(TARGETDIR)/lib/firmware/IPQ8074

	###HAWKEYE IPQ8074
	install -m 644 $(BDF_QCA8074)/bdwlan_coex.b93 $(TARGETDIR)/lib/firmware/IPQ8074/bdwlan_coex.b93
	install -m 644 $(BDF_QCA8074)/bdwlan_ctl.b93 $(TARGETDIR)/lib/firmware/IPQ8074/bdwlan_ctl.b93
	install -m 644 ./$(HW_boardData)/FCC/bdwlan.b93 $(TARGETDIR)/lib/firmware/IPQ8074/bdwlan.b93
	install -m 644 ./$(HW_boardData)/FCC/bdwlan.b93 $(TARGETDIR)/lib/firmware/IPQ8074/bdwlan.bin

	for i in ${REGULATORY} ; do \
	mkdir -p $(TARGETDIR)/lib/firmware/IPQ8074/$$i ; \
	install -m 644 ./$(HW_boardData)/$$i/bdwlan.b93 $(TARGETDIR)/lib/firmware/IPQ8074/$$i/bdwlan.b93 ; \
	install -m 644 ./$(HW_boardData)/$$i/bdwlan.b93 $(TARGETDIR)/lib/firmware/IPQ8074/$$i/bdwlan.bin ; \
	done

	###Cascade QCA9984 4x4
	rm -f $(TARGETDIR)/lib/firmware/QCA9984/hw.1/boarddata_0.bin
	rm -f $(TARGETDIR)/lib/firmware/QCA9984/hw.1/boarddata_1.bin
	rm -f $(TARGETDIR)/lib/firmware/QCA9984/hw.1/boarddata_2.bin
	install -m 644 ./$(HW_boardData)/FCC/boardData_QCA9984_CUS239_low_band_5G_v1_005.bin $(TARGETDIR)/lib/firmware/QCA9984/hw.1/boardData_QCA9984_CUS239_low_band_5G_v1_005.bin
	install -m 644 ./$(HW_boardData)/FCC/boardData_QCA9984_CUS239_low_band_5G_v1_005.bin $(TARGETDIR)/lib/firmware/QCA9984/hw.1/boarddata_0.bin
	install -m 644 ./$(HW_boardData)/FCC/boardData_QCA9984_CUS239_low_band_5G_v1_005.bin $(TARGETDIR)/lib/firmware/QCA9984/hw.1/boarddata_1.bin
	install -m 644 ./$(HW_boardData)/FCC/boardData_QCA9984_CUS239_low_band_5G_v1_005.bin $(TARGETDIR)/lib/firmware/QCA9984/hw.1/boarddata_2.bin    

	for i in ${REGULATORY} ; do \
	mkdir -p $(TARGETDIR)/lib/firmware/QCA9984/hw.1/$$i ; \
	install -m 644 ./$(HW_boardData)/$$i/boardData_QCA9984_CUS239_low_band_5G_v1_005.bin $(TARGETDIR)/lib/firmware/QCA9984/hw.1/$$i/boardData_QCA9984_CUS239_low_band_5G_v1_005.bin ; \
	install -m 644 ./$(HW_boardData)/$$i/boardData_QCA9984_CUS239_low_band_5G_v1_005.bin $(TARGETDIR)/lib/firmware/QCA9984/hw.1/$$i/boarddata_0.bin ; \
	install -m 644 ./$(HW_boardData)/$$i/boardData_QCA9984_CUS239_low_band_5G_v1_005.bin $(TARGETDIR)/lib/firmware/QCA9984/hw.1/$$i/boarddata_1.bin ; \
	install -m 644 ./$(HW_boardData)/$$i/boardData_QCA9984_CUS239_low_band_5G_v1_005.bin $(TARGETDIR)/lib/firmware/QCA9984/hw.1/$$i/boarddata_2.bin ; \
	done
endif	
endif
endif

clean:

