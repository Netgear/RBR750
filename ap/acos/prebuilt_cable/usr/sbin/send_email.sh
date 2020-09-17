#!/bin/sh
# Author: Elizabeth 20170503
# Purpose: use for cron to send email by sending signal to email daemon
# Notes: 2019/2/12 Set fw_email_send_all=0 for 'Send logs by schedule', =1 for GUI 'Send Log' button pressed
# Spec. define: Schedule Once the log sends out by e-mail successfully, then it will be deleted from the logs

nvram set fw_email_send_all=0
kill -SIGUSR2 `nvram get fw_email_pid`

