#!/bin/sh

# save the thermal log in memory. Reboot will lost thermal log data

# File size limitation: There will be 2 files, thermal-log1.txt and thermal-log2.txt
file_num=1

touch /etc/burn_in_test_log
sleep 10
/sbin/burn_in &
while [ 1 ]
do
	
	filesize=$(stat -c%s /etc/burn_in_test_log)
	# The maximum of each file is 10MB 10485760
	if [ $filesize -ge 10485760 ]; then
		echo "filesize if over, change to another burn_in-log file"
		if [ $file_num -eq 1 ]; then
			killall burn_in
			cp /etc/burn_in_test_log /etc/burn_in_test_log$file_num
			sleep 20
			rm -rf /etc/burn_in_test_log
			/sbin/burn_in &
			file_num=2;
		else
			killall burn_in
			cp /etc/burn_in_test_log /etc/burn_in_test_log$file_num
			sleep 20
			rm -rf /etc/burn_in_test_log
			/sbin/burn_in &		
			file_num=1;
		fi
		# Once 1 file has reached the maximum(10MB), start write to another file
		[ -f /etc/burn_in_test_log$file_num ] && rm -rf /etc/burn_in_test_log$file_num
	fi
	sleep 13
	echo "======================next time=====================" >> /etc/burn_in_test_log$file_num
done

