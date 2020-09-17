#!/bin/sh
#
# Basic RE configuration for rbk150 board
# 


if [ "$1" = "bridge" ]; then
	uci delete network.wan
	uci set network.lan.proto='dhcp'
	uci set network.lan.force_link='1'
	uci set network.lan.type='bridge'
	uci set network.lan.ip6assign='60'
	uci set network.lan.ieee1905managed='1'
	uci set network.lan.multicast_querier='1'
	uci set network.lan.igmp_snooping=1
	uci set network.lan.ifname='eth0 eth1 eth2 eth3 eth4 "$1".100 "$2".100 "$3".100 "$4".100'
else
	uci set network.lan.proto=dhcp
	uci set network.lan.ifname='eth0 eth1 eth2 eth3 eth4'
	uci delete network.wan;
	uci set network.lan.multicast_querier=1
	uci set network.lan.igmp_snooping=1
fi
 
uci commit network
uci set dhcp.lan.ignore=1
uci commit dhcp
/etc/init.d/network restart
/etc/init.d/dnsmasq restart
 
uci set wireless.wifi2.htmode=HT80
uci set wireless.wifi1.htmode=HT40
uci set wireless.wifi0.htmode=HT80
 
 
uci set wireless.wifi0.wsplcd_unmanaged=1
uci set wireless.wifi0.repacd_auto_create_vaps=0
uci set wireless.wifi1.repacd_auto_create_vaps=0
uci set wireless.wifi2.repacd_auto_create_vaps=0
 
uci set wireless.@wifi-device[2].channel=157
uci set wireless.@wifi-device[1].channel=6
uci set wireless.@wifi-device[0].channel=44
 
 
uci set wireless.@wifi-iface[2].network=lan
uci set wireless.@wifi-iface[2].device=wifi2
uci set wireless.@wifi-iface[2].mode=sta
uci set wireless.@wifi-iface[2].wds=1
uci set wireless.@wifi-iface[2].ssid='Ice_BH_Orbi_0822'
uci set wireless.@wifi-iface[2].encryption=psk2+ccmp
uci set wireless.@wifi-iface[2].key=1234567890
uci set wireless.@wifi-iface[2].wsplcd_unmanaged=1
uci set wireless.@wifi-iface[2].repacd_security_unmanaged=1
 
uci set wireless.@wifi-iface[0].ul_hyst=3
 
uci add wireless wifi-iface
uci set wireless.@wifi-iface[3].network=lan
uci set wireless.@wifi-iface[3].device=wifi1
uci set wireless.@wifi-iface[3].mode=sta
uci set wireless.@wifi-iface[3].wds=1
uci set wireless.@wifi-iface[3].ssid='Ice_BH_Orbi_0822'
uci set wireless.@wifi-iface[3].encryption=psk2+ccmp
uci set wireless.@wifi-iface[3].key=1234567890
uci set wireless.@wifi-iface[3].wsplcd_unmanaged=1
uci set wireless.@wifi-iface[3].repacd_security_unmanaged=1
 
uci add wireless wifi-iface
uci set wireless.@wifi-iface[4].network=lan
uci set wireless.@wifi-iface[4].device=wifi2
uci set wireless.@wifi-iface[4].mode=ap
uci set wireless.@wifi-iface[4].wds=1
uci set wireless.@wifi-iface[4].ssid='Ice_BH_Orbi_0822'
uci set wireless.@wifi-iface[4].encryption=psk2+ccmp
uci set wireless.@wifi-iface[4].key=1234567890
uci set wireless.@wifi-iface[4].backhaul_ap=1
uci set wireless.@wifi-iface[4].repacd_security_unmanaged=1
uci set wireless.@wifi-iface[4].wsplcd_unmanaged=1
 
uci add wireless wifi-iface
uci set wireless.@wifi-iface[5].network=lan
uci set wireless.@wifi-iface[5].device=wifi1
uci set wireless.@wifi-iface[5].mode=ap
uci set wireless.@wifi-iface[5].wds=1
uci set wireless.@wifi-iface[5].ssid='Ice_BH_Orbi_0822'
uci set wireless.@wifi-iface[5].encryption=psk2+ccmp
uci set wireless.@wifi-iface[5].key=1234567890
uci set wireless.@wifi-iface[5].backhaul_ap=1
uci set wireless.@wifi-iface[5].wsplcd_unmanaged=1
uci set wireless.@wifi-iface[5].repacd_security_unmanaged=1
 
uci add wireless wifi-iface
uci set wireless.@wifi-iface[6].device=wifi1
uci set wireless.@wifi-iface[6].network=lan
uci set wireless.@wifi-iface[6].mode=ap_smart_monitor
uci set wireless.@wifi-iface[6].ssid=AEV_MON_REP1_0
uci set wireless.@wifi-iface[6].encryption=none
uci set wireless.@wifi-iface[6].neighbourfilter=1
uci set wireless.@wifi-iface[6].disable=0
uci set wireless.@wifi-iface[6].wsplcd_unmanaged=1
uci set wireless.@wifi-iface[6].repacd_security_unmanaged=1
 
