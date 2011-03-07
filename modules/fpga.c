#include "lse64.h"
#include "config.h"
#include <sys/mman.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <stdlib.h>

// Mapping of FPGA in the Armadeus

#define FPGA_ADDRESS 0xD6000000

static volatile void *regp;

// Get a pointer to the registers the FPGA firmware implements

static void *locate_regs( void )
{
	void *ra;
	int fd = open("/dev/mem", O_RDWR|O_SYNC);
	if (fd < 0) {
		perror("/dev/mem");
		exit( EXIT_FAILURE );
	}


	ra = mmap(0, 8192, PROT_READ|PROT_WRITE, MAP_SHARED, 
		fd, FPGA_ADDRESS);
	if (ra == MAP_FAILED) {
		perror("mmap");
		exit( EXIT_FAILURE );
	}
	
	return ra;
}


// FPGA register access

static void getreg( void )
{
	*sp = * (volatile uint16_t *) (regp + *sp);
}


static void putreg( void )
{
	cell reg = *sp++;
	
	* (volatile uint16_t *) (regp + reg) = *sp++;
}


/* My Initializer */
//void _init( void )
void __attribute__((constructor)) mod_init(void) {
        build_primitive( getreg, "@fpga" );
        build_primitive( putreg, "!fpga" );
        regp = locate_regs();
}
	
/* Test function for LSE Module */
int lse_mod_test(void) { return 1; }
