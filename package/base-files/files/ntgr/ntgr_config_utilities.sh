#!/bin/sh
MAIN_VID=4094
GUEST_VID=4093

# Disable mcsd
disable_mcsd() {
	uci set mcsd.config.Enable='0'
	uci commit mcsd
	/etc/init.d/mcsd stop
	/etc/init.d/mcsd disable
}

# Delete all VAP from wireless config
delete_vaps()
{
	while uci delete wireless.@wifi-iface[0];
	do
	:
	done
}

config_default_radio() {
	uci batch <<-EOF
		set wireless.wifi0=wifi-device
		set wireless.wifi0.type='qcawificfg80211'
		set wireless.wifi0.hwmode='11axa'
		set wireless.wifi0.htmode='HT80'
		set wireless.wifi0.channel='44'
		set wireless.wifi0.macaddr="$(cat /sys/class/net/wifi0/address)"
		set wireless.wifi0.repacd_auto_create_vaps='0'
		set wireless.wifi0.no_vlan='1'
		set wireless.wifi0.disabled='0'
		set wireless.wifi0.qwrap_enable='0'
		set wireless.wifi0.dbdc_enable='0'
		set wireless.wifi1=wifi-device
		set wireless.wifi1.type='qcawificfg80211'
		set wireless.wifi1.channel='1'
		set wireless.wifi1.hwmode='11axg'
		set wireless.wifi1.htmode='HT40+'
		set wireless.wifi1.macaddr="$(cat /sys/class/net/wifi1/address)"
		set wireless.wifi1.repacd_auto_create_vaps='0'
		set wireless.wifi1.no_vlan='1'
		set wireless.wifi1.disabled='0'
		set wireless.wifi1.qwrap_dbdc_enable='0'
		set wireless.wifi1.dbdc_enable='0'
		set wireless.wifi2=wifi-device
		set wireless.wifi2.type='qcawificfg80211'
		set wireless.wifi2.hwmode='11axa'
		set wireless.wifi2.htmode='HT80'
		set wireless.wifi2.channel='157'
		set wireless.wifi2.macaddr="$(cat /sys/class/net/wifi2/address)"
		set wireless.wifi2.repacd_auto_create_vaps='0'
		set wireless.wifi2.no_vlan='1'
		set wireless.wifi2.disabled='0'
		set wireless.wifi2.qwrap_enable='0'
		set wireless.wifi2.dbdc_enable='0'
		set wireless.qcawifi=qcawifi
		set wireless.qcawifi.wps_pbc_extender_enhance='1'
		set wireless.qcawifi.wps_pbc_overwrite_ap_settings_all='1'
		set wireless.qcawifi.wps_pbc_overwrite_sta_settings_all='1'
	EOF
}

# Config Backaul AP
# $1 - radio: wifi1 or wifi2
# $2 - ssid
# $3 - key
# $4 - ifname
# $5 - disabled
# $6 - mode : 1 means RE, other than 1 is CAP
# Exampe: config_backhaul_ap "wifi2" "Orbi-Backhaul" "12345678" "" "0" "1"
# Means config "Orbi-Backhaul" with key 12345678, ifname not defined, disabled=0, mode=1 (RE) and CAP if 6th arg is empty
config_backhaul_ap() {
	local radio="$1" # wifi1 wifi2
	local ssid="$2"
	local key="$3"
	local ifname="$4"
	local disabled="$5"
	local mode="$6"
	local wiface
	local athnewind=""
	local root_distance=""
	local bintval=""

	if [ "$radio" = "wifi2" ]; then
		wiface="BhAp5"
	elif [ "$radio" = "wifi1" ]; then
		wiface="BhAp2"
	else
		return; # Invalid value for Backhaul
	fi
	if [ "$mode" = "1" ]; then
		root_distance=255
		athnewind=1
		bintval=102
	fi
	uci batch <<-EOF
		set wireless.$wiface=wifi-iface
		set wireless.$wiface.network=backhaul
		set wireless.$wiface.device="$radio"
		set wireless.$wiface.mode=ap
		set wireless.$wiface.wds=1
		set wireless.$wiface.ssid="$ssid"
		set wireless.$wiface.encryption=psk2+ccmp
		set wireless.$wiface.key="$key"
		set wireless.$wiface.backhaul_ap=1
		set wireless.$wiface.backhaul_ntgr=1
		set wireless.$wiface.rept_spl=1
		set wireless.$wiface.wps_pbc='1'
		set wireless.$wiface.wps_pbc_enable='1'
		set wireless.$wiface.wps_pbc_start_time='61'
		set wireless.$wiface.wps_pbc_duration='180'
		set wireless.$wiface.wps_pbc_noclone='0'
		set wireless.$wiface.wsplcd_unmanaged=1
		set wireless.$wiface.repacd_security_unmanaged=1
		set wireless.$wiface.rrm='1'
		set wireless.$wiface.ifname="$ifname"
		set wireless.$wiface.disabled="$disabled"
		set wireless.$wiface.root_distance="$root_distance"
		set wireless.$wiface.athnewind="$athnewind"
		set wireless.$wiface.rts='64'
		set wireless.$wiface.bintval="$bintval"
		set wireless.$wiface.dtim_period='1'
	EOF
}

