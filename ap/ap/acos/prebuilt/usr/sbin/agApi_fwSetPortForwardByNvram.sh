#!/bin/sh

LOCKFILE="/tmp/agApi_fwSetPortForwardByNvram.sh.lock"
COUNTER=0
set -o noclobber
while ! echo $$ > $LOCKFILE; do
    echo "wait for agApi_fwSetPortForwardByNvram.sh finished"
	COUNTER=$((${COUNTER}+1))
    sleep 1
	if [ "$COUNTER" == "10" ]; then
		break;
	fi
done
set +o noclobber

i=1
LAN_IFNAME="`nvram get lan_ifname`"
FWPF_RULE="`nvram get inbound_policy_tbl`"
WAN_IP_ADDR="`nvram get wan_ipaddr`"
DHCP_START="`nvram get dhcp_start`"

WAN_PROTO="`nvram get wan_proto`"

if [ "$WAN_PROTO" == "pppoe" ]; then
	WAN_IFNAME="ppp0"
elif [ "$WAN_PROTO" == "pptp" ]; then
	WAN_IFNAME="ppp0"
elif [ "$WAN_PROTO" == "l2tp" ]; then
	WAN_IFNAME="ppp0"
else
	WAN_IFNAME="`nvram get wan_ifname`"
fi

RET=`iptables -t nat -F PORTFORWARD_NAT_CHAIN`
RET=`iptables -t nat -F POST_PORTFORWARD_CHAIN`
RET=`iptables -t filter -F PORTFORWARD_FORWARD_IN_CHAIN`

