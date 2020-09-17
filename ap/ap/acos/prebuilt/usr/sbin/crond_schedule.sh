#!/bin/sh

SAVE_CONFIG="/etc/crontabsroot"
USED_CONFIG="/etc/crontabs/root"

del_crond_conf()
{
    rm -rf $SAVE_CONFIG
    rm -rf $USED_CONFIG
}

cp_crond_conf()
{
    cp -f $SAVE_CONFIG $USED_CONFIG
}

restart_crond()
{
    echo "###### Restart crond now."

    del_crond_conf

    ## Add your tasks here
    ## To generate crond conf
    ## /usr/sbin/wifi_schedule_conf.sh start
    /usr/sbin/email_schedule_conf.sh start
    #######

    cp_crond_conf
    sync
    killall -9 crond
    sleep 1
    /etc/init.d/cron start
    /etc/init.d/cron enable
}

if [ "$1" == "restart" ]; then
    restart_crond
fi