# Config Fronthaul AP
# $1 - radio: wifi0 or wifi1
# $2 - ssid
# $3 - key
# $4 - ifname
# $5 - disabled
# $6 - mode : 1 means RE, other than 1 is CAP
# Example: config_fronthaul_ap "wifi1" "ORBI-999" "12345678" "ath1" "0" "1"
# Means config "ORBI-999" with key 12345678, ifname=ath1, disabled=0, mode=1 (RE) and CAP if 6th arg is empty
config_fronthaul_ap() {
	local radio="$1" # wifi0 wifi1
	local ssid="$2"
	local key="$3"
	local ifname="$4"
	local disabled="$5"
	local mode="$6"
	local wiface
	local athnewind=""
	local root_distance=""
	local bintval=""

	if [ "$radio" = "wifi1" ]; then
		wiface="FhAp2"
	elif [ "$radio" = "wifi0" ]; then
		wiface="FhAp5"
	else
		return; # Invalid value for Fronthaul
	fi
	if [ "$mode" = "1" ]; then
		root_distance=255
		athnewind=1
		bintval=102
	fi
	uci batch <<-EOF
		set wireless.$wiface=wifi-iface
		set wireless.$wiface.device="$radio"
		set wireless.$wiface.network='lan'
		set wireless.$wiface.mode='ap'
		set wireless.$wiface.ssid="$ssid"
		set wireless.$wiface.encryption='psk2+ccmp'
		set wireless.$wiface.key="$key"
		set wireless.$wiface.wps_pbc='1'
		set wireless.$wiface.wps_pbc_enable='1'
		set wireless.$wiface.wps_pbc_noclone='1'
		set wireless.$wiface.wps_pbc_start_time='0'
		set wireless.$wiface.wps_pbc_duration='60'
		set wireless.$wiface.wds='1'
		set wireless.$wiface.rrm='1'
		set wireless.$wiface.ifname="$ifname"
		set wireless.$wiface.disabled="$disabled"
		set wireless.$wiface.root_distance="$root_distance"
		set wireless.$wiface.athnewind="$athnewind"
		set wireless.$wiface.rts='64'
		set wireless.$wiface.bintval="$bintval"
	EOF
}

