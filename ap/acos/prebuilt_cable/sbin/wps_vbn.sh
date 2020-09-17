#!/bin/sh
. /lib/functions.sh
# Foxconn WPS Virtual pushbutton
local fh_2g_if
local fh_5g_if
local guest_2g_if
local guest_5g_if
config_load wireless
 
echo "" > /dev/console
echo "WPS VIRTUAL PUSH BUTTON EVENT DETECTED" > /dev/console

echo "ap server mode" > /dev/console
rm /tmp/acos_wps_status

killall -SIGUSR1 acos_wps_led
sleep 2

config_get fh_2g_if "FhAp2" ifname
#echo "$fh_2g_if"  > /dev/console
config_get fh_5g_if "FhAp5" ifname 
#echo "$fh_5g_if"  > /dev/console
config_get guest_2g_if "Guest2" ifname
#echo "$guest_2g_if"  > /dev/console
config_get guest_5g_if "Guest5" ifname
#echo "$guest_5g_if"  > /dev/console
for dir in /var/run/hostapd-*; do
    [ -d "$dir" ] || continue
    for vap_dir in $dir/ath* $dir/wlan*; do
        [ -r "$vap_dir" ] || continue
        nopbn=`iwpriv "${vap_dir#"$dir/"}"   get_nopbn  |   cut -d':' -f2`
        if [ "${vap_dir#"$dir/"}" != "$fh_2g_if" -a "${vap_dir#"$dir/"}" != "$fh_5g_if" -a "${vap_dir#"$dir/"}" != "$guest_2g_if" -a "${vap_dir#"$dir/"}" != "$guest_5g_if" ]; then 
            if [ $nopbn != 1 ]; then
                echo  "${vap_dir#"$dir/"}" > /dev/console
                hostapd_cli -i "${vap_dir#"$dir/"}" -p "$dir" wps_pbc
            fi
        fi
    done
done
#        fi
