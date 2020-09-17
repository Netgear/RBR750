#!/bin/sh
# Foxconn WPS Virtual pushbutton to Reset WPS
/usr/sbin/nvram set wps_led_state=2
/usr/sbin/nvram set wps_start=none
for dir in /var/run/hostapd-*; do
                [ -d "$dir" ] || continue
                for vap_dir in $dir/ath* $dir/wlan*; do
                [ -r "$vap_dir" ] || continue
                nopbn=`iwpriv "${vap_dir#"$dir/"}"   get_nopbn  |   cut -d':' -f2`
                if [ $nopbn != 1 ]; then
                        hostapd_cli -i "${vap_dir#"$dir/"}" -p "$dir" wps_cancel
                fi
				done
	echo "" > /dev/console
    echo "RESET WPS EVENT DETECTED" > /dev/console
    rm -rf /overlay/*
done

