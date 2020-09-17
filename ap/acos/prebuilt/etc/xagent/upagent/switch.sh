#!/bin/sh

source ./upagent.$1.env || exit 1

d2 -c UpCfg[0].UpBaseURL ${UP_BACKEND_BASE}

d2 -c upstatus[0].addXdeviceIdDone false

/etc/init.d/UPAgent restart
