/* $Id: lse64.h,v 1.2 2006/09/09 15:58:13 jpd Exp $ */

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


#include <stdint.h>
#include <stdio.h>
#include "config.h"

typedef int64_t cell;
typedef double fcell;

extern cell	*stack,		/* base of stack */
        *sp,		/* top active item */
        *rstack,	/* base of return stack */
        *rsp,		/* top active return stack item */
        *lc,		/* interpreter's location counter */
		*deftop,	/* top of definition dictionary */
		*deflast,   /* last entry in definition dictionary */
		*constop,   /* top of constant dictionary */
		*constlast; /* last entry of constant dictionary */
		
extern cell flag;   /* the flag register */

extern cell cptr;		/* compile pointer */
extern cell cbuf;		/* loc of compile buffer */

extern FILE *in, *out;

extern cell gargc;
extern char **gargv;

/*
 * Offsets of fields within a dictionary entry.
 */
 
#define		D_PRIM  0			/* pointer to primitive func */
#define		D_PREV  1			/* pointer to previous dictionary entry */
#define		D_PRECEDENCE	2   /* precedence of this word in compilation */
#define		D_DATA  3			/* data field */

/*
 * Offsets of additional fields within definitions.
 */

#define		D_NAME			(D_DATA+0)  /* pointer to name */
#define		D_BODY			(D_DATA+1)  /* The contents */

/*
 *  Precedence values
 */

#define		DP_COMPILED		0   /* Normal compiled word */
#define		DP_IMMEDIATE	1   /* Execute instead of compiling */
#define		DP_SOONER		2   /* Execute before compiling previous word */

/*
 * These should eventually be configured based on host resources
 */
 
#define	STACK_DIM	1024
#define RSTACK_DIM	1024
#define	DEFMEM_DIM		(8*1024*1024)
#define CONSTMEM_DIM	(1024*1024)
#define CBUF_DIM	1024

/*
 * Primitive functions
 */

typedef void prim(void);

prim
fetch, store, fetchp, storep, append, stringconstant, constant, variable, add,
subtract, multiply, divide, mod, neg, fadd, fsubtract, fmultiply,
fdivide, fmodd, fneg, and, or, xor, not, dupp, swap, drop, pick, rot,
equal, greater, less, equal0, greater0, less0, fequal, fgreater,
fless, fequal0, fgreater0, fless0, setflag, clearflag, exitrue,
exitfalse, exitword, iftrue, iffalse, repeat, repeatrue, s2r, r2s,
rcopy, rdrop, put, unget, get, skip_space, skip_to_nl, get_token,
fixate_literal_string, get_name, make_def, interpret, fixate_colon_def,
make_named_constant, make_array, make_constant, find, compile_to_def,
compile_to_buffer, literal, noop, cells, putd, putx, putf, depth, nl, space,
put_string, get_stdin, get_stdout, get_stderr, get_in, put_in, get_out,
put_out, flush, eofq, ioerrq, ToNumber, HexNumber, delay, sysexit, ffloat, fix,
openf, closef, iterate_init, iterate, anditerate, count, fussy, fast, ifelse,
argc, arg, now, scan, jfalse, sprint1, openfd, loadmod, moderror;

void build_primitive( prim p, char *name );
void build_named_constant( cell c, char *name );

/* strings.c stuff */
char *cstring( char *b, int n );

/*
 * $Log: lse64.h,v $
 * Revision 1.2  2006/09/09 15:58:13  jpd
 * Release 0.3 with license and copyright notices
 *
 * Revision 1.1  2005/07/03 15:37:19  jpd
 * *** empty log message ***
 *
 * Revision 1.13  2005/04/21 17:16:15  jpd
 * Output redirection.
 *
 * Revision 1.12  2005/04/11 21:10:28  jpd
 * sprint1 to convert one arg.
 *
 * Revision 1.11  2005/03/29 22:12:46  jpd
 * Added @@+ and then. Fixed naming process so heritage is possible.
 *
 * Revision 1.10  2005/03/02 21:32:09  jpd
 * Added now to read system clock.
 *
 * Revision 1.9  2005/03/02 02:10:53  jpd
 * Argument processing.
 * First arg is a file to load.
 *
 * Revision 1.8  2004/11/02 17:06:48  jpd
 * Added ifelse
 *
 * Revision 1.7  2004/10/09 16:06:34  jpd
 * Fixed a bunch of hanging issues.
 * Now have a strict interpreter for debugging.
 * Bad compilation no longer leaves a def behind.
 * Interrupts and aborts work.
 * error( and )error for messages to stdout.
 *
 * Revision 1.6  2004/09/24 21:47:57  jpd
 * Implemented iterate and helpers.
 * Can now check for COMEDI.
 *
 * Revision 1.5  2004/09/23 15:06:38  jpd
 * Added file operations.
 *
 * Revision 1.4  2004/09/20 23:29:50  jpd
 * Analog output works, at least at software level.
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

