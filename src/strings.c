/* $Id: strings.c,v 1.2 2006/09/09 15:58:13 jpd Exp $ */

/*
    Copyright 2004, 2005, 2006 John P. Doty and Matthew P. Wampler-Doty

    This file is part of LSE64.

    LSE64 is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    LSE64 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with LSE64; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include "lse64.h"
#include <sys/param.h>
#include <stdlib.h>

/*
 * cstring() pops an LSE string pointer from the stack, returning
 * a pointer to an equivalent C char string.
 */

char *cstring( char *b, int n )
{
	cell *s = (cell *)(intptr_t)*sp++;
	int nc, nseg, i;
	wchar_t wb[MAXPATHLEN+1], *wbp;
	static char sb[MAXPATHLEN+1];
	char *bp;
	
	if( !b ) { b = sb; n = MAXPATHLEN+1; }  /* use the static buffer */
	b[--n] = 0;								/* force null termination */
	bp = b;
	
	for( nc = *s++; nc > 0; nc -= MAXPATHLEN ) {
		if( nc > MAXPATHLEN ) nseg = MAXPATHLEN; else nseg = nc;
		wbp = wb;
		for( i = 0; i < nseg; i += 1 ) *wbp++ = *s++;
		*wbp++ = 0;
		n -= wcstombs( bp, wb, n );
		if( n <= 0 ) break;
	}
	return b;
}

/*
 * ToNumber converts a string to a number.
 * If the whole string can be converted assuming it's an integer,
 * an integer results and the flag is 1.
 * Otherwise, if the whgole string can be converted assuming it's
 * a floating point number, a floating point number results and the flag is 1.
 * Otherwise, 0 results and the flag is 0,
 */

void ToNumber( void )
{
	char *b = cstring( 0, 0 );
	char *ep;
	cell i = strtoll( b, &ep, 10 );
	double d;
	
	if( !*ep ) {		/* found the null termination */
		*--sp = i;
		flag = 1;
		return;
	}
	
	d = strtod( b, &ep );
	
	if( !*ep ) {		/* found the null termination */
		*(fcell *)--sp = d;
		flag = 1;
		return;
	}
	
	*--sp = flag = 0;   /* conversion failed */
}

/*
 * HexNumber converts a string to a number.
 * If the whole string can be converted,
 * an integer results and the flag is 1.
 * Otherwise, 0 results and the flag is 0,
 */

void HexNumber( void )
{
	char *b = cstring( 0, 0 );
	char *ep;
	cell i = strtoull( b, &ep, 16 );
	
	if( !*ep ) {		/* found the null termination */
		*--sp = i;
		flag = 1;
		return;
	}
	
	*--sp = flag = 0;   /* conversion failed */
}

/*
 * mode path fopen yields c
 */

void openf( void )
{
	char path[MAXPATHLEN+1];
	FILE *f;
	
	(void) cstring( path, MAXPATHLEN+1 );
	f = fopen( path, cstring( NULL, 0 ));
	*--sp = (intptr_t) f;
	if( f ) flag = 1; else flag = 0;
}

/*
 * mode fd fdopen yields c
 */

void openfd( void )
{
	FILE *f;
	int fd = *sp++;
	char *mode = cstring( NULL, 0 );
	
	f = fdopen( fd, mode);
	*--sp = (intptr_t) f;
	if( f ) flag = 1; else flag = 0;
}


/*
 * n fmt sprint1 yields s
 */

void sprint1( void )
{
	static cell cb[101];
	char b[101];
	char *f = cstring( 0, 0 );
	int n;
	
	cb[0] = snprintf( b, 101, f, *sp++ );
	if( cb[0] > 100 ) cb[0] = 100;
	
	for( n = 0; n < cb[0]; n+=1 ) cb[ n + 1] = b[ n ];
	
	*--sp = (cell) cb;
}
	
	

/*
 * $Log: strings.c,v $
 * Revision 1.2  2006/09/09 15:58:13  jpd
 * Release 0.3 with license and copyright notices
 *
 * Revision 1.1  2005/07/03 15:37:19  jpd
 * *** empty log message ***
 *
 * Revision 1.7  2005/04/21 17:16:15  jpd
 * Output redirection.
 *
 * Revision 1.6  2005/04/11 21:10:28  jpd
 * sprint1 to convert one arg.
 *
 * Revision 1.5  2004/09/28 17:15:52  jpd
 * Use unsigned conversion for \ to avoid overflows.
 * Now builds installable and test versions.
 *
 * Revision 1.4  2004/09/23 15:06:38  jpd
 * Added file operations.
 *
 * Revision 1.3  2004/09/19 20:56:06  jpd
 * Getting really usable!
 * First cut at comedi.
 *
 * Revision 1.2  2004/09/19 01:24:48  jpd
 * Compiler bugs fixed.
 * Now using fast ITC interpreter.
 *
 */


