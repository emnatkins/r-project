/*
 *  R : A Computer Language for Statistical Data Analysis
 *  Copyright (C) 1995, 1996  Robert Gentleman and Ross Ihaka
 *  Copyright (C) 1998, The R Development Core Team
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
 */

#include "Defn.h"

#include <time.h>

/*  Platform
 *
 *  Return various platform dependent strings.  This is similar to
 *  "Machine", but for strings rather than numerical values.  These
 *  two functions should probably be amalgamated.
 */

#ifdef Unix
#define OSTYPE      "Unix"
#define FILESEP     "/"
#define DYNLOADEXT  ".so"
#endif

#ifdef Macintosh
#define OSTYPE      "Macintosh"
#define FILESEP     ":"
#define DYNLOADEXT  ".dll"
#endif

#ifdef Win32
#define OSTYPE      "Windows"
#define FILESEP     "\\"
#define DYNLOADEXT  ".dll"
#endif

static char *R_OSType = OSTYPE;
static char *R_FileSep = FILESEP;
static char *R_DynLoadExt = DYNLOADEXT;

SEXP do_Platform(SEXP call, SEXP op, SEXP args, SEXP rho)
{
    SEXP value, names;
    checkArity(op, args);
    PROTECT(value = allocVector(VECSXP, 3));
    PROTECT(names = allocVector(STRSXP, 3));
    STRING(names)[0] = mkChar("OS.type");
    STRING(names)[1] = mkChar("file.sep");
    STRING(names)[2] = mkChar("dynlib.ext");
    VECTOR(value)[0] = mkString(R_OSType);
    VECTOR(value)[1] = mkString(R_FileSep);
    VECTOR(value)[2] = mkString(R_DynLoadExt);
    setAttrib(value, R_NamesSymbol, names);
    UNPROTECT(2);
    return value;
}
      
/*  date
 *
 *  Return the current date in a standard format.  This uses standard
 *  POSIX calls which should be available on each platform.  We should
 *  perhaps check this in the configure script.
 */

char *R_Date()
{     
    time_t t;
    time(&t);
    return ctime(&t);
}     

SEXP do_date(SEXP call, SEXP op, SEXP args, SEXP rho)
{
    char *date;
    checkArity(op, args);
    return mkString(R_Date());
}

/*  show.file
 *
 *  Display a file so that a user can view it.  The function calls
 *  "R_ShowFile" which is a platform dependent hook that arranges
 *  for the file to be displayed. A reasonable approach would be to
 *  open a read-only edit window with the file displayed in it.
 *
 *  FIXME : this should in fact take a vector of filenames and titles
 *  and display them concatenated in a window.  For a pure console
 *  version, write down a pipe to a pager.
 */

SEXP do_fileshow(SEXP call, SEXP op, SEXP args, SEXP rho)
{
    SEXP fn, tl;
    checkArity(op, args);
    fn = CAR(args);
    tl = CADR(args);
    if (!isString(fn) || length(fn) < 1 || STRING(fn)[0] == R_NilValue)
	errorcall(call, "invalid filename\n");
    if (!isString(tl) || length(tl) < 1 || STRING(tl)[0] == R_NilValue)
	errorcall(call, "invalid filename\n");
    if (!R_ShowFile(R_ExpandFileName(CHAR(STRING(fn)[0])),
                    CHAR(STRING(tl)[0])))
	error("unable to display file \"%s\"\n", CHAR(STRING(fn)[0]));
    return R_NilValue;
}


/*  append.file
 *
 *  Given two file names as arguments and arranges for
 *  the second file to be appended to the second.
 */

#define APPENDBUFSIZE 512

static int R_AppendFile(char *file1, char *file2)
{
    FILE *fp1, *fp2;
    char buf[APPENDBUFSIZE];
    int nchar, status;
    if((fp1 = fopen(file1, "a")) == NULL) {
        return 0;
    }
    if((fp2 = fopen(file2, "r")) == NULL) {
        fclose(fp1);
        return 0;
    }
    while((nchar = fread(buf, 1, APPENDBUFSIZE, fp2)) == APPENDBUFSIZE)
        if(fwrite(buf, 1, APPENDBUFSIZE, fp1) != APPENDBUFSIZE) {
            goto append_error;
        }
    if(fwrite(buf, 1, nchar, fp1) != nchar) {
        goto append_error;
    }
    status = 1;
 append_error:
    if (status == 0)
	warning("write error during file append!\n");
    fclose(fp1);
    fclose(fp2);
    return status;
}

