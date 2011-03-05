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
#include <dlfcn.h>
#include <sys/param.h>
#include "lse64.h"

void loadmod(void) {
	char static modpath[MAXPATHLEN+1];
	int (*lse_mod_test)(void) ;
	char * modname = cstring(0,0);
#ifdef PKGLIBDIR
	void *modp = dlopen(strcat(strcat(strcpy(modpath, PKGLIBDIR "/mod"),modname),".so"), RTLD_LAZY);
	if ( ! modp ) modp = dlopen(strcat(strcat(strcpy(modpath, PKGLIBDIR "/mod"),modname),".0"), RTLD_LAZY);
#else
	void *modp = dlopen(strcat(strcat(strcpy(modpath, "../modules/.libs/mod"),modname),".so"), RTLD_LAZY);
	if ( ! modp ) modp =  dlopen(strcat(strcat(strcpy(modpath, "../modules/.libs/mod"),modname),".0"), RTLD_LAZY);
#endif
	flag = 0;
	if (modp == NULL) return;
	if ( !(lse_mod_test = (int (*)(void)) dlsym(modp, "lse_mod_test")) ) return;
	flag = lse_mod_test();
}

void moderror(void) {
	printf("%s",dlerror());
}
