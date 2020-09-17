/***************************************************************************
***
***    Copyright 2005  Hon Hai Precision Ind. Co. Ltd.
***    All Rights Reserved.
***    No portions of this material shall be reproduced in any form without the
***    written permission of Hon Hai Precision Ind. Co. Ltd.
***
***    All information contained in this document is Hon Hai Precision Ind.  
***    Co. Ltd. company private, proprietary, and trade secret property and 
***    are protected by international intellectual property laws and treaties.
***
****************************************************************************/

#ifndef AMBIT_UTIL_H
#define AMBIT_UTIL_H

#include <sys/socket.h>
#include <netinet/in.h>
#include "misc.h"
#ifndef MAX_XSS_CONVERT_LEN
#define MAX_XSS_CONVERT_LEN 5   /* strlen("&#40;") */
#endif

#ifndef EQ
#define EQ ==
#endif

#ifndef NEQ
#define NEQ !=
#endif

#ifndef MINVAL
#define MINVAL(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef BOOL
#define BOOL    int
#endif

#ifndef isxdigit
#define isxdigit(c)	(('0' <= (c) && (c) <= '9') \
			 || ('a' <= (c) && (c) <= 'f') \
			 || ('A' <= (c) && (c) <= 'F'))
#endif

#ifndef isdigit
#define isdigit(x) (((x) >= '0') && ((x) <= '9'))
#endif

#ifndef isspace
#define isspace(c)	((c) == ' ')
#endif

#ifndef islf
#define islf(c)	((c) == '\n')
#endif

#ifndef iscr
#define iscr(c)	((c) == '\r')
#endif


/* Debug print */
#ifdef DEBUG
#define dprintf(fmt, args...) printf("%s: " fmt, __FUNCTION__, ## args)
#else
#define dprintf(fmt, args...)
#endif

/* Return NUL instead of NULL if undefined */
#define safe_getenv(s) (getenv(s) ? : "")

#ifndef MEVENT_2012_05_16

/*
 * Concatenate two strings together into a caller supplied buffer
 * @param	s1	first string
 * @param	s2	second string
 * @param	buf	buffer large enough to hold both strings
 * @return	buf
 */
static inline char * strcat_r(const char *s1, const char *s2, char *buf)
{
	strcpy(buf, s1);
	strcat(buf, s2);
	return buf;
}	
#endif

extern char * inet_ntoa_b(struct in_addr ina, char *buf);
extern void convert2upper(char * strPtr, int strLen);
extern char *itoa(unsigned int i);
extern void strlwr(char *String);
extern int strnicmp (char *s1, char *s2, int n);
extern int stricmp (char *s1, char *s2);
extern char *base64enc (const char *p, char *buf, int len);
extern void base64decode (char *string);
extern int base64val (char c);
extern char *stristr (const char *a, const char *b);

#ifdef LINUX26
extern int getTokens(char *str, char *delimiter, char token[][128], int maxNumToken); /* wklin modified, 05/06/2009 */
#else
extern int getTokens(char *str, char *delimiter, char token[][], int maxNumToken);
#endif
/* Foxconn added start, Wins, 06/27/2010 */
extern int config_nvram_list(char *nvmItemList, char *nvmItem, char *delimiter, int dir, int maxNumItem);
/* Foxconn added end, Wins, 06/27/2010 */
extern int isValidIpAddr(char *ipAddr);
extern int isValidNetworkAddr(char *ipAddr);
extern int isValidNetmask(char *mask);
extern int isLanSubnet(char *ipAddr);
#ifdef OPENVPN_SUPPORT
extern int isOpenvpnLanSubnet(char *ipAddr); /* Foxconn added by Max Ding, 04/10/2014 OpenVPN: support tun and tap at the same time */
#endif
/* Foxconn add start, Max Ding, 10/31/2008 for @RU_two_wan */
#ifdef STATIC_PPPOE
extern int isSecWanSubnet(char *ipAddr);
#endif
/* Foxconn add end, Max Ding, 10/31/2008 */
extern int isWanSubnet(char *ipAddr);
extern int isNumber(char *str);
extern int isHexDigit(char c);
extern int isHexString(char *str);
extern int isValidMacAddr(char *macAddr);
extern void trimMacAddr(char *macAddr);
extern void formatMacAddr(char *macAddr);
extern void strToLower(char *str);
extern void strToUpper(char *str);
extern int cliStr2NumCheck(char *string, int lower, int upper);
extern void HexToAscii (char *hexKey, char *asciiKey);
extern void CharToHexString(char *pChar, char *pHexString);

extern int ether_atoe(const char *a, unsigned char *e);
extern char *ether_etoa(const unsigned char *e, char *a);