uci add wireless wifi-iface
uci set wireless.@wifi-iface[7].device=wifi0
uci set wireless.@wifi-iface[7].network=lan
uci set wireless.@wifi-iface[7].mode=ap_smart_monitor
uci set wireless.@wifi-iface[7].ssid=AEV_MON_REP1_1
uci set wireless.@wifi-iface[7].encryption=none
uci set wireless.@wifi-iface[7].neighbourfilter=1
uci set wireless.@wifi-iface[7].set_monrxfilter=1
uci set wireless.@wifi-iface[7].disable=0
uci set wireless.@wifi-iface[7].wsplcd_unmanaged=1
uci set wireless.@wifi-iface[7].repacd_security_unmanaged=1
 
 
uci commit wireless
 
uci commit wireless
 
#BTM clients as dual band support irrespective of actual support
 
uci set lbd.StaDB.MarkAdvClientAsDualBand=1
 
#To Allow Active steering of BTM clients for the first attempt
uci set lbd.SteerExec_Adv.StartInBTMActiveState=1
 
uci set lbd.IdleSteer.RSSISteeringPoint_UG='10'
uci set lbd.ActiveSteer.TxRateXingThreshold_DG='6000'
uci set lbd.ActiveSteer.RateRSSIXingThreshold_DG='0'
uci set lbd.ActiveSteer.TxRateXingThreshold_UG='20000'
uci set lbd.ActiveSteer.RateRSSIXingThreshold_UG='20'
uci set lbd.IAS.Enable_W2='0'
uci set lbd.IAS.Enable_W5='0'
uci set hyd.Monitor.DisableMonitoring='0'
uci set hyd.Monitor.DisableMonitoringLegacyClients='0'
uci set hyd.Monitor.DisableSteeringInactiveLegacyClients='0'
uci set hyd.Monitor.DisableSteeringActiveLegacyClients='0'
uci set hyd.Monitor.MonitorTimer='60'
uci set hyd.Monitor.MonitorResponseTimeout='5'
uci set hyd.Monitor.DisableSteeringMax11kUnfriendlyClients='0'
uci commit hyd
uci set lbd.UnifiedStats=UnifiedStats
uci set lbd.UnifiedStats.Enable_Unified_Stats_W2='1'
uci set lbd.UnifiedStats.Enable_Unified_Stats_W5='1'
 
uci set lbd.config.InactDetectionFromTx='1'
 
uci commit lbd
 
uci set hyd.Topology.ENABLE_NOTIFICATION_UNICAST=1
uci set hyd.Topology.PERIODIC_QUERY_INTERVAL=15
uci set hyd.PathChWlan.MaxMediumUtilizationForLC_W2=0
uci set hyd.PathChWlan.MaxMediumUtilizationForLC_W5=99
 
uci set hyd.IEEE1905Settings.AvoidDupRenew=1
uci set hyd.IEEE1905Settings.AvoidDupTopologyNotification=1
uci set hyd.PathSelect.EnableBadLinkStatsSwitchFlow='0'
 
#Single backhaul enablement for Orbi
 
uci set hyd.hy.ForwardingMode='SINGLE'
 
uci set hyd.IEEE1905Settings.AvoidDupRenew=1   
uci set hyd.IEEE1905Settings.AvoidDupTopologyNotification=1
 
uci commit hyd
 
uci set wsplcd.config.HyFiSecurity=1;
uci set wsplcd.config.ConfigSta=0
 
uci commit wsplcd
 
uci set repacd.repacd.Enable=1
uci set repacd.repacd.ConfigREMode=son
 
#Independent repeater mode and global_wds FR-37307 are mutual exclusive
uci set repacd.WiFiLink.ManageVAPInd=1
 
 
uci set repacd.WiFiLink.5GBackhaulEvalTimeShort=180
uci set repacd.WiFiLink.5GBackhaulEvalTimeLong=300
uci set repacd.WiFiLink.2GBackhaulEvalTime=180
uci set repacd.WiFiLink.BSSIDAssociationTimeout=75
uci set repacd.WiFiLink.RateNumMeasurements=10
uci set repacd.WiFiLink.MinAssocCheckPostBSSIDConfig=10
 
uci set repacd.WiFiLink.RSSIThresholdPrefer2GBackhaul='-82'
 
uci set repacd.WiFiLink.PreferCAPSNRThreshold='20'
 
uci set repacd.WiFiLink.MoveFromCAPSNRHysteresis5G='7'
 
uci commit repacd
 
/etc/init.d/repacd start

