/* $Id: primitives.c,v 1.2 2006/09/09 15:58:13 jpd Exp $ */

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
#include <stdlib.h>
#include <limits.h>
#include <wctype.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

/*
 * Basic memory access
 */
 
void fetch( void ) { *sp = *(cell*)(intptr_t)*sp; }
void fetchp( void ) { *sp = (intptr_t)*(cell**)(intptr_t)*sp; }
void store( void ) { cell *w = (cell *)(intptr_t)*sp++; *w = *sp++; }
void storep( void ) { cell **w = (cell **)(intptr_t)*sp++; *w = (cell *)(intptr_t)(*sp++); }
void append( void ) {		/* @!+ */
	cell *vpp = (cell *)(intptr_t)*sp++; 
	cell *vp = (cell *)(intptr_t) *vpp; 
	*vp++ = *sp++; *vpp = (intptr_t) vp; }
void scan( void ) {		/* @@+ */
	cell *vpp = (cell *)(intptr_t)*sp++;
	cell *vp = (cell *)(intptr_t) *vpp; 
	*--sp = *vp++; *vpp = (intptr_t) vp; }
/*
 * Constants and variables
 */

void stringconstant( void ) { cell *p = (cell *)(intptr_t)*lc; *--sp = (intptr_t) (p + D_DATA ); }
void constant( void ) { cell *p = (cell *)(intptr_t)*lc; *--sp = p[ D_DATA ]; }
void variable( void ) { cell *p = (cell *)(intptr_t)*lc; *--sp = (intptr_t)(p + D_BODY); }

/*
 * Arithmetic
 */
 
void add( void ) { cell w = *sp++; *sp += w; }
void subtract( void ) { cell w = *sp++; *sp -= w; }
void multiply( void ) { cell w = *sp++; *sp *= w; }
void divide( void ) { cell w = *sp++; *sp /= w; }
void mod( void ) { cell w = *sp++; *sp %= w; }
void neg( void ) { *sp = -*sp; }

void fadd( void ) { fcell w = *(fcell*)sp++; *(fcell*)sp += w; }
void fsubtract( void ) { fcell w = *(fcell*)sp++; *(fcell*)sp -= w; }
void fmultiply( void ) { fcell w = *(fcell*)sp++; *(fcell*)sp *= w; }
void fdivide( void ) { fcell w = *(fcell*)sp++; *(fcell*)sp /= w; }
void fmodd( void ) { fcell w = *(fcell*)sp++; *(fcell*)sp = fmod(*(fcell*)sp, w); }
void fneg( void ) { *(fcell*)sp = -*(fcell*)sp; }

void and( void ) { cell w = *sp++; *sp &= w; }
void or( void ) { cell w = *sp++; *sp |= w; }
void xor( void ) { cell w = *sp++; *sp ^= w; }
void not( void ) { *sp = ~*sp; }

void ffloat( void ) { *(double *)sp = (double) *sp; }
void fix( void ) { *sp = (cell) *(double *)sp; }

/*
 * Stack Gymnastics
 */
 
void dupp( void ) { cell w = *sp; *--sp = w; }
void swap( void ) { cell w = *sp; *sp = sp[1]; sp[1] = w; }
void drop( void ) { sp += 1; }
void pick( void ) { *sp = sp[ *sp ]; }
void rot( void ) { cell w0 = sp[0], w1 = sp[1]; sp[0] = sp[2]; sp[1] = w0; sp[2] = w1; }

/*
 * Predicates
 */

void equal( void ) { cell w = *sp++; flag = ( *sp++ == w ); }
void greater( void ) { cell w = *sp++; flag = ( *sp++ > w ); }
void less( void ) { cell w = *sp++; flag = ( *sp++ < w ); }
void equal0( void ) { flag = ( *sp++ == 0 ); }
void greater0( void ) { flag = ( *sp++ > 0 ); }
void less0( void ) { flag = ( *sp++ < 0 ); }

void fequal( void ) { fcell w = *(fcell*)sp++; flag = ( *(fcell*)sp++ == w ); }
void fgreater( void ) { fcell w = *(fcell*)sp++; flag = ( *(fcell*)sp++ > w ); }
void fless( void ) { fcell w = *(fcell*)sp++; flag = ( *(fcell*)sp++ < w ); }
void fequal0( void ) { flag = ( *(fcell*)sp++ == 0. ); }
void fgreater0( void ) { flag = ( *(fcell*)sp++ > 0. ); }
void fless0( void ) { flag = ( *(fcell*)sp++ < 0. ); }

void setflag( void ) { flag = 1; }
void clearflag( void ) {flag = 0; }

/*
 * Conditional and unconditional return
 */

