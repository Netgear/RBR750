#!/bin/sh

COUNT=0
LOCKFILE=/tmp/parent_hyt_result.sh.lock
RESULT_TMP=/tmp/parent_hyt_result.tmp
RESULT_FINAL=/tmp/parent_hyt_result
DEBUG="`nvram get parent_mac_debug`"

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
    echo ">>>>>>>>>> update parent MAC <<<<<<<<<<"
fi

(echo "td s2";sleep 2) | hyt | grep "Upstream Device" | head -n 1 | cut -d' ' -f6 > $RESULT_TMP
# Avoid race condition, r/w at the same time.
cp -f $RESULT_TMP $RESULT_FINAL
# Clean up flag
clean_flags
