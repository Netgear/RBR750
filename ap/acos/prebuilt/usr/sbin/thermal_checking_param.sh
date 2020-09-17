#!/bin/sh

str_find="`cat ./thermal_checking.conf | grep \"$1\"`"
str_replace="${1}=${2}"
sed -i "s/$str_find/$str_replace/g" ./thermal_checking.conf
