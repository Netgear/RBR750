#!/bin/sh

COUNT=0
LOCKFILE=/tmp/parent_hyt_result.sh.lock
RESULT_TMP=/tmp/hyt_result.tmp
RESULT_FINAL=/tmp/hyt_result
DEBUG="`nvram get all_mesh_mac_debug`"

clean_flags()
{
    rm -rf $LOCKFILE
    # Triggered flag.. set in heartbeat
    # rm -rf "/tmp/.get_parent_hyt_mac"
}

set -o noclobber
while ! echo $$ > $LOCKFILE;
do
    echo ">>>>>>>>>> Wait for updating MAC...COUNT=$COUNT <<<<<<<<<<"
    COUNT=`expr $COUNT + 1`
    sleep 1
    # if can not get the key in 5 seconds, give up !
    if [ $COUNT -gt 4 ]; then
        echo ">>>>>>>>>> Can not get key in 5 seconds, give up ! <<<<<<<<<<"
        clean_flags
        break
    fi
done
set +o noclobber

if [ "$DEBUG" == "1" ]; then
    echo ">>>>>>>>>> update Base and Satellite MAC <<<<<<<<<<"
fi

(echo "td s2";sleep 2) | hyt | grep "QCA IEEE 1905.1 " | awk -F "device: " '{print $2}' | cut -d',' -f1 > $RESULT_TMP
# Avoid race condition, r/w at the same time.
cp -f $RESULT_TMP $RESULT_FINAL
# Clean up flag
clean_flags
