/*************
 *
 * $Id:$
 *
 *  Name:      d2api.h
 *
 *  Purpose:   Define API for access to DAL 2
 *
 *  Note:
 *
 * Copyright: (c) 2017 Netgear, Inc.
 *            All rights reserved
 *
 **************/
#ifndef _D2_API_H_
#define _D2_API_H_

#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

#ifdef	__cplusplus
extern "C" {
#endif

/*
 * Supported types
 */
enum d2_types
{
	D2_bool_TYPE,
	D2_uint32_TYPE,
	D2_uint64_TYPE,
	D2_asciistring_TYPE,
	D2_string_TYPE,
	D2_enum_TYPE,
	D2_mac_TYPE,
	D2_float_TYPE,
	D2_ipv4_TYPE,
	D2_ipv6_TYPE
};


enum d2_error
{
	D2_OK,
	D2_TIMEOUT,
	D2_SHUTDOWN,
	D2_NOSUCH_ITEM,
	D2_TOO_LONG,
	D2_INVALID_INPUT,
	D2_WRONG_TYPE,
	D2_ENCODING,
	D2_ERROR,
	D2_UNKNOWN_ERROR
};

/********************************************************************
 *
 * Conversion routines for custom types: IPv4, IPv6, MAC
 *
 */

/*******
 * IPv4 in big endian/network order (@see d2_ipv42string)
 */
struct d2_ipv4
{
	uint8_t ip[4];
};
static inline int d2_ipv42string( const struct d2_ipv4 * ip, char * str, size_t strlen )
{
	return snprintf( str, strlen, "%d.%d.%d.%d", ip->ip[0], ip->ip[1], ip->ip[2],ip->ip[3] );
}
int d2_string2ipv4( const char * str, struct d2_ipv4 * ip);

/*******
 * IPv6 in big endian/network order (@see d2_ipv62string)
 */
struct d2_ipv6
{
	/* IPv4 in big endian/network order */
	uint8_t ip[16];
};
static inline int d2_ipv62string( const struct d2_ipv6 * ip, char * str, size_t strlen )
{
	return snprintf( str, strlen, "%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X:%02X%02X",
			         ip->ip[0], ip->ip[1], ip->ip[2],ip->ip[3],
					 ip->ip[4], ip->ip[5], ip->ip[6],ip->ip[7],
					 ip->ip[8], ip->ip[9], ip->ip[10],ip->ip[11],
					 ip->ip[12], ip->ip[13], ip->ip[14],ip->ip[15]);
}
int d2_string2ipv6( const char * str, struct d2_ipv6 * ip );

/*******
 *   MAC - Ethernet MAC address
 */
struct d2_mac
{
	uint8_t mac[6];
};
static inline int d2_mac2string( const struct d2_mac *mac, char * str, size_t strlen )
{
	return snprintf( str, strlen, "%02X:%02X:%02X:%02X:%02X:%02X",
			         mac->mac[0], mac->mac[1], mac->mac[2], mac->mac[3], mac->mac[4], mac->mac[5] );
}
int d2_string2mac( const char * str, struct d2_mac * mac );

/********************************************************************
 *
 * D2 API
 *
 */
void * d2_init( void );
void * d2_init_ex( const char * sockname );
void   d2_destroy( void * );

/* Get error string */
const char * d2_error_string( enum d2_error err );

/* install error callback for long running operations */
enum d2_error d2_error_cb( void *d2h, void (*errcb)( void * up), void * up );

/*
 * Manage subscriptions and callback
 */
enum d2_error d2_subscribe_cb( void *, void (*)(const char * tbl, int index, const char * fld, void * up), void * up );
enum d2_error d2_subscribe( void *, const char * table, bool onoff );

/*
 *
 */
enum d2_error d2_lock_table( void *, const char * table );
enum d2_error d2_unlock_table( void *, const char * table );

/*
 * SET operations
 */
enum d2_error d2_set_enum( void *, const char * table, unsigned int index, const char * field, const char * value );
enum d2_error d2_set_string( void *, const char * table, unsigned int index, const char * field, const char * value );
enum d2_error d2_set_ascii( void *, const char * table, unsigned int index, const char * field, const char * value );
enum d2_error d2_set_bool( void *, const char * table, unsigned int index, const char * field, bool value );
enum d2_error d2_set_uint( void *, const char * table, unsigned int index, const char * field, uint32_t value );
enum d2_error d2_set_int( void *, const char * table, unsigned int index, const char * field, int32_t value );
enum d2_error d2_set_int64( void *, const char * table, unsigned int index, const char * field, int64_t value );
enum d2_error d2_set_float( void *, const char * table, unsigned int index, const char * field, float value );
enum d2_error d2_set_mac( void *, const char * table, unsigned int index, const char * field, const struct d2_mac * valuep );
enum d2_error d2_set_ipv4( void *, const char * table, unsigned int index, const char * field, const struct d2_ipv4 * valuep );
enum d2_error d2_set_ipv6( void *, const char * table, unsigned int index, const char * field, const struct d2_ipv6 * valuep );

/*
 * GET operations
 */
enum d2_error d2_get_enum( void *, const char * table, unsigned int index, const char * field, char * buf, uint32_t buflen );
enum d2_error d2_get_string( void *, const char * table, unsigned int index, const char * field, char * buf, uint32_t buflen );
enum d2_error d2_get_ascii( void *, const char * table, unsigned int index, const char * field, char * buf, uint32_t buflen );
enum d2_error d2_get_bool( void *, const char * table, unsigned int index, const char * field, bool * buf );
enum d2_error d2_get_ipv4( void *dbapih, const char * table, unsigned int index, const char * field, struct d2_ipv4 * buf );
enum d2_error d2_get_ipv6( void *dbapih, const char * table, unsigned int index, const char * field, struct d2_ipv6 * buf );
enum d2_error d2_get_mac( void *dbapih, const char * table, unsigned int index, const char * field, struct d2_mac * buf );
enum d2_error d2_get_uint( void *dbapih, const char * table, unsigned int index, const char * field, uint32_t * buf );
enum d2_error d2_get_int( void *dbapih, const char * table, unsigned int index, const char * field, int32_t * buf );
enum d2_error d2_get_int64( void *dbapih, const char * table, unsigned int index, const char * field, int64_t * buf );
enum d2_error d2_get_float( void *, const char * table, unsigned int index, const char * field, float * buf );


/*
 * retrieve META data, describing fields
 */
enum d2_error d2_get_fieldinfo_byindex( void *d2h,
		                                const char * table, unsigned int index, int field_index,
										int * type, uint32_t * maxlen,
										char * fieldname, uint32_t namelen,
										char * xtra, uint32_t xtralen );
enum d2_error d2_get_fieldinfo( void *d2h,
		                        const char * table, unsigned int index, const char * field,
								int * type, uint32_t * maxlen,
								char * xtra, uint32_t xtralen );

/********************************************************************
 *
 * Utilities
 *
 */
bool d2_verify_utf8_encoding( const void * bp, size_t dl );
bool d2_verify_printable_ascii( const void * bp, size_t dl );


#ifdef	__cplusplus
}
#endif

#endif
