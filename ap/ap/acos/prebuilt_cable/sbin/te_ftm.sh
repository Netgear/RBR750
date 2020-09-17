#!/bin/sh
wifi down 
sleep 1
rmmod wifi_3_0 
sleep 1
rmmod wifi_2_0
sleep 1
rmmod qca_ol
sleep 1
insmod qca_ol hw_mode_id=1 testmode=1 cfg80211_config=1 
sleep 1
insmod wifi_3_0 
sleep 1
insmod wifi_2_0 
sleep 1
diag_socket_app -a 192.168.1.20 &
sleep 1
/etc/init.d/ftm start 
sleep 1
/usr/sbin/ftm -n & 
ftm -n -dd &
echo "************************************************************"
echo ">>>> TE FTM Starting DONE."
echo "************************************************************"
