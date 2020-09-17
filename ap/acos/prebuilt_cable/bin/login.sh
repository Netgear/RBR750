#!/bin/sh
# Copyright (C) 2006-2011 OpenWrt.org

if [ "$(/usr/sbin/nvram get disable_telnet_login)" = "1" ] ;then
cat << EOF
 === IMPORTANT ============================
 Use 'passwd' to set your login password
 this will disable telnet and enable SSH
 ------------------------------------------
EOF
        exec /bin/ash --login
fi

echo -n "login: "
read telnet_login
echo -n "Password: "
read -s telnet_passwd
if [ "$telnet_passwd" = "" ] ;then
read -s telnet_passwd
fi
if [ "$telnet_login" = "$(/usr/sbin/nvram get http_username)" ] && [ "$telnet_passwd" = "$(/usr/sbin/nvram get http_passwd)" ] ;then
cat << EOF
=== IMPORTANT ============================
 Use 'passwd' to set your login password
 this will disable telnet and enable SSH
------------------------------------------
EOF
exec /bin/ash --login
else
        echo "Login failed."
        exit 0
fi
