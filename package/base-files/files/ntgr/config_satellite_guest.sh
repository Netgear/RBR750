#!/bin/sh

. /ntgr/ntgr_config_utilities.sh

disable_mcsd
# Use wireless backhaul names as 4 parameters below
if [ "$1" = "bridge" ]; then
	config_network_re_bridge ath2 ath11 ath21 ath12
else
	config_network_re ath2 ath11 ath21 ath12
fi

config_firewall 0

config_default_radio

# Delete all wifi-ifaces
delete_vaps
 
# Fronthaul 5GHz interface - 0 ath0
config_fronthaul_ap "wifi0" "ORBI-911" "password" "" "0" "1"
# Fronthaul 2.4GHz interface - 1 ath1
config_fronthaul_ap "wifi1" "ORBI-911" "password" "" "0" "1"
# Backhaul 5 GHz interface -2 ath2
config_backhaul_ap "wifi2" "ORBI-911-Spl" "1234567890" "" "0" "1"
# Backhaul 2.4 STA interface -3 ath11
config_backhaul_sta "wifi1" "ORBI-911-Spl" "1234567890" "" 0
# Guest network 5 GHz interface - 4 ath01
config_guest_ap "wifi0" "RBI-911-Guest" "" "" "0" "1"
# Backhaul 2.4 GHz interface - 5 ath12
config_backhaul_ap "wifi1" "ORBI-911-Spl" "1234567890" "" "0" "1"
# Backhaul 5 GHz STA interface - 6 ath21
config_backhaul_sta "wifi2" "ORBI-911-Spl" "1234567890" "" 0
# Guest network 2.4 GHz interface - 7 ath13
config_guest_ap "wifi1" "RBI-911-Guest" "" "" "0" "1"
# Smart monitor 5 GHz interface - 8 ath02
config_smart_mon "wifi0" "AEV_MON_AP0" "" "0" "1"
# Smart monitor 2.4 GHz interface - 9 ath14
config_smart_mon "wifi1" "AEV_MON_AP1" "" "0" "1"

uci commit wireless

config_lbd 0

config_hyd 0

config_wsplcd 0

config_repacd 0

/etc/init.d/dnsmasq restart

/etc/init.d/repacd start