until [ "$FWPF_RULE" == "" ];
do
    #Mos 2017/12/06, change Portforward GUI to support multiple port ranges (30, 50-60, 65500-65510)
    FWPF_RULE="`nvram get inbound_policy_tbl | cut -d'@' -f$i`"
    FWPF_PROTO="`echo $FWPF_RULE | cut -d';' -f3`"
    FWPF_EX_SPORT="`echo $FWPF_RULE | cut -d';' -f4`"
    FWPF_EX_EPORT="`echo $FWPF_RULE | cut -d';' -f5`"
    FWPF_IN_IPADDR="`echo $FWPF_RULE | cut -d';' -f6`"
    FWPF_IN_SPORT="`echo $FWPF_RULE | cut -d';' -f7`"
    FWPF_IN_EPORT="`echo $FWPF_RULE | cut -d';' -f8`"

    #echo FWPF_RULE=$FWPF_RULE
    #echo FWPF_PROTO=$FWPF_PROTO
    #echo FWPF_EX_SPORT=$FWPF_EX_SPORT
    #echo FWPF_EX_EPORT=$FWPF_EX_EPORT
    #echo FWPF_IN_SPORT=$FWPF_IN_SPORT
    #echo FWPF_IN_EPORT=$FWPF_IN_EPORT
    #echo FWPF_IN_IPADDR=$FWPF_IN_IPADDR

    if [ "$FWPF_PROTO" == "0" ]; then
        FWPF_PROTO_STR="tcp"
    elif [ "$FWPF_PROTO" == "1" ]; then
        FWPF_PROTO_STR="udp"
    elif [ "$FWPF_PROTO" == "15" ]; then
        FWPF_PROTO_STR="all"
    # We set default as tcp
    else
        FWPF_PROTO_STR="tcp"
    fi
	
    # I dont know why they set ex_eport to 0 if ex_eport = ex_sport
    if [ "$FWPF_EX_EPORT" == "0" ]; then
        FWPF_EX_EPORT=$FWPF_EX_SPORT
    fi
    # Everything is ready. Start commands...
    # Inbound Traffic
    # TCP/UDP case
    if [ "$FWPF_PROTO_STR" == "all" ]; then
        # tcp
        RET=`iptables -t nat -A PORTFORWARD_NAT_CHAIN -i ${WAN_IFNAME} -p tcp --dport ${FWPF_EX_SPORT}:${FWPF_EX_EPORT} -j DNAT --to-destination ${FWPF_IN_IPADDR}:${FWPF_IN_SPORT}-${FWPF_IN_EPORT} --onebyone`
        # udp
        RET=`iptables -t nat -A PORTFORWARD_NAT_CHAIN -i ${WAN_IFNAME} -p udp --dport ${FWPF_EX_SPORT}:${FWPF_EX_EPORT} -j DNAT --to-destination ${FWPF_IN_IPADDR}:${FWPF_IN_SPORT}-${FWPF_IN_EPORT} --onebyone`

        # tcp
        RET=`iptables -t filter -A PORTFORWARD_FORWARD_IN_CHAIN -p tcp -d ${FWPF_IN_IPADDR} --dport ${FWPF_IN_SPORT}:${FWPF_IN_EPORT} -j LOG --log-prefix PORT_FORWARD --log-level 5`
        # udp
        RET=`iptables -t filter -A PORTFORWARD_FORWARD_IN_CHAIN -p udp -d ${FWPF_IN_IPADDR} --dport ${FWPF_IN_SPORT}:${FWPF_IN_EPORT} -j LOG --log-prefix PORT_FORWARD --log-level 5`

        # tcp
        RET=`iptables -t filter -A PORTFORWARD_FORWARD_IN_CHAIN -p tcp -d ${FWPF_IN_IPADDR} --dport ${FWPF_IN_SPORT}:${FWPF_IN_EPORT} -j ACCEPT`
        # udp
        RET=`iptables -t filter -A PORTFORWARD_FORWARD_IN_CHAIN -p udp -d ${FWPF_IN_IPADDR} --dport ${FWPF_IN_SPORT}:${FWPF_IN_EPORT} -j ACCEPT`

		# nat loopback
		if [ "$WAN_IP_ADDR" != "0.0.0.0" ]; then
			# tcp
			RET=`iptables -t nat -A PORTFORWARD_NAT_CHAIN  -p tcp -i ${LAN_IFNAME} -s ${DHCP_START}/24 -d ${WAN_IP_ADDR}/32 --dport ${FWPF_EX_SPORT}:${FWPF_EX_EPORT} -j DNAT --to-destination ${FWPF_IN_IPADDR}:${FWPF_IN_SPORT}-${FWPF_IN_EPORT} --onebyone`
			# udp
			RET=`iptables -t nat -A PORTFORWARD_NAT_CHAIN  -p udp -i ${LAN_IFNAME} -s ${DHCP_START}/24 -d ${WAN_IP_ADDR}/32 --dport ${FWPF_EX_SPORT}:${FWPF_EX_EPORT} -j DNAT --to-destination ${FWPF_IN_IPADDR}:${FWPF_IN_SPORT}-${FWPF_IN_EPORT} --onebyone`

			# tcp
			RET=`iptables -t nat -A POST_PORTFORWARD_CHAIN -p tcp -s ${DHCP_START}/24 -o ${LAN_IFNAME} -d ${FWPF_IN_IPADDR} --dport ${FWPF_IN_SPORT}:${FWPF_IN_EPORT} -j SNAT --to-source ${WAN_IP_ADDR}`
			# udp
			RET=`iptables -t nat -A POST_PORTFORWARD_CHAIN -p udp -s ${DHCP_START}/24 -o ${LAN_IFNAME} -d ${FWPF_IN_IPADDR} --dport ${FWPF_IN_SPORT}:${FWPF_IN_EPORT} -j SNAT --to-source ${WAN_IP_ADDR}`
		fi

        # TCP or UDP case
    else
        RET=`iptables -t nat -A PORTFORWARD_NAT_CHAIN -i ${WAN_IFNAME} -p ${FWPF_PROTO_STR} --dport ${FWPF_EX_SPORT}:${FWPF_EX_EPORT} -j DNAT --to-destination ${FWPF_IN_IPADDR}:${FWPF_IN_SPORT}-${FWPF_IN_EPORT} --onebyone`

        RET=`iptables -t filter -A PORTFORWARD_FORWARD_IN_CHAIN -p ${FWPF_PROTO_STR} -d ${FWPF_IN_IPADDR} --dport ${FWPF_IN_SPORT}:${FWPF_IN_EPORT} -j LOG --log-prefix PORT_FORWARD --log-level 5`

        RET=`iptables -t filter -A PORTFORWARD_FORWARD_IN_CHAIN -p ${FWPF_PROTO_STR} -d ${FWPF_IN_IPADDR} --dport ${FWPF_IN_SPORT}:${FWPF_IN_EPORT} -j ACCEPT`
		
		# nat loopback
		if [ "$WAN_IP_ADDR" != "0.0.0.0" ]; then
			RET=`iptables -t nat -A PORTFORWARD_NAT_CHAIN  -p ${FWPF_PROTO_STR} -i ${LAN_IFNAME} -s ${DHCP_START}/24 -d ${WAN_IP_ADDR}/32 --dport ${FWPF_EX_SPORT}:${FWPF_EX_EPORT} -j DNAT --to-destination ${FWPF_IN_IPADDR}:${FWPF_IN_SPORT}-${FWPF_IN_EPORT} --onebyone`

			RET=`iptables -t nat -A POST_PORTFORWARD_CHAIN -p ${FWPF_PROTO_STR} -s ${DHCP_START}/24 -o ${LAN_IFNAME} -d ${FWPF_IN_IPADDR} --dport ${FWPF_IN_SPORT}:${FWPF_IN_EPORT} -j SNAT --to-source ${WAN_IP_ADDR}`
		fi
    fi

    i=`expr $i + 1`
    #Mos 2017/12/06, change Portforward GUI to support multiple port ranges (30, 50-60, 65500-65510)
    #FWPF_RULE="`nvram get pf_services_tbl | cut -d'@' -f$i`"
    FWPF_RULE="`nvram get inbound_policy_tbl | cut -d'@' -f$i`"
done

echo "Done"
rm -f $LOCKFILE
