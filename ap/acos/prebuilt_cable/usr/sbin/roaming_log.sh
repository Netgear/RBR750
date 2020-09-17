#!/bin/sh

# Collect basi debug information
# File size limitation: There will be 2 files, wireless-log1.txt and wireless-log2.txt

file_num=1
local max_filesize=$(nvram get roaming_debug_max_log_size)

[  "$max_filesize" = "" ] && max_filesize=3

max_filesize=$(($max_filesize*1024*1024))

local enable_roaming_debug=$(nvram get enable_roaming_debug)

if [ "$enable_roaming_debug" != "1" ];then
    (echo "dbg redirect -off"; sleep 2) | hyt
    exit 0
fi

(echo "dbg redirect /tmp/dbghere_$file_num.log"; sleep 2) | hyt

while [ 1 ]
do

    sleep 3
    filesize=`ls -l /tmp/dbghere_$file_num.log | awk '{print $5}'`
    if [ $filesize -ge $max_filesize ]; then
        if [ $file_num -eq 1 ]; then
            file_num=2;
        else
            file_num=1;
        fi
        (echo "dbg redirect /tmp/dbghere_$file_num.log"; sleep 2) | hyt
    fi

done