#!/bin/sh


NUM_2G=`nvram get wifi_2g_sche_rule_count`
NUM_5G=`nvram get wifi_5g_sche_rule_count`
SAVE_CONFIG="/etc/crontabsroot"
USED_CONFIG="/etc/crontabs/root"
DEL_INDEX=0
WHICH_BAND=0

wifi_conf_start()
{
    i=1
    wifi_enable=`nvram get wifi_2g_sche_enable`
    if [ "$wifi_enable" == "1" ]; then
        while [ $i -le $NUM_2G ];
        do
            START=`nvram get wifi_2g_sche_rule$i | cut -d';' -f1`
            END=`nvram get wifi_2g_sche_rule$i | cut -d';' -f2`

            if [ "$START" != "" ]; then
                echo "$START" >> $SAVE_CONFIG
            fi

            if [ "$START" != "" ]; then
                echo "$END" >> $SAVE_CONFIG
            fi

            i=`expr $i + 1`
        done
    fi

    i=1
    wifi_enable=`nvram get wifi_5g_sche_enable`
    if [ "$wifi_enable" == "1" ]; then
        while [ $i -le $NUM_5G ];
        do
            START=`nvram get wifi_5g_sche_rule$i | cut -d';' -f1`
            END=`nvram get wifi_5g_sche_rule$i | cut -d';' -f2`

            if [ "$START" != "" ]; then
                echo "$START" >> $SAVE_CONFIG
            fi

            if [ "$START" != "" ]; then
                echo "$END" >> $SAVE_CONFIG
            fi

            i=`expr $i + 1`
        done
    fi
}

wifi_nvram_delete()
{
    i=1
    j=1
    if [ "$WHICH_BAND" == "2g" ]; then
        while [ $i -le $NUM_2G ];
        do
            j=`expr $i + 1`
            THIS=`nvram get wifi_2g_sche_rule$i`
            NEXT=`nvram get wifi_2g_sche_rule$j`

            if [ $i -ge $DEL_INDEX ];then
                nvram set wifi_2g_sche_rule$i="$NEXT"
            fi
            i=`expr $i + 1`
        done
        i=`expr $NUM_2G - 1`
        nvram set wifi_2g_sche_rule_count="$i"

    elif [ "$WHICH_BAND" == "5g" ]; then
        while [ $i -le $NUM_5G ];
        do
            j=`expr $i + 1`
            THIS=`nvram get wifi_5g_sche_rule$i`
            NEXT=`nvram get wifi_5g_sche_rule$j`

            if [ $i -ge $DEL_INDEX ];then
                nvram set wifi_5g_sche_rule$i="$NEXT"
            fi
            i=`expr $i + 1`
        done
        i=`expr $NUM_5G - 1`
        nvram set wifi_5g_sche_rule_count="$i"
    fi

    nvram commit
}

if [ "$1" == "start" ]; then
    wifi_conf_start

elif [ "$1" == "delete" ]; then
    WHICH_BAND=$2
    DEL_INDEX=$3
    wifi_nvram_delete
    wifi_conf_start
fi
