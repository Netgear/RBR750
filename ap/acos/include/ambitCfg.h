/***************************************************************************
#***
#***    Copyright 2018  Hon Hai Precision Ind. Co. Ltd.
#***    All Rights Reserved.
#***    No portions of this material shall be reproduced in any form without the
#***    written permission of Hon Hai Precision Ind. Co. Ltd.
#***
#***    All information contained in this document is Hon Hai Precision Ind.  
#***    Co. Ltd. company private, proprietary, and trade secret property and 
#***    are protected by international intellectual property laws and treaties.
#***
#****************************************************************************
***
***    Filename: ambitCfg.h
***
***    Description:
***         This file is specific to each project. Every project should have a
***    different copy of this file.
***        Included from ambit.h which is shared by every project.
***
***    History:
***
***	   Modify Reason		                                        Author		         Date		                Search Flag(Option)
***	--------------------------------------------------------------------------------------
***    File Creation                                            
*******************************************************************************/


#ifndef _AMBITCFG_H
#define _AMBITCFG_H

#include "../include/projectConfig.h"

#define WW_VERSION           1 /* WW SKUs */
#define NA_VERSION           2 /* NA SKUs */
#define JP_VERSION           3
#define GR_VERSION           4
#define PR_VERSION           5
#define KO_VERSION           6
#define RU_VERSION           7
#define SS_VERSION           8
#define PT_VERSION           9
#define TWC_VERSION          10
#define BRIC_VERSION         11
#define SK_VERSION           12

#define WLAN_REGION          WW_VERSION
#define FW_REGION            WW_VERSION   /* true f/w region */

/*formal version control*/
#define AMBIT_HARDWARE_VERSION     "U12H404T00"
#define AMBIT_SOFTWARE_VERSION     getSWVersion()
#define AMBIT_UI_VERSION           getODMVersion()
#define STRING_TBL_VERSION         "3.0.0.24_2.1.30.3"

#define AMBIT_PRODUCT_NAME         "RBR750"
#define AMBIT_PRODUCT_ALIAS_TRI     "Nighthawk X8"
#define AMBIT_PRODUCT_DESIGN        "GECKO"
#define AMBIT_PRODUCT_MODEL         AMBIT_PRODUCT_NAME
#define AMBIT_PRODUCT_DESCRIPTION   "Orbi 11AX Router RBR750"
#define AMBIT_PRODUCT_BRAND         "NETGEAR"
#define AMBIT_MANUFACTURER          "NETGEAR, Inc."
#define AMBIT_MANUFACTURER_URL      "http://www.netgear.com"
#define AMBIT_MODEL_URL             "http://www.mywifiext.net"
#define DOCUMENT_URL                "http://downloadcenter.netgear.com"
#define UPnP_MODEL_URL              "RBR750.aspx"
#define UPnP_MODEL_DESCRIPTION      "802.11ax"
#if defined(DUAL_TRI_BAND_HW_SUPPORT)
#define AMBIT_PRODUCT_NAME_TRI_BAND         "RBR750"
#define AMBIT_PRODUCT_DESCRIPTION_TRI_BAND  "Orbi 11AX Router RBR750"
#define UPnP_MODEL_URL_TRI_BAND             "http://www.netgear.com/Products/RoutersandGateways/RangeMaxNEXTWirelessRoutersandGateways/RBR750.aspx"
#endif

#define AMBIT_NVRAM_VERSION  "1" /* digital only */
#define AMBIT_NVRAM_VERSION2  "2" /* digital only */

#define SMART_WIZARD_SPEC_VERSION "0.7"  /* This is specification version of smartwizard 2.0 */
#define AMBIT_SOAP_VERSION          "3.14"

#define DF_2G_ROOTAP_SSID           "Orbi_New"
#define DF_5G_ROOTAP_SSID           "Orbi_New"
#define AP_MODE_DEFAULT_2G_SSID     "Orbi_New"

#define KWILT_VERSION "20150818"
#define KWILT_TARBALL_SIZE  6580335

//By Orbi 2.1 spec, device default name pp.8
#define ORBI_DEVICE_NAME "Orbi 11AX Router"
#define ORBI_SATELLITE_DEVICE_NAME "Orbi Satellite-"  //it sule be Orbi Satellite-1, Orbi Satellite-2 ....

//#define ORBI_DEVICE_NAME "Orbi Router"   //For Router
//#define ORBI_DEVICE_NAME "Orbi Satellite"   //For Satellite

