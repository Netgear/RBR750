#!/bin/sh
#This script will be called at boot time for marking all previously connected device states to down
index=0

while [ $index -lt 256 ]
do
   echo $index
   d2 -c dilsignature[$index].Devicestate down
   index=`expr $index + 1`
done