# Config Guest
# $1 - radio: wifi0 or wifi1
# $2 - ssid
# $3 - key
# $4 - ifname
# $5 - disabled
# $6 - mode : 1 means RE, other than 1 is CAP
# Example: config_guest_ap "wifi1" "NETGEAR-Guest" "" "ath21" "0" "1"
# Means config "NETGEAR-Guest" without security, ifname=ath21, disabled=0 , mode=1 (RE) and CAP if 6th arg is empty
config_guest_ap() {
	local radio="$1" # wifi0 wifi1
	local ssid="$2"
	local key="$3"
	local ifname="$4"
	local disabled="$5"
	local mode="$6"
	local wiface encryption
	local athnewind=""
	local root_distance=""
	local bintval=""

	if [ "$radio" = "wifi1" ]; then
		wiface="Guest2"
	elif [ "$radio" = "wifi0" ]; then
		wiface="Guest5"
	else
		return; # Invalid value for guest
	fi
	if [ -z "$key" ]; then
		encryption="none"
	else
		encryption="psk2+ccmp"
	fi
	if [ "$mode" = "1" ]; then
		root_distance=255
		athnewind=1
		bintval=102
	fi
	# Guest ap created with default ifdown = 1
	uci batch <<-EOF
		set wireless.$wiface=wifi-iface
		set wireless.$wiface.network='guest'
		set wireless.$wiface.device="$radio"
		set wireless.$wiface.mode='ap'
		set wireless.$wiface.ssid="$ssid"
		set wireless.$wiface.encryption="$encryption"
		set wireless.$wiface.key="$key"
		set wireless.$wiface.wds='1'
		set wireless.$wiface.rrm='1'
		set wireless.$wiface.ifdown='1'
		set wireless.$wiface.disabled="$disabled"
		set wireless.$wiface.ifname="$ifname"
		set wireless.$wiface.root_distance="$root_distance"
		set wireless.$wiface.athnewind="$athnewind"
		set wireless.$wiface.rts='64'
		set wireless.$wiface.bintval="$bintval"
	EOF
}
# Config Backaul STA
# $1 - radio: wifi0 or wifi1
# $2 - ssid
# $3 - key
# $4 - ifname
# $5 - disabled
# Exampe: config_backhaul_sta "wifi2" "Orbi-Backhaul" "12345678" "" 0
# Means config "Orbi-Backhaul" with key 12345678, ifname not defined, disabled=0
config_backhaul_sta() {
	local radio="$1" # wifi1 wifi0
	local ssid="$2"
	local key="$3"
	local ifname="$4"
	local disabled="$5"
	local wiface

	if [ "$radio" = "wifi2" ]; then
		wiface="BhSta5"
	elif [ "$radio" = "wifi1" ]; then
		wiface="BhSta2"
	else
		return; # Invalid value for Backhaul STA
	fi

	uci batch <<-EOF
		set wireless.$wiface=wifi-iface
		set wireless.$wiface.network='backhaul'
		set wireless.$wiface.device="$radio"
		set wireless.$wiface.mode='sta'
		set wireless.$wiface.wds='1'
		set wireless.$wiface.ssid="$ssid"
		set wireless.$wiface.encryption='psk2+ccmp'
		set wireless.$wiface.rept_spl='1'
		set wireless.$wiface.wsplcd_unmanaged='1'
		set wireless.$wiface.repacd_security_unmanaged='1'
		set wireless.$wiface.key="$key"
		set wireless.$wiface.wps_pbc='1'
		set wireless.$wiface.wps_pbc_noclone='0'
		set wireless.$wiface.wps_pbc_enable='1'
		set wireless.$wiface.wps_pbc_start_time='61'
		set wireless.$wiface.wps_pbc_duration='180'
		set wireless.$wiface.ul_hyst='3'
		set wireless.$wiface.wps_pbc_skip_connected_sta='1'
		set wireless.$wiface.athnewind='1'
		set wireless.$wiface.ifname="$ifname"
		set wireless.$wiface.disabled="$disabled"
		set wireless.$wiface.backhaul_ntgr='1'
		set wireless.$wiface.re_scalingfactor='85'
		set wireless.$wiface.caprssi='27'
		set wireless.$wiface.root_distance='255'
		set wireless.$wiface.rts='64'
	EOF
}

# Config Smart monitor
# $1 - radio: wifi0 or wifi1
# $2 - ssid
# $3 - ifname
# $4 - disabled
# $5 - mode : 1 means RE, other than 1 is CAP
# Example: config_smart_mon "wifi0" "AEV_MON_AP1" "ath22" "0" "1"
# Means config "AEV_MON_AP1" without security, ifname=ath22, disabled=0 , mode=1 (RE) and CAP if 5th arg is empty
config_smart_mon() {
	local radio="$1" # wifi0 wifi1
	local ssid="$2"
	local ifname="$3"
	local disabled="$4"
	local mode="$5"
	local wiface
	local set_monrxfilter=""
	local athnewind=""
	local root_distance=""
	local bintval=""

	if [ "$radio" = "wifi1" ]; then
		wiface="SM2"
	elif [ "$radio" = "wifi0" ]; then
		wiface="SM5"
		set_monrxfilter=1
	else
		return; # Invalid value for Smart Monitor
	fi
	if [ "$mode" = "1" ]; then
		root_distance=255
		athnewind=1
		bintval=102
	fi

	uci batch <<-EOF
		set wireless.$wiface=wifi-iface
		set wireless.$wiface.device="$radio"
		set wireless.$wiface.network='lan'
		set wireless.$wiface.mode='ap_smart_monitor'
		set wireless.$wiface.ssid="$ssid"
		set wireless.$wiface.encryption='none'
		set wireless.$wiface.neighbourfilter='1'
		set wireless.$wiface.set_monrxfilter="$set_monrxfilter"
		set wireless.$wiface.disabled="$disabled"
		set wireless.$wiface.wsplcd_unmanaged='1'
		set wireless.$wiface.repacd_security_unmanaged='1'
		set wireless.$wiface.wds='1'
		set wireless.$wiface.ifname="$ifname"
		set wireless.$wiface.root_distance="$root_distance"
		set wireless.$wiface.athnewind="$athnewind"
		set wireless.$wiface.bintval="$bintval"
	EOF
}


