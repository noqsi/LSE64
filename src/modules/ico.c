#include "lse64.h"
#include "config.h"
#include <pigpio.h>
#include <signal.h>

extern void interrupt( int sig );	// LSE64's interrupt handler

// Right shift for positive n, left for negative

#define SHIFT(x,n) (((n)>0)?(x)>>(n):(x)<<(-(n)))

#define MOVE_BIT(x,s,d) SHIFT(((x)&(1<<(s))),((s)-(d)))

// Bit	GPIO
// P7	18
// P6	22
// P5	23
// P4	10
// P3	9
// P2	7
// P1	19
// P0	16

static unsigned bits2gp( unsigned x ) {

	return
	MOVE_BIT( x, 7, 18 ) |
	MOVE_BIT( x, 6, 22 ) |
	MOVE_BIT( x, 5, 23 ) |
	MOVE_BIT( x, 4, 10 ) |
	MOVE_BIT( x, 3, 9 ) |
	MOVE_BIT( x, 2, 7 ) |
	MOVE_BIT( x, 1, 19 ) |
	MOVE_BIT( x, 0, 16 );

}

static unsigned gp2bits( unsigned x ) {

	return
	MOVE_BIT( x, 18, 7 ) |
	MOVE_BIT( x, 22, 6 ) |
	MOVE_BIT( x, 23, 5 ) |
	MOVE_BIT( x, 10, 4 ) |
	MOVE_BIT( x, 9, 3 ) |
	MOVE_BIT( x, 7, 2 ) |
	MOVE_BIT( x, 19, 1 ) |
	MOVE_BIT( x, 16, 0 );

}

#define PPDIR 20	// direction bit. 1 = read, 0 = write
#define PPCLK 21	// Pulse high for transfer
#define PPRSEL 17	// Register select bit

static void set_port_mode( unsigned mode ) {
	static unsigned current_mode = 9999;	// force init first time
	
	if( mode == current_mode ) return;
	
	if( mode == PI_OUTPUT ) gpioWrite( PPDIR, 0 );
	
	gpioSetMode( 18, mode );
	gpioSetMode( 22, mode );
	gpioSetMode( 23, mode );
	gpioSetMode( 10, mode );
	gpioSetMode( 9, mode );
	gpioSetMode( 7, mode );
	gpioSetMode( 19, mode );
	gpioSetMode( 16, mode );
	
	if( mode == PI_INPUT ) gpioWrite( PPDIR, 1);
	
	current_mode = mode;
}

// LSE word implementations

// rs is register select (0 or 1), d is 8 bit contents


// rs ico-get yields d

static void ico_get( void ) {
	
	set_port_mode( PI_INPUT );
	
	gpioWrite( PPRSEL, *sp );
	gpioWrite( PPCLK, 1 );
	*sp = gp2bits( gpioRead_Bits_0_31() );
	gpioWrite( PPCLK, 0 );
}

// d rs ico-put yields nothing

static void ico_put( void ) {
	
	set_port_mode( PI_OUTPUT );
	
	gpioWrite( PPRSEL, *sp++ );
	gpioWrite_Bits_0_31_Clear( bits2gp( ~*sp ));
	gpioWrite_Bits_0_31_Set( bits2gp( *sp++ ));
	gpioWrite( PPCLK, 1 );
	gpioWrite( PPCLK, 0 );
}

// rs ico-poll yields d
// Reads data passively without a clock

static void ico_poll( void ) {

	set_port_mode( PI_INPUT );
	
	gpioWrite( PPRSEL, *sp );
	*sp = gp2bits( gpioRead_Bits_0_31() );
}

static int pigpio_ok;

void __attribute__((constructor)) mod_init(void) {
	
	if (gpioInitialise() < 0)
	{
		pigpio_ok = 0;
	}
	else
	{
		pigpio_ok = 1;
	}
	
        build_primitive( ico_get, "ico-get" );
        build_primitive( ico_put, "ico-put" );
        build_primitive( ico_poll, "ico-poll" );

	gpioWrite( PPCLK, 0 );		// inactive
	gpioWrite( PPDIR, 0 );		// tell FPGA not to drive
        gpioSetMode( PPCLK, PI_OUTPUT );	// Make sure
	gpioWrite( PPCLK, 0 );
        gpioSetMode( PPDIR, PI_OUTPUT );
	gpioWrite( PPDIR, 0 );
	gpioSetMode( PPRSEL, PI_OUTPUT);
	
// pigpio wraps signal() and overrides existing handlers, so
// restore the ones LSE64 handles.
	
	gpioSetSignalFunc( SIGINT, interrupt );
	gpioSetSignalFunc( SIGABRT, interrupt );

}
	
/* Test function for LSE Module */
int lse_mod_test(void) { return pigpio_ok; }
