/************
 *
 * Filename:    cjsonapi.h
 * Purpose:     DIL APIs related to parsing and creation of json 
 * Copyright:   (c) Netgear Inc.
 *              2019 All rights reserved
 * Author:      @ VVDN TECHNOLOGIES
 *
 ************/
#ifndef cjsonapi_h__
#define cjsonapi_h__

//Supported Parsing Functions for parsing the given json file and fetch the values into defined structure
extern int cjson_parse_and_fetch(const char *buff, unsigned int cjson_id , void *struct_ptr);


//Supported function for creation of Json file based on provided structure containing values of elements
extern int cjson_create_json_file(cJSON *root, unsigned int cjson_id , void *struct_ptr, int total_count);


#define CJSON_BUFF_4 4
#define CJSON_BUFF_8 8
#define CJSON_BUFF_16 16
#define CJSON_BUFF_32 32
#define CJSON_BUFF_64 64
#define CJSON_BUFF_256 256
#define CJSON_MAC_LEN     18

#define CJSON_MAX_INDEX 256


#define ERROR_CJSON_INVALID_POINTER -2
#define ERROR_CJSON_PARSING_FAILED -3
#define ERROR_CJSON_NULL_STRUCT_POINTER -4
struct interface_info
{
	char name[CJSON_BUFF_8];
        char type[CJSON_BUFF_8];
}__attribute__((packed));

#define CJSON_MAX_INTERFACE 6

//JSON containing router information.
//(Code:CJSON_ROUTER_INFORMATION) 
struct router_information
{
	char router_name[CJSON_BUFF_32];
        char model_name[CJSON_BUFF_32];
        char feature_set_level[CJSON_BUFF_16];
        struct interface_info interface[CJSON_MAX_INTERFACE];
}__attribute__((packed));

//JSON object containing one or more key / value pairs describing properties of the device.
//(Code:CJSON_DEVICE_DISCOVERY)
struct device_discovery
{
        char device_did[CJSON_BUFF_64];
        char device_displayname[CJSON_BUFF_32];
        char device_type[CJSON_BUFF_32];
        char device_manufacturer[CJSON_BUFF_32];
	char device_mac[CJSON_MAC_LEN];
	char device_ip[CJSON_BUFF_16];
        char device_os[CJSON_BUFF_16];
        char device_os_version[CJSON_BUFF_16];
        int traffic_blocked;
}__attribute__((packed));

//JSON whenever the status of the Bitdefender subscription changes.
//(Code:CJSON_SUBSCRIPTION_CHANGED)
struct subscription_changed
{
	char status[CJSON_BUFF_8];
        long int expiry_time;
}__attribute__((packed));

//JSON document contains information describing the result (success or failure) of the operation.
//(Code:CJSON_REGISTERATION_STATUS)
struct registeration_status
{
	char error[CJSON_BUFF_32];
        int successful;
}__attribute__((packed));

//array of JSON objects which describes the changes that took place since the last event of this kind
//(Code:CJSON_DEVICELIST_UPDATED)
struct device_list_updated
{
	char device_id[CJSON_BUFF_64];
        char status[CJSON_BUFF_8];
}__attribute__((packed));

//representing the version info in JSON format
//(Code:CJSON_VERSIONINFO)
struct version_info
{
        char patch_level[CJSON_BUFF_32];
	char version[CJSON_BUFF_16];
}__attribute__((packed));

//JSON containing device information
//(Code:CJSON_DEVICEINFO)
struct device_info
{
	char device_id[CJSON_BUFF_64];
        char name[CJSON_BUFF_64];
        int traffic_blocked;
} __attribute__((packed));

//JSON containing device online status information. Formats recognized:
//1. Object of type DeviceOnlineStatus 
//2. Array of DeviceOnlineStatus objects, containing the entire set of online devices
//(Code:CJSON_DEVICE_ONLINE_STATUS)
struct device_online_status
{
	char device_mac[CJSON_MAC_LEN];
        char ip[CJSON_BUFF_16];        
        char interface[CJSON_BUFF_32];
        char hostname[CJSON_BUFF_64];
        int  is_online;
}__attribute__((packed));

struct device_mac_info
{
        char mac[CJSON_MAC_LEN];
        char ip[CJSON_BUFF_16];
}__attribute__((packed));

#define CJSON_MAX_MAC_INFO 10

//list of devices discovered in the network can be retrieved using the get_devices function
//(Code:CJSON_DEVICE_LIST_ONDEMAND)
struct device_list_ondemand
{
	char device_did[CJSON_BUFF_64];
        char device_manufacturer[CJSON_BUFF_32];
        struct device_mac_info   dev_mac[CJSON_MAX_MAC_INFO];
        char displayname[CJSON_BUFF_32];
        char devicetype[CJSON_BUFF_32];
        char deviceos[CJSON_BUFF_32];
        char deviceosversion[CJSON_BUFF_32];
        int  traffic_blocked;
}__attribute__((packed));

//a stringified JSON object containing the identifier data of the satellite to be removed
//(Code:CJSON_SATELLITEINFO)
struct satellite_remove_info
{
        char mac[CJSON_MAC_LEN];
};

enum cjson_structure_code{
        CJSON_ROUTER_INFORMATION=1,             // 1- struct router_information
        CJSON_DEVICE_DISCOVERY,    		// 2- struct device_discovery
        CJSON_SUBSCRIPTION_CHANGED,             // 3- struct subscription_changed
        CJSON_REGISTERATION_STATUS,             // 4- struct registeration_status
        CJSON_DEVICELIST_UPDATED,               // 5- struct device_list_updated
        CJSON_VERSIONINFO,              	// 6- struct version_info
        CJSON_DEVICEINFO, 			// 7- struct device_info
        CJSON_DEVICE_ONLINE_STATUS,         	// 8- struct device_online_status
        CJSON_DEVICE_LIST_ONDEMAND,        	// 9- struct device_list_ondemand
        CJSON_SATELLITEINFO,                 	// 10-struct satellite_remove_info
        CJSON_MULTI_DEVICELIST,        		// 11-struct device_list_updated (array)
        CJSON_MULTI_DEVICE_DISCOVERY,           // 12-struct device_discovery(array)
        CJSON_MULTI_DEVICE_ONLINE_STATUS,       // 13-struct device_online_status(array)
        CJSON_MULTI_DEVICE_LIST_ONDEMAND,       // 14-struct device_list_ondemand (array)
        CJSON_MULTI_ONLY_DID,               	// 15-struct device_list_ondemand (includes only did array)
        CJSON_LAST_TAG,
};


#endif