#define SOAP_URN_VENDER_NAME "urn:NETGEAR-ROUTER"
#define SOAP_USER_AGENT_NAME "Foxconn SOAPC Tool 1.0"

#define SOAP_ORBI_AUTHENTICATE_KEYWORD "NETGEAR_Orbi_"
/****************************************************************************
 * Board-specific defintions
 *
 ****************************************************************************/

/* Interface definitions */
#if defined(ORBI_AX_HW_HE_V2)
#define WAN_IF_NAME_NUM             "eth4"
#else
#define WAN_IF_NAME_NUM             "eth0"
#endif

#define WLAN_IF_NAME_NUM            "ath0"
#define WLAN_N_IF_NAME_NUM          "ath1"
#define WLAN_5G_2_IF_NAME_NUM       "ath2"
#define WDS_IF_NAME_NUM             "wds0.1"    /* WDS interface */
#define WLAN_CLI_IF_NAME_NUM        "apcli0"
#define WLAN_5G_CLI_IF_NAME_NUM     "apclii0"

#define LAN_BR_IF_NAME_NUM          "br-lan"
#define LAN_BR1_IF_NAME_NUM         "br-lan1"
#define GUEST_BR_IF_NAME_NUM        "br-guest"
/* Foxconn add start by aspen Bai, 11/13/2008 */
#ifdef MULTIPLE_SSID
#define MIN_BSSID_NUM       2
#define MAX_BSSID_NUM       4

#define WLAN_BSS1_NAME_NUM          "ath01"     /* Multiple BSSID #2 */
#define WLAN_BSS2_NAME_NUM          "ath02"     /* Multiple BSSID #3 */
#define WLAN_BSS3_NAME_NUM          "ath03"     /* Multiple BSSID #4 */

/* Foxconn add start, Tony W.Y. Wang, 03/22/2010 @For 5G*/
#define WLAN_5G_BSS1_NAME_NUM       "ath11"     /* Multiple BSSID #2 */
#define WLAN_5G_BSS2_NAME_NUM       "ath12"     /* Multiple BSSID #3 */
#define WLAN_5G_BSS3_NAME_NUM       "ath13"     /* Multiple BSSID #4 */
/* Foxconn add end, Tony W.Y. Wang, 03/22/2010 @For 5G*/

/* Foxconn add start, Tony W.Y. Wang, 03/22/2010 @For 5G-2*/
#define WLAN_5G_2_BSS1_NAME_NUM       "ath21"     /* Multiple BSSID #2 */
#define WLAN_5G_2_BSS2_NAME_NUM       "ath22"     /* Multiple BSSID #3 */
#define WLAN_5G_2_BSS3_NAME_NUM       "ath23"     /* Multiple BSSID #4 */
/* Foxconn add end, Tony W.Y. Wang, 03/22/2010 @For 5G-2*/

#endif /* MULTIPLE_SSID */
/* Foxconn add end by aspen Bai, 11/13/2008 */

#define WLAN_2G_ANTENA              "2x2"
#define WLAN_5G_ANTENA              "2x2"

/* Smart Mesh definitions */
#define NTGR_VENDOR_IE_FLAG_PRODUCT_TYPE    (0x000200)
#define NTGR_VENDOR_IE_FLAG_PRODUCT_ID      (0x0F0000)

/* Rate definitions */
#define RATE_STR_2G_BG              "54M"
#define RATE_STR_2G_HT20            "573.5M"
#define RATE_STR_2G_HT40            "1147M"

#define RATE_STR_5G_BG              "54M"
#define RATE_STR_5G_HT20            "573.5M"
#define RATE_STR_5G_HT40            "1147M"
#define RATE_STR_5G_HT80            "2402M"


#define OPMODE_STR_2G_BG            "wlan_mode_54"
#define OPMODE_STR_2G_HT20          "wlan_mode_573_5"
#define OPMODE_STR_2G_HT40          "wlan_mode_1147"

#define OPMODE_STR_5G_BG            "wlan_mode_54"
#define OPMODE_STR_5G_HT20          "wlan_mode_573_5"
#define OPMODE_STR_5G_HT40          "wlan_mode_1147"
#define OPMODE_STR_5G_HT80          "wlan_mode_2402"

#define WLMODE_AC_HT20              "wlan_mode_573_5"
#define WLMODE_AC_HT40              "wlan_mode_1147"
#define WLMODE_AC_HT80              "wlan_mode_2402"
#define AC_RATE_STR_HT20            "573.5M"
#define AC_RATE_STR_HT40            "1147M"
#define AC_RATE_STR_HT80            "2402M"



