/*
 *  Mathlib : A C Library of Special Functions
 *  Copyright (C) 1998 Ross Ihaka
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  SYNOPSIS
 *
 *    #include "Mathlib.h"
 *    double gamma(double x);
 *
 *  DESCRIPTION
 *
 *    This function computes the value of the gamma function.
 *
 *  NOTES
 *
 *    This function is a translation into C of a Fortran subroutine
 *    by W. Fullerton of Los Alamos Scientific Laboratory.
 *
 *    The accuracy of this routine compares (very) favourably
 *    with those of the Sun Microsystems portable mathematical
 *    library.
 */

#include "Mathlib.h"

double gamma(double x)
{
    static double gamcs[42] = {
	+.8571195590989331421920062399942e-2,
	+.4415381324841006757191315771652e-2,
	+.5685043681599363378632664588789e-1,
	-.4219835396418560501012500186624e-2,
	+.1326808181212460220584006796352e-2,
	-.1893024529798880432523947023886e-3,
	+.3606925327441245256578082217225e-4,
	-.6056761904460864218485548290365e-5,
	+.1055829546302283344731823509093e-5,
	-.1811967365542384048291855891166e-6,
	+.3117724964715322277790254593169e-7,
	-.5354219639019687140874081024347e-8,
	+.9193275519859588946887786825940e-9,
	-.1577941280288339761767423273953e-9,
	+.2707980622934954543266540433089e-10,
	-.4646818653825730144081661058933e-11,
	+.7973350192007419656460767175359e-12,
	-.1368078209830916025799499172309e-12,
	+.2347319486563800657233471771688e-13,
	-.4027432614949066932766570534699e-14,
	+.6910051747372100912138336975257e-15,
	-.1185584500221992907052387126192e-15,
	+.2034148542496373955201026051932e-16,
	-.3490054341717405849274012949108e-17,
	+.5987993856485305567135051066026e-18,
	-.1027378057872228074490069778431e-18,
	+.1762702816060529824942759660748e-19,
	-.3024320653735306260958772112042e-20,
	+.5188914660218397839717833550506e-21,
	-.8902770842456576692449251601066e-22,
	+.1527474068493342602274596891306e-22,
	-.2620731256187362900257328332799e-23,
	+.4496464047830538670331046570666e-24,
	-.7714712731336877911703901525333e-25,
	+.1323635453126044036486572714666e-25,
	-.2270999412942928816702313813333e-26,
	+.3896418998003991449320816639999e-27,
	-.6685198115125953327792127999999e-28,
	+.1146998663140024384347613866666e-28,
	-.1967938586345134677295103999999e-29,
	+.3376448816585338090334890666666e-30,
	-.5793070335782135784625493333333e-31
    };

    /* pi is the obvious constant */
    /* sq2pil is 0.5*ln(2*pi) = ln(sqrt(2*pi)) */

    static double pi = 3.1415926535897932384626433832795;
    static double sq2pil = .91893853320467274178032973640562;
    static int ngam = 0;
    static double xmin = 0.;
    static double xmax = 0.;
    static double xsml = 0.;
    static double dxrel = 0.;

    int i, n;
    double y;
    double sinpiy, value;

    if (ngam == 0) {
	ngam = chebyshev_init(gamcs, 42, 0.1 * d1mach(3));
	gammalims(&xmin, &xmax);
	xsml = exp(fmax2(log(d1mach(1)), -log(d1mach(2)))+0.01);
	dxrel = sqrt(d1mach(4));
    }

#ifdef IEEE_754
    if(ISNAN(x)) return x;
#endif

    y = fabs(x);

    if (y <= 10) {

	/* Compute gamma(x) for -10 <= x <= 10. */
	/* Reduce the interval and find gamma(1 + y) for */
	/* 0 <= y < 1 first of all. */

	n = x;
	if(x < 0) --n;
	y = x - n;
	--n;
	value = chebyshev_eval(y * 2 - 1, gamcs, ngam) + .9375;
	if (n == 0)
	    return value;

	if (n <= 0) {

            /* compute gamma(x) for x < 1 */

	    n = -n;

	    /* If the argument is exactly zero or a negative integer */
	    /* then return Inf. I personally think that gamma(0) should */
	    /* return NaN, but this seems to be the common practice. */

	    if (x == 0 || (x < 0 && x + (n - 2) == 0)) {
		ML_ERROR(ME_RANGE);
		return ML_NAN;
	    }

	    /* The answer is less than half precision */
	    /* because x too near a negative integer. */

	    if (x < -0.5 && fabs(x - (int)(x - 0.5) / x) < dxrel) {
		ML_ERROR(ME_PRECISION);
	    }

	    /* The argument is so close to 0.0 */
	    /* that the result would overflow. */

	    if (y < xsml) {
		ML_ERROR(ME_RANGE);
		if(x > 0) return ML_POSINF;
		else return ML_NEGINF;
	    }

	    for (i = 1; i <= n; i++) {
		value /= x + (i - 1);
	    }
	    return value;

	}
	else {

	    /* gamma(x) for x >= 2 and x <= 10 */

	    for (i = 1; i <= n; i++) {
		value = (y + i) * value;
	    }
	    return value;
	}
    }
    else {

        /* gamma(x) for |x| > 10.  recall y = fabs(x). */

	if (x > xmax) {			/* Overflow */
	    ML_ERROR(ME_RANGE);
	    return ML_POSINF;
	}

	if (x < xmin) {			/* Underflow */
	    ML_ERROR(ME_UNDERFLOW);
	    return ML_UNDERFLOW;
	}

	value = exp((y - 0.5) * log(y) - y + sq2pil + lgammacor(y));

	if (x > 0)
	    return value;

	if (fabs((x - (int)(x - 0.5))/x) < dxrel){

	    /* The answer is less than half precision because */
	    /* the argument is too near a negative integer. */

	    ML_ERROR(ME_PRECISION);
	}

	sinpiy = sin(pi * y);
	if (sinpiy == 0) {		/* Negative integer arg - overflow */
	    ML_ERROR(ME_RANGE);
	    return ML_POSINF;
	}

	return -pi / (y * sinpiy * value);
    }
}
