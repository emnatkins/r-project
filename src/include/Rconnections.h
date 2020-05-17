/*
 *  R : A Computer Language for Statistical Data Analysis
 *  Copyright (C) 2000-2020   The R Core Team.
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

/* Internal header, not installed */

#ifndef R_CONNECTIONS_H_
#define R_CONNECTIONS_H_

/* the visible part of the connections */
#include <R_ext/Connections.h>

/* NB: this is a private header, and not installed.  The internals of
       connections are private and subject to change without notice.
       A subset can be accessed using R_ext/Connections.h but read
       the warning in that file for details. */

typedef enum {HTTPsh, FTPsh, HTTPSsh, FTPSsh} UrlScheme;

/* used in internet module */
typedef struct urlconn {
    void *ctxt;
    UrlScheme type;
    char *headers;
} *Rurlconn;

/* used in internet module */
typedef struct sockconn {
    int port;
    int server;
    int fd;
    int timeout;
    char *host;
    char inbuf[4096], *pstart, *pend;
    int serverfd;
} *Rsockconn;

typedef struct servsockconn {
    int fd;
    int port;
} *Rservsockconn;

/* used in X11 module */
typedef struct clpconn {
    char *buff;
    int pos, len, last, sizeKB;
    Rboolean warned;
} *Rclpconn;

#define init_con	Rf_init_con
#define con_pushback	Rf_con_pushback

typedef struct RconnData {
    char* class;
    char* description;
    int enc; /* the encoding of 'description' */
    Rboolean isGzcon;
    int nPushBack, posPushBack; /* number of lines, position on top line */
    char **PushBack;
    int save, save2;
    char encname[101];
    /* will be iconv_t, which is a pointer. NULL if not in use */
    void *inconv, *outconv;
    /* The idea here is that no MBCS char will ever not fit */
    char iconvbuff[25], oconvbuff[50], *next, init_out[25];
    short navail, inavail;
    Rboolean EOF_signalled;
    void *id;
    void *ex_ptr;
    int status; /* for pipes etc */
    unsigned char *buff;
    size_t buff_len, buff_stored_len, buff_pos;
} *RconnectionData;
    
int Rconn_fgetc(Rconnection con);
int Rconn_ungetc(int c, Rconnection con);
size_t Rconn_getline(Rconnection con, char *buf, size_t bufsize);
int Rconn_printf(Rconnection con, const char *format, ...);
Rconnection getConnection(int n);
Rconnection getConnection_no_err(int n);
Rboolean switch_stdout(int icon, int closeOnExit);
void init_con(Rconnection new, const char *description, int enc,
	      const char * const mode);
Rconnection R_newurl(const char *description, const char * const mode,
		     SEXP headers, int type);
Rconnection R_newsock(const char *host, int port, int server, int serverfd, const char * const mode, int timeout);
Rconnection in_R_newsock(const char *host, int port, int server, int serverfd, const char *const mode, int timeout);
Rconnection R_newservsock(int port);
Rconnection in_R_newservsock(int port);

Rconnection R_newunz(const char *description, const char * const mode);
int dummy_fgetc(Rconnection con);
int dummy_vfprintf(Rconnection con, const char *format, va_list ap);
int getActiveSink(int n);
void con_pushback(Rconnection con, Rboolean newLine, char *line);

int Rsockselect(int nsock, int *insockfd, int *ready, int *write, double timeout);

Rconnection new_connection(const char *description, int enc, const char *mode,
			   const char *class);
    
#define set_iconv Rf_set_iconv
void set_iconv(Rconnection con);
#endif