SEXP do_fileappend(SEXP call, SEXP op, SEXP args, SEXP rho)
{
    SEXP f1, f2, ans;
    int i, n, n1, n2;
    checkArity(op, args);
    f1 = CAR(args); n1 = length(f1);
    f2 = CADR(args); n2 = length(f2);
    if (!isString(f1))
        errorcall(call, "invalid first filename\n");
    if (!isString(f2))
        errorcall(call, "invalid second filename\n");
    if (n1 < 1)
	errorcall(call, "nothing to append to\n");
    if (n2 < 1)
	return allocVector(LGLSXP, 0);
    n = (n1 > n2) ? n1 : n2;
    PROTECT(ans = allocVector(LGLSXP, n));
    for(i = 0; i < n; i++) {
        if (STRING(f1)[i%n1] == R_NilValue || STRING(f2)[i%n2] == R_NilValue)
            LOGICAL(ans)[i] = 0;
        else 
            LOGICAL(ans)[i] =
		R_AppendFile(R_ExpandFileName(CHAR(STRING(f1)[i%n1])),
			     R_ExpandFileName(CHAR(STRING(f2)[i%n2])));
    }
    UNPROTECT(1);
    return ans;
}

SEXP do_filecreate(SEXP call, SEXP op, SEXP args, SEXP rho)
{
    SEXP fn, ans;
    FILE *fp;
    int i, n;
    checkArity(op, args);
    fn = CAR(args);
    if (!isString(fn)) 
        errorcall(call, "invalid filename argument\n");
    n = length(fn);
    PROTECT(ans = allocVector(LGLSXP, n));
    for (i = 0; i < n; i++) {
	LOGICAL(ans)[i] = 0;
	if (STRING(fn)[i] != R_NilValue &&
	    (fp = fopen(CHAR(STRING(fn)[i]), "w")) != NULL) {
	    LOGICAL(ans)[i] = 1;
	    fclose(fp);
	}
    }
    UNPROTECT(1);
    return ans;
}

SEXP do_fileremove(SEXP call, SEXP op, SEXP args, SEXP rho)
{
    SEXP f, ans;
    int i, n;
    checkArity(op, args);
    f = CAR(args);     
    if (!isString(f))
        errorcall(call, "invalid first filename\n");
    n = length(f);
    PROTECT(ans = allocVector(LGLSXP, n));
    for (i = 0; i < n; i++) {
	if (STRING(f)[i] != R_NilValue)
	    LOGICAL(ans)[i] =
		(remove(R_ExpandFileName(CHAR(STRING(f)[i]))) == 0);
    }
    UNPROTECT(1);
    return ans;
}

#ifndef Macintosh
#include <sys/types.h>
#endif
#include "dirent.h"
#ifdef HAVE_REGCOMP
#include "regex.h"
#endif

#define DIRNAMEBUFSIZE 256

static SEXP filename(char *dir, char *file)
{
    SEXP ans;
    if (dir) {
	ans = allocString(strlen(dir) + strlen(R_FileSep) + strlen(file));
	sprintf(CHAR(ans), "%s%s%s", dir, R_FileSep, file);
    }
    else {
	ans = allocString(strlen(file));
	sprintf(CHAR(ans), "%s", file);
    }
    return ans;
}

SEXP do_listfiles(SEXP call, SEXP op, SEXP args, SEXP rho)
{
    SEXP d, p, ans;
    DIR *dir;
    struct dirent *de;
    int allfiles, fullnames, count, pattern;
    int i, ndir;
    char *dnp, dirname[DIRNAMEBUFSIZE];
#ifdef HAVE_REGCOMP
    regex_t reg;
#endif
    checkArity(op, args);
    d = CAR(args);  args = CDR(args);
    if (!isString(d))
	errorcall(call, "invalid directory argument\n");
    p = CAR(args);  args = CDR(args);
    if (isNull(p) || (isString(p) && length(p) < 1))
	pattern = 0;
    else if (isString(p) && length(p) >= 1 && STRING(p)[0] != R_NilValue)
	pattern = 1;
    else
	errorcall(call, "invalid pattern argument\n");
    allfiles = asLogical(CAR(args)); args = CDR(args);
    fullnames = asLogical(CAR(args));
    ndir = length(d);
    for (i = 0; i < ndir ; i++) {
	dnp = R_ExpandFileName(CHAR(STRING(d)[i]));
	if (strlen(dnp) >= DIRNAMEBUFSIZE)
	    error("directory/folder path name too long\n");
	strcpy(dirname, dnp);
	if ((dir = opendir(dirname)) == NULL)
	    errorcall(call, "invalid directory/folder name\n");
#ifdef HAVE_REGCOMP
	if (pattern && regcomp(&reg, CHAR(STRING(p)[0]), REG_EXTENDED))
	    errorcall(call, "invalid pattern regular expression\n");
#else
	warning("pattern specification is not available in \"list.files\"\n");
#endif
	count = 0;
	while (de = readdir(dir)) {
	    if (allfiles || !R_HiddenFile(de->d_name))
#ifdef HAVE_REGCOMP
		if (pattern) {
		    if(regexec(&reg, de->d_name, 0, NULL, 0) == 0)
			count++;
		}
		else
#endif
		    count++;
	}
	closedir(dir);
    }
    PROTECT(ans = allocVector(STRSXP, count));
    count = 0;
    for (i = 0; i < ndir ; i++) {
	dnp = R_ExpandFileName(CHAR(STRING(d)[i]));
	if (strlen(dnp) >= DIRNAMEBUFSIZE)
	    error("directory/folder path name too long\n");
	strcpy(dirname, dnp);
	if (fullnames)
	    dnp = dirname;
	else
	    dnp = NULL;
	if ((dir = opendir(dirname)) == NULL)
	    errorcall(call, "invalid directory/folder name\n");
	while (de = readdir(dir)) {
	    if (allfiles || !R_HiddenFile(de->d_name))
#ifdef HAVE_REGCOMP
		if (pattern) {
		    if (regexec(&reg, de->d_name, 0, NULL, 0) == 0)
			STRING(ans)[count++] = filename(dnp, de->d_name);
		}
		else
#endif
		    STRING(ans)[count++] = filename(dnp, de->d_name);
	}
	closedir(dir);
    }
#ifdef HAVE_REGCOMP
    regfree(&reg);
#endif
    ssort(STRING(ans), count);
    UNPROTECT(1);
    return ans;
}

