#!/bin/sh
# This wait should be continous in nature and thread should act as daemon listening for any change in RegionName field in DAL table
while true
#Step 1 : Check if d2d process is already started or not 
OUTPUT="$(ps | grep d2d | wc -l)"
#echo "${OUTPUT}"
MODELNAME="$(echo -n $(d2 general[0].DeviceModel) | tail -c 5)" 
if [[ "${MODELNAME}" = "RBR50" ]] ;
then
	if [ ${OUTPUT} -gt 2 ]
	then
  		echo Enjoy !!!!! D2D running
	else
  		echo D2D stopped running
  		exit 0
	fi
else
	if [ ${OUTPUT} -gt 1 ]
	then
  		echo Enjoy !!!!! D2D running
	else
  		echo D2D stopped running
  		exit 0
	fi
fi
do
d2 -w general[0].RegionName
#Step 2 :Check the latest Region Name Value updated*/
REGIONNAME="$(echo -n $(d2 general[0].RegionName) | tail -c 2)"

#Step 3:Convert the RegionName to corresponding Region Code*/
#Allowed strings are maintained as per chronological order of country code as per Router Specifications
#Router Spec requirement section 25.7 
#"regionName"               "regionCode"         Comment
#"NA"                       "1"                  North America
#"WW"                       "2"                  Worldwide/Europe
#"GR"                       "3"                  Germany
#"PR"                       "4"                  China
#"RU"                       "5"                  Russia
#"BZ"                       "6"                  Brazil
#"IN"                       "7"                  India
#"KO"                       "8"                  Korea
#"JP"                       "9"                  Japan
#"AU"                       "10"                 Australia
#"US"                       "11"                 US
#"CA"                       "12"                 Canada

case ${REGIONNAME} in
   na)	REGIONCODE=1  ;;
   ww)	REGIONCODE=2  ;;
   gr)	REGIONCODE=3  ;;
   pr)	REGIONCODE=4  ;;
   ru)	REGIONCODE=5  ;;
   bz)	REGIONCODE=6  ;;
   in)	REGIONCODE=7  ;;
   ko)	REGIONCODE=8  ;;
   jp)  REGIONCODE=9  ;;
   au)	REGIONCODE=10 ;;
   us)	REGIONCODE=11 ;;
   ca)	REGIONCODE=12 ;;
   *)
     echo "This should not happen:Illegal Value Passed";;
esac

#Step 4:Update DAL Table to new Region Code*/
d2 -c general[0].Region $REGIONCODE
case $? in 
   *)
   continue 
esac

done

