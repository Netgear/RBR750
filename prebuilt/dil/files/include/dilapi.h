/************
 *
 * Filename:    dilapi.h
 * Purpose:     DIL APIs related to pre-processing,post-processing and GC 
 * Copyright:   (c) Netgear Inc.
 *              2018 All rights reserved
 * Author:      @ VVDN TECHNOLOGIES
 *
 ************/
#ifndef dilapi_h__
#define dilapi_h__

#define RAW_MAC "ff:ff:ff:ff:ff:ff"
#define MAC_ADDR_LEN 17
#define IP_ADDR_LEN 16 //15 bytes + 1 extra
#define MAX_MAC_ADRR_COUNT 256
#define DIL_ENTRY_THRESHOLD 180
#define LOWERCASE_ASCII_MIN 97
#define LOWERCASE_ASCII_MAX 122
#define UPPERCASE_ASCII_MIN 65
#define UPPERCASE_ASCII_MAX 90
#define ASCII_VALUE_RANGE 32
#define NAME_STRING 16
#define ENUM_NAME_STRING_LEN 8
#define EPOCH_TIME_LENGTH 50

#define MAX_DIL_INDEX 256
#define GC_MAX_REMOVAL_COUNT 10 //This will be used for maintaining the group of offline device status

#define ETH_ADDR_LEN    6
#define MAC_ADDRESS_SEGMENT     3
#define FIRST_BYTE     0
#define SECOND_BYTE    1
#define MAC_ADDRESS_PART     	2
#define FALSE           0
#define TRUE            1
#define MAC_STR_LEN     18
#define SEPERATOR       ':'
#define DELIM           "." 
#define ARRAY_SIZE 64
#define BUFF_LEN_4 4
#define BUFF_LEN_8 8
#define BUFF_LEN_16 16
#define BUFF_LEN_32 32
#define BUFF_LEN_64 64
#define BUFF_LEN_256 256

struct postprocessor_args_devicename_devicetype
{
	char devicetype[BUFF_LEN_32+1]; //Max length is 32 bytes as per D2 table + 1 byte for terminating byte for string
	char devicename[BUFF_LEN_64+1];
	char devicemodel[BUFF_LEN_32+1];
	char devicebrand[BUFF_LEN_32+1];
  	char MACData[MAC_ADDR_LEN+1]; 
  	int producer_id;
}__attribute__((packed));

struct device_signature
{
        char device_hua[BUFF_LEN_256+1];//Max length is 256 bytes as per D2 table + 1 byte for terminating byte for string 
        char device_dhcphostname[BUFF_LEN_64+1]; 
        char device_dhcpvendor[BUFF_LEN_64+1]; 
        char device_dhcpoptions[BUFF_LEN_64+1]; 
        char device_connected_interface[BUFF_LEN_32+1];
        char device_category[BUFF_LEN_32+1];
        char device_mac[MAC_ADDR_LEN+1]; 
        char device_ip[IP_ADDR_LEN];
        char device_state[ENUM_NAME_STRING_LEN];
}__attribute__((packed));

struct device_detail
{
        char device_hostname[BUFF_LEN_64+1]; //Max length is 64 bytes as per D2 table + 1 byte for terminating byte for string
        char device_macvendor[BUFF_LEN_64+1]; 
        char device_uuid[BUFF_LEN_64+1];
        char device_model[BUFF_LEN_32+1];
        char device_os[BUFF_LEN_32+1];
        char device_brand[BUFF_LEN_32+1]; 
        char device_mac[MAC_ADDR_LEN+1]; 
        char device_type[BUFF_LEN_32+1]; 
}__attribute__((packed));

struct connected_devinfo
{
        char device_ssid[BUFF_LEN_32];
        char device_mac[MAC_ADDR_LEN+1]; 
        char device_conn_apmac[MAC_ADDR_LEN+1];
        char device_linkrate[BUFF_LEN_8+1]; //Max length is 8 bytes as per D2 table + 1 byte for terminating byte for string
        char device_download_bw[BUFF_LEN_8+1];
        char device_uploadbw[BUFF_LEN_8+1];
        char device_signalstrength[BUFF_LEN_8+1];
}__attribute__((packed));

