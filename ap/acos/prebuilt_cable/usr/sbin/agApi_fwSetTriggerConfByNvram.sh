#!/bin/sh


LOCKFILE="/tmp/agApi_fwSetTriggerConfByNvram.sh.lock"
COUNTER=0
set -o noclobber
while ! echo $$ > $LOCKFILE; do
    echo "wait for agApi_fwSetTriggerConfByNvram.sh finished"
	COUNTER=$((${COUNTER}+1))
    sleep 1
	if [ "$COUNTER" == "10" ]; then
		break;
	fi
done
set +o noclobber

i=0
LAN_IFNAME="`nvram get lan_ifname`"
FWPT_COUNT="`nvram get fwpt_count`"
FWPT_TIMEOUT="`nvram get fwpt_timeout`"

FWPT_TMP_FILE="/tmp/fwpt.addr.tmp"

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

RET=`iptables -t nat -F PORTTRIGGER_NAT_CHAIN`
RET=`iptables -t filter -F PORTTRIGGER_FORWARD_IN_CHAIN`

#echo "Start agApi_fwSetTriggerConfByNvram.sh"

RET=`echo ${FWPT_TIMEOUT} > /proc/porttrigger_timeout`

while [ $i -lt $FWPT_COUNT ];
do
    echo "nvram=`nvram get fwpt$i`"

    FWPT_ENABLE="`nvram get fwpt$i | cut -d':' -f1`"
    FWPT_IPARRR_HEX="`nvram get fwpt$i | cut -d':' -f4`"
    FWPT_MATCH_PROTO="`nvram get fwpt$i | cut -d':' -f5`"
    FWPT_MATCH_PORT="`nvram get fwpt$i | cut -d':' -f6`"
    FWPT_TRIGGER_PROTO="`nvram get fwpt$i | cut -d':' -f9`"
    FWPT_TRIGGER_SPORT="`nvram get fwpt$i | cut -d':' -f10`"
    FWPT_TRIGGER_DPORT="`nvram get fwpt$i | cut -d':' -f11`"

    #echo LAN_IFNAME=$LAN_IFNAME
    #echo WAN_IFNAME=$WAN_IFNAME
    #echo FWPT_ENABLE=$FWPT_ENABLE
    #echo FWPT_IPARRR_HEX=$FWPT_IPARRR_HEX
    #echo FWPT_MATCH_PROTO=$FWPT_MATCH_PROTO
    #echo FWPT_MATCH_PORT=$FWPT_MATCH_PORT
    #echo FWPT_TRIGGER_PROTO=$FWPT_TRIGGER_PROTO
    #echo FWPT_TRIGGER_SPORT=$FWPT_TRIGGER_SPORT
    #echo FWPT_TRIGGER_DPORT=$FWPT_TRIGGER_DPORT

    if [ "$FWPT_ENABLE" != "1" ]; then
        i=`expr $i + 1`
        continue;
    fi

    if [ "$FWPT_MATCH_PROTO" == "6" ]; then
        FWPT_MATCH_PROTO_STR="tcp"
    elif [ "$FWPT_MATCH_PROTO" == "17" ]; then
        FWPT_MATCH_PROTO_STR="udp"
    # We set default as tcp
    else
        FWPT_MATCH_PROTO_STR="tcp"
    fi

    if [ "$FWPT_TRIGGER_PROTO" == "6" ]; then
        FWPT_TRIGGER_PROTO_STR="tcp"
    elif [ "$FWPT_TRIGGER_PROTO" == "17" ]; then
        FWPT_TRIGGER_PROTO_STR="udp"
    elif [ "$FWPT_TRIGGER_PROTO" == "100" ]; then
        FWPT_TRIGGER_PROTO_STR="all"
    # We set default as tcp
    else
        FWPT_TRIGGER_PROTO_STR="tcp"
    fi

    # Check IP address
    FWPT_IPARRR_STR=""
    echo "$FWPT_IPARRR_HEX" > $FWPT_TMP_FILE
    FWPT_IPADDR_CHAR=`wc -c $FWPT_TMP_FILE | cut -d' ' -f1`
    if [ $FWPT_IPADDR_CHAR -eq 9 ]; then
        IP1=${FWPT_IPARRR_HEX:6:2}
        IP2=${FWPT_IPARRR_HEX:4:2}
        IP3=${FWPT_IPARRR_HEX:2:2}
        IP4=${FWPT_IPARRR_HEX:0:2}
        FWPT_IPARRR_STR="-s $((0x${IP1})).$((0x${IP2})).$((0x${IP3})).$((0x${IP4}))"
    fi

    # Everything is ready. Start commands...
    # Outbound Traffic
    if [ "$FWPT_MATCH_PROTO_STR" == "tcp" ]; then
        RET=`iptables -t nat -A PORTTRIGGER_NAT_CHAIN -i ${LAN_IFNAME} ${FWPT_IPARRR_STR} -p tcp --dport ${FWPT_MATCH_PORT} -j TRIGGER --trigger-type out --trigger-proto ${FWPT_TRIGGER_PROTO_STR} --trigger-match ${FWPT_TRIGGER_SPORT}-${FWPT_TRIGGER_DPORT} --trigger-relate ${FWPT_TRIGGER_SPORT}-${FWPT_TRIGGER_DPORT}`

    elif [ "$FWPT_MATCH_PROTO_STR" == "udp" ]; then
        RET=`iptables -t nat -A PORTTRIGGER_NAT_CHAIN -i ${LAN_IFNAME} ${FWPT_IPARRR_STR} -p udp --dport ${FWPT_MATCH_PORT} -j TRIGGER --trigger-type out --trigger-proto ${FWPT_TRIGGER_PROTO_STR} --trigger-match ${FWPT_TRIGGER_SPORT}-${FWPT_TRIGGER_DPORT} --trigger-relate ${FWPT_TRIGGER_SPORT}-${FWPT_TRIGGER_DPORT}`
    fi


    # Inbound Traffic
    if [ "$FWPT_TRIGGER_PROTO_STR" == "all" ]; then
    # tcp + udp
        RET=`iptables -t nat -A PORTTRIGGER_NAT_CHAIN -i ${WAN_IFNAME} -p tcp --dport ${FWPT_TRIGGER_SPORT}:${FWPT_TRIGGER_DPORT} -j TRIGGER --trigger-type dnat`
        RET=`iptables -t nat -A PORTTRIGGER_NAT_CHAIN -i ${WAN_IFNAME} -p udp --dport ${FWPT_TRIGGER_SPORT}:${FWPT_TRIGGER_DPORT} -j TRIGGER --trigger-type dnat`

        RET=`iptables -t filter -A PORTTRIGGER_FORWARD_IN_CHAIN -p tcp --dport ${FWPT_TRIGGER_SPORT}:${FWPT_TRIGGER_DPORT} -j LOG --log-prefix PORT_FORWARD --log-level 5`
        RET=`iptables -t filter -A PORTTRIGGER_FORWARD_IN_CHAIN -p udp --dport ${FWPT_TRIGGER_SPORT}:${FWPT_TRIGGER_DPORT} -j LOG --log-prefix PORT_FORWARD --log-level 5`

        RET=`iptables -t filter -A PORTTRIGGER_FORWARD_IN_CHAIN -p tcp --dport ${FWPT_TRIGGER_SPORT}:${FWPT_TRIGGER_DPORT} -j TRIGGER --trigger-type in`
        RET=`iptables -t filter -A PORTTRIGGER_FORWARD_IN_CHAIN -p udp --dport ${FWPT_TRIGGER_SPORT}:${FWPT_TRIGGER_DPORT} -j TRIGGER --trigger-type in`
    # tcp or udp
    else
        RET=`iptables -t nat -A PORTTRIGGER_NAT_CHAIN -i ${WAN_IFNAME} -p ${FWPT_TRIGGER_PROTO_STR} --dport ${FWPT_TRIGGER_SPORT}:${FWPT_TRIGGER_DPORT} -j TRIGGER --trigger-type dnat`

        RET=`iptables -t filter -A PORTTRIGGER_FORWARD_IN_CHAIN -p ${FWPT_TRIGGER_PROTO_STR} --dport ${FWPT_TRIGGER_SPORT}:${FWPT_TRIGGER_DPORT} -j LOG --log-prefix PORT_FORWARD --log-level 5`

        RET=`iptables -t filter -A PORTTRIGGER_FORWARD_IN_CHAIN -p ${FWPT_TRIGGER_PROTO_STR} --dport ${FWPT_TRIGGER_SPORT}:${FWPT_TRIGGER_DPORT} -j TRIGGER --trigger-type in`
    fi

    i=`expr $i + 1`
done

echo "Done"
rm -f $LOCKFILE
