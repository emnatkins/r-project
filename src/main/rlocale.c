/*
 *  R : A Computer Language for Statistical Data Analysis
 *  Copyright (C) 2005-7   The R Development Core Team
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
 *  http://www.r-project.org/Licenses/
 */

/*  This file was contributed by Ei-ji Nakama.
 *  See also the comments in R_ext/rlocale.h.
 *
 *  It provides replacements for the wctype functions on
 *  Windows (where they are not correct in e.g. Japanese)
 *  AIX (missing)
 *  MacOS X in CJK (where these just call the ctype functions)
 *
 *  It also provides wc[s]width, where widths of CJK fonts are often
 *  wrong in vendor-supplied versions and in Marcus Kuhn's version
 *  used for Windows in R 2.[12].x.
 */


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>
#include <stdlib.h>

#ifndef SUPPORT_MBCS
void Ri18n_wcswidth(void)
{
   return;
}
void Ri18n_wcwidth(void)
{
   return;
}
void Ri18n_wctype(void)
{
   return;
}
void Ri18n_iswctype(void)
{
   return;
}

#else /* SUPPORT_MBCS */

#define IN_RLOCALE_C 1 /* used in rlocale.h */
#include <R_ext/rlocale.h>
#include "rlocale_data.h"

#include <wctype.h>
#include <wchar.h>
#include <ctype.h>
#include <locale.h>
#include <limits.h>
#include <R_ext/Riconv.h>

static int wcwidthsearch(int wint, const struct interval_wcwidth *table,
			 int max, int locale)
{
    int min = 0;
    int mid;
    max--;

    if (wint < table[0].first || wint > table[max].last) return 0;
    while (max >= min) {
	mid = (min + max) / 2;
	if (wint > table[mid].last)
	    min = mid + 1;
	else if (wint < table[mid].first)
	    max = mid - 1;
	else{
	    return(table[mid].mb[locale]);
	}
    }
    return -1;
}

typedef struct {
    char *name;
    int locale;
} cjk_locale_name_t;

static cjk_locale_name_t cjk_locale_name[] = {
    {"CHINESE(SINGAPORE)_SIGNAPORE",		MB_zh_SG},
    {"CHINESE_SIGNAPORE",			MB_zh_SG},
    {"CHINESE(PRC)_PEOPLE'S REPUBLIC OF CHINA",	MB_zh_CN},
    {"CHINESE_PEOPLE'S REPUBLIC OF CHINA",	MB_zh_CN},
    {"CHINESE_MACAU S.A.R.",			MB_zh_HK},
    {"CHINESE(PRC)_HONG KONG",		        MB_zh_HK},
    {"CHINESE_HONG KONG S.A.R.",		MB_zh_HK},
    {"CHINESE(TAIWAN)_TAIWAN",			MB_zh_TW},
    {"CHINESE_TAIWAN",				MB_zh_TW},
    {"CHINESE-S",                               MB_zh_CN},
    {"CHINESE-T",                               MB_zh_TW},
    {"JAPANESE_JAPAN",				MB_ja_JP},
    {"JAPANESE",				MB_ja_JP},
    {"KOREAN_KOREA",				MB_ko_KR},
    {"KOREAN",				        MB_ko_KR},
    {"ZH_TW",                                   MB_zh_TW},
    {"ZH_CN",                                   MB_zh_CN},
    {"ZH_CN.BIG5",                              MB_zh_TW},
    {"ZH_HK",                                   MB_zh_HK},
    {"ZH_SG",                                   MB_zh_SG},
    {"JA_JP",				MB_ja_JP},
    {"KO_KR",				        MB_ko_KR},
    {"ZH",				        MB_zh_CN},
    {"JA",				MB_ja_JP},
    {"KO",				        MB_ko_KR},
    {"",				        MB_UTF8},
};