extern unsigned long calculate_checksum (int action, char *s, int size);
extern unsigned short hd_cksum(unsigned short *addr, int len);
extern int trimstr (char *str, unsigned int str_len);
extern unsigned char cat2HexNumTochar(char first, char second);
extern int is_blankLine (char *pStr);
#ifndef LOG_CATEGORY_ENUM
#define LOG_CATEGORY_ENUM

typedef enum log_category_enum
{
    LOG_CATEGORY_NONE       = (0),
    
    LOG_CATEGORY_ALLOW      = (1 << 0),
    LOG_CATEGORY_BLOCK      = (1 << 1),
    LOG_CATEGORY_LOGIN      = (1 << 2),
    LOG_CATEGORY_INFO       = (1 << 3),
    LOG_CATEGORY_FW         = (1 << 4),
    LOG_CATEGORY_PFPT       = (1 << 5),
    LOG_CATEGORY_WL         = (1 << 6),
/* Foxconn add start, Jenny Zhao, 12/17/2010  @Spec 2.0:add log*/
    LOG_CATEGORY_AUTOCONN   = (1 << 7),
    LOG_CATEGORY_WLSCHE     = (1 << 8),
/* Foxconn add end, Jenny Zhao, 01/25/2011 */
    LOG_CATEGORY_VPN        = (1 << 9),

    LOG_CATEGORY_ALL        = (0xFFFF)
} log_category;

typedef enum log_event_enum
{
    LOG_EVEN_NONE               = (LOG_CATEGORY_NONE << 16) | 0,
    
    LOG_EVENT_INIT              = (LOG_CATEGORY_INFO << 16) | 1,
    LOG_EVENT_TIME_SYNC         = (LOG_CATEGORY_INFO << 16) | 2,
    LOG_EVENT_INET_CONN         = (LOG_CATEGORY_INFO << 16) | 3,
    LOG_EVENT_INET_DISCONN      = (LOG_CATEGORY_INFO << 16) | 4,
    LOG_EVENT_IDLE_TIMEOUT      = (LOG_CATEGORY_INFO << 16) | 5,
    LOG_EVENT_WLAN_REJ          = (LOG_CATEGORY_WL << 16) | 6,
    LOG_EVENT_ASSIGN_IP         = (LOG_CATEGORY_INFO << 16) | 7,
    LOG_EVENT_UPNP_SET          = (LOG_CATEGORY_INFO << 16) | 8,
    LOG_EVENT_DDNS              = (LOG_CATEGORY_INFO << 16) | 9,
    LOG_EVENT_ADMIN_LOGIN       = (LOG_CATEGORY_LOGIN << 16) | 10,
    LOG_EVENT_REMOTE_LOGIN      = (LOG_CATEGORY_LOGIN << 16) | 11,
    LOG_EVENT_ADMIN_LOGIN_FAIL  = (LOG_CATEGORY_LOGIN << 16) | 12,
    LOG_EVENT_REMOTE_LOGIN_FAIL = (LOG_CATEGORY_LOGIN << 16) | 13,
    LOG_EVENT_SITE_ALLOW        = (LOG_CATEGORY_ALLOW << 16) | 14,
    LOG_EVENT_SITE_BLOCK        = (LOG_CATEGORY_BLOCK << 16) | 15,
    LOG_EVENT_SERV_BLOCK        = (LOG_CATEGORY_BLOCK << 16) | 16,
    LOG_EVENT_EMAIL_SEND        = (LOG_CATEGORY_INFO << 16) | 17,
    LOG_EVENT_EMAIL_FAIL        = (LOG_CATEGORY_INFO << 16) | 18,
    LOG_EVENT_WLAN_ACL_DENY     = (LOG_CATEGORY_WL << 16) | 19,
    LOG_EVENT_WLAN_ACL_ALLOW    = (LOG_CATEGORY_WL << 16) | 20,
    LOG_EVENT_ATTACK            = (LOG_CATEGORY_FW << 16) | 21,
    LOG_EVENT_REMOTE_ACCESS_LAN = (LOG_CATEGORY_PFPT << 16) | 22,
    LOG_EVENT_TM_WATER_MARK     = (LOG_CATEGORY_INFO << 16) | 23,
    LOG_EVENT_TM_REACH_LIMIT    = (LOG_CATEGORY_INFO << 16) | 24,

    LOG_EVENT_PKT_ACCEPT        = (LOG_CATEGORY_ALLOW << 16) | 25,
    LOG_EVENT_PKT_DROP          = (LOG_CATEGORY_BLOCK << 16) | 26,
    LOG_EVENT_CLEAR_LOG         = (LOG_CATEGORY_INFO << 16) | 27,
    
/* Foxconn add start, Jenny Zhao, 12/17/2010  @Spec 2.0:add log*/
    LOG_EVENT_INET_AUTO_CONN     = (LOG_CATEGORY_AUTOCONN << 16) | 28,
    LOG_EVENT_WL_BY_SCHE         = (LOG_CATEGORY_WLSCHE << 16) | 29,
/* Foxconn add end, Jenny Zhao, 01/25/2011 */

	/* wklin added start, 03/11/2011 */
    /* events from userspace */
    LOG_EVENT_USER_INFO          = (LOG_CATEGORY_INFO << 16) | 30,
    /* wklin added end, 03/11/2011 */

    LOG_EVENT_USB_ATTACH         = (LOG_CATEGORY_INFO << 16) | 31,
    LOG_EVENT_USB_DETACH         = (LOG_CATEGORY_INFO << 16) | 32,

/* Foxconn add start, Jenny Zhao, 06/09/2011  @Spec 2.0:add log 25/26*/
	LOG_EVENT_USB_REMOTE_LOGIN         = (LOG_CATEGORY_FW << 16) | 33,
    LOG_EVENT_USB_REMOTE_LOGIN_FAIL    = (LOG_CATEGORY_FW << 16) | 34,
/* Foxconn add end, Jenny Zhao, 06/09/2011 */
/*fxcn add by dennis start,11/16/2012,add WPS aplockdown log*/           
    LOG_EVENT_WPS_LOCKDOWN      = (LOG_CATEGORY_WL << 16) | 38,
/*fxcn add by dennis end,11/16/2012,add WPS aplockdown log*/
    #ifdef QTERICS_INTEGRATION
    LOG_EVENT_AUTO_UPDATE       = (LOG_CATEGORY_WL << 16) | 39,   
    #endif
    LOG_EVENT_VPN_SERVICE       = (LOG_CATEGORY_VPN << 16) | 40,
    LOG_EVENT_ALL               = (LOG_CATEGORY_ALL << 16) | 0xFFFF
} log_event;
#endif

