#!/bin/sh
source /usr/sbin/thermal_checking.conf

# AUTHOR       : Elizabeth (elizabeth.xy.kwan@mail.foxconn.com)
# CREATED DATE : 2017/08/22
# REVISION     : 2018/01/09 1. Add SLEEP_TIME
#                           2. If on_board_high == 1, change -hi0 to fixed value 25 
#                2018/01/12 Change on_board_low condition from || to &&
#                2018/01/19 1. Remove ON_BOARD_CABLE_HIGH_THRES and ON_BOARD_CABLE_LOW_THRES
#                           2. Add ON_CHIP_LOW_THRES_0 ON_CHIP_LOW_THRES_1 ON_CHIP_LOW_THRES_2
#                2018/01/24 1. Set wifi0 wifi1 wifi2 low threshold when initiate
#                           2. Modify actions when on_board_low ($on_board_wifi_temp < $ON_BOARD_WIFI_LOW_THRES)
#                           3. Remove on_board_cable related 
#                2018/01/24 MAJOR CHANGES: Rewrite code to simplify 

#echo $ON_CHIP_HIGH_THRES_0
#echo $ON_CHIP_HIGH_THRES_1
#echo $ON_CHIP_HIGH_THRES_2
#echo $ON_BOARD_WIFI_HIGH_THRES
#echo $ON_BOARD_WIFI_LOW_THRES
#echo $ON_CHIP_LOW_THRES_0
#echo $ON_CHIP_LOW_THRES_1
#echo $ON_CHIP_LOW_THRES_2
#echo $DUTY0
#echo $DUTY1
#echo $DUTY2
#echo $BASE_CUSTOM_0
#echo $BASE_CUSTOM_1
#echo $BASE_CUSTOM_2
#echo $SLEEP_TIME

hi1=145
hi2=155
hi3=165
lo2=135
lo3=145

# INITIALIZATION ------------------------------------------------------------------------
thermaltool -i wifi0 -set -e 1 -hi0 $ON_CHIP_HIGH_THRES_0 -hi1 $hi1 -hi2 $hi2 -hi3 $hi3
thermaltool -i wifi1 -set -e 1 -hi0 $ON_CHIP_HIGH_THRES_1 -hi1 $hi1 -hi2 $hi2 -hi3 $hi3
thermaltool -i wifi2 -set -e 1 -hi0 $ON_CHIP_HIGH_THRES_2 -hi1 $hi1 -hi2 $hi2 -hi3 $hi3

thermaltool -i wifi0 -set -e 1 -lo1 $ON_CHIP_LOW_THRES_0 -lo2 $lo2 -lo3 $lo3 
thermaltool -i wifi1 -set -e 1 -lo1 $ON_CHIP_LOW_THRES_1 -lo2 $lo2 -lo3 $lo3 
thermaltool -i wifi2 -set -e 1 -lo1 $ON_CHIP_LOW_THRES_2 -lo2 $lo2 -lo3 $lo3 

thermaltool -i wifi0 -set -off0 0
thermaltool -i wifi1 -set -off0 0
thermaltool -i wifi2 -set -off0 0
# ---------------------------------------------------------------------------------------

echo "thermal_checking.sh: get base temperature from on-chip sensors"

base_0=`thermaltool -i wifi0 -get | grep "sensor" | sed -E 's/.*[ ]([0-9]+)[,].*/\1/g'`
base_1=`thermaltool -i wifi1 -get | grep "sensor" | sed -E 's/.*[ ]([0-9]+)[,].*/\1/g'`
base_2=`thermaltool -i wifi2 -get | grep "sensor" | sed -E 's/.*[ ]([0-9]+)[,].*/\1/g'`

sleep 5
echo "thermal_checking.sh: base_0: $base_0"
echo "thermal_checking.sh: base_1: $base_1"
echo "thermal_checking.sh: base_2: $base_2"