# Config CAP network 
# Caller has to provide Backhaul interface names
# $1 - BackHaul AP interface name (ath2)
# $2 - Backhaul AP interface name (ath1X)
config_network_cap() {

	uci batch <<-EOF
		set network.lan.ipaddr=192.168.1.1
		set network.wan.ifname='eth4'
		set network.wan6.ifname='eth4'
		set network.lan.multicast_querier=1
		set network.lan.igmp_snooping=1
		set network.lan.ifname='eth0 eth1 eth2 eth3 "$1"."$MAIN_VID" "$2"."$MAIN_VID"'
		set network.guest=interface
		set network.guest.type='bridge'
		set network.guest.proto='static'
		set network.guest.ipaddr='192.168.2.1'
		set network.guest.netmask='255.255.255.0'
		set network.guest.ifname='eth0."$GUEST_VID" eth1."$GUEST_VID" eth2."$GUEST_VID" eth3."$GUEST_VID" "$1"."$GUEST_VID" "$2"."$GUEST_VID"'
		set network.guest.ieee1905managed='1'
		set network.guest.force_link='1'
		set network.guest.family=ipv4
		commit network
		delete dhcp.guest
		set dhcp.guest=dhcp
		set dhcp.guest.interface=guest
		set dhcp.guest.start=100
		set dhcp.guest.limit=150
		set dhcp.guest.leasetime='12h'
		set dhcp.guest.dhcpv6=server
		set dhcp.guest.ra=server
		commit dhcp
	EOF
}

# Config CAP network
# Caller has to provide Backhaul interface names
# $1 - BackHaul AP interface name (ath2)
# $2 - Backhaul AP interface name (ath1X)
config_network_cap_bridge() {

	uci batch <<-EOF
		delete network.wan
		set network.lan.ipaddr=192.168.1.1
		set network.wan6.ifname='eth0'
		set network.lan.multicast_querier=1
		set network.lan.proto='dhcp'
		set network.lan.force_link='1'
		set network.lan.ieee1905managed='1'
		set network.lan.ip6assign='60'
		set network.lan.type='bridge'
		set network.lan.gateway='static'
		set network.lan.igmp_snooping=1
		set network.lan.ifname='eth0 eth1 eth2 eth3 eth4 "$1"."$MAIN_VID" "$2".$MAIN_VID'
		set network.guest=interface
		set network.guest.type='bridge'
		set network.guest.proto='static'
		set network.guest.ipaddr='192.168.2.1'
		set network.guest.netmask='255.255.255.0'
		set network.guest.ifname='eth0."$GUEST_VID" eth1."$GUEST_VID" eth2."$GUEST_VID" eth3."$GUEST_VID" eth4."$GUEST_VID" "$1"."$GUEST_VID" "$2"."$GUEST_VID"'
		set network.guest.ieee1905managed='1'
		set network.guest.force_link='1'
		set network.guest.family=ipv4
		commit network
		delete dhcp.guest
		set dhcp.guest=dhcp
		set dhcp.guest.interface=guest
		set dhcp.guest.start=100
		set dhcp.guest.limit=150
		set dhcp.guest.leasetime='12h'
		set dhcp.guest.dhcpv6=server
		set dhcp.guest.ra=server
		commit dhcp
	EOF
}

# Config RE network 
# Caller has to provide 4 Backhaul interface names
# $1 - 5G BackHaul AP interface name (ath2..)
# $2 - 2G Backhaul AP interface name (ath1..)
# $3 - 5G Backhaul STA interface name (ath2..)
# $4 - 2G Backhaul STA interface name (ath1..)


