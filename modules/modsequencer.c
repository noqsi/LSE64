/* $Id: modsequencer.c,v 1.3 2006/09/09 16:12:44 jpd Exp $ */

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
#include "config.h"
#include <linux/lp.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static int port = -1;

static void open_lp( void )
{
	if( port > 0 ) return;	/* already open */
        port = open( "/dev/lp0", O_WRONLY, 0 );
        if( port < 0 ) {
                perror( "Can't open sequencer (/dev/lp0)" );
                exit( 1 );
        }
}

static void reset_seq( void )
{
	open_lp();
        if( ioctl( port, LPRESET, 0 )) {
                perror("Can't reset sequencer (/dev/lp0)");
                exit( 1 );
        }
}

static void putseq( void )
{
	char c = *sp++;
	if( 1 != write( port, &c, 1 )) {
		perror( "Can't write to sequencer (/dev/lp0)" );
		exit( 1 );
	}
}

static void check_busy( void )
{
        int s;
        if( ioctl( port, LPGETSTATUS, &s )) {
                perror( "Can't get sequencer (/dev/lp0) interface status"
                        );
        exit( 1 );
        }
        if( s & LP_PBUSY ) flag = 0;    /* inverted logic */
        else flag = 1;
}

/* My Initializer */
//void _init( void )
void __attribute__((constructor)) mod_init(void) {
        build_primitive( reset_seq, "reset>seq" );
        build_primitive( putseq, "8>seq" );
        build_primitive( check_busy, "seqbusy?" );
}
	
/* Test function for LSE Module */
int lse_mod_test(void) { return 1; }

/*
 * $Log: modsequencer.c,v $
 * Revision 1.3  2006/09/09 16:12:44  jpd
 * Release 0.3 with copyright and license notices
 *
 * Revision 1.2  2005/07/07 19:51:05  jpd
 * Now have seqbusy? in modsequencer.c
 * Small pixel stuff now works with modules.
 *
 * Revision 1.1  2005/07/03 15:37:19  jpd
 * *** empty log message ***
 *
 * Revision 1.3  2004/11/14 18:12:46  jpd
 * Fixed up some names.
 *
 * Revision 1.2  2004/11/13 00:05:21  jpd
 * Sequencer wrapper compiles on Linux.
 *
 * Revision 1.1  2004/11/12 21:53:02  jpd
 * Added support to drive sequencer through /dev/lp0.
 *
 */
