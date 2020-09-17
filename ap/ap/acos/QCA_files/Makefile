#***************************************************************************
#***
#***    Copyright 2005-2018  Hon Hai Precision Ind. Co. Ltd.
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

ifeq ($(CONFIG_CABLE_ORBI_HW),y)
HW_boardData:=HW20181016_CBR40
REGULATORY := FCC
endif

ifeq ($(CONFIG_ORBI_AX_HW),y)
HW_boardData:=BDF
#REGULATORY := FCC IC ETSI CTICK SRRC
REGULATORY := FCC
endif


all: install

install:
	mkdir -p $(TARGETDIR)/lib/upgrade
	mkdir -p $(TARGETDIR)/sbin
	mkdir -p $(TARGETDIR)/etc/rc.d
	mkdir -p $(TARGETDIR)/etc/init.d
	mkdir -p $(TARGETDIR)/etc/config
	mkdir -p $(TARGETDIR)/etc/te
	mkdir -p $(TARGETDIR)/etc/orbi
	mkdir -p $(TARGETDIR)/bin
	install -m 755 login.sh $(TARGETDIR)/bin
	install -m 755 common.sh $(TARGETDIR)/lib/upgrade
	install -m 755 platform.sh $(TARGETDIR)/lib/upgrade	
	install -m 755 sysupgrade $(TARGETDIR)/sbin
ifeq ($(CONFIG_CABLE_ORBI_HW),y)
ifeq ($(CONFIG_CABLE_WAN),y)
	install -m 644 network_cable_wan $(TARGETDIR)/etc/config/network
endif
else ifeq ($(CONFIG_ORBI_AX_HW),y)
ifeq ($(CONFIG_ORBI_AX_HW_OAK),y)

ifeq ($(CONFIG_CABLE_WAN),y)
	install -m 644 network_orbiax_oak_cable_wan $(TARGETDIR)/etc/config/network
	install -m 644 network_orbiax_oak_cable_wan $(TARGETDIR)/etc/orbi/network_orbiax
else
	install -m 644 network_orbiax_oak $(TARGETDIR)/etc/config/network
	install -m 644 network_orbiax_oak $(TARGETDIR)/etc/orbi/network_orbiax	
endif
else
	install -m 644 network_orbiax $(TARGETDIR)/etc/config/network
	install -m 644 network_orbiax $(TARGETDIR)/etc/orbi/
endif	
else 
	#install -m 644 network $(TARGETDIR)/etc/config/network
endif
	install -m 755 3ap.sh $(TARGETDIR)/sbin
	install -m 755 S99te_test $(TARGETDIR)/etc/rc.d/S99te_test
	# Mos 2017/10/24, add restart_daemon for restart_all_processes
	# install -m 755 S99restart_daemon $(TARGETDIR)/etc/rc.d/S99restart_daemon
ifeq ($(CONFIG_CABLE_ORBI_HW),y)
ifeq ($(CONFIG_EMMC),y)
	install -m 755 common_emmc.sh $(TARGETDIR)/lib/upgrade/common.sh
	install -m 755 platform_emmc.sh $(TARGETDIR)/lib/upgrade/platform.sh
endif
	install -m 755 update_smp_affinity_cable.sh $(TARGETDIR)/lib/update_smp_affinity.sh
	##CSR8811 bluebooth
	mkdir -p $(TARGETDIR)/etc/bluetooth
	install -m 755 ./CSR8811/csr $(TARGETDIR)/sbin
	install -m 644 ./CSR8811/pb_207_csr8x11_rev4.coex.pio.5.1.4_DK07_CSR_QFN.psr $(TARGETDIR)/etc/bluetooth/pb_207_csr8x11.psr
	
	## QCA patch, jon 20171220, echo "performance" > /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor, original, echo "ondemand"
	###install -m 755 powerctl $(TARGETDIR)/etc/init.d/

	## QCA patch, jon 20180110, QCA#03294100 Open [RBW30] Require a patches that Ethernet port on Orbi might go down
	###install -m 755 dakota_edma_rss_disable_acl_config_acl_new.sh $(TARGETDIR)/lib
else
	#install -m 755 update_smp_affinity.sh $(TARGETDIR)/lib
	install -m 755 platform_orbiax.sh $(TARGETDIR)/lib/upgrade/platform.sh
endif	
	install -m 755 ./monitor/orbimonitor.sh $(TARGETDIR)/sbin
	install -m 644 profile $(TARGETDIR)/etc
	##removed install -m 755 hyd $(TARGETDIR)/etc/init.d/
	#install -m 755 wsplcd $(TARGETDIR)/etc/init.d/

	install -m 644 wireless.orbi_wp.uci $(TARGETDIR)/etc/orbi/
	install -m 644 wireless.orbi_base.uci $(TARGETDIR)/etc/orbi/	
	install -m 755 wps_vbn.sh $(TARGETDIR)/sbin
	install -m 755 wps_reset.sh $(TARGETDIR)/sbin
	install -m 755 sw_restart.sh $(TARGETDIR)/sbin

	# 2017/07/14 Mos, remove workaround and implement url match in xt_string
	# 2017/06/01 Mos, workaround to disable ecm if blocksite enabled
	#install -m 755 S54stop_ecm $(TARGETDIR)/etc/rc.d/S54stop_ecm
	# jon 20171229, Fix reset button still work in TE test mode.
	mkdir -p $(TARGETDIR)/etc/rc.button
	install -m 775 reset $(TARGETDIR)/etc/rc.button/reset 

	install -m 644 device-info $(TARGETDIR)/etc/config
ifeq ($(CONFIG_ORBI_AX_HW),y)
	install -m 755 te_ftm.sh $(TARGETDIR)/sbin
	install -m 644 banner_orbiax $(TARGETDIR)/etc/banner
endif
	
	# jack tseng 04/18/2018 add: integrate circle agent files and database files
ifeq ($(CONFIG_CIRCLE_PARENTAL_CONTROL),y)	
	mkdir -p $(TARGETDIR)/lib/circle
	install -m 755 ./circle/database.bin $(TARGETDIR)/lib/circle/database.bin
	install -m 755 ./circle/firmware.bin $(TARGETDIR)/lib/circle/firmware.bin
	install -m 755 ./circle/circled $(TARGETDIR)/bin/circled
endif


clean:

