/* $Id: loadmod.c,v 1.2 2006/09/09 15:58:13 jpd Exp $ */

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

#include <stdio.h>
#include <string.h>
#include <ltdl.h>
#include <sys/param.h>
#include "lse64.h"

void loadmod(void) {
	int (*lse_mod_test)(void) ;
	char * modname = cstring(0,0);
        lt_dlhandle modp;

        // Initialize libtool
        lt_dlinit(); 
        // Add local modules to search path
        lt_dladdsearchdir("./modules/");

#ifdef PKGLIBDIR
        // Add global modules to search path
        lt_dladdsearchdir(PKGLIBDIR);
#endif
        // try to load the module
        modp = lt_dlopenext(modname);

        // If we fail, set the flag to 0 and exit
	flag = 0;
	if ( ! modp ) return;

        // Else make sure the module is sane
	if ( !(lse_mod_test = (int (*)(void)) lt_dlsym(modp, "lse_mod_test")) ) return;
        // If all is well, then the test value 
        // should return 1, so se the flag to this and 
	flag = lse_mod_test();
}

void moderror(void) {
	printf("%s",lt_dlerror());
}