SEXP do_Rhome(SEXP call, SEXP op, SEXP args, SEXP rho)
{
    char *path;
    checkArity(op, args);
    if (!(path = R_HomeDir()))
	error("unable to determine R home location\n");
    return mkString(path);
}

SEXP do_fileexists(SEXP call, SEXP op, SEXP args, SEXP rho)
{
    SEXP file, ans;
    int i, nfile;
    FILE *fp;
    checkArity(op, args);
    if (!isString(file = CAR(args)))
        errorcall(call, "invalid file argument\n");
    nfile = length(file);
    ans = allocVector(LGLSXP, nfile);
    for(i = 0; i < nfile; i++) {
	LOGICAL(ans)[i] = 0;
        if (STRING(file)[i] != R_NilValue)
	    LOGICAL(ans)[i] = R_FileExists(CHAR(STRING(file)[i]));
    }
    return ans;
}

static int filbuf(char *buf, FILE *fp)
{
    int c;
    while((c = fgetc(fp)) != EOF) {
	if (c == '\n' || c == '\r') {
	    *buf = '\0';
	    return 1;
	}
	*buf++ = c;
    }
    return 0;
}

SEXP do_indexsearch(SEXP call, SEXP op, SEXP args, SEXP rho)
{
    SEXP topic, path, indexname, sep;
    char linebuf[256], topicbuf[256], *p;
    int i, npath, ltopicbuf, html;
    FILE *fp;

    topic = CAR(args); args = CDR(args);
    if(!isString(topic) || length(topic) < 1 || isNull(topic))
	error("invalid \"topic\" argument\n");
    path = CAR(args); args = CDR(args);
    if(!isString(path) || length(path) < 1 || isNull(path))
	error("invalid \"path\" argument\n");
    indexname = CAR(args); args = CDR(args);
    if(!isString(indexname) || length(indexname) < 1 || isNull(indexname))
	error("invalid \"indexname\" argument\n");
    sep = CAR(args); args = CDR(args);
    if(!isString(sep) || length(sep) < 1 || isNull(sep))
	error("invalid \"sep\" argument\n");
    html = asLogical(CAR(args));
    sprintf(topicbuf, "%s\t", CHAR(STRING(topic)[0]));
    ltopicbuf = strlen(topicbuf);
    npath = length(path);
    for (i = 0; i < npath; i++) {
	sprintf(linebuf, "%s%s%s%s%s",
		CHAR(STRING(path)[i]),
		CHAR(STRING(sep)[0]),
		"help", CHAR(STRING(sep)[0]),
		CHAR(STRING(indexname)[0]));
	if ((fp = fopen(linebuf, "rt")) == NULL)
	    error("unable to open index file");
	while (filbuf(linebuf, fp)) {
	    if(strncmp(linebuf, topicbuf, ltopicbuf) == 0) {
		p = &linebuf[ltopicbuf - 1];
		while(isspace(*p)) p++;
		fclose(fp);
		if (html)
		    sprintf(topicbuf, "%s%s%s%s%s%s",
			    CHAR(STRING(path)[i]),
			    CHAR(STRING(sep)[0]), 
			    "html", CHAR(STRING(sep)[0]),
			    p, ".html");
		else
		    sprintf(topicbuf, "%s%s%s%s%s",
			    CHAR(STRING(path)[i]),
			    CHAR(STRING(sep)[0]), 
			    "help", CHAR(STRING(sep)[0]), p);
		return mkString(topicbuf);
	    }
	}
	fclose(fp);
    }
    return mkString("");
}