int Ri18n_wcwidth(wchar_t c)
{
    char lc_str[128];
    unsigned int i, j;

    static char *lc_cache = "";
    static int lc = 0;

    if (0 != strcmp(setlocale(LC_CTYPE, NULL), lc_cache)) {
	strncpy(lc_str, setlocale(LC_CTYPE, NULL), sizeof(lc_str));
	for (i = 0, j = strlen(lc_str); i < j && i < sizeof(lc_str); i++)
	    lc_str[i] = toupper(lc_str[i]);
	for (i = 0; i < (sizeof(cjk_locale_name)/sizeof(cjk_locale_name_t));
	     i++) {
	    if (0 == strncmp(cjk_locale_name[i].name, lc_str,
			     strlen(cjk_locale_name[i].name))) {
		lc = cjk_locale_name[i].locale;
		break;
	    }
	}
    }

    return(wcwidthsearch(c, table_wcwidth,
			 (sizeof(table_wcwidth)/sizeof(struct interval_wcwidth)),
			 lc));
}

int Ri18n_wcswidth (const wchar_t *s, size_t n)
{
    int rs = 0;
    while ((n-- > 0) && (*s != L'\0'))
    {
	int now = Ri18n_wcwidth (*s);
	if (now == -1) return -1;
	rs += now;
	s++;
    }
    return rs;
}

#if defined(Win32) || defined(_AIX) || defined(__APPLE_CC__)
static int wcsearch(int wint, const struct interval *table, int max)
{
    int min = 0;
    int mid;
    max--;

    if (wint < table[0].first || wint > table[max].last)
	return 0;
    while (max >= min) {
	mid = (min + max) / 2;
	if (wint > table[mid].last)
	    min = mid + 1;
	else if (wint < table[mid].first)
	    max = mid - 1;
	else
	    return 1;
    }
    return 0;
}
#endif


/*********************************************************************
 *  There is MacOS with a CSI(CodeSet Independence) system.
 *  (wchar_t != unicode)
 *  However, it is Unicode at the time of UTF-8.
 ********************************************************************/
#if defined(__APPLE_CC__)
/* allow for both PPC and Intel platforms */
#ifdef WORDS_BIGENDIAN
static const char UNICODE[] = "UCS-4BE";
#else
static const char UNICODE[] = "UCS-4LE";
#endif

