/*
 * misc.h
 *
 * Copyright (c) GoAhead Software Inc., 1995-2000. All Rights Reserved.
 *
 * See the file "license.txt" for usage and redistribution license requirements
 *
 * $Id: uemf.h,v 1.3 2007-02-01 07:41:01 winfred Exp $
 *
 * Copied from Ralink GoAhead web Server
 */

#ifndef _h_MISC
#define _h_MISC 1

#include	<stdarg.h>

#define TRACE_MAX			(4096 - 48)
#define VALUE_MAX_STRING	(4096 - 48)
#define FMT_STATIC_MAX		256				/* Maximum for fmtStatic calls */
#define BUFSIZE				1024

#ifndef CHAR_T_DEFINED
#define CHAR_T_DEFINED 1
#define	T(s) 				s
typedef char				char_t;
#define	TSZ(x)				(sizeof(x))
#define	TASTRL(x)			(strlen(x) + 1)
#endif /* ! CHAR_T_DEFINED */

#define a_assert(C)		if (1) ; else

#ifndef max
#define max(a,b)  (((a) > (b)) ? (a) : (b))
#endif /* max */

#ifndef min
#define min(a,b)  (((a) < (b)) ? (a) : (b))
#endif /* min */


/*
 *	Block allocation (balloc) definitions
 */
#ifndef B_L
#define B_L				T(__FILE__), __LINE__
#define B_ARGS_DEC		char_t *file, int line
#define B_ARGS			file, line
#endif /* B_L */

#define balloc(B_ARGS, num) malloc(num)
#define bfree(B_ARGS, p) free(p)
#define bfreeSafe(B_ARGS, p) \
	if (p) { free(p); } else
#define brealloc(B_ARGS, p, num) realloc(p, num)

extern void		trace(int lev, char_t *fmt, ...);
extern void		traceRaw(char_t *buf);

extern int		fmtValloc(char_t **s, int n, char_t *fmt, va_list arg);
extern int		fmtAlloc(char_t **s, int n, char_t *fmt, ...);
extern int		fmtStatic(char_t *s, int n, char_t *fmt, ...);
#endif
