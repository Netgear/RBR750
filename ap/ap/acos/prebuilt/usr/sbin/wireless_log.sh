#!/bin/sh

# Collect basi debug information
# File size limitation: There will be 2 files, wireless-log1.txt and wireless-log2.txt

file_num=1
local max_filesize=$(nvram get wifi_debug_max_log_size)

[  "$max_filesize" = "" ] && max_filesize=5

max_filesize=$(($max_filesize*1024*1024))
local wifi_debug_option=$(nvram get wifi_debug_option)
local enable_wifi_debug=$(nvram get enable_wifi_debug)

if [ "$enable_wifi_debug" = "1" ];then
	iwpriv ath0 dbgLVL $wifi_debug_option
	iwpriv ath1 dbgLVL $wifi_debug_option
	iwpriv ath01 dbgLVL $wifi_debug_option
	iwpriv ath2 dbgLVL $wifi_debug_option
fi


while [ 1 ]
do
echo "-------------------------------ps---------------------------------" >> /tmp/wireless-log$file_num.txt
	ps >>/tmp/wireless-log$file_num.txt
echo "-------------------------------iwconfig---------------------------------" >> /tmp/wireless-log$file_num.txt
	iwconfig >>/tmp/wireless-log$file_num.txt
echo "--------------------------wlanconfig athX list--------------------------" >> /tmp/wireless-log$file_num.txt
	echo "wlanconfig ath0 list:" >>/tmp/wireless-log$file_num.txt
	wlanconfig ath0 list >>/tmp/wireless-log$file_num.txt
	echo "wlanconfig ath1 list:" >>/tmp/wireless-log$file_num.txt
	wlanconfig ath1 list >>/tmp/wireless-log$file_num.txt
	echo "wlanconfig ath2 list:" >>/tmp/wireless-log$file_num.txt
	wlanconfig ath2 list >>/tmp/wireless-log$file_num.txt
	echo "wlanconfig ath01 list:" >>/tmp/wireless-log$file_num.txt
	wlanconfig ath01 list >>/tmp/wireless-log$file_num.txt
echo "------------------------------athstats----------------------------------" >> /tmp/wireless-log$file_num.txt
	athstats >>/tmp/wireless-log$file_num.txt
echo "-------------current topology information via Wi-Fi SON-----------------" >> /tmp/wireless-log$file_num.txt
	(echo "td s2"; sleep 2) | hyt |tee >>/tmp/wireless-log$file_num.txt
echo "---------Satellite's current backhaul is (ath01)2G or (ath2)5G ?--------" >> /tmp/wireless-log$file_num.txt
	hyctl show >>/tmp/wireless-log$file_num.txt
	hyctl gethdtbl br-lan 100 | grep ath >>/tmp/wireless-log$file_num.txt
	
echo "---------------------Wi-Fi SON daemons status---------------------------" >> /tmp/wireless-log$file_num.txt
	echo "Wi-Fi SON daemons status" >>/tmp/wireless-log$file_num.txt
	echo -n "hyd_enable =">>/tmp/wireless-log$file_num.txt;
	nvram get hyd_enable >>/tmp/wireless-log$file_num.txt
	echo -n "repacd_enable =">>/tmp/wireless-log$file_num.txt;
	nvram get repacd_enable >>/tmp/wireless-log$file_num.txt
	echo -n "wsplcd_enable=">>/tmp/wireless-log$file_num.txt;
	nvram get wsplcd_enable >>/tmp/wireless-log$file_num.txt
        [ -f /tmp/radardetect.log ] && {
            echo "------------------------------radardetect-------------------------------" >> /tmp/wireless-log$file_num.txt
            cat /tmp/radardetect.log >> /tmp/wireless-log$file_num.txt
        }

	echo "========================next loop==================================" >>/tmp/wireless-log$file_num.txt
	sleep 1
	filesize=`ls -l /tmp/wireless-log$file_num.txt | awk '{print $5}'`

	if [ $filesize -ge $max_filesize ]; then
	echo "wifi log filesize is over, change to another wireless-log file"
		if [ $file_num -eq 1 ]; then
			file_num=2;
		else
			file_num=1;
		fi
	# Once 1 file has reached the maximum, start write to another file
	[ -f /tmp/wireless-log$file_num.txt ] && rm -rf /tmp/wireless-log$file_num.txt
	
	fi
	sleep 60

done

