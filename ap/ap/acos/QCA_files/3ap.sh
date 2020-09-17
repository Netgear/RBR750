#!/bin/sh
echo '---3 radio AP mode----'
rm -f /sbin/acos_service
rm -f /etc/rc.d/S09rcd
rm -f /etc/rc.d/S19acos_rc
rm -f /etc/rc.d/S52wsplcd
rm -f /etc/rc.d/S55hyd
rm -f /etc/rc.d/S55lbd
rm -f /etc/rc.d/S97repacd

rm -f /etc/config/wireless
wifi detect > /etc/config/wireless

uci set wireless.wifi0.disabled=0
uci set wireless.wifi0.macaddr=00:00:00:00:00:01
uci set wireless.wifi0.hwmode=11ng
uci set wireless.wifi0.htmode=HT40
uci set wireless.wifi0.channel=6
uci set wireless.@wifi-iface[0]=wifi-iface
uci set wireless.@wifi-iface[0].device=wifi0
uci set wireless.@wifi-iface[0].network=lan
uci set wireless.@wifi-iface[0].mode=ap
uci set wireless.@wifi-iface[0].disablecoext=1
uci set wireless.@wifi-iface[0].ssid=OrbiQ-2G
uci set wireless.@wifi-iface[0].encryption=none
uci set wireless.@wifi-iface[0].key=12345678

uci set wireless.wifi1.disabled=0
uci set wireless.wifi1.macaddr=00:00:00:00:00:02
uci set wireless.wifi1.hwmode=11ac
uci set wireless.wifi1.htmode=HT80
uci set wireless.wifi1.channel=157
uci set wireless.@wifi-iface[1]=wifi-iface
uci set wireless.@wifi-iface[1].device=wifi1
uci set wireless.@wifi-iface[1].network=lan
uci set wireless.@wifi-iface[1].mode=ap
uci set wireless.@wifi-iface[1].ssid=OrbiQ-5GH-1
uci set wireless.@wifi-iface[1].encryption=none
uci set wireless.@wifi-iface[1].key=12345678

uci set wireless.wifi2.disabled=0
uci set wireless.wifi2.macaddr=00:00:00:00:00:03
uci set wireless.wifi2.hwmode=11ac
uci set wireless.wifi2.htmode=HT80
uci set wireless.wifi2.channel=48
uci set wireless.@wifi-iface[2]=wifi-iface
uci set wireless.@wifi-iface[2].device=wifi2
uci set wireless.@wifi-iface[2].network=lan
uci set wireless.@wifi-iface[2].mode=ap
uci set wireless.@wifi-iface[2].ssid=OrbiQ-5GL-2
uci set wireless.@wifi-iface[2].encryption=none
uci set wireless.@wifi-iface[2].key=12345678
uci commit

cat << EOF > /etc/rc.d/S99te_test
#!/bin/sh
touch /tmp/udhcpd.leases
touch /tmp/udhcpd_resrv.conf
udhcpd /etc/udhcpd.conf
utelnetd &
EOF

chmod +x /etc/rc.d/S99te_test

cat << EOF > /etc/udhcpd.conf 
pidfile /tmp/udhcpd.pid
start 192.168.1.128
end 192.168.1.200
interface br-lan
remaining yes
lease_file /tmp/udhcpd.leases
option subnet 255.255.255.0
option router 192.168.1.250
option dns 192.168.1.250
option lease 86400
option wins 
option domain 
EOF



reboot