/* GPIO definitions */
/* Foxconn modified start, Wins, 04/11/2011 */

#define GPIO_POWER_LED_GREEN        2
#define GPIO_POWER_LED_GREEN_STR    "2"
#define GPIO_POWER_LED_AMBER        3
#define GPIO_POWER_LED_AMBER_STR    "3"

/*
#define GPIO_LOGO_LED_1             1
#define GPIO_LOGO_LED_1_STR         "1"
#define GPIO_LOGO_LED_2             9
#define GPIO_LOGO_LED_2_STR         "9"
*/

#define GPIO_WAN_LED                8
#define GPIO_WAN_LED_2              9


#define GPIO_WIFI_2G_LED            13
#define GPIO_WIFI_5G_LED            12
//#define GPIO_WIFI_SUMMARY_LED       15
#define GPIO_SUPPORT_INIT_LED       15
#define WPS_LED_GPIO                14
#define GPIO_PLC_LED		         1

#if (defined INCLUDE_USB_LED)
#define GPIO_LED_USB        17
#define GPIO_USB_LED        17   /* USB1 LED. */
#define GPIO_USB1_LED       17   /* USB2.0 LED. */
#define GPIO_USB2_LED       18   /* USB3.0 LED. */
#endif /*INCLUDE_USB_LED*/

#define GPIO_USB1_ON_OFF    0

#define enable_usb_ports() { \
     system("gpio 0 1"); \
    }

#define disable_usb_ports() { \
     system("gpio 0 0"); \
    }

/*Buttons definitions, original in ~/swresetd/swresetd_config.h */
#define GPIO_RESET_BUTTON               20 /* Input */
#define GPIO_WPS_BUTTON                 4

#ifdef INCLUDE_WIFI_BUTTON
#define GPIO_WIFI_BUTTON                5
#endif  /* INCLUDE_WIFI_BUTTON */

#ifdef INCLUDE_SUPPORT_INIT_BUTTON
#define GPIO_SUPPORT_INIT_BUTTON        5
#endif  /* INCLUDE_SUPPORT_INIT_BUTTON */

#ifdef LED_GPIO_SWITCH
#define GPIO_LED_SWITCH                 16
#define GPIO_LED_BUTTON                 GPIO_LED_SWITCH
#endif  /* LED_GPIO_SWITCH */


#define ML_MTD_RD                   "/dev/mtdblock"
#define ML_MTD_WR                   "/dev/mtd"
/* MTD definitions */

#define FLASH_MTD_ML_SIZE           0x20000   /* 128k */














































#ifdef NAND_NEW_PARTITION
#define DATA2_MTD                   "mtd35"
#define DATA2_MTD _RD               "/dev/mtdblock35"
#define DATA2_MTD _WR               "/dev/mtd35"














#endif /*#if defined(FX_NAND_BOOT)*/

#define FS_UNZIP_LANG_TBL    "/www/string_table"
#define FS_ZIP_LANG_TBL      "/tmp/string_table_rbw50.bz2"

/* wklin added start, 11/22/2006 */
/* The following definition is used in acosNvramConfig.c and acosNvramConfig.h
 * to distingiush between Foxconn's and Broadcom's implementation.
 */
#define BRCM_NVRAM          /* use broadcom nvram instead of ours */

/* The following definition is to used as the key when doing des
 * encryption/decryption of backup file.
 * Have to be 7 octects.
 */
#define BACKUP_FILE_KEY         "NtgrBak"
/* wklin added end, 11/22/2006 */

/* Foxconn Perry added start, 2011/04/13, for document URL */
//#define DOCUMENT_URL		"http://documentation.netgear.com/wndr4500/enu/202-10581-01/index.htm"
/* Foxconn Perry added end, 2011/04/13, for document URL */

/* Foxconn Perry added start, 2011/08/17, for USB Support level */
#define USB_support_level        "29"       /* pling modified 5->13, add bit 4 for Readyshare Vault *//*kathy modified 13->29, add bit 16 for ReadyCLOUD */
/* Foxconn Perry added end, 2011/08/17, for USB Support level */

