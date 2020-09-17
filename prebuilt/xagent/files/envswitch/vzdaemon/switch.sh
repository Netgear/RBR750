#!/bin/sh

source ./vzdaemon.$1.env || exit 1

# save arlo enabled flag
ENFLG=$(d2 -s arlocfg.enabled)

# force-disable arlo!
d2 -c arlocfg.enabled false

# re-config arlo daemon 
d2 -c arlocfg[0].vz_mcs_url    "${VZ_MCS_URL}"
d2 -c arlocfg[0].vz_server_url "${VZ_SERVER_ULR}"
d2 -c arlocfg[0].vz_update_url "${VZ_UPDATE_URL}"
d2 -c arlocfg[0].vz_log_level  "${VZ_LOG_LEVEL}"

# re-config arlo AP
d2 -c arlocfg[0].SSID       -- arlos_`cat /dev/urandom | tr -cd 'a-zA-Z0-9_-' | head -c 25`;
d2 -c arlocfg[0].passphrase --       `cat /dev/urandom | tr -cd 'a-zA-Z0-9_-' | head -c 32`
d2 -c arloctrl.restart true

rm -rf /tmp/arlo/media

d2 -c arlocfg.enabled ${ENFLG}
