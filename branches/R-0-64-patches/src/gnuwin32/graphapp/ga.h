/*
 *
 *  R : A Computer Language for Statistical Data Analysis
 *  file ga.h
 *  Copyright (C) 1998--1999  Guido Masarotto
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

/*
   New declarations.
*/

#ifndef __GA__VERSION
#define __GA__VERSION 2.45(1)
#include "graphapp.h"

/* renamed functions */
void gamainloop(void);
void gabeep(void);

/* windows.c */
#define Border      	0x10100000L
int ismdi();

/* gmenus.c */
typedef struct {
  char *nm;
  menufn fn;
  menuitem m;
} MenuItem;

#define STARTMENU {"#STARTMENU", 0, 0}
#define ENDMENU {"#ENDMENU", 0, 0}
#define STARTSUBMENU {"#STARTSUBMENU", 0, 0}
#define ENDSUBMENU {"#ENDSUBMENU", 0, 0}
#define MDIMENU {"#MDIMENU", 0, 0}
#define LASTMENUITEM {0, 0, 0}
menu newmdimenu();
typedef menu popup;
popup newpopup();
menubar gmenubar(actionfn fn, MenuItem []);
popup gpopup(actionfn fn, MenuItem []);
void gchangepopup(window w, popup p);
/* next is limited to current window... */
void gchangemenubar(menubar mb);

/* winalloc.c */
#include <stddef.h> /* for size_t */
void *winmalloc(size_t size);
void  winfree(void *block);
void *winrealloc(void *block, size_t newsize);
char *winstrdup(char *s);

/* tooltips.c */
int addtooltip(control c, char *tp);

/* status.c */
int addstatusbar();
void setstatus(char *text);

/* dialogs.c */
void setuserfilter(char *);
void    askchangedir();

/*  rgb.c */
rgb     nametorgb(char *colourname);

/* clipboard.c */
void    copytoclipboard(drawing src);

/* gimage.c */
image  bitmaptoimage(bitmap bm);

/* printer.c  */
typedef objptr printer;
printer newprinter(double w,  double h);
void    nextpage();

/* metafile.c */
typedef objptr metafile;
metafile newmetafile(char *name, rect r);


/* thread safe and extented  drawing functions (gdraw.c) */
#define lSolid 0
#define lDash  (5 | (4<<4))
#define lShortDash  (3 | (4<<4))
#define lLongDash  (8 | (4<<4))
#define lDot   (1 | (4<<4))
#define lDashDot (5 | (4<<4) | (1<<8) | (4<<12))
#define lShortDashDot (3 | (4<<4) | (1<<8) | (4<<12))
#define lLongDashDot (8 | (4<<4) | (1<<8) | (4<<12))
#define lDashDotDot    (5 | (4<<4) | (1<<8) | (3<<12) | (1<<16) | (4<< 20))
#define lShortDashDotDot    (3 | (4<<4) | (1<<8) | (3<<12) | (1<<16) | (4<< 20))
#define lLongDashDotDot    (8 | (4<<4) | (1<<8) | (3<<12) | (1<<16) | (4<< 20))

rect  ggetcliprect(drawing d);
void  gsetcliprect(drawing d, rect r);
void  gbitblt(bitmap db, bitmap sb, point p, rect r);
void  gscroll(drawing d, point dp, rect r);
void  ginvert(drawing d, rect r);
rgb   ggetpixel(drawing d, point p);
void  gsetpixel(drawing d, point p, rgb c);
void  gdrawline(drawing d, int width, int style, rgb c, point p1, point p2);
void  gdrawrect(drawing d, int width, int style, rgb c, rect r);
void  gfillrect(drawing d, rgb fill, rect r);
void  gdrawellipse(drawing d, int width, rgb border, rect r);
void  gfillellipse(drawing d, rgb fill, rect r);
void  gdrawpolygon(drawing d, int width, int style, rgb c, point *p, int n);
void  gfillpolygon(drawing d, rgb fill, point *p, int n);
int   gdrawstr(drawing d, font f, rgb c, point p, char *s);
rect  gstrrect(drawing d, font f, char *s);
point gstrsize(drawing d, font f, char *s);
int   gstrwidth(drawing d ,font f, char *s);
void  gcharmetric(drawing d, font f, int c, int *ascent, int *descent,
		  int *width);
font  gnewfont(drawing d,char *face, int style, int size, double rot);
int   ghasfixedwidth(font f);

/* pixels */
int   devicewidth(drawing dev);
int   deviceheight(drawing dev);
/* mm */
int   devicewidthmm(drawing dev);
int   deviceheightmm(drawing dev);
/* pixels per inch */
int   devicepixelsx(drawing dev);
int   devicepixelsy(drawing dev);


/* gbuttons.c */
/* horizontal, vertical and control scrollbar */
#define HWINSB 0
#define VWINSB 1
#define CONTROLSB 2
void gchangescrollbar(scrollbar sb, int which, int where, int max, 
		      int pagesize, int disablenoscroll);
void gsetcursor(drawing d, cursor c);
control newtoolbar(int height);

/* cursor.c */
extern cursor CrossCursor;


#endif /* __GA__VERSION */

