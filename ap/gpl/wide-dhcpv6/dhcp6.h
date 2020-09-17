/*	$KAME: dhcp6.h,v 1.56 2005/03/20 06:46:09 jinmei Exp $	*/
/*
 * Copyright (C) 1998 and 1999 WIDE Project.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the project nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE PROJECT AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef __DHCP6_H_DEFINED
#define __DHCP6_H_DEFINED

#ifdef __sun__
#define	__P(x)	x
typedef uint8_t u_int8_t;
#ifndef	U_INT16_T_DEFINED
#define	U_INT16_T_DEFINED
typedef uint16_t u_int16_t;
#endif
#ifndef	U_INT32_T_DEFINED
#define	U_INT32_T_DEFINED
typedef uint32_t u_int32_t;
#endif
typedef uint64_t u_int64_t;
#ifndef CMSG_SPACE
#define	CMSG_SPACE(l) \
	((unsigned int)_CMSG_HDR_ALIGN(sizeof (struct cmsghdr) + (l)))
#endif
#ifndef CMSG_LEN
#define	CMSG_LEN(l) \
	((unsigned int)_CMSG_DATA_ALIGN(sizeof (struct cmsghdr)) + (l))
#endif
#endif


/* Error Values */
#define DH6ERR_FAILURE		16
#define DH6ERR_AUTHFAIL		17
#define DH6ERR_POORLYFORMED	18
#define DH6ERR_UNAVAIL		19
#define DH6ERR_OPTUNAVAIL	20

/* Message type */
#define DH6_SOLICIT	1
#define DH6_ADVERTISE	2
#define DH6_REQUEST	3
#define DH6_CONFIRM	4
#define DH6_RENEW	5
#define DH6_REBIND	6
#define DH6_REPLY	7
#define DH6_RELEASE	8
#define DH6_DECLINE	9
#define DH6_RECONFIGURE	10
#define DH6_INFORM_REQ	11
#define DH6_RELAY_FORW	12
#define DH6_RELAY_REPLY	13
#define DH6_CONF_REPLY  14      /* Foxconn Bernie added, 2014/11/27 @ reply after received confirm msg.*/


/* Predefined addresses */
#define DH6ADDR_ALLAGENT	"ff02::1:2"
#define DH6ADDR_ALLSERVER	"ff05::1:3"
#define DH6PORT_DOWNSTREAM	"546"
#define DH6PORT_UPSTREAM	"547"

/* Protocol constants */

/* timer parameters (msec, unless explicitly commented) */
#define SOL_MAX_DELAY	1000
#define SOL_TIMEOUT	1000
#define SOL_MAX_RT	3600000  /* Foxconn Bernie modified, 2014/11/27 */
#define INF_TIMEOUT	1000
#define INF_MAX_RT	120000
#define REQ_TIMEOUT	1000
#define REQ_MAX_RT	30000
#define REQ_MAX_RC	10	/* Max Request retry attempts */
#define REN_TIMEOUT	10000	/* 10secs */
#define REN_MAX_RT	600000	/* 600secs */
#define REB_TIMEOUT	10000	/* 10secs */
#define REB_MAX_RT	600000	/* 600secs */
#define REL_TIMEOUT	2000	/* 1 sec */ /* Foxconn Bernie modified, 2014/11/27 */
#define REL_MAX_RC	5
/* Foxconn Bernie added start, 2014/11/27 */
#define DEC_TIMEOUT	1000
#define DEC_MAX_RC	5
#define CNF_TIMEOUT	1000
#define CNF_MAX_RT	4000
#define CNF_MAX_RD	10000
/* Foxconn Bernie added end, 2014/11/27 */

#define DHCP6_DURATION_INFINITE 0xffffffff
#define DHCP6_DURATION_MIN 30

#define DHCP6_RELAY_MULTICAST_HOPS 32
#define DHCP6_RELAY_HOP_COUNT_LIMIT 32

#define DHCP6_IRT_DEFAULT 86400	/* 1 day */
#define DHCP6_IRT_MINIMUM 600

