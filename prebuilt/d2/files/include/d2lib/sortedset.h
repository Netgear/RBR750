/*************
 *
 * $Id:$
 *
 *  Name:      sortedset.h
 *
 *  Purpose:   Utility to manage a set of items in sorted order
 *
 *  Note:
 *
 * Copyright: (c) 2017 Netgear, Inc.
 *            All rights reserved
 *
 **************/
#ifndef SORTEDSET_H_
#define SORTEDSET_H_

#include <stdlib.h>
#include <inttypes.h>

#ifdef	__cplusplus
extern "C" {
#endif

void * sortedSet_init(
		uint32_t initsize,
		int (*compare)( const void * const * i1, const void * const * i2 )
		);
void sortedSet_destroy( void * ssh );
void * sortedSet_find( void * ssh, const void * key );
void * sortedSet_first( void * ssh );
int sortedSet_len( void * ssh );
int sortedSet_add( void * ssh, const void * item );
int sortedSet_rem( void * ssh, const void * item );
void sortedSet_foreach( void * ssh,
		                void (*foo)( const void * item, uint32_t index, void * up),
						void *up );

#ifdef	__cplusplus
}
#endif

#endif /* SORTEDSET_H_ */
