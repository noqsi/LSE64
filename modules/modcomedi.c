/* $Id: modcomedi.c,v 1.4 2006/09/09 16:12:43 jpd Exp $ */

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
#include "comedilib.h"

static comedi_t *devices[16];

static comedi_t *lookup_dev( unsigned dev )
{
	static char devname[] = "/dev/comedixx";
	
	if( dev >= 16 ) {
		fprintf( stderr, "Comedi device number %d too large.\n", dev );
		return NULL;
	}
	if( !devices[dev] ) {
		sprintf( devname, "/dev/comedi%u", dev );
		devices[dev] = comedi_open( devname );
		if( !devices[dev] ) comedi_perror( devname );
	}
	return devices[dev];
}

/*
 * data aref range chan subdev dev !daq yields nothing
 */

static void data_write( void )
{
	comedi_t *dev = lookup_dev( *sp++ );
	unsigned int subdev = *sp++;
	unsigned int chan = *sp++;
	unsigned int range = *sp++;
	unsigned int aref = *sp++;
	lsampl_t data = *sp++;
	int c = comedi_data_write( dev, subdev, chan, range, aref, data );
	if( c < 1 ) comedi_perror( "!daq " );
}

/*
 * data chan subdev dev !dio yields nothing
 */

static void dio_write( void )
{
	comedi_t *dev = lookup_dev( *sp++ );
	unsigned int subdev = *sp++;
	unsigned int chan = *sp++;
	unsigned int data = *sp++;
	int c = comedi_dio_write( dev, subdev, chan, data );
	if( c < 1 ) comedi_perror( "!dio " );
}

/*
 * data chan subdev dev <dio> yields nothing
 */

static void dio_config( void )
{
	comedi_t *dev = lookup_dev( *sp++ );
	unsigned int subdev = *sp++;
	unsigned int chan = *sp++;
	unsigned int data = *sp++;
	int c = comedi_dio_config( dev, subdev, chan, data );
	if( c < 1 ) comedi_perror( "<dio> " );
}

/*
 * aref range chan subdev dev @daq yields data
 */

static void data_read( void )
{
	comedi_t *dev = lookup_dev( *sp++ );
	unsigned int subdev = *sp++;
	unsigned int chan = *sp++;
	unsigned int range = *sp++;
	unsigned int aref = *sp++;
	lsampl_t data;
	int c = comedi_data_read( dev, subdev, chan, range, aref, &data );
	if( c < 1 ) comedi_perror( "@daq " );
	*--sp = data;
}

/*
 * chan subdev dev @dio yields data
 */

static void dio_read( void )
{
	comedi_t *dev = lookup_dev( *sp++ );
	unsigned int subdev = *sp++;
	unsigned int chan = *sp++;
	unsigned int data;
	int c = comedi_dio_read( dev, subdev, chan, &data );
	if( c < 1 ) comedi_perror( "@dio " );
	*--sp = data;
}

/*
 * data mask subdev dev !@dio yields newdata
 */

static void dio_bitfield( void )
{
	comedi_t *dev = lookup_dev( *sp++ );
	unsigned int subdev = *sp++;
	unsigned int mask = *sp++;
	unsigned int data = *sp++;
	int c = comedi_dio_bitfield( dev, subdev, mask, &data );
	if( c < 0 ) comedi_perror( "!@dio " );
	*--sp = data;
}


/*
 * Initializer Function
 */

//void _init(void) {
void __attribute__((constructor)) mod_init(void) {
	build_primitive( data_write, "!daq" );
	build_primitive( data_read, "@daq" );
	build_primitive( dio_write, "!dio" );
	build_primitive( dio_read, "@dio" );
	build_primitive( dio_config, "<dio>" );
	build_primitive( dio_bitfield, "!@dio" );
	build_named_constant( AREF_GROUND, "AREF_GROUND" );
	build_named_constant( AREF_COMMON, "AREF_COMMON" );
	build_named_constant( AREF_DIFF, "AREF_DIFF" );
	build_named_constant( AREF_OTHER, "AREF_OTHER" );
	build_named_constant( COMEDI_INPUT, "COMEDI_INPUT" );
	build_named_constant( COMEDI_OUTPUT, "COMEDI_OUTPUT" );

}

/* Test function for LSE Module */
int lse_mod_test(void) { return 1; }

/*
 * $Log: modcomedi.c,v $
 * Revision 1.4  2006/09/09 16:12:43  jpd
 * Release 0.3 with copyright and license notices
 *
 * Revision 1.3  2005/08/08 23:26:35  jpd
 * DIO support.
 *
 * Revision 1.2  2005/07/08 16:02:21  jpd
 * Change names of !dac and @adc to !daq and @daq to more accurately
 * reflect their function. Added more to docs.
 *
 * Revision 1.1  2005/07/03 15:37:19  jpd
 * *** empty log message ***
 *
 * Revision 1.6  2004/09/25 00:06:18  jpd
 * Fix conditional comedi binding, minor bug in \
 *
 * Revision 1.5  2004/09/24 21:47:57  jpd
 * Implemented iterate and helpers.
 * Can now check for COMEDI.
 *
 * Revision 1.4  2004/09/24 19:54:07  jpd
 * Now using automake/autoconf.
 *
 * Revision 1.3  2004/09/20 23:29:50  jpd
 * Analog output works, at least at software level.
 *
 * Revision 1.2  2004/09/20 16:22:13  jpd
 * A bit more added to the comedi code.
 *
 */