void exitrue( void ) { if( flag ) lc = (cell *)(intptr_t)*rsp++; }
void exitfalse( void ) { if( !flag ) lc = (cell *)(intptr_t)*rsp++; }
void exitword( void ) { lc = (cell *)(intptr_t)*rsp++; }

/*
 * Tail call
 */

void jfalse( void ) { 		/* jump if false */
	cell t = *++lc; 
	if( !flag ) lc = ((cell *)(intptr_t) t) + D_BODY - 1 ;
}

/*
 * Goeke-style unstructured flow control.
 */
 
void iftrue( void ) { if( !flag ) lc+=1; }
void iffalse( void ) { if(flag) lc+=1;  }
void repeat( void ) { lc = ((cell *)(intptr_t)(*rsp++)) - 1; }
void repeatrue( void ) { if( flag ) lc = ((cell *)(intptr_t)(*rsp++)) - 1; }
void iterate_init( void ) { cell c = *sp++; if( c <= 0 ) lc+=2; else *--rsp = c; }
void iterate( void ) { if( --(*rsp) > 0 ) lc -= 2; else rsp += 1; }
void anditerate( void ) { if( (--(*rsp) > 0)  && flag ) lc -= 2; else rsp += 1; }
void count( void ) { *--sp = rsp[1]; }

/*
 * Return stack
 */
 
void s2r( void ) { *--rsp = *sp++; }
void r2s( void ) { *--sp = *rsp++; }
void rcopy( void ) { *--sp = *rsp; }
void rdrop( void ) { rsp += 1; }

/*
 * Misc
 */

void literal( void ) { *--sp = *++lc; } /* quote: push next cell onto stack */
void noop( void ) {}					/* do nothing */
void cells( void ) { cell w = *sp++; *sp += w * sizeof( cell ); }   /* offset by cells */

/*
 * Character I/O
 * Support multibyte characters POSIX style, one wide character per cell.
 */    

void put( void )
{
    char mbc[MB_LEN_MAX];
    int n = wctomb( mbc, (wchar_t) *sp++ );
    if( n > 0 && fwrite( mbc, sizeof(char), n, out ) != n )
        flag = 0;
    else
        flag = 1;
}

static int have_ungotten;
static wchar_t ungotten;

void unget( void )
{
	if( have_ungotten ) {
		flag = 1;
		drop();
		return;
	}
	have_ungotten = 1;
	ungotten = *sp++;
	flag = 0;
}

void get( void )
{
    static char linebuf[256], *lbp = linebuf;
    static int nlb = 0;
    int n;
    wchar_t wc;
	
	if( have_ungotten ) {
		flag = 1;
		*--sp = ungotten;
		have_ungotten = 0;
		return;
	}
    
    if( !nlb ) {
        if( NULL == fgets( linebuf, sizeof(linebuf)/sizeof(char), in )) {
            *--sp = flag = 0;
            return;
        }
        lbp = linebuf;
        nlb = strlen( linebuf );
    }
    
    n = mbtowc( &wc, lbp, nlb );
    
    if( n <= 0 ) {  /* might need more bytes */
        for( n = 0; n < nlb; n += 1 ) linebuf[n] = lbp[n]; /* shift what's there over */
        
        if( NULL == fgets( linebuf+nlb, sizeof(linebuf)/sizeof(char)-nlb, in )) {
            *--sp = flag = 0;
            return;
        }
        lbp = linebuf;
        nlb = strlen( linebuf );
        n = mbtowc( &wc, lbp, nlb );	/* try again */
        if( n <= 0 ) {			/* give up */
            *--sp = flag = 0;
            return;
        }
    }
    
    *--sp = wc;
    nlb -= n;
    lbp += n;
	flag = 1;
}

/*
 * skip_space skips over whitespace in input.
 * If it sets the flag to true, the next get() will obtain a non-space character.
 * Otherwise, a false flag indicates that it encountered end of line, end of file, or an I/O error.
 */

void skip_space( void )
{
	for(;;){
		get();
		if( !flag ) { drop(); return; }
		if( !iswspace( *sp )) {
			unget();
			flag = 1;
			return;
		}
		if( *sp++ == '\n' ) {
			flag = 0;
			return;
		}
	}
}

/*
 * Skip characters up to and including the next newline.
 * Yields false on EOF or error, otherwise true.
 */

void skip_to_nl( void )
{
	for(get(); flag && *sp != '\n'; get()) drop();
	drop();
}

void get_token( void )
{
	int n;
	
	skip_space();
	if( !flag ) return;
	
	for( n = 0;; n += 1) {
		get();
		if( !flag ) break;
		if( iswspace( *sp ) ) break;
		constop[ D_DATA + 1 + n ] = *sp++;
	}
	constop[ D_DATA ] = n;
	if( flag ) unget(); else drop();
	flag = 1;
}
    
static void print_string( cell * s )	/* handy for debugging */
{
	int len = *s++;
	
	while( len-- ) { *--sp = *s++; put(); }
}