struct device_index_info {
        char device_hua[BUFF_LEN_256];
        char device_dhcpoptions[BUFF_LEN_64];
        char device_dhcphostname[BUFF_LEN_64];
        char device_dhcpvendor[BUFF_LEN_64];
        char device_hostname[BUFF_LEN_64];
        char DeviceMacVendor[BUFF_LEN_64];
        char DisplayNameCustom[BUFF_LEN_64];
        char DisplayNameDPI[BUFF_LEN_64];
        char DisplayNameNetgear[BUFF_LEN_64];
        char device_uuid[BUFF_LEN_64];
        char device_brand[BUFF_LEN_32];
        char device_category[BUFF_LEN_32];
        char device_model[BUFF_LEN_32];
        char device_os[BUFF_LEN_32];
        char DeviceOSVersion[BUFF_LEN_32];
        char DeviceTypeCustom[BUFF_LEN_32+1];
        char DeviceTypeDPI[BUFF_LEN_32];
        char DeviceTypeNetgear[BUFF_LEN_32];
        char DisplayModelCustom[BUFF_LEN_32+1];
        char DisplayBrandCustom[BUFF_LEN_32+1];
        char FingResponsestatus[BUFF_LEN_32];
        char device_connected_interface[BUFF_LEN_32];
        char IsTagged[BUFF_LEN_32];
        char DeviceSSID[BUFF_LEN_32];
        char device_mac[MAC_ADDR_LEN+1];
        char device_conn_apmac[MAC_ADDR_LEN+1];
        char Timestamp[BUFF_LEN_16];
        char device_ip[IP_ADDR_LEN];
        char device_state[ENUM_NAME_STRING_LEN];
        char allow_block[ENUM_NAME_STRING_LEN];
        char devicelinkrate[ENUM_NAME_STRING_LEN+1];
        char uploadbw[ENUM_NAME_STRING_LEN+1];
        char downloadbw[ENUM_NAME_STRING_LEN+1];
        char signalstrength[BUFF_LEN_8+1];
        int Producerid;
}__attribute__((packed));

struct gc_index_epochtime_map
{
	unsigned int epochtime;
	unsigned int index ;
};
struct signature_info
{
        char MACData[MAC_ADDR_LEN+1];
        unsigned int field_code;
        char generic_data[BUFF_LEN_256]; 
        int field_length;
};
extern int update_device_signature (struct device_signature, int producer_id) ;
extern int update_device_detail (struct device_detail, int producer_id) ;
extern int update_connected_device_info (struct connected_devinfo, int producer_id) ;
extern int update_device_displayname (char* mac_address, char* display_name, int name_size, int producer_id ) ;
extern int update_device_displaytype (char* mac_address ,char* new_display_type,int device_type_size, int producer_id) ;
extern int update_device_displaymodel (char* mac_address, char* device_model, int name_size, int producer_id );
extern int update_device_displaybrand (char* mac_address, char* device_brand, int name_size, int producer_id );
extern char* get_netgear_device_type( int id_num );
extern int get_netgear_device_id(char* netgear_device_type, int type_size);
extern int dil_init(void);
extern int dil_uninit(void);
extern int check_macaddress_entry(char* device_mac);
extern char* check_macaddress_by_index(int index);
extern int get_total_dil_entries_used(void);
extern int dil_table_enquiry(unsigned int occupied_index[], int mem_length  );
extern int get_device_info_at_index(struct device_index_info *dev_info, unsigned int index);
extern int get_device_info_by_mac(struct device_index_info *dev_info, char* mac_address);
extern int get_dil_upgrade_status(bool *status );
extern int set_dil_upgrade_status(bool value );
extern int dil_update_table_field(char* mac_address ,unsigned int field_code,void *new_value, unsigned int field_length);
extern int dil_device_state_info(unsigned int occupied_index[], int mem_length ,unsigned int state_code);
extern int dil_remove_macaddress_entry(char* device_mac);
extern int dil_weekly_refresh_request(void);
extern int dil_set_ACL(bool value );
extern int dil_get_ACL();
extern int escapeJSON(char* input,char* output,unsigned int length);
extern int IsJsonEncodeNeeded(char* input ,unsigned int length);

