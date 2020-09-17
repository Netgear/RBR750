#***************************************************************************
#***
#***    Copyright © 2017 Foxconn Industrial Internet Co., Ltd.
#***    All Rights Reserved.
#***    No portions of this material shall be reproduced in any form without the
#***    written permission of Foxconn Industrial Internet Co. Ltd.
#***
#***    All information contained in this document is Foxconn Industrial Internet  
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


ifeq ($(PROFILE),$(filter $(PROFILE),RBR850 RBS850))   
HW_boardData:=RBR850_HE_V2
bdfNum:=b215
REGULATORY := FCC IC ETSI CTICK SRRC MKK SGP VNM IND KOR
endif

ifeq ($(PROFILE),$(filter $(PROFILE),RBR750 RBS750))  
HW_boardData:=RBR750_OAK
bdfNum:=b213
REGULATORY := FCC IC ETSI CTICK SRRC MKK SGP VNM IND KOR FCC_QORVO
endif

ifeq ($(PROFILE),CBR750)
HW_boardData:=CBR750_OAK
bdfNum:=b213
REGULATORY := FCC IC ETSI CTICK SRRC MKK SGP VNM IND KOR
endif


all: install

install:
	mkdir -p $(TARGETDIR)/lib/firmware/IPQ8074

	###PIL_IMAGES
	install -m 644 -D $(WLAN_PROC_HWV2_PIL_IMAGES)/*.* $(TARGETDIR)/lib/firmware/IPQ8074

	###HAWKEYE IPQ8074
	install -m 644 ./$(HW_boardData)/FCC/bdwlan.$(bdfNum) $(TARGETDIR)/lib/firmware/IPQ8074/bdwlan.$(bdfNum)
	install -m 644 ./$(HW_boardData)/FCC/bdwlan.$(bdfNum) $(TARGETDIR)/lib/firmware/IPQ8074/bdwlan.bin

	for i in ${REGULATORY} ; do \
	mkdir -p $(TARGETDIR)/lib/firmware/IPQ8074/$$i ; \
	install -m 644 ./$(HW_boardData)/$$i/bdwlan.$(bdfNum) $(TARGETDIR)/lib/firmware/IPQ8074/$$i/bdwlan.$(bdfNum) ; \
	install -m 644 ./$(HW_boardData)/$$i/bdwlan.$(bdfNum) $(TARGETDIR)/lib/firmware/IPQ8074/$$i/bdwlan.bin ; \
	done

clean:
	rm -rf $(TARGETDIR)/lib/firmware/IPQ8074