#define VENDOR_IANA_ID_BROADBAND_FORUM 3561
#define VENDOR_IANA_ID_CHARTER 9914
#define VENDOR_BROADBAND_FORUM_ACSURL_SUBOPT 1
#define VENDOR_BROADBAND_FORUM_PROVISIONINGCODE_SUBOPT 2
#define VENDOR_BROADBAND_FORUM_CWMPRETRYMINWAITINTERVAL_SUBOPT 3
#define VENDOR_BROADBAND_FORUM_CWMPRETRYINTERVALMULTI_SUBOPT 4
#define VENDOR_BROADBAND_FORUM_X_CHARTER_COM_ACSID_SUBOPT 234

/* DUID: DHCP unique Identifier */
struct duid {
	size_t duid_len;	/* length */
	char *duid_id;		/* variable length ID value (must be opaque) */
};

struct dhcp6_vbuf {		/* generic variable length buffer */
	int dv_len;
	caddr_t dv_buf;
};

/* option information */
struct dhcp6_ia {		/* identity association */
	u_int32_t iaid;
	u_int32_t t1;
	u_int32_t t2;
};

struct dhcp6_prefix {		/* IA_PA */
	u_int32_t pltime;
	u_int32_t vltime;
	struct in6_addr addr;
	int plen;
};

struct dhcp6_statefuladdr {	/* IA_NA */
	u_int32_t pltime;
	u_int32_t vltime;
	struct in6_addr addr;
	int plen; /* Foxconn Bernie added, 2014/11/27 */
};

/* Foxconn Bernie added start, 2014/11/27 */
struct iana_information
{
	/*iana_information*/
	char iaaddr[256];
	u_int32_t pltime;
	u_int32_t vltime;
	int plen;
	int isinvalid_t1t2; /* Foxconn Bernie added, 2015/01/16 @ ce logo*/

};

struct iapd_information
{
	/*iapd_information*/
	char v6str[50];
	char prefix[50];
	int plen;
	u_int32_t iaid;
	int duid_len;
	char duid[260];
	u_int32_t pltimePD;
	u_int32_t vltimePD;

};
/* Foxconn Bernie added end, 2014/11/27 */


/* Internal data structure */
typedef enum { DHCP6_LISTVAL_NUM = 1,
	       DHCP6_LISTVAL_STCODE, DHCP6_LISTVAL_ADDR6,
	       DHCP6_LISTVAL_IAPD, DHCP6_LISTVAL_PREFIX6,
	       DHCP6_LISTVAL_IANA, DHCP6_LISTVAL_STATEFULADDR6,
	       DHCP6_LISTVAL_NTP,
	       DHCP6_LISTVAL_VENDORCLASS, DHCP6_LISTVAL_VENDORCLASS_DATA,
	       DHCP6_LISTVAL_VENDOR, DHCP6_LISTVAL_VENDOR_OUI, DHCP6_LISTVAL_VENDOR_SN, DHCP6_LISTVAL_VENDOR_PD,
	       DHCP6_LISTVAL_VENDOR_CHTR_ORO, DHCP6_LISTVAL_VENDOR_CHTR_DEVTYPE, DHCP6_LISTVAL_VENDOR_CHTR_ECMESAFE,
	       DHCP6_LISTVAL_VENDOR_CHTR_DEVSN, DHCP6_LISTVAL_VENDOR_CHTR_DEVHWVER, DHCP6_LISTVAL_VENDOR_CHTR_DEVSWVER,
	       DHCP6_LISTVAL_VENDOR_CHTR_DEVBLVER, DHCP6_LISTVAL_VENDOR_CHTR_DEVVENDEROUI, DHCP6_LISTVAL_VENDOR_CHTR_DEVMODELID,
	       DHCP6_LISTVAL_VENDOR_CHTR_DEVVENDERID,
	       DHCP6_LISTVAL_VENDOR_BBF_ACSURL, DHCP6_LISTVAL_VENDOR_BBF_PROVISIONINGCODE,
	       DHCP6_LISTVAL_VENDOR_BBF_CWMPRETRYMINWAITINTERVAL,DHCP6_LISTVAL_VENDOR_BBF_CWMPRETRYINTERVALMULTI,DHCP6_LISTVAL_VENDOR_BBF_X_CHARTER_COM_ACSID,
	       DHCP6_LISTVAL_VBUF
} dhcp6_listval_type_t;
TAILQ_HEAD(dhcp6_list, dhcp6_listval);
struct dhcp6_listval {
	TAILQ_ENTRY(dhcp6_listval) link;

