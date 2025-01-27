
/* This is a modified version of the file "arena.h" from 
   "C Interfaces and Implementations", by David R. Hanson.
   The license is below.
*/
/* 

The author of this software is David R. Hanson.

Copyright (c) 1994,1995,1996,1997 by David R. Hanson. All Rights Reserved.

Permission to use, copy, modify, and distribute this software for any
purpose, subject to the provisions described below, without fee is
hereby granted, provided that this entire notice is included in all
copies of any software that is or includes a copy or modification of
this software and in all copies of the supporting documentation for
such software.

THIS SOFTWARE IS BEING PROVIDED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
WARRANTY. IN PARTICULAR, THE AUTHOR DOES MAKE ANY REPRESENTATION OR
WARRANTY OF ANY KIND CONCERNING THE MERCHANTABILITY OF THIS SOFTWARE OR
ITS FITNESS FOR ANY PARTICULAR PURPOSE.

David Hanson / drh@microsoft.com / http://www.research.microsoft.com/~drh/
$Id: //DTV/MP_BR/DTV_X_IDTV0801_002298_3_001/android/kk-4.x/external/valgrind/main/VEX/unused/arena.h#1 $
*/

/* $Id: //DTV/MP_BR/DTV_X_IDTV0801_002298_3_001/android/kk-4.x/external/valgrind/main/VEX/unused/arena.h#1 $ */

#ifndef _CII_ARENA_H
#define _CII_ARENA_H

//#include "except.h"
#define T Arena_T
typedef struct T *T;
//extern const Except_T Arena_NewFailed;
//extern const Except_T Arena_Failed;
extern T    Arena_new    (void);
extern void Arena_dispose(T *ap);
extern void *Arena_alloc (T arena, long nbytes,
	const char *file, int line);
extern void *Arena_calloc(T arena, long count,
	long nbytes, const char *file, int line);
extern void  Arena_free  (T arena);
#undef T

#endif /* ndef _CII_ARENA_H */
