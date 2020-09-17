#!/bin/sh

source ./remGenie.$1.env || exit 1

/etc/init.d/remGenie stop

d2 -c rgeniecfg[0].genie_remote_url ${RGENIE_REMOTE_URL}