#if defined(DUAL_TRI_BAND_HW_SUPPORT)
#define DUAL_BAND_HW_VER                 "RBR750"
/* Foxconn add start, Snoopy.wu, 04/07/2015 */
#define DUAL_BAND_NTGR_SPECIFIC_HW_ID    ""
#define DUAL_BAND_NTGR_GENERIC_HW_ID     ""
/* Foxconn add end, Snoopy.wu, 04/07/2015 */
#define TRI_BAND_HW_VER                  "RBR750"
/* Foxconn add start, Snoopy.wu, 04/07/2015 */
#define TRI_BAND_NTGR_SPECIFIC_HW_ID     "VEN_01f2&amp;DEV_003e&amp;REV_01"
#define TRI_BAND_NTGR_GENERIC_HW_ID      "VEN_01f2&amp;DEV_8000&amp;SUBSYS_01&amp;REV_01 VEN_01f2&amp;DEV_8000&amp;REV_01"
/* Foxconn add end, Snoopy.wu, 04/07/2015 */
/* Foxconn add start, Snoopy.wu, 05/15/2015 */
#define Reboot_Waiting_Time "3300"
/* Foxconn add end, Snoopy.wu, 05/15/2015 */

#endif

#define ORBI_MINI_ROUTER_HW_VER         "RBR750"

#define WIRELESS_SETTINGS_PAGE                  "WLG_wireless_dual_band_r10.htm"
#define WIRELESS_GUEST_SETTINGS_PAGE            "WLG_wireless_dual_band_2.htm"
#define WIRELESS_ADV_SETTINGS_PAGE              "WLG_adv_dual_band2.htm"

#define WIRELESS_TRI_BAND_SETTINGS_PAGE         "WLG_wireless_tri_band.htm"
#define WIRELESS_TRI_BAND_GUEST_SETTINGS_PAGE   "WLG_wireless_tri_band_2.htm"
#define WIRELESS_TRI_BAND_ADV_SETTINGS_PAGE     "WLG_adv_tri_band2.htm"

#define STATUS_DUAL_BAND_PAGE                   "ADVANCED_home2.htm"
#define STATUS_TRI_BAND_PAGE                    "ADVANCED_home2_tri_band.htm"


#define CE_DFS_ENABLE       "0"
#define FCC_DFS_ENABLE      "0"
#define TELEC_DFS_ENABLE    "0"

#define FLASH_SIZE      (96*1024*1024)
#define MAX_SIZE_FILE   96*1024*1024
#define MAX_SIZE_KERNEL (MAX_SIZE_FILE)
#define MAX_SIZE_ROOTFS (MAX_SIZE_FILE)
#define MAX_SIZE_FILE_OPENSOURCE    MAX_SIZE_FILE
#define MAX_SIZE_KERNEL_OPENSOURCE  MAX_SIZE_FILE
#define MAX_SIZE_ROOTFS_OPENSOURCE  MAX_SIZE_FILE	
#define BOARD_ID_OPENSOURCE         "U12H340T99_OPENSOURCE"
#define OPENSOURCE_START_MTD_IDX    (1)
#define OPENSOURCE_END_MTD_IDX      (1)	
#define IMAGE_BLOCK_SIZE        0x20000
#define MAX_IMAGE_SIZE  MAX_SIZE_FILE

#define RALINK_NVRAM_SIZE           (0x10000)
#define RALINK_NVRAM_MTD_RD         "/dev/mtdblock2"
#define RALINK_NVRAM_MTD_WR         "/dev/mtd2"
#define RALINK_BD_SIZE              (0x10000)
#define RALINK_BD_MTD_RD            "/dev/mtdblock3"
#define RALINK_BD_MTD_WR            "/dev/mtd3"
#define SIZE_BOOTCODE               (0x30000)
#define SIZE_ML                     ((ST_SUPPORT_NUM)*(FLASH_MTD_ML_SIZE))	
#define SIZE_TM                     (0)	
#define SIZE_POT                    (0x10000)	
#define SIZE_BD                     (0x10000)
#define SIZE_NARAM                  (0x80000)
#define SIZE_KERNEL                 (FLASH_SIZE - SIZE_BOOTCODE - SIZE_NARAM \
                                     - SIZE_BD - SIZE_POT - SIZE_TM - SIZE_ML \
                                     - RALINK_NVRAM_SIZE - RALINK_BD_SIZE)

//#define MAX_SIZE_FILE               (SIZE_KERNEL - 8)
//#define MAX_SIZE_KERNEL             (MAX_SIZE_FILE)
//#define MAX_SIZE_ROOTFS             (MAX_SIZE_FILE)
/*EX3700 end*/

#endif /*_AMBITCFG_H*/