	dhcp6_listval_type_t type;

	union {
		int uv_num;
		u_int16_t uv_num16;
		u_int32_t uv_num32;
		struct in6_addr uv_addr6;
		struct dhcp6_prefix uv_prefix6;
		struct dhcp6_statefuladdr uv_statefuladdr6;
		struct dhcp6_ia uv_ia;
		struct dhcp6_vbuf uv_vbuf;
	} uv;

	struct dhcp6_list sublist;
};
#define val_num uv.uv_num
#define val_num16 uv.uv_num16
#define val_num32 uv.uv_num32
#define val_addr6 uv.uv_addr6
#define val_ia uv.uv_ia
#define val_prefix6 uv.uv_prefix6
#define val_statefuladdr6 uv.uv_statefuladdr6
#define val_vbuf uv.uv_vbuf

struct dhcp6_optinfo {
	struct duid clientID;	/* DUID */
	struct duid serverID;	/* DUID */

	int rapidcommit;	/* bool */
	int pref;		/* server preference */
	int32_t elapsed_time;	/* elapsed time (from client to server only) */
	int64_t refreshtime;	/* info refresh time for stateless options */
	char sol_map_min[512];		/* SOL_MAP_MIN from DHCPv6 Server*/ /* Foxconn Bernie added, 2015/01/20 @ Support option95*/
	long sol_max_rt;		/* Foxconn Bernie added , 2015/9/14@ SOL_MAX_RT from DHCPv6 Server*/
	char sol_user_class[512];		/* SOL_USER_CLASS from DHCPv6 Server*/ /* Foxconn Bernie added, 2016/07/27 @ Support option15*/

	struct dhcp6_list iapd_list; /* list of IA_PD */
	struct dhcp6_list iana_list; /* list of IA_NA */
	struct dhcp6_list reqopt_list; /* options in option request */
	struct dhcp6_list stcode_list; /* status code */
	struct dhcp6_list sip_list; /* SIP server list */
	struct dhcp6_list sipname_list; /* SIP domain list */
	struct dhcp6_list dns_list; /* DNS server list */
	struct dhcp6_list dnsname_list; /* Domain Search list */
	struct dhcp6_list ntp_list; /* NTP server list */
	struct dhcp6_list prefix_list; /* prefix list */
	struct dhcp6_list nis_list; /* NIS server list */
	struct dhcp6_list nisname_list; /* NIS domain list */
	struct dhcp6_list nisp_list; /* NIS+ server list */
	struct dhcp6_list nispname_list; /* NIS+ domain list */
	struct dhcp6_list bcmcs_list; /* BCMC server list */
	struct dhcp6_list bcmcsname_list; /* BCMC domain list */
	struct dhcp6_list vendorclass_list; /* Vendor-class option list*/
	struct dhcp6_list vendoropt_list; /* Vendor-specific option list*/

	struct dhcp6_vbuf relay_msg; /* relay message */
#define relaymsg_len relay_msg.dv_len
#define relaymsg_msg relay_msg.dv_buf

	struct dhcp6_vbuf ifidopt; /* Interface-id */
#define ifidopt_len ifidopt.dv_len
#define ifidopt_id ifidopt.dv_buf

/* Foxconn Bernie added start, 2014/11/27 */
	struct dhcp6_vbuf reconfig_msg; /* reconfig message */
#define recmsg_len reconfig_msg.dv_len
#define recmsg_msg reconfig_msg.dv_buf