#define ISWFUNC(ISWNAME) static int Ri18n_isw ## ISWNAME (wint_t wc) \
{	                                                             \
  char    mb_buf[MB_LEN_MAX+1];			                     \
  size_t  mb_len;                                                    \
  int     ucs4_buf[2];				                     \
  size_t  wc_len;                                                    \
  void   *cd;					                     \
  char    fromcode[128];                                             \
  char   *_mb_buf;						     \
  char   *_wc_buf;						     \
  size_t  rc ;							     \
								     \
  strncpy(fromcode, locale2charset(NULL), sizeof(fromcode));         \
  if(0 == strcmp(fromcode, "UTF-8"))				     \
       return wcsearch(wc,table_w ## ISWNAME , table_w ## ISWNAME ## _count);\
  memset(mb_buf, 0, sizeof(mb_buf));				     \
  memset(ucs4_buf, 0, sizeof(ucs4_buf));			     \
  wcrtomb( mb_buf, wc, NULL);					     \
  if((void *)(-1) != (cd = Riconv_open(UNICODE, fromcode))) {	     \
      wc_len = sizeof(ucs4_buf);		                     \
      _wc_buf = (char *)ucs4_buf;				     \
      mb_len = strlen(mb_buf);					     \
      _mb_buf = (char *)mb_buf;					     \
      rc = Riconv(cd, (const char **)&_mb_buf, (size_t *)&mb_len,	     \
		  (char **)&_wc_buf, (size_t *)&wc_len);	     \
      Riconv_close(cd);						     \
      wc = ucs4_buf[0];                                              \
      return wcsearch(wc,table_w ## ISWNAME , table_w ## ISWNAME ## _count); \
  }                                                                  \
  return(-1);                                                        \
}
#endif

/*********************************************************************
 *  iswalpha etc. does not function correctly for Windows
 *  iswalpha etc. does not function at all in AIX.
 *  all locale wchar_t == UNICODE
 ********************************************************************/
#if defined(Win32) || defined(_AIX)
#define ISWFUNC(ISWNAME) static int Ri18n_isw ## ISWNAME (wint_t wc) \
{									\
    return wcsearch(wc,table_w ## ISWNAME , table_w ## ISWNAME ## _count); \
}
#endif

/*********************************************************************
 *  iswalpha etc. does function normally for Linux
 ********************************************************************/
#ifndef ISWFUNC
#define ISWFUNC(ISWNAME) static int Ri18n_isw ## ISWNAME (wint_t wc) \
{                                                                       \
    return isw ## ISWNAME (wc); \
}
/* Solaris 8 is missing iswblank.  Its man page is missing iswcntrl,
   but the function is there.  MinGW used not to have iswblank until
   mingw-runtime-3.11. */
#ifndef HAVE_ISWBLANK
#define iswblank(wc) iswctype(wc, wctype("blank"))
#endif
#endif

/* These are the functions which C99 and POSIX define.  However,
   not all are used elsewhere in R, but they are used in Ri18n_iswctype. */

    ISWFUNC(upper)
    ISWFUNC(lower)
    ISWFUNC(alpha)
    ISWFUNC(digit)
    ISWFUNC(xdigit)
    ISWFUNC(space)
    ISWFUNC(print)
    ISWFUNC(graph)
    ISWFUNC(blank)
    ISWFUNC(cntrl)
    ISWFUNC(punct)
    /*  defined below in terms of digit and alpha
    ISWFUNC(alnum)
    */

wctype_t Ri18n_wctype(const char *);
int Ri18n_iswctype(wint_t, wctype_t);

static int Ri18n_iswalnum (wint_t wc)
{
    return (Ri18n_iswctype(wc, Ri18n_wctype("digit")) ||
	    Ri18n_iswctype(wc, Ri18n_wctype("alpha"))    );
}


/*
 * iswctype
 */
typedef struct {
    char * name;
    wctype_t wctype;
    int(*func)(wint_t);
} Ri18n_wctype_func_l ;

static const Ri18n_wctype_func_l Ri18n_wctype_func[] = {
    {"upper",  1<<0,  Ri18n_iswupper},
    {"lower",  1<<1,  Ri18n_iswlower},
    {"alpha",  1<<2,  Ri18n_iswalpha},
    {"digit",  1<<3,  Ri18n_iswdigit},
    {"xdigit", 1<<4,  Ri18n_iswxdigit},
    {"space",  1<<5,  Ri18n_iswspace},
    {"print",  1<<6,  Ri18n_iswprint},
    {"graph",  1<<7,  Ri18n_iswgraph},
    {"blank",  1<<8,  Ri18n_iswblank},
    {"cntrl",  1<<9,  Ri18n_iswcntrl},
    {"punct",  1<<10, Ri18n_iswpunct},
    {"alnum",  1<<11, Ri18n_iswalnum},
    {NULL,     0,     NULL}
};

wctype_t Ri18n_wctype(const char *name)
{
    int i;

    for (i = 0 ; Ri18n_wctype_func[i].name != NULL &&
	     0 != strcmp(Ri18n_wctype_func[i].name, name) ; i++ );
    return Ri18n_wctype_func[i].wctype;
}

int Ri18n_iswctype(wint_t wc, wctype_t desc)
{
    int i;

    for (i = 0 ; Ri18n_wctype_func[i].wctype != 0 &&
	     Ri18n_wctype_func[i].wctype != desc ; i++ );
    return (*Ri18n_wctype_func[i].func)(wc);
}

#endif /* SUPPORT_MBCS */