while true; do
    #echo "thermal_checking.sh: wait for 60s.."
    #sleep 60;
    echo "thermal_checking.sh: wait for "$SLEEP_TIME"s.."
    sleep $SLEEP_TIME;
    
    flag_check_both=0
    chip_thermal_0=`thermaltool -i wifi0 -get | grep "sensor" | sed -E 's/.*[ ]([0-9]+)[,].*/\1/g'`
    chip_thermal_1=`thermaltool -i wifi1 -get | grep "sensor" | sed -E 's/.*[ ]([0-9]+)[,].*/\1/g'`
    chip_thermal_2=`thermaltool -i wifi2 -get | grep "sensor" | sed -E 's/.*[ ]([0-9]+)[,].*/\1/g'`
    
    if [ "$chip_thermal_0" -lt "$base_0" ]; then 
        base_0=$chip_thermal_0;
    else
        flag_check_both=1
    fi
    
    if [ "$chip_thermal_1" -lt "$base_1" ]; then 
        base_1=$chip_thermal_1;
    else
        flag_check_both=1
    fi
    
    if [ "$chip_thermal_2" -lt "$base_2" ]; then 
        base_2=$chip_thermal_2;
    else
        flag_check_both=1
    fi
    
    echo "thermal_checking.sh: chip_thermal_0: $chip_thermal_0"
    echo "thermal_checking.sh: chip_thermal_1: $chip_thermal_1"
    echo "thermal_checking.sh: chip_thermal_2: $chip_thermal_2"
    
    on_chip_high=0
    if [ "1" = "1" ]; then 
        echo "thermal_checking.sh: checking both on-board and on-chip sensors.."
        if [ "$on_chip_high" = "0" ]; then
            echo "thermal_checking.sh: checking on-board sensors.."
            
            check_thermalctrl_wifi=`thermalctrl get temp 2>&1 | grep Error`
            if [ -z "$check_thermalctrl_wifi" ]; then
                on_board_wifi_temp=`thermalctrl get temp | awk '{print $(NF)}'`
                echo "thermal_checking.sh: on_board_wifi_temp $on_board_wifi_temp"  >> /tmp/OTP.log
            else
                echo "thermal_checking.sh: cannot get on board wifi temperature. Exiting.."
                exit
            fi
            
            if [ "$on_board_wifi_temp" -gt "$ON_BOARD_WIFI_HIGH_THRES" ]; then
                echo "thermal_checking.sh: on-board temperature > high threshold. OTP ON"
                echo "thermal_checking.sh: on-board temperature > high threshold. OTP ON"  >> /tmp/OTP.log
                thermaltool -i wifi0 -set -e 1 -hi0 40 -hi1 $hi1 -hi2 $hi2 -hi3 $hi3
                thermaltool -i wifi1 -set -e 1 -hi0 40 -hi1 $hi1 -hi2 $hi2 -hi3 $hi3
                thermaltool -i wifi2 -set -e 1 -hi0 40 -hi1 $hi1 -hi2 $hi2 -hi3 $hi3
                
                #thermaltool -i wifi0 -set -e 1 -lo1 $ON_CHIP_LOW_THRES_0 -lo2 $lo2 -lo3 $lo3
                #thermaltool -i wifi1 -set -e 1 -lo1 $ON_CHIP_LOW_THRES_1 -lo2 $lo2 -lo3 $lo3
                #thermaltool -i wifi2 -set -e 1 -lo1 $ON_CHIP_LOW_THRES_2 -lo2 $lo2 -lo3 $lo3
                
                thermaltool -i wifi0 -set -off0 $DUTY0
                thermaltool -i wifi1 -set -off0 $DUTY1
                thermaltool -i wifi2 -set -off0 $DUTY2
                
                # Mos 2018/12/13 start, [RA] add for onboard otp detect
                echo "on" > /tmp/OTP_result.log
            elif [ "$on_board_wifi_temp" -lt "$ON_BOARD_WIFI_LOW_THRES" ]; then
                echo "thermal_checking.sh: on-board temperature < low threshold. OTP OFF"
                echo "thermal_checking.sh: on-board temperature < low threshold. OTP OFF"  >> /tmp/OTP.log
                thermaltool -i wifi0 -set -e 1 -hi0 $ON_CHIP_HIGH_THRES_0 -hi1 $hi1 -hi2 $hi2 -hi3 $hi3
                thermaltool -i wifi1 -set -e 1 -hi0 $ON_CHIP_HIGH_THRES_1 -hi1 $hi1 -hi2 $hi2 -hi3 $hi3
                thermaltool -i wifi2 -set -e 1 -hi0 $ON_CHIP_HIGH_THRES_2 -hi1 $hi1 -hi2 $hi2 -hi3 $hi3

                #thermaltool -i wifi0 -set -e 1 -lo1 $ON_CHIP_LOW_THRES_0 -lo2 $lo2 -lo3 $lo3 
                #thermaltool -i wifi1 -set -e 1 -lo1 $ON_CHIP_LOW_THRES_1 -lo2 $lo2 -lo3 $lo3 
                #thermaltool -i wifi2 -set -e 1 -lo1 $ON_CHIP_LOW_THRES_2 -lo2 $lo2 -lo3 $lo3 

                thermaltool -i wifi0 -set -off0 0
                thermaltool -i wifi1 -set -off0 0
                thermaltool -i wifi2 -set -off0 0
                
                # Mos 2018/12/13 start, [RA] add for onboard otp detect
                echo "off" > /tmp/OTP_result.log
            fi
        else
            # Mos 2018/12/13 start, [RA] add for onboard otp detect
            echo "off" > /tmp/OTP_result.log
        fi
    fi
done

        