	int reconfig_flag; /* reconfig accept option flag */
/* Foxconn Bernie added end, 2014/11/27 */

/* Foxconn Bernie added start, 2016/07/28 */
        struct dhcp6_vbuf userclass_msg; /* userclass message */
#define userclass_len userclass_msg.dv_len
#define userclass_buf userclass_msg.dv_buf

/* Foxconn Bernie added end, 2016/07/28 */


	u_int authflags;
#define DHCP6OPT_AUTHFLAG_NOINFO	0x1
	int authproto;
	int authalgorithm;
	int authrdm;
	/* the followings are effective only when NOINFO is unset */
	u_int64_t authrd;
	union {
		struct {
			u_int32_t keyid;
			struct dhcp6_vbuf realm;
			int offset; /* offset to the HMAC field */
		} aiu_delayed;
		struct {
			int type;
			int offset; /* offset to the HMAC field */
			char val[16]; /* key value */
		} aiu_reconfig;
	} authinfo;
#define delayedauth_keyid authinfo.aiu_delayed.keyid
#define delayedauth_realmlen authinfo.aiu_delayed.realm.dv_len
#define delayedauth_realmval authinfo.aiu_delayed.realm.dv_buf
#define delayedauth_offset authinfo.aiu_delayed.offset
#define reconfigauth_type authinfo.aiu_reconfig.type
#define reconfigauth_offset authinfo.aiu_reconfig.offset
#define reconfigauth_val authinfo.aiu_reconfig.val
};

/* DHCP6 base packet format */
struct dhcp6 {
	union {
		u_int8_t m;
		u_int32_t x;
	} dh6_msgtypexid;
	/* options follow */
} __attribute__ ((__packed__));
#define dh6_msgtype	dh6_msgtypexid.m
#define dh6_xid		dh6_msgtypexid.x
#define DH6_XIDMASK	0x00ffffff

/* DHCPv6 relay messages */
struct dhcp6_relay {
	u_int8_t dh6relay_msgtype;
	u_int8_t dh6relay_hcnt;
	struct in6_addr dh6relay_linkaddr; /* XXX: badly aligned */
	struct in6_addr dh6relay_peeraddr; /* ditto */
	/* options follow */
} __attribute__ ((__packed__));

/* options */
#define DH6OPT_SOLMAPMIN 95
#define DH6OPT_SOLMAXRT 82 /* Foxconn Bernie added , 2015/9/14@ support sol max rt option */
#define DH6OPT_CLIENTID	1
#define DH6OPT_SERVERID	2
#define DH6OPT_IA_NA 3
#define DH6OPT_IA_TA 4
#define DH6OPT_IAADDR 5
#define DH6OPT_ORO 6
#define DH6OPT_PREFERENCE 7
#  define DH6OPT_PREF_UNDEF -1
#  define DH6OPT_PREF_MAX 255
#define DH6OPT_ELAPSED_TIME 8
#  define DH6OPT_ELAPSED_TIME_UNDEF -1
#define DH6OPT_RELAY_MSG 9
/* #define DH6OPT_SERVER_MSG 10: deprecated */
#define DH6OPT_AUTH 11

/* Foxconn Bernie added start, 2014/11/27 */
#define DH6OPT_AUTH_RECONFIG_KEY 1
#define DH6OPT_AUTH_RECONFIG_HMACMD5 2
/* Foxconn Bernie added end, 2014/11/27 */
#  define DH6OPT_AUTH_PROTO_DELAYED 2
#  define DH6OPT_AUTH_RRECONFIGURE 3
#  define DH6OPT_AUTH_ALG_HMACMD5 1
#define DH6OPT_UNICAST 12
#define DH6OPT_STATUS_CODE 13
#  define DH6OPT_STCODE_SUCCESS 0
#  define DH6OPT_STCODE_UNSPECFAIL 1
#  define DH6OPT_STCODE_NOADDRSAVAIL 2
#  define DH6OPT_STCODE_NOBINDING 3
#  define DH6OPT_STCODE_NOTONLINK 4
#  define DH6OPT_STCODE_USEMULTICAST 5
#  define DH6OPT_STCODE_NOPREFIXAVAIL 6

