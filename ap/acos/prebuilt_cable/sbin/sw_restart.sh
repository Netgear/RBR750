#!/bin/sh

killall -9 heartbeat

rc restart
sleep 5

acos_service restart