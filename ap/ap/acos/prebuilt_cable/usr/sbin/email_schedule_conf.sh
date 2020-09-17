#!/bin/sh
#
# Write config file for crond

SAVE_CONFIG="/etc/crontabsroot"
#EMAIL_SUBJECT="NETGEAR `nvram get product_description` Log"
EMAIL_SMTP_AUTH_FLAG="-auth"
### Temporarily used since log is not ready yet.
EMAIL_BODY_LOG="/tmp/ripd.conf"

EMAIL_ENABLE=`nvram get fw_email_enable`
EMAIL_TO_ADDR=`nvram get fw_email_to`
EMAIL_SMTP_DOMAIN=`nvram get fw_email_smtp`
EMAIL_SMTP_PORT=`nvram get fw_email_port`
EMAIL_SMTP_AUTH_ENABLE=`nvram get fw_email_auth`
EMAIL_SMTP_AUTH_USER=`nvram get fw_email_user`
EMAIL_SMTP_AUTH_PWD=`nvram get fw_email_pwd`
EMAIL_BLOCK_SITE_ENABLE=`nvram get fw_email_block`
EMAIL_SCHEDULE_MODE=`nvram get fw_email_sche_mode`
EMAIL_SCHEDULE_DAY=`nvram get fw_email_sche_day`
EMAIL_SCHEDULE_HOUR=`nvram get fw_email_sche_hour`
EMAIL_SCHEDULE_AM=`nvram get fw_email_sche_am`

TIME_ZONE=`nvram get time_zone`

email_conf_start()
{
    ACTION=$1

    if [ "$EMAIL_SMTP_AUTH_ENABLE" == "0" ]; then
        EMAIL_SMTP_AUTH_FLAG=""
        EMAIL_SMTP_AUTH_USER="`nvram get fw_email_from`"
        EMAIL_SMTP_AUTH_PWD="NGR1234567890"
    fi

    if [ "$EMAIL_ENABLE" == "1" ]; then
        ## When log is full
        if [ "$EMAIL_SCHEDULE_MODE" == "0" ]; then
            echo "#### Send email would be triggered by Log manager."

        ## Hourly
        elif [ "$EMAIL_SCHEDULE_MODE" == "1" ]; then
            ## Send immediately
            if [ "$ACTION" == "send" ]; then
                kill -SIGUSR2 `nvram get fw_email_pid`

            ## Write configuration file
            elif [ "$ACTION" == "conf" ]; then
                echo "00 * * * * /bin/sh /usr/sbin/send_email.sh" >> $SAVE_CONFIG
            fi

        ## Daily
        elif [ "$EMAIL_SCHEDULE_MODE" == "2" ]; then
            if [ "$EMAIL_SCHEDULE_AM" == "pm" ]; then
                EMAIL_SCHEDULE_HOUR=`expr $EMAIL_SCHEDULE_HOUR + 12`
            fi

            ## Send immediately
            if [ "$ACTION" == "send" ]; then
                kill -SIGUSR2 `nvram get fw_email_pid`

            ## Write configuration file
            elif [ "$ACTION" == "conf" ]; then
                EMAIL_SCHEDULE_MIN=00
                
                # echo "00 $EMAIL_SCHEDULE_HOUR * * * /bin/sh /usr/sbin/send_email.sh" >> $SAVE_CONFIG
                echo "$EMAIL_SCHEDULE_MIN $EMAIL_SCHEDULE_HOUR * * * /bin/sh /usr/sbin/send_email.sh" >> $SAVE_CONFIG
            fi

        ## Weekly
        elif [ "$EMAIL_SCHEDULE_MODE" == "3" ]; then
            if [ "$EMAIL_SCHEDULE_AM" == "pm" ]; then
                EMAIL_SCHEDULE_HOUR=`expr $EMAIL_SCHEDULE_HOUR + 12`
            fi

            # Send immediately
            if [ "$ACTION" == "send" ]; then
                kill -SIGUSR2 `nvram get fw_email_pid`

            ## Write configuration file
            elif [ "$ACTION" == "conf" ]; then
                EMAIL_SCHEDULE_MIN=00
                
                # echo "00 $EMAIL_SCHEDULE_HOUR * * $EMAIL_SCHEDULE_DAY /bin/sh /usr/sbin/send_email.sh" >> $SAVE_CONFIG
                echo "$EMAIL_SCHEDULE_MIN $EMAIL_SCHEDULE_HOUR * * $EMAIL_SCHEDULE_DAY /bin/sh /usr/sbin/send_email.sh" >> $SAVE_CONFIG
            fi

        fi
    fi
}

email_send_immediately()
{
    echo "#### Send email immediately.."
    email_conf_start send
}


if [ "$1" == "start" ]; then
    email_conf_start conf

elif [ "$1" == "send_mail" ]; then
    ## This is for block site manager or others to call
    email_send_immediately
fi