#define DH6OPT_RAPID_COMMIT 14
#define DH6OPT_USER_CLASS 15
#define DH6OPT_VENDOR_CLASS 16
#define DH6OPT_VENDOR_OPTS 17
/* TR-069a5 : Table 106 – Encapsulated Vendor-Specific Option-Data fields */
#define DH6OPT_VENDOR_SUBOPTS_OUI 11
#define DH6OPT_VENDOR_SUBOPTS_SN 12
#define DH6OPT_VENDOR_SUBOPTS_PD 13

#define DH6OPT_VENDOR_CHTR_SUBOPTS_ORO 1
#define DH6OPT_VENDOR_CHTR_SUBOPTS_DEVTYPE 2
#define DH6OPT_VENDOR_CHTR_SUBOPTS_ECMESAFE 3
#define DH6OPT_VENDOR_CHTR_SUBOPTS_DEVSN 4
#define DH6OPT_VENDOR_CHTR_SUBOPTS_DEVHWVER 5
#define DH6OPT_VENDOR_CHTR_SUBOPTS_DEVSWVER 6
#define DH6OPT_VENDOR_CHTR_SUBOPTS_DEVBLVER 7
#define DH6OPT_VENDOR_CHTR_SUBOPTS_DEVVENDEROUI 8
#define DH6OPT_VENDOR_CHTR_SUBOPTS_DEVMODELID 9
#define DH6OPT_VENDOR_CHTR_SUBOPTS_DEVVENDERID 10

#define DH6OPT_INTERFACE_ID 18
#define DH6OPT_RECONF_MSG 19
#define DH6OPT_RECONF_ACCEPT 20 /* Foxconn Bernie added, 2014/11/27 */

#define DH6OPT_SIP_SERVER_D 21
#define DH6OPT_SIP_SERVER_A 22
#define DH6OPT_DNS 23
#define DH6OPT_DNSNAME 24
#define DH6OPT_IA_PD 25
#define DH6OPT_IA_PD_PREFIX 26
#define DH6OPT_NIS_SERVERS 27
#define DH6OPT_NISP_SERVERS 28
#define DH6OPT_NIS_DOMAIN_NAME 29
#define DH6OPT_NISP_DOMAIN_NAME 30
#define DH6OPT_SNTP 31
#define DH6OPT_REFRESHTIME 32
 #define DH6OPT_REFRESHTIME_UNDEF -1
#define DH6OPT_BCMCS_SERVER_D 33
#define DH6OPT_BCMCS_SERVER_A 34
#define DH6OPT_GEOCONF_CIVIC 36
#define DH6OPT_REMOTE_ID 37
#define DH6OPT_SUBSCRIBER_ID 38
#define DH6OPT_CLIENT_FQDN 39

#define DH6OPT_NTP 56
#define DH6OPT_NTP_SUBOPTION_SRV_ADDR 1
#define DH6OPT_NTP_SUBOPTION_MC_ADDR 2
#define DH6OPT_NTP_SUBOPTION_SRV_FQDN 3

/* The followings are KAME specific. */

struct dhcp6opt {
	u_int16_t dh6opt_type;
	u_int16_t dh6opt_len;
	/* type-dependent data follows */
} __attribute__ ((__packed__));

/* DUID type 1 */
struct dhcp6opt_duid_type1 {
	u_int16_t dh6_duid1_type;
	u_int16_t dh6_duid1_hwtype;
	u_int32_t dh6_duid1_time;
	/* link-layer address follows */
} __attribute__ ((__packed__));

/* Foxconn Bernie added, 2014/11/27 @Support DUID-LL (link-layer address)*/
	 /* DUID type 3 */