config_network_re() {
	uci batch <<-EOF
		delete network.wan
		set network.lan.proto='dhcp'
		set network.lan.multicast_querier='1'
		set network.lan.igmp_snooping=1
		set network.lan.ifname='eth0 eth1 eth2 eth3 eth4 "$1"."$MAIN_VID" "$2"."$MAIN_VID" "$3"."$MAIN_VID" "$4"."$MAIN_VID"'
		set network.guest=interface
		set network.guest.type='bridge'
		set network.guest.ifname='eth0."$GUEST_VID" eth1."$GUEST_VID" eth2."$GUEST_VID" eth3."$GUEST_VID" eth4."$GUEST_VID" "$1"."$GUEST_VID" "$2"."$GUEST_VID" "$3"."$GUEST_VID" "$4"."$GUEST_VID"'
		set network.guest.ieee1905managed='1'
		set network.guest.force_link='1'
		set network.guest.family=ipv4
		set network.guest.bridge_empty='1'
		commit network
		set dhcp.lan.ignore=1
		delete dhcp.guest
		set dhcp.guest=dhcp
		set dhcp.guest.ignore=1
		commit dhcp
	EOF
}

# Config RE network
# Caller has to provide 4 Backhaul interface names
# $1 - 5G BackHaul AP interface name (ath2..)
# $2 - 2G Backhaul AP interface name (ath1..)
# $3 - 5G Backhaul STA interface name (ath2..)
# $4 - 2G Backhaul STA interface name (ath1..)

config_network_re_bridge() {
	uci batch <<-EOF
		delete network.wan
		set network.lan.proto='dhcp'
		set network.lan.force_link='1'
		set network.lan.type='bridge'
		set network.lan.ip6assign='60'
		set network.lan.ieee1905managed='1'
		set network.lan.multicast_querier='1'
		set network.lan.igmp_snooping=1
		set network.lan.ifname='eth0 eth1 eth2 eth3 eth4 "$1"."$MAIN_VID" "$2"."$MAIN_VID" "$3"."$MAIN_VID" "$4"."$MAIN_VID"'
		set network.guest=interface
		set network.guest.type='bridge'
		set network.guest.ifname='eth0."$GUEST_VID" eth1."$GUEST_VID" eth2."$GUEST_VID" eth3."$GUEST_VID" eth4."$GUEST_VID" "$1"."$GUEST_VID" "$2"."$GUEST_VID" "$3"."$GUEST_VID" "$4"."$GUEST_VID"'
		set network.guest.ieee1905managed='1'
		set network.guest.force_link='1'
		set network.guest.family=ipv4
		set network.guest.bridge_empty='1'
		commit network
		set dhcp.lan.ignore=1
		delete dhcp.guest
		set dhcp.guest=dhcp
		set dhcp.guest.ignore=1
		commit dhcp
	EOF
}