void put_string( void ) { print_string( (cell *)(intptr_t)*sp++ ); }

/*
 * fixate_literal_string() takes a temporary string at the end of the
 * string dictionary. It checks to see if the string is already present
 * in the dictionary. If not, it makes it into a new definition. In either
 * case, it pushes the permanent address of the string on the stack.
 */

void fixate_literal_string( void )
{
	cell *tmp = constop + D_DATA;
	cell *ent;
	for( ent = constlast; ent ; ent = (cell *) (intptr_t)ent[ D_PREV ] ) {	/* look for previous definition */
		cell *t = tmp;
		cell *e = ent + D_DATA;
		cell n = *t++;
		if( ent[ D_PRIM ] != (cell) stringconstant ) continue;  /* not a string */
		if( n != *e++ ) continue;					/* lengths don't match */
		while( n-- ) if( *t++ != *e++ ) break;   /* strings don't match */
		if( n < 0 ) {							/* match! */
			*--sp = (intptr_t)(ent + D_DATA);		/* we have a match, return it */
			return;
		}
	}
	/* If we get to here, nothing matched. Make new entry of the temp.
	 * Note that the string itself is already in the right place:
	 * we just have to build a literal dictionary entry around it,
	 * and adjust constop and constlast.
	 */
	constop[ D_PRIM ] = (cell) stringconstant;
	constop[ D_PREV ] = (cell)(intptr_t) constlast;
	constop[ D_PRECEDENCE ] = DP_COMPILED;
	constlast = constop;
	constop += D_DATA + 1 + *tmp;
	*--sp = (intptr_t) (tmp);
}

/* get the next word name in the input stream */

void get_name( void )
{
	get_token();
	if( flag ) fixate_literal_string();
	else *--sp = 0;		/* placeholder for string pointer on failure */
}

void make_def( void )		/* expects a pointer to the name literal on the stack */
{
	deftop[ D_PRIM ] = (cell) noop;			/* mostly harmless default */

	deftop[ D_PRECEDENCE ] = DP_COMPILED;   /* default */
	deftop[ D_NAME ] = (intptr_t) *sp++;				/* grab name from stack */
	deftop[ D_PREV ] = (intptr_t) deflast;
	deflast = deftop;						/* now we're it */
	deftop = deftop + D_BODY;				/* initially, body is empty */
}

void interpret( void ) { 
	*--rsp = (intptr_t) lc; lc = ((cell *) (intptr_t) *lc) + D_BODY - 1; 
}

void fixate_colon_def( void )
{
	deflast[ D_PRIM ] = (cell) interpret;
	deftop = (cell *) (intptr_t) cptr;
}

void named_constant( void ) { *--sp = ((cell *)(intptr_t)*lc)[D_BODY]; }

void make_named_constant( void )	/* value, pointer to name literal on stack */
{
	make_def();
	deflast[ D_PRIM ] = (cell) named_constant;
	*deftop++ = *sp++;
}

void make_array( void )  /* size, pointer to name literal on stack */
{
	cell n;
	make_def();
	deflast[ D_PRIM ] = (cell) variable;
	for( n = *sp++; n > 0; n -= 1 ) *deftop++ = 0;
}

void make_constant( void )  /* expects the constant value on the stack */
{
	cell *ent;
	
	for( ent = constlast; ent ; ent = (cell *)(intptr_t) ent[ D_PREV ] ) {	/* look for previous definition */
		if( ent[D_PRIM] == (cell) constant && ent[D_DATA] == *sp ) {	/* got a match */
			*sp = (intptr_t) ent;											/* return it */
			return;
		}
	}
	constop[ D_PRIM ] = (intptr_t) constant;	/* no match, build new entry */
	constop[ D_PREV ] = (intptr_t) constlast;
	constop[ D_PRECEDENCE ] = DP_COMPILED;
	constop[ D_DATA ] = *sp;
	constlast = constop;
	constop += D_DATA + 1;
	*sp = (intptr_t) constlast;					/* return it */
}


void find( void )
{
	cell *def;
	for( def = deflast; def; def = (cell *)(intptr_t) def[ D_PREV ] ) {
		if( def[ D_NAME ] == *sp ) {	/* found it */
			*sp = (intptr_t) def;
			flag = 1;
			return;
		}
	}
	*sp = 0;
	flag = 0;
}

void compile_to_def( void )
{
	cptr = (intptr_t) deftop;
}

void compile_to_buffer( void )
{
	cptr = cbuf;
}

void putd( void ) { fprintf( out, "%lld", *sp++); }
void putx( void ) { fprintf( out, "%llx", *sp++); }
void putf( void ) { fprintf( out, "%lg", *(fcell *)sp++ ); }

