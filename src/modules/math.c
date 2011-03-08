/* $Id: modmath.c,v 1.2 2006/09/09 16:12:44 jpd Exp $ */

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
#include <math.h>
#include <errno.h>
#include <stdio.h>

/*
 * Library Functions
 */

void facos( void ) { *(fcell*)sp = acos(*(fcell*)sp); }
void facosh( void ) { *(fcell*)sp = acosh(*(fcell*)sp); }
void fasin( void ) { *(fcell*)sp = asin(*(fcell*)sp); }
void fasinh( void ) { *(fcell*)sp = asin(*(fcell*)sp); }
void fatan( void ) { *(fcell*)sp = atan(*(fcell*)sp); }
void fatanh( void ) { *(fcell*)sp = atanh(*(fcell*)sp); }
void fatan2( void ) { fcell w = *(fcell*)sp++; *(fcell*)sp = atan2(*(fcell*)sp, w); }
void fcbrt( void ) { *(fcell*)sp = cbrt(*(fcell*)sp); }
void fceil( void ) { *(fcell*)sp = ceil(*(fcell*)sp); }
void fcos( void ) { *(fcell*)sp = cos(*(fcell*)sp); }
void fcosh( void ) { *(fcell*)sp = cosh(*(fcell*)sp); }
void fexp( void ) { *(fcell*)sp = exp(*(fcell*)sp); }
void fexpm1( void ) { *(fcell*)sp = expm1(*(fcell*)sp); }
void ffloor( void ) { *(fcell*)sp = floor(*(fcell*)sp); }
void ffrexp( void ) { int i; *(fcell*)sp = frexp(*(fcell*)sp, &i); *--sp=(cell)i; }
void filogb( void ) { *sp = (cell) ilogb(*(fcell*)sp); }
void fldexp( void ) { int i = (int)*sp++; *(fcell*)sp = ldexp(*(fcell*)sp, i); *--sp=(cell)i; }
void flog( void ) { *(fcell*)sp = log(*(fcell*)sp); }
void flog10( void ) { *(fcell*)sp = log10(*(fcell*)sp); }
void flog1p( void ) { *(fcell*)sp = log1p(*(fcell*)sp); }
void flogb( void ) { *(fcell*)sp = logb(*(fcell*)sp); }
void ffmodf( void ) { double e; *(fcell*)sp = modf(*(fcell*)sp, &e); *--sp = (fcell) e; }
void fnextafter( void ) { fcell w = *(fcell*)sp++; *(fcell*)sp = nextafter(*(fcell*)sp, w); }
void fremainder( void ) { fcell w = *(fcell*)sp++; *(fcell*)sp = remainder(*(fcell*)sp, w); }
void frint( void ) { *(fcell*)sp = rint(*(fcell*)sp); }
void fpow( void ) { fcell w = *(fcell*)sp++; *(fcell*)sp = pow(*(fcell*)sp,w);}
void fsin( void ) { *(fcell*)sp = sin(*(fcell*)sp); }
void fsinh( void ) { *(fcell*)sp = sinh(*(fcell*)sp); }
void fsqrt( void ) { *(fcell*)sp = sqrt(*(fcell*)sp); }
void ftan( void ) { *(fcell*)sp = tan(*(fcell*)sp); }
void ftanh( void ) { *(fcell*)sp = tanh(*(fcell*)sp); }
void ferf( void ) { *(fcell*)sp = erf(*(fcell*)sp); }
void ferfc( void ) { *(fcell*)sp = erfc(*(fcell*)sp); }
void fhypot( void ) { fcell y = *(fcell*)sp++; *(fcell*)sp = hypot(*(fcell*)sp,y);}
void fj0( void ) { *(fcell*)sp = j0(*(fcell*)sp); }
void fj1( void ) { *(fcell*)sp = j1(*(fcell*)sp); }
void fjn( void ) { fcell e = *(fcell*)sp++; *(fcell*)sp = jn((int)*sp, e); }
void flgamma( void ) { fcell e; e = lgamma(*(fcell*)sp); *sp = (cell) signgam; *(fcell*)--sp = e; } /* We return the sign too */
void fy0( void ) { *(fcell*)sp = y0(*(fcell*)sp); }
void fy1( void ) { *(fcell*)sp = y1(*(fcell*)sp); }
void fyn( void ) { fcell e = *(fcell*)sp++; *(fcell*)sp = yn((int)*sp, e); }
void fisnan( void ) { flag = isnan(*(fcell*)sp); }
void geterrno( void ) { *--sp = errno; }
void puterrno( void ) { errno = *sp++; }
	    	
/*
 * Initializer Function
 */

//void _init(void) {
void __attribute__((constructor)) mod_init(void) {
	build_primitive(facos, "acos");
	build_primitive(facosh, "acosh");
	build_primitive(fasin, "asin");
	build_primitive(fasinh, "asinh");
	build_primitive(fatan, "atan");
	build_primitive(fatanh, "atanh");
	build_primitive(fatan2, "atan2");
	build_primitive(fcbrt, "cbrt");
	build_primitive(fceil, "ceil");
	build_primitive(fcos, "cos");
	build_primitive(fcosh, "cosh");
	build_primitive(fexp, "exp");
	build_primitive(fexpm1, "expm1");
	build_primitive(ffloor, "floor");
	build_primitive(ffrexp, "frexp");
	build_primitive(filogb, "ilogb");
	build_primitive(fldexp, "ldexp");
	build_primitive(flog, "log");
	build_primitive(flog10, "log10");
	build_primitive(flog1p, "log1p");
	build_primitive(flogb, "logb");
	build_primitive(ffmodf, "modf");
	build_primitive(fpow, "pow");
	build_primitive(fsin, "sin");
	build_primitive(fsinh, "sinh");
	build_primitive(fsqrt, "sqrt");
	build_primitive(ftan, "tan");
	build_primitive(ftanh, "tanh");
	build_primitive(ferf, "erf");
	build_primitive(ferfc, "erfc");
	build_primitive(flgamma, "gamma");
	build_primitive(fj0, "j0");
	build_primitive(fj1, "j1");
	build_primitive(fjn, "jn");
	build_primitive(flgamma, "lgamma");
	build_primitive(fisnan, "nan?");
	build_primitive(fnextafter, "nextafter");
	build_primitive(fremainder, "remainder");
	build_primitive(frint, "rint");
	build_primitive(puterrno, ">errno");
	build_primitive(geterrno, "errno>");

	/* Constants */
	build_named_constant(M_E, "M_E");
	build_named_constant(M_LOG2E, "M_LOG2E");
	build_named_constant(M_LOG10E, "M_LOG10E");
	build_named_constant(M_LN2, "M_LN2");
	build_named_constant(M_LN10, "M_LN10");
	build_named_constant(M_PI, "M_PI");
	build_named_constant(M_PI_2, "M_PI_2");
	build_named_constant(M_PI_4, "M_PI_4");
	build_named_constant(M_1_PI, "M_1_PI");
	build_named_constant(M_2_PI, "M_2_PI");
	build_named_constant(M_2_SQRTPI, "M_2_SQRTPI");
	build_named_constant(M_SQRT2, "M_SQRT2");
	build_named_constant(M_SQRT1_2, "M_SQRT1_2");
	//build_named_constant(MAXFLOAT, "MAXFLOAT");

	build_named_constant(HUGE_VAL, "HUGE_VAL");
	build_named_constant(EDOM, "EDOM");
	build_named_constant(ERANGE, "ERANGE");

}

/* Test function for LSE Module */
int lse_mod_test(void) { return 1; }
