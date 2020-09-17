#!/bin/sh
#
# Copyright: (c) 2018 Netgear, Inc.
#            All rights reserved
#

# package list
PKGS="xagent remGenie upagent bst"


switch() {
	for pkg in ${PKGS};
	do
		echo "Switching ${pkg} to ${1} environment" > /dev/kmsg
		( cd ${pkg}; ./switch.sh $1 )
	done
}

while [ 1 ];
do
	# wait for value change
	d2 -w XagentCtrl.xcenv
	
	case "$(d2 -s XagentCtrl.xcenv)" in
	dev)
	   # switch config to dev environment
	   switch dev
	   ;;
	qa)
	   # switch config to QA environment
	   switch qa
	   ;;
	qa2)
	   # switch config to QA environment
	   switch qa2
	   ;;
	beta)
	   # switch config to BETA environment
	   switch beta
	   ;;
	staging)
	   # switch config to staging environment
	   switch staging
	   ;;
	prod)
	   # switch config to production environment
	   switch prod
	   ;;
	*)
	   # some sort of error
	   echo "Unsupported xcloud env: $(d2 -s XagentCtrl.xcenv)" > /dev/kmsg 
	   ;;
	esac
	
	# reset value back
	d2 -c XagentCtrl.xcenv none

done