void depth( void ) { cell w = stack - sp + STACK_DIM; *--sp = w; }

void nl( void ) { fprintf( out, "\n" ); }
void space( void ) { fprintf( out, " " ); }

/*
 * I/O Control
 */

void get_stdin( void ) { *--sp = (intptr_t) stdin; }
void get_stdout( void ) { *--sp = (intptr_t) stdout; }
void get_stderr( void ) { *--sp = (intptr_t) stderr; }
void get_in( void ) { *--sp = (intptr_t) in; }
void put_in( void ) { in = (FILE *)(intptr_t)*sp++; }
void get_out( void ) { *--sp = (intptr_t) out; }
void put_out( void ) { out = (FILE *)(intptr_t)*sp++; }
void flush( void ) { fflush( out ); }
void eofq( void ) { flag = feof( in ); }
void ioerrq( void ) { flag = ferror( (FILE *)(intptr_t)*sp++ ); }
void closef( void ) { flag = !fclose( (FILE *)(intptr_t)*sp++ ); }

/*
 * System interface
 */

void delay( void ) { usleep( *sp++ ); }
void sysexit( void ) { exit( *sp++ ); }
void now( void )
{
	struct timeval tv;
	const cell million = 1000000;
	
	(void) gettimeofday( &tv, NULL );
	*--sp = tv.tv_usec + million * (cell) tv.tv_sec;
}

/*
 * fussy interpreter for debugging
 */

static int befussy;

void fussy( void ) 
{
	befussy = 1;
	while( befussy ) {
		if( sp > stack + STACK_DIM ) {
			fprintf( stderr, "Stack underflow!\n" );
			abort();
		}
		if( sp < stack ) {
			fprintf( stderr, "Stack overflow!\n" );
			abort();
		}
		if( rsp > rstack + RSTACK_DIM ) {
			fprintf( stderr, "Return stack underflow!\n" );
			abort();
		}
		if( rsp < rstack ) {
			fprintf( stderr, "Return stack overflow!\n" );
			abort();
		}
		(*(prim *)(intptr_t)*(cell *)(intptr_t)(*++lc))();
	}
}

void fast( void ) { befussy = 0; }

void ifelse( void )
{
	cell *l = ++lc;
	if( !flag ) return;		/* just skip next word */
	(*(prim *)(intptr_t)*(cell *)(intptr_t)(*l))();  /* interpret next word */
	if( lc != l ) {			/* some sort of branch happened */
		if( interpret == (prim *)(intptr_t)*(cell *)(intptr_t)(*l)) {		/* if the branch was a call */
			*rsp = (cell)(intptr_t)((cell *)(intptr_t)(*rsp) + 1);			/* do the skip a level up */
		}   /* if it wasn't a call, don't worry, we're done */
		return;
	}
	lc += 1;	/* skip the else word */
}

/* argc yields n */
void argc( void ) { *--sp = gargc; }

/* n arg yields sp
 * sp is zero and flag is false for n out of range.
 */
void arg( void )
{
	unsigned n = *sp++;
	char *ap;
	cell nc = 0;
	cell *cp = constop + D_DATA + 1;
	
	if( n >= gargc ) {
		*--sp = flag = 0;
		return;
	}
	ap = gargv[ n ];
	while( *ap ) {			/* copy the string */
		*cp++ = *ap++;
		nc += 1;
	}
	constop[ D_DATA ] = nc;		/* put the count where it belongs */
	fixate_literal_string();
	flag = 1;
}


/*
 * $Log: primitives.c,v $
 * Revision 1.2  2006/09/09 15:58:13  jpd
 * Release 0.3 with license and copyright notices
 *
 * Revision 1.1  2005/07/03 15:37:19  jpd
 * *** empty log message ***
 *
 * Revision 1.11  2005/03/29 22:12:46  jpd
 * Added @@+ and then. Fixed naming process so heritage is possible.
 *
 * Revision 1.10  2005/03/02 21:59:52  jpd
 * Need to include sys/time.h on MacOSX.
 * Documented actual preformance of usec and now.
 *
 * Revision 1.9  2005/03/02 21:32:09  jpd
 * Added now to read system clock.
 *
 * Revision 1.8  2005/03/02 02:10:53  jpd
 * Argument processing.
 * First arg is a file to load.
 *
 * Revision 1.7  2004/11/02 17:06:48  jpd
 * Added ifelse
 *
 * Revision 1.6  2004/10/09 16:06:34  jpd
 * Fixed a bunch of hanging issues.
 * Now have a strict interpreter for debugging.
 * Bad compilation no longer leaves a def behind.
 * Interrupts and aborts work.
 * error( and )error for messages to stdout.
 *
 * Revision 1.5  2004/09/24 21:47:57  jpd
 * Implemented iterate and helpers.
 * Can now check for COMEDI.
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


