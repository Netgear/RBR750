#!/bin/sh

usage() {
	cat <<EOF
Usage: [mcs|orbi|smpreset] [athN]
Example:
 mcs    : $0 mcs athN ;show mcs counter per 5 seconds 
 orbi   : $0 orbi ; check both 5G backhaul and fronthaul mcs counter per 5 seconds
 smpreset : $0 smpreset ; reset wifi1 smp_affinity to signle CPU1
 tcpdump: running tcpdump and output on /tmp/BR.cap
EOF
	exit 1
}

mcs() {
i=0
##rfs_wxt_get_cpu_by_irq[131]:INFO:IRQ is bound to more than one CPU
#echo 2 > /proc/irq/201/smp_affinity
while true
do
i=$((i+1))
iwpriv $1 txrx_fw_st_rst 0x3fff
sleep 5
echo "i=$i ==>" && uptime
if [ "$1" == "ath0" -o "$1" == "ath01" ]; then
echo "===2G[$1]===" 
else
echo "===5G[$1]==="
fi 
dmesg -c && iwpriv $1 txrx_fw_stats 6 && iwpriv $1 txrx_fw_stats 3 && dmesg | grep -v "rfs_wxt_get_cpu_by_irq"
echo ""
done

}


orbi() {
i=0
##rfs_wxt_get_cpu_by_irq[131]:INFO:IRQ is bound to more than one CPU
#echo 2 > /proc/irq/201/smp_affinity
while true
do
i=$((i+1))
iwpriv ath1 txrx_fw_st_rst 0x3fff && iwpriv ath2 txrx_fw_st_rst 0x3fff
sleep 5
echo "i=$i ==>" && uptime
iwconfig && wlanconfig ath2 list sta
dmesg -c
echo "===5G[ath2] fronthaul===" 
iwpriv ath2 txrx_fw_stats 6 && iwpriv ath2 txrx_fw_stats 3
dmesg | grep -v "rfs_wxt_get_cpu_by_irq"
dmesg -c
echo "===5G[ath1] backhaul==="
iwpriv ath1 txrx_fw_stats 6 && iwpriv ath1 txrx_fw_stats 3
dmesg | grep -v "rfs_wxt_get_cpu_by_irq"
echo ""
done

}

tcpdump() {
#sleep 70
while true
do
#dhcpc_get_ip=`nvram get dhcpc_get_ip`
if [ -d /sys/class/net/br-lan -a -d /sys/class/net/ath2 ]; then
	killall tcpdump
	sleep 5
	echo '===>running tcpdump...' > /dev/console
	/usr/sbin/nvram set capture_in_process=1
	/usr/sbin/tcpdump -i br-lan -w /tmp/BR.pcap -C 10 -W 1 > /dev/console 2> /dev/console &
	exit 1
fi
sleep 20
done
}


case "$1" in
	mcs) mcs "$2";;
	orbi) orbi;;
	smpreset) echo 2 > /proc/irq/201/smp_affinity;;
	tcpdump) tcpdump;;
	*) usage;;
esac