# Config default firewall
# for router: config_firewall 1
# for satellite: config_firewall 0
config_firewall() {
	local type="$1"
	local masq=""

	[ "$type" -gt 0 ] && masq=1
 
	uci batch <<-EOF
		set firewall.lan_zone=zone
		set firewall.lan_zone.masq="$masq"
		delete firewall.guest_zone
		set firewall.guest_zone=zone
		set firewall.guest_zone.name=guest
		set firewall.guest_zone.network=guest
		set firewall.guest_zone.input=REJECT
		set firewall.guest_zone.forward=REJECT
		set firewall.guest_zone.output=ACCEPT
		set firewall.guest_zone.masq="$masq"
		delete firewall.guest_forwarding
		set firewall.guest_forwarding=forwarding
		set firewall.guest_forwarding.src=guest
		set firewall.guest_forwarding.dest=wan
		delete firewall.guest_rule_dns
		set firewall.guest_rule_dns=rule
		set firewall.guest_rule_dns.name='Allow DNS Queries'
		set firewall.guest_rule_dns.src=guest
		set firewall.guest_rule_dns.dest_port=53
		set firewall.guest_rule_dns.proto=udp
		set firewall.guest_rule_dns.target=ACCEPT
		delete firewall.guest_rule_dhcp
		set firewall.guest_rule_dhcp=rule
		set firewall.guest_rule_dhcp.name='Allow DHCP request'
		set firewall.guest_rule_dhcp.src=guest
		set firewall.guest_rule_dhcp.src_port=68
		set firewall.guest_rule_dhcp.dest_port=67
		set firewall.guest_rule_dhcp.proto=udp
		set firewall.guest_rule_dhcp.target=ACCEPT
		set firewall.guest_rule_dhcp.family=ipv4
		delete firewall.guest_rule_ltogaccess
		set firewall.guest_rule_ltogaccess=rule
		set firewall.guest_rule_ltogaccess.name='Block-lan-Guest-forwarding'
		set firewall.guest_rule_ltogaccess.src='lan'
		set firewall.guest_rule_ltogaccess.dest='guest'
		set firewall.guest_rule_ltogaccess.proto='0'
		set firewall.guest_rule_ltogaccess.family='any'
		set firewall.guest_rule_ltogaccess.target='REJECT'
		delete firewall.guest_rule_gtolaccess
		set firewall.guest_rule_gtolaccess=rule
		set firewall.guest_rule_gtolaccess.name='Block-Guest-lan-forwarding'
		set firewall.guest_rule_gtolaccess.src='guest'
		set firewall.guest_rule_gtolaccess.dest='lan'
		set firewall.guest_rule_gtolaccess.proto='0'
		set firewall.guest_rule_gtolaccess.family='any'
		set firewall.guest_rule_gtolaccess.target='REJECT'
		commit firewall
	EOF
	if [ "$type" = "1" ]; then
		uci batch <<-EOF
		delete firewall.guest_rule_ltog_icmp
		set firewall.guest_rule_ltog_icmp=rule
		set firewall.guest_rule_ltog_icmp.src=lan
		set firewall.guest_rule_ltog_icmp.proto=icmp
		set firewall.guest_rule_ltog_icmp.target=REJECT
		set firewall.guest_rule_ltog_icmp.dest_ip=192.168.2.1
		EOF
	else
		uci batch <<-EOF
		delete firewall.guest_rule_gtogaccess
		set firewall.guest_rule_gtogaccess=rule
		set firewall.guest_rule_gtogaccess.name='Allow-Guest-Guest-forwarding'
		set firewall.guest_rule_gtogaccess.src='guest'
		set firewall.guest_rule_gtogaccess.dest='guest'
		set firewall.guest_rule_gtogaccess.proto='0'
		set firewall.guest_rule_gtogaccess.family='any'
		set firewall.guest_rule_gtogaccess.target='ACCEPT'
		EOF
	fi
	uci commit
	uci commit firewall

}