struct dhcp6opt_duid_type3 {
	u_int16_t dh6_duid3_type;
	u_int16_t dh6_duid3_hwtype;
	u_int8_t  dh6_duid3_mac[6];
	/* link-layer address follows */
} __attribute__ ((__packed__));
/* Foxconn Bernie added end, 2014/11/27 */
/* Status Code */
struct dhcp6opt_stcode {
	u_int16_t dh6_stcode_type;
	u_int16_t dh6_stcode_len;
	u_int16_t dh6_stcode_code;
} __attribute__ ((__packed__));

/*
 * General format of Identity Association.
 * This format applies to Prefix Delegation (IA_PD) and Non-temporary Addresses
 * (IA_NA)
 */
struct dhcp6opt_ia {
	u_int16_t dh6_ia_type;
	u_int16_t dh6_ia_len;
	u_int32_t dh6_ia_iaid;
	u_int32_t dh6_ia_t1;
	u_int32_t dh6_ia_t2;
	/* sub options follow */
} __attribute__ ((__packed__));

/* IA Addr */
struct dhcp6opt_ia_addr {
	u_int16_t dh6_ia_addr_type;
	u_int16_t dh6_ia_addr_len;
	struct in6_addr dh6_ia_addr_addr;
	u_int32_t dh6_ia_addr_preferred_time;
	u_int32_t dh6_ia_addr_valid_time;
} __attribute__ ((__packed__));

/* IA_PD Prefix */
struct dhcp6opt_ia_pd_prefix {
	u_int16_t dh6_iapd_prefix_type;
	u_int16_t dh6_iapd_prefix_len;
	u_int32_t dh6_iapd_prefix_preferred_time;
	u_int32_t dh6_iapd_prefix_valid_time;
	u_int8_t dh6_iapd_prefix_prefix_len;
	struct in6_addr dh6_iapd_prefix_prefix_addr;
} __attribute__ ((__packed__));

/* Authentication */
struct dhcp6opt_auth {
	u_int16_t dh6_auth_type;
	u_int16_t dh6_auth_len;
	u_int8_t dh6_auth_proto;
	u_int8_t dh6_auth_alg;
	u_int8_t dh6_auth_rdm;
	u_int8_t dh6_auth_rdinfo[8];
	/* authentication information follows */
} __attribute__ ((__packed__));

struct dhcp6opt_ntpsub {
	u_int16_t subopt_code;
	u_int16_t subopt_len;
	char subopt_val[256];
} __attribute__ ((__packed__));

struct dhcp6opt_vendorclass {
	u_int32_t dh6_ia_iaid;
} __attribute__ ((__packed__));

struct dhcp6opt_vendorclass_data {
	u_int16_t vendorclass_len;
	char vendorclass_data[256];
} __attribute__ ((__packed__));

struct dhcp6opt_vendor {
	u_int32_t dh6_ia_iaid;
	/* sub options follow */
} __attribute__ ((__packed__));

struct dhcp6opt_vendorsub {
	u_int16_t subopt_id;
	u_int16_t subopt_len;
	char subopt_val[256];
} __attribute__ ((__packed__));

struct dhcp6opt_vendorsubsub {
	u_int8_t subopt_id;
	u_int8_t subopt_len;
	char subopt_val[256];
} __attribute__ ((__packed__));


enum { DHCP6_AUTHPROTO_UNDEF = -1, DHCP6_AUTHPROTO_DELAYED = 2,
       DHCP6_AUTHPROTO_RECONFIG = 3 };
enum { DHCP6_AUTHALG_UNDEF = -1, DHCP6_AUTHALG_HMACMD5 = 1 };
enum { DHCP6_AUTHRDM_UNDEF = -1, DHCP6_AUTHRDM_MONOCOUNTER = 0 };

/* Foxconn Bernie added start, 2014/11/27 @for dad parse*/
struct ifproc_info {
	struct ifproc_info *next;
	struct in6_addr addr;
	int index;
	int plen;
	int scope;
	int flags;
};
/* Foxconn Bernie added end, 2014/11/27 */

#endif /*__DHCP6_H_DEFINED*/