enum state_code
{
	OCCUPPIED,	//when all occupied entries are needed
  	ONLINE,       	//when all online entries are needed
  	OFFLINE,      	//when all offline entries are needed
  	UNOCCUPIED,   	//when all non occupied entries are needed
};

enum dil_table_field_code{
//	FIELD_MACADDESS=0, 		// 0- MAC address
	FIELD_DEVICE_HUA=1,		// 1- Device Hua
	FIELD_DEVICE_DHCPOPTIONS,       // 2- Device DHCPOptions
	FIELD_DHCPHOSTNAME,		// 3- Device DHCP Hostname
	FIELD_DHCPVENDOR,		// 4- Device DHCP Vendor
	FIELD_IPADDRESS,                // 5- Device IP Address
	FIELD_DEVICESTATE,		// 6- Device State
	FIELD_DEVICECONNECTEDINTERFACE, // 7- Device Connected Interface
	FIELD_DEVICETYPECUSTOM,		// 8- Device Type Custom
	FIELD_DEVICEDISPLAYNAME,        // 9- Device Display Name
        FIELD_ISTAGGED,                 // 10-Is Tagged field
	FIELD_DEVICEMODELCUSTOM,        // 11-Device Model Custom
	FIELD_ALLOWORBLOCK,             // 12-AllowOrBlock
	FIELD_SSID,                     // 13-Device SSID
	FIELD_LINKRATE,                 // 14-Device LinkRate
	FIELD_DOWNLOADBW,               // 15-Device Download Bandwidth
	FIELD_UPLOADBW,                 // 16-Device Upload Bandwidth
	FIELD_SIGNALSTRENGTH,           // 17-Device Signal Strength
	FIELD_CONN_AP_MAC,              // 18-Connected AP Mac Address
	FIELD_DEVICEBRANDCUSTOM,        // 19-Device Brand Custom
	FIELD_DEVICECATEGORY,           // 20-Device Category
	FIELD_DEVICEUUID,                 // 21-Device UUID
    FIELD_LAST_TAG,
};

#define MAX_FIELD_CODE FIELD_LAST_TAG - FIELD_DEVICE_HUA

enum error_code
{
	ERROR_OK = 0,
	ERROR_NOK = 1,
	ERROR_INIT = -1,
	ERROR_INVALID_INPUT = -2,
        ILLEGAL_PRODUCER_ID = -3, 
        ERROR_DIL_TABLE_FILLED = -4,
	GC_NO_OFFLINE_DEVICE_ERROR = -5,
        GC_NO_CLEANUP_NEEDED_ERROR = -6, 
	OUT_OF_RANGE_INDEX_ERROR = -7 ,  
	ILLEGAL_MAC_ADDRESS = -8 ,
        ERROR_ILLEGAL_POINTER_ENQUIRY = -9 ,
        ERROR_WRONG_LENGTH_ENQUIRY = -10, 
        ERR_NULL_POINTER= -11,
        ERROR_UNOCCUPIED_INDEX_ENTRY=-12, 
	ILLEGAL_IP_ADDRESS=-13,
        ERR_INCORRECT_FIELD_CODE=-14,
        ERR_ILLEGAL_LENGTH=-15,
        ERROR_INTERNAL_DIL_LIB=-16,
        ERROR_ONLINE_DEVICE_DELETION=-17,
        ERROR_OFFLINE_INTERNET_STATUS=-18,
        ERROR_ALREADY_WEEKLY_REFRESH_GOING=19,
        ERROR_INTERNETSTATUS_DOWN=20,
	ERROR_NOK_ACL = -21,
};
enum producerid {
	dummy_producer1=0,
	dummy_producer2=10,
	circle=20,
	dummy_producer3=30,
	orbi=40,
	odm=50,
	dummy_producer4=60,
	bd=70,
	fing=80,
	dummy_producer5=90,
	dummy_producer6=100
} producer;
#define MAX_BITMASK_ELEMENT 8
#define SetBit(A,k)     ( A[(k/32)] |= (1 << (k%32)) )
#define ClearBit(A,k)   ( A[(k/32)] &= ~(1 << (k%32)) ) 
#define TestBit(A,k)    ( A[(k/32)] & (1 << (k%32)) ) 
#endif  // dilapi_h_
