/*
 *  R : A Computer Language for Statistical Data Analysis
 *  Copyright (C) 1998-2000   Robert Gentleman, Ross Ihaka
 *                             and the R Development Core Team
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
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *
 * Application Routines, typically implemented in  ../appl/
 * ----------------------------------------------  ========
 */

#ifndef R_APPLIC_H_
#define R_APPLIC_H_

#include "R_ext/Boolean.h"
#include "R_ext/RS.h"		/* F77_... */

#ifdef  __cplusplus
extern "C" {
#endif

void R_approx(double *, double *, int *, double *, int *,
	      int *, double *, double *, double *);

void bakslv(double *, int *, int *,
	    double *, int *, int *,
	    double *, int *, int *);

void bincode (double *x, int *n, double *breaks, int *nb,
	      int *code, int *right, int *include_border, int *naok);
void bincount(double *x, int *n, double *breaks, int *nb, int *count,
	      int *right, int *include_border, int *naok);
/*unused*/
void bincode2(double *, int *, double *, int *, int *, int *, int *);

/* chisqsim.c: */
void chisqsim(int *nrow, int *ncol, int *nrowt, int *ncolt, int *n,
	      int *b, double *expected, int *observed, double *fact,
	      int *jwork, double *results);

/* chull.c -- comments in the source */
void R_chull(int *n, double *x, int *m, int *in,
	   int *ia, int *ib,
	   int *ih, int *nh, int *il);

/* cpoly.c : */
void R_cpolyroot(double *opr, double *opi, int *degree,
		 double *zeror, double *zeroi, Rboolean *fail);
/* More `Complex Polynomial Utilities' could be exported:

   polyev(...)
   errev(...)
   cpoly_cauchy(...)
   cpoly_scale(...)
   cdivid(...)
*/


/* cumsum.c : */
void R_cumsum(double *, int *, double *, double *);

/* distance.c */
double R_euclidean(double *, int, int, int, int);
double R_maximum(double *, int, int, int, int);
double R_manhattan(double *, int, int, int, int);
double R_canberra(double *, int, int, int, int);
double R_dist_binary(double *, int, int, int, int);
enum { EUCLIDEAN=1, MAXIMUM, MANHATTAN, CANBERRA, BINARY };/* == 1,2,... */
void R_distance(double *, int *, int *, double *, int *, int *);

/* eigen.c */
int F77_NAME(cg)(int *nm, int *n, double *ar, double *ai,
		 double *wr, double *wi, int *matz, double *zr, double *zi,
		 double *fv1, double *fv2, double *fv3, int *ierr);
int F77_NAME(ch)(int *nm, int *n, double *ar, double *ai,
		 double *w, int *matz, double *zr, double *zi,
		 double *fv1, double *fv2, double *fm1, int *ierr);
int F77_NAME(rg)(int *nm, int *n, double *a, double *wr, double *wi,
		 int *matz, double *z, int *iv1, double *fv1, int *ierr);
int F77_NAME(rs)(int *nm, int *n, double *a, double *w,
		 int *matz, double *z, double *fv1, double *fv2, int *ierr);

/* fft.c */
/* NOTE:  The following functions use GLOBAL (static) variables !!
 * ----   some of R-core think that this should be changed,
 *        which will INEVITABLY extend the argument lists ...!
 */
void fft_factor(int n, int *pmaxf, int *pmaxp);

Rboolean fft_work(double *a, double *b, int nseg, int n, int nspn,
/* TRUE: success */ int isn, double *work, int *iwork);

/* fortran.c   is covered by ./Fortran.h */

/* lowess.c */
/*void clowess(double *x, double *y, int n,
	     double f, int nsteps, double delta,
	     double *ys, double *rw, double *res);*/
void lowess(double *x, double *y, int *n,
	    double *f, int *nsteps, double *delta,
	    double *ys, double *rw, double *res);

/* machar.c */
void machar(int *ibeta, int *it, int *irnd, int *ngrd, int *machep, int *negep,
	int *iexp, int *minexp, int *maxexp, double *eps,
	double *epsneg, double *xmin, double *xmax);

/* massdist.c */
void massdist(double *x, int *nx, double *xlow, double *xhigh,
	      double *y, int *ny);

void R_max_col(double *matrix, int *nr, int *nc, int *maxes);

/* pretty.c */
double R_pretty0(double *lo, double *up, int *ndiv, int min_n,
	       double shrink_sml, double high_u_fact[],
	       int eps_correction, int return_bounds);
void R_pretty(double *lo, double *up, int *ndiv, int *min_n,
	    double *shrink_sml, double *high_u_fact, int *eps_correction);

/* rowsum.c */
void R_rowsum(int *dim, double *na_x, double *x, double *group);

/* splines.c */
void spline_coef(int *method, int *n, double *x, double *y,
		 double *b, double *c, double *d, double *e);
void spline_eval(int *method, int *nu, double *u, double *v,
		 int *n, double *x, double *y,
		 double *b, double *c, double *d);
void natural_spline(int n, double *x, double *y,
		    double *b, double *c, double *d);
void fmm_spline(int n, double *x, double *y, double *b, double *c, double *d);
void periodic_spline(int n, double *x, double *y,
		     double *b, double *c, double *d, double *e);


/* stem.c */
Rboolean stemleaf(double *x, int *n, double *scale, int *width, double *atom);

/* ...............*/

/* strsignif.c */
void str_signif(char *x, int *n, char **type, int *width, int *digits,
		char **format, char **flag, char **result);

/* tabulate.c */
void R_tabulate(int *x, int *n, int *nbin, int *ans);


/* uncmin.c : */

/* type of pointer to the target and gradient functions */
typedef void (*fcn_p)(int, double *, double *, void *);

/* type of pointer to the hessian functions */
typedef void (*d2fcn_p)(int, int, double *, double *, void *);

void fdhess(int n, double *x, double fval, fcn_p fun, void *state,
	    double *h, int nfd, double *step, double *f, int ndigit,
	    double *typx);

void optif9(int nr, int n, double *x, 
	    fcn_p fcn, fcn_p d1fcn, d2fcn_p d2fcn,
	    void *state, double *typsiz, double fscale, int method, 
	    int iexp, int *msg, int ndigit, int itnlim, int iagflg,
	    int iahflg, double dlt, double gradtl, double stepmx,
	    double steptl, double *xpls, double *fpls, double *gpls,
	    int *itrmcd, double *a, double *wrk, int *itncnt);

void optif0(int nr, int n, double *x, fcn_p fcn, void *state,
	    double *xpls, double *fpls, double *gpls, int *itrmcd,
	    double *a, double *wrk);


/* ALL ../appl/<foobar>.f	[semi-automatically by
 *				 f2c -A -P *.f; cat *.P > all.h	 and editing]
 */
typedef double (*D_fp)();
typedef /* Subroutine */ int (*S_fp)();

/* ../appl/blas.f ---> see also ./Linpack.h - "extern"s all of BLAS+Linpack*/
double F77_NAME(dasum)(int *n, double *dx, int *incx);
int F77_NAME(daxpy)(int *n, double *da, double *dx, int *incx,
		    double *dy, int *incy);
int F77_NAME(dcopy)(int *n, double *dx, int *incx, double *dy, int *incy);
double F77_NAME(ddot)(int *n, double *dx, int *incx, double *dy, int *incy);
double F77_NAME(dmach)(int *job);
double F77_NAME(dnrm2)(int *n, double *x, int *incx);
int F77_NAME(drot)(int *n, double *dx, int *incx, double *dy, int *incy,
		   double *c__, double *s);
int F77_NAME(drotg)(double *da, double *db, double *c__, double *s);
int F77_NAME(dscal)(int *n, double *da, double *dx, int *incx);
int F77_NAME(dswap)(int *n, double *dx, int *incx, double *dy, int *incy);
int F77_NAME(idamax)(int *n, double *dx, int *incx);
/*----*/
int F77_NAME(ch2inv)(double *x, int *ldx, int *n, double *v, int *info);
int F77_NAME(chol)(double *a, int *lda, int *n, double *v, int *info);

int F77_NAME(dpoco)(double *a, int *lda, int *n, double *rcond,
		    double *z__, int *info);
int F77_NAME(dpodi)(double *a, int *lda, int *n, double *det, int *job);
int F77_NAME(dpofa)(double *a, int *lda, int *n, int *info);
int F77_NAME(dposl)(double *a, int *lda, int *n, double *b);
/* find qr decomposition, dqrdc2() is basis of R's qr() */
int F77_NAME(dqrdc)(double *x, int *ldx, int *n, int *p,
		    double *qraux, int *jpvt, double *work, int *job);
int F77_NAME(dqrdc2)(double *x, int *ldx, int *n, int *p,
		     double *tol, int *rank,
		     double *qraux, int *pivot, double *work);
int F77_NAME(dqrls)(double *x, int *n, int *p, double *y, int *ny,
		    double *tol, double *b, double *rsd,
		    double *qty, int *k,
		    int *jpvt, double *qraux, double *work);
/* solve for QR coefficients */
int F77_NAME(dqrsl)(double *x, int *ldx, int *n, int *k,
		    double *qraux, double *y,
		    double *qy, double *qty, double *b,
		    double *rsd, double *xb, int *job, int *info);

/* ../appl/dqrutl.f */
int F77_NAME(dqrqty)(double *x, int *n, int *k, double *qraux,
		     double *y, int *ny, double *qty);
int F77_NAME(dqrqy)(double *x, int *n, int *k, double *qraux,
		    double *y, int *ny, double *qy);
int F77_NAME(dqrcf)(double *x, int *n, int *k, double *qraux,
		    double *y, int *ny, double *b, int *info);
int F77_NAME(dqrrsd)(double *x, int *n, int *k, double *qraux,
		     double *y, int *ny, double *rsd);
int F77_NAME(dqrxb)(double *x, int *n, int *k, double *qraux,
		    double *y, int *ny, double *xb);
/*---*/

int F77_NAME(dsvdc)(double *x, int *ldx, int *n, int *p,
		    double *s, double *e,
		    double *u, int *ldu, double *v, int *ldv,
		    double *work, int *job, int *info);
int F77_NAME(dtrco)(double *t, int *ldt, int *n, double *rcond,
		    double *z__, int *job);
int F77_NAME(dtrsl)(double *t, int *ldt, int *n, double *b, int *job,
		    int *info);

double Brent_fmin(double ax, double bx, double (*f)(double, void *),
		  void *info, double tol);
int F77_NAME(lminfl)(double *x, int *ldx, int *n, int *k, double *qraux,
		     double *resid, double *hat, double *coef, double *sigma);

/* ../appl/zeroin.c */
double R_zeroin(double ax, double bx, double (*f)(double, void *), void *info,
		double *tol, int *maxit);

/* ../appl/lbfgsb.c */
void setulb(int n, int m, double *x, double *l, double *u, int *nbd,
	    double *f, double *g, double factr, double *pgtol,
	    double *wa, int * iwa, char *task, int iprint,
	    int *lsave, int *isave, double *dsave);

/* ../appl/loglin.c */
void loglin(int *nvar, int *dim, int *ncon, int *config, int *ntab,
	    double *table, double *fit, int *locmar, int *nmar, double *marg,
	    int *nu, double *u, double *maxdev, int *maxit,
	    double *dev, int *nlast, int *ifault);
#ifdef  __cplusplus
}
#endif

#endif /* R_APPLIC_H_ */
