/*
 *  R : A Computer Language for Statistical Data Analysis
 *  Copyright (C) 2000-2017   The R Core Team.
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
 *  along with this program; if not, a copy is available at
 *  https://www.R-project.org/Licenses/
 */


/* This header is not part of the R API (see 'Writing R Extensions') */

#ifndef R_EXT_CONNECTIONS_H_
#define R_EXT_CONNECTIONS_H_

#include <R_ext/Boolean.h>

#if defined(__cplusplus) && !defined(DO_NOT_USE_CXX_HEADERS)
# include <cstddef>
# include <cstdarg>
#else
# include <stddef.h> /* for size_t */
# include <stdarg.h> /* for va_list */
#endif

/* IMPORTANT: we do not expect future connection APIs to be
   backward-compatible so if you use this, you *must* check the
   version and proceed only if it matches what you expect.

   We explicitly reserve the right to change the connection
   implementation without a compatibility layer.
 */
#define R_CONNECTIONS_VERSION 2

struct RconnData;

/* this allows the opaque pointer definition to be made available 
   in Rinternals.h */
#ifndef HAVE_RCONNECTION_TYPEDEF
typedef struct Rconn  *Rconnection;
#endif
struct Rconn {
    struct RconnData *data;
    void *private;
    char mode[5];
    Rboolean text, isopen, incomplete, canread, canwrite, canseek, blocking;
    Rboolean UTF8out;    
    Rboolean (*open)(struct Rconn *);
    void (*close)(struct Rconn *); /* routine closing after auto open */
    void (*destroy)(struct Rconn *); /* when closing connection */
    int (*vfprintf)(struct Rconn *, const char *, va_list);
    int (*fgetc)(struct Rconn *);
    int (*fgetc_internal)(struct Rconn *);
    double (*seek)(struct Rconn *, double, int, int);
    void (*truncate)(struct Rconn *);
    int (*fflush)(struct Rconn *);
    size_t (*read)(void *, size_t, size_t, struct Rconn *);
    size_t (*write)(const void *, size_t, size_t, struct Rconn *);
};

#ifdef  __cplusplus
extern "C" {
#endif

SEXP   R_new_custom_connection(const char *description, const char *mode, const char *class_name, Rconnection *ptr);
size_t R_ReadConnection(Rconnection con, void *buf, size_t n);
size_t R_WriteConnection(Rconnection con, void *buf, size_t n);
Rconnection R_GetConnection(SEXP sConn); // added in R 3.3.0
char const *R_ConnectionClass(Rconnection con);
char const *R_ConnectionDescription(Rconnection con);    
			      
#ifdef  __cplusplus
}
#endif

#endif
