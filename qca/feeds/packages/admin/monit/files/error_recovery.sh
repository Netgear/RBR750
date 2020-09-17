#!/bin/sh

monitor_file="/tmp/error_monitor"
monit_conf_file="/etc/monitrc"

#check if monitor file is present else create
if [ ! -f $monitor_file ]; then
	touch $monitor_file
fi

#get the timestamp of previous failure
prev_time=`grep $1= $monitor_file | cut -d '=' -f2`

#initialise the previous time if not present

if [ ! $prev_time ]; then
	prev_time=0
	echo $1=$prev_time >> $monitor_file
fi

#get the gateway ip if unable to get before,  update the monitrc config file and reload it
if [ $1 == "gateway_ping" ]; then
     gateway_obtained=`grep gateway_status= $monitor_file | cut -d '=' -f2`
     if [ ! $gateway_obtained ]; then
	echo gateway_status=192.168.1.1 >> $monitor_file
     fi
     
     gateway_ip=`route -n | grep UG | awk '{print $2}'`
     if [ ! $gateway_obtained ] || [ x$gateway_obtained != x$gateway_ip ]; then
	if [ $gateway_ip ]; then
		sed -i "s/check host gateway with address.*/check host gateway with address $gateway_ip/g" $monit_conf_file
		sed -i "s,gateway_status=.*,gateway_status=$gateway_ip,g" $monitor_file
		/usr/bin/monit -g recovery_monitor monitor
		sleep 2
		/usr/bin/monit reload
		exit
	fi
     fi
fi

if [ $2 == "failed" ]; then
	cur_time=`date +%s`

	if [ $prev_time -eq 0 ]; then
		prev_time=$cur_time
	fi

	time_diff=`expr $cur_time - $prev_time`
	repacd_pid=`/bin/pidof repacd-run.sh | /usr/bin/awk '{print $1}'`

            #time difference need to be 600 secs in came gw not reachable and
            #300 in case of vap status down to restart repacd

	if ( [ $time_diff -ge 600 ] && [ $repacd_pid ] ) || ( [ $time_diff -ge 300 ] && [ $1 != "gateway_ping" ] ); then

		/usr/bin/monit -g recovery_monitor unmonitor
		/etc/init.d/repacd restart

		prev_time=0
		checker_name="$1_recovery_counter"
		checker_value=`grep $checker_name $monitor_file | cut -d '=' -f2`

		if [ ! $checker_value ]; then
			checker_value=0
			echo $checker_name=$checker_value >> $monitor_file
		fi

		checker_value=`expr $checker_value + 1`
	        sed -i "s,$checker_name=.*,$checker_name=$checker_value,g" $monitor_file
		cat $monitor_file
		sleep 2
		/usr/bin/monit -g recovery_monitor monitor

	fi
else
        prev_time=0
fi
sed -i "s,$1=.*,$1=$prev_time,g" $monitor_file