/* Foxconn Perry added start, 2013/03/12, for SOAP API http handler */
#define UPNP_SA_RESPONSE "/tmp/tmp_upnp_sa_response.txt"
/* Foxconn Perry added end, 2013/03/12, for SOAP API http handler */
extern int ambitWriteLog(char *pcLog, int iLen, log_event event);
extern int ambitSetLogFilter(unsigned int log_filter);
/* foxconn modified end, zacker, 05/20/2010, @log_cat */

extern int is_russian_case(void); /* foxconn added, zacker, 07/08/2011 */

extern void replaceString(char *input_str, int size, char *ori_str, char *new_str);

extern int doKillPid(char_t *fmt, ...);
extern int doSystem(char_t *fmt, ...);
extern char *getNthValue(int index, char *values);
extern char *setNthValue(int index, char *old_values, char *new_value);
extern int getValueCount(char *values);
extern int checkSemicolon(char *str);
extern int getNthValueSafe(int index, char *value, char delimit, char *result, int len);
extern int deleteNthValueMulti(int index[], int count, char *value, char delimit);
extern char *racat(char *s, int i);
extern char *convertWifiIfaceToDevice(char *iFace, char *buf);
int SYSTEM(const char *format, ...);

#if defined(WIZ_LOG) 
extern int add_log_title(char *str_in_out, unsigned int str_out_len, char *step);
//extern int add_log_title(char *str_in_out, unsigned int str_out_len);
extern int wizardWriteLog(char *pcLog, int iLen);
extern int upload_wizard_log(void);
#endif

/* Foxconn added start pling 12/10/2014 */
/* NTGR IPv6 Auto Detection spec change.
 *  The following define is moved from ap/acos/rc/ipv6.c
 */
#if (defined ACOS_IPV6RD)
typedef struct {
  char interface[32];
  char enable;
  unsigned int ipv4_masklen;
  char ipv6_prefix[32];
  char br_addr[64];    // In TR-181 spec, it supports max 4 br addrs, we reserves the space for it.
} ipv6rd_t;
extern int get_6rd_auto_params(ipv6rd_t *ipv6rd);
extern int getinst_ipv6rd_rules_shared(ipv6rd_t* ipv6rd, int index);
#endif
/* Foxconn added end pling 12/10/2014 */

#if (defined MULTI_LAN)
extern int isMultiLanSubnet(char *ipAddr);
#endif
#ifdef SECURITY_ENHANCEMENT
extern int samba_tdb_backup(char * username, char * password);
extern int samba_tdb_restore(void);
#endif
#endif