config_lbd() {
	local Mode="$1"
	local LowRSSIAPSteerThreshold_CAP

	[ -z "$1" ] && return

	if [ "$Mode" = "1" ]; then
		LowRSSIAPSteerThreshold_CAP="20"
	else
		LowRSSIAPSteerThreshold_CAP="25"
	fi

	uci batch <<-EOF
		set lbd.config.BlacklistOtherESS='1'
		set lbd.config.InactDetectionFromTx='5'
		set lbd.config.InactCheckInterval='5'
		set lbd.config.EnableAckRSSI='1'
		set lbd.config.LogModCounter='10'
		set lbd.config.ClientClassificationEnable='1'
		set lbd.IdleSteer.RSSISteeringPoint_UG='20 20'
		set lbd.IdleSteer.NormalInactTimeout='5 5'
		set lbd.IdleSteer.OverloadInactTimeout='5'
		set lbd.ActiveSteer.TxRateXingThreshold_UG='20000 20000'
		set lbd.ActiveSteer.RateRSSIXingThreshold_UG='20 30'
		set lbd.Offload.MUOverloadThreshold_W2='80'
		set lbd.Offload.MUOverloadThreshold_W5='99'
		set lbd.Offload.MUSafetyThreshold_W5='98'
		set lbd.Offload.MUSafetyThreshold_W2='60'
		set lbd.IAS.Enable_W2='0'
		set lbd.IAS.Enable_W5='0'
		set lbd.StaDB.MarkAdvClientAsDualBand='1 0'
		set lbd.SteerExec.SteeringProhibitTime='60 120'
		set lbd.SteerExec.BTMSteeringProhibitShortTime='15 110'
		set lbd.SteerExec.DisableLegacySteering='0 0'
		set lbd.APSteer.LowRSSIAPSteerThreshold_CAP="$LowRSSIAPSteerThreshold_CAP"
		set lbd.APSteer.LowRSSIAPSteerThreshold_RE='25'
		set lbd.APSteer.LowRSSIAPSteerThreshold_CAP_W5='30 30'
		set lbd.APSteer.LowRSSIAPSteerThreshold_RE_W5='30 30'
		set lbd.APSteer.LowRSSIAPSteerThreshold_CAP_W2='40 35'
		set lbd.APSteer.LowRSSIAPSteerThreshold_RE_W2='40 35'
		set lbd.APSteer.APSteerToRootMinRSSIIncThreshold='10'
		set lbd.APSteer.DownlinkRSSIThreshold_W5='-70'
		set lbd.config_Adv.AgeLimit='5'
		set lbd.StaDB_Adv.LegacyUpgradeAllowedCnt='3 3'
		set lbd.StaMonitor_Adv.RSSIMeasureSamples_W2='2'
		set lbd.StaMonitor_Adv.RSSIMeasureSamples_W5='2'
		set lbd.BandMonitor_Adv.MUReportPeriod='15'
		set lbd.BandMonitor_Adv.LoadBalancingAllowedMaxPeriod='10'
		set lbd.Estimator_Adv.11kProhibitTimeShort='15 110'
		set lbd.Estimator_Adv.11kProhibitTimeLong='60 110'
		set lbd.Estimator_Adv.PhyRateScalingForAirtime='95'
		set lbd.Estimator_Adv.BcnrptPassiveDuration='110'
		set lbd.Estimator_Adv.ActDetectMinInterval='5'
		set lbd.Estimator_Adv.Max11kUnfriendly='5'
		set lbd.Estimator_Adv.ActDetectMinPktPerSec='4'
		set lbd.SteerExec_Adv.StartInBTMActiveState='1'
		set lbd.SteerExec_Adv.AuthRejMax='2 2'
		set lbd.SteerExec_Adv.MaxSteeringUnfriendly='43200 86400'
		set lbd.SteerExec_Adv.TargetLowRSSIThreshold_W5='10 15'
		set lbd.SteerExec_Adv.BlacklistTime='60'
		set lbd.SteerExec_Adv.BTMUnfriendlyTime='30'
		set lbd.SteerExec_Adv.MaxBTMUnfriendly='120'
		set lbd.SteerExec_Adv.MaxBTMActiveUnfriendly='120'
		set lbd.SteerExec_Adv.SteeringUnfriendlyTime='600 600'
		set lbd.SteerAlg_Adv.MinTxRateIncreaseThreshold='20'
		set lbd.UnifiedStats=UnifiedStats
		set lbd.UnifiedStats.Enable_Unified_Stats_W2='1'
		set lbd.UnifiedStats.Enable_Unified_Stats_W5='1'
		set lbd.Offload.OffloadingMinRSSI='20 20'
		set lbd.StaDB_Adv.LegacyUpgradeMonitorDur='2100 2100'
		set lbd.SteerExec_Adv.LegacyUpgradeUnfriendlyTime='10800 21600'
		set lbd.SteerExec_Adv.TargetLowRSSIThreshold_W2='5 5'
		set lbd.Estimator_Adv.BcnrptActiveDuration='50 50'
		set lbd.IdleSteer.AuthAllow='0 0'
		set lbd.Estimator_Adv.RSSIDiff_EstW5FromW2='-15 -15'
		set lbd.Estimator_Adv.RSSIDiff_EstW2FromW5='5 5'
		commit lbd
	EOF
}


config_hyd() {
	local Mode="$1"

	[ -z "$1" ] && return
	if [ "$Mode" = "1" ]; then
		hyd_mode=HYROUTER
	else
		hyd_mode=HYCLIENT
	fi
	uci batch <<-EOF
		set hyd.config.Mode="$hyd_mode"
		set hyd.config.Enable='1'
		set hyd.hy.ForwardingMode='SINGLE'
		set hyd.@Vlanid[0].vid='$MAIN_VID'
		set hyd.@Vlanid[1].vid='$GUEST_VID'
		set hyd.PathChWlan.MaxMediumUtilizationForLC_W2='0'
		set hyd.PathChWlan.CPULimitedTCPThroughput_W2='4294967295'
		set hyd.PathChWlan.CPULimitedUDPThroughput_W2='4294967295'
		set hyd.PathChWlan.MaxMediumUtilizationForLC_W5='99'
		set hyd.PathChWlan.CPULimitedTCPThroughput_W5='4294967295'
		set hyd.PathChWlan.CPULimitedUDPThroughput_W5='4294967295'
		set hyd.Topology.PERIODIC_QUERY_INTERVAL='15'
		set hyd.Topology.ENABLE_NOTIFICATION_UNICAST='1'
		set hyd.PathSelect.EnableBadLinkStatsSwitchFlow='0'
		set hyd.IEEE1905Settings.AvoidDupRenew='1'
		set hyd.IEEE1905Settings.AvoidDupTopologyNotification='1'
		set hyd.Monitor.DisableMonitoring='0'
		set hyd.Monitor.DisableMonitoringLegacyClients='0'
		set hyd.Monitor.DisableSteeringInactiveLegacyClients='0'
		set hyd.Monitor.DisableSteeringActiveLegacyClients='0'
		set hyd.Monitor.DisableSteeringMax11kUnfriendlyClients='0'
		commit hyd
	EOF

}

