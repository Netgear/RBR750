#!/bin/sh

. /ntgr/ntgr_config_utilities.sh

disable_mcsd

# Use wireless backhaul names as 2 parameters below
if [ "$1" = "bridge" ]; then
	config_network_cap_bridge ath2 ath11
else
	config_network_cap ath2 ath11
fi

config_firewall 1

config_default_radio

# Delete all wifi-ifaces
delete_vaps

# Fronthaul 5GHz interface
config_fronthaul_ap "wifi0" "ORBI-911" "password" "" 0
# Fronthaul 2.4GHz interface
config_fronthaul_ap "wifi1" "ORBI-911" "password" "" 0
# Backhaul 5 GHz interface
config_backhaul_ap "wifi2" "ORBI-911-Spl" "1234567890" "" 0
# Backhaul 2.4
config_backhaul_ap "wifi1" "ORBI-911-Spl" "1234567890" "" 0
# Guest network 5 GHz
config_guest_ap "wifi0" "RBI-911-Guest" "" "" 0
# Guest network 2.4 GHz
config_guest_ap "wifi1" "RBI-911-Guest" "" "" 0

# Smart monitor 5 GH
config_smart_mon "wifi0" "AEV_MON_AP0" "" 0
# Smart monitor 2.4
config_smart_mon "wifi1" "AEV_MON_AP1" "" 0

uci commit wireless

config_lbd 1

config_hyd 1

config_wsplcd 1

config_repacd 1

/etc/init.d/dnsmasq restart
/etc/init.d/repacd start

