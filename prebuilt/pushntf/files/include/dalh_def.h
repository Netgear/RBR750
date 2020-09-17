/************
 *
 * Filename:    dalh_def.h
 * Purpose:     definition for dal structures
 * Copyright:   (c) Netgear Inc.
 * 				2019 All rights reserved
 * Author:      @ VVDN TECHNOLOGIES
 *
 ************/

/* Define to prevent recursive inclusion---------------------*/

#ifndef _DALH_DEF_H_
#define _DALH_DEF_H_
#include <d2lib/d2api.h>
#ifdef __cplusplus
extern "C" {
#endif

/*Define buffer size */

#define BUFF_LEN_8 8
#define BUFFER_SIZE_16 16
#define BUFFER_SIZE_32 32
#define BUFFER_SIZE_64 64
#define BUFFER_SIZE_128 128
#define BUFFER_SIZE_256 256

/* Defines for the return values */
#define FAIL -1
#define ERROR_FWUPGRADE_TIMER -17
#define ERROR_REFRESH_TIMER -18
#define ERROR_RWUPGRADE_TIMER  -19

/*
 * Supported Error Types
 */
enum check_d2_error {

  ERROR_OK_DALH,
  ERROR_TIMEOUT,
  ERROR_SHUTDOWN,
  ERROR_NOSUCH_ITEM,
  ERROR_TOO_LONG,
  ERROR_INVALID_INPUT_DIL,
  ERROR_WRONG_TYPE,
  ERROR_ENCODING,
  ERROR_D2,
  ERROR_UNKNOWN,
  ERROR_MAC_ADDRESS_MISSING,
  ERROR_NOTHING_TO_UPDATE,
};

enum processId {
  UPAGENT = 1,
  SOAP = 2,
  WEB_GUI = 3,
  DIL_POSTPROCESSOR = 4,
  DIL_PREPROCESSOR = 5,
};

enum featureId {
  NDN = 1,
  DTN_TAG = 2,
  DTN_UNTAG = 3,
  BST_ON_DEMAND = 4,
  BST_SCHEDULE = 5,
  AllowCdMAC = 6,
  BlockCdMAC = 7,
  CDIL= 8,
  DEVCUSTOM= 9,
  REMOVEMAC= 10,
};

struct Queue {
  char TagMAC[BUFFER_SIZE_32];
  char TagStatus[BUFFER_SIZE_16];
  char cdil_command[BUFFER_SIZE_16];
  uint32_t fId; //stores the feature Id
  uint32_t ndnIdx; //stores the NDN index
  char bstStatus[10];
  char AllowMAC[BUFFER_SIZE_32];
  char BlockMAC[BUFFER_SIZE_32]; 
  char GenericMAC[BUFFER_SIZE_32];
  char customname[BUFFER_SIZE_64+1];
  char customtype[BUFFER_SIZE_32+1];
  char custombrand[BUFFER_SIZE_32+1];
  char custommodel[BUFFER_SIZE_32+1];
}__attribute__((packed));

/* DALHConfig data attributes */
struct getDalhCfgInfo{
	char TaggedMAC[BUFFER_SIZE_32];
	char TaggedStatus[BUFFER_SIZE_16];
} __attribute__((packed));

/* DALHConfig data attributes */
struct getAccessControlInfo{
	char AllowedMAC[BUFFER_SIZE_32];
	char BlockedMAC[BUFFER_SIZE_32];
} __attribute__((packed));

/* DALHConfig custom device data information */
struct getcustumdeviceinfo{
	char custom_mac[BUFFER_SIZE_32];
        char custom_name[BUFFER_SIZE_64+1];
        char custom_type[BUFFER_SIZE_32+1];
        char custom_brand[BUFFER_SIZE_32+1];
        char custom_model[BUFFER_SIZE_32+1];
} __attribute__((packed));

/* DAL */
char remove_mac[BUFFER_SIZE_32];
char DtnTagMac[BUFFER_SIZE_32];
char DtnTagStatus[BUFFER_SIZE_16];
unsigned int TagStatusSize;
uint32_t ndnIndex;
uint32_t ndnIndexAttr;
uint32_t featureId;
uint32_t processId;
char bstStatusdata[10];
char cdil_command_info[BUFFER_SIZE_16];
char AllowCdMACdata[BUFFER_SIZE_32];
char BlockCdMACdata[BUFFER_SIZE_32];
char AllowBlockStatus[BUFFER_SIZE_16];
int get_cdil_command();
int signal_user2_handler();
void getNdnAttributes(void);
void notifyUpagentForNdn(void);
void notifyDilForDtn(void);
void getDtnAttributes(void);
void notifyDaemonForBST(void);
void getBSTAttributes(void);
void getCDILAttributes(void);
void execute_cdil_request(void);
void getAllowAttributes(void);
void getBlockAttributes(void);
void notifyDilForAllow(void);
void notifyDilForBlock(void);
/* //will be used further
enum check_d2_error get_DilDeviceMAC(char *MacAddress, unsigned int idx);
*/

#ifdef __cplusplus
}
#endif
#endif //_DALH_DEF_H_