config_wsplcd() {
	local Mode="$1"
	local RunMode DeepClone
	local configSta
	[ -z "$1" ] && return

	if [ "$Mode" = "1" ]; then
		RunMode="REGISTRAR"
		DeepClone="1"
		configSta="1"
	else
		RunMode="ENROLLEE"
		DeepClone="0"
		configSta="0"
	fi
	uci batch <<-EOF
		set wsplcd.config.HyFiSecurity='1'
		set wsplcd.config.RunMode="$RunMode"
		set wsplcd.config.ManageVAPInd='0'
		set wsplcd.config.DeepClone="$DeepClone"
		set wsplcd.config.DebugLevel='DEBUG'
		set wsplcd.config.MapEnable='0'
		set wsplcd.config.ConfigSta="$configSta"
		set wsplcd.config.WriteDebugLogToFile="NONE"
		commit wsplcd
	EOF

}

config_repacd() {
	local Mode="$1"
	local RateNumMeasurements r5GBackhaulEvalTimeShort
	local r5GBackhaulEvalTimeLong BSSIDAssociationTimeout
	local r2GBackhaulEvalTime ManageVAPInd GatewayConnectedMode

	[ -z "$1" ] && return

	if [ "$Mode" = "1" ]; then
		repacd_role="CAP"
		AssociationTimeout="170"
		RateNumMeasurements="5"
		r5GBackhaulEvalTimeShort="330"
		r5GBackhaulEvalTimeLong="1800"
		r2GBackhaulEvalTime="1800"
		BSSIDAssociationTimeout="90"
		ManageVAPInd="0"
		GatewayConnectedMode="CAP"
	else
		repacd_role="NonCAP"
		AssociationTimeout="170"
		RateNumMeasurements="5"
		r5GBackhaulEvalTimeShort="330"
		r5GBackhaulEvalTimeLong="1800"
		r2GBackhaulEvalTime="180"
		BSSIDAssociationTimeout="90"
		ManageVAPInd="1"
		GatewayConnectedMode="AP"
	fi

	uci batch <<-EOF
		set repacd.repacd.Enable='1'
		set repacd.repacd.TrafficSeparationEnabled='1'
		set repacd.repacd.TrafficSeparationActive='1'
		set repacd.repacd.NetworkBackhaul='backhaul'
		set repacd.repacd.NetworkGuest='guest'
		set repacd.repacd.ConfigREMode='son'
		set repacd.repacd.Role="$repacd_role"
		set repacd.repacd.GatewayConnectedMode="$GatewayConnectedMode"
		set repacd.WiFiLink.AssociationTimeout="$AssociationTimeout"
		set repacd.WiFiLink.RateNumMeasurements="$RateNumMeasurements"
		set repacd.WiFiLink.RateThresholdMax5GInPercent='95'
		set repacd.WiFiLink.RateScalingFactor='85'
		set repacd.WiFiLink.5GBackhaulEvalTimeShort="$r5GBackhaulEvalTimeShort"
		set repacd.WiFiLink.5GBackhaulEvalTimeLong="$r5GBackhaulEvalTimeLong"
		set repacd.WiFiLink.PreferCAPSNRThreshold5G='35'
		set repacd.WiFiLink.MoveFromCAPSNRHysteresis5G='5'
		set repacd.WiFiLink.2GBackhaulEvalTime="$r2GBackhaulEvalTime"
		set repacd.WiFiLink.BSSIDAssociationTimeout="$BSSIDAssociationTimeout"
		set repacd.WiFiLink.ManageVAPInd="$ManageVAPInd"
		set repacd.WiFiLink.SNRThresholdPrefer2GBackhaul='20'
		set repacd.WiFiLink.RateThresholdPrefer2GBackhaulInPercent='3'
		set repacd.WiFiLink.EnableSNRBasedPrefer2GBackhaul='1'
		set repacd.WiFiLink.MaxMeasuringStateAttempts='30'
		commit repacd
	EOF
}
