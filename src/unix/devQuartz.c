 /*
 *  R : A Computer Language for Statistical Data Analysis
 *  file devQuartz.c
 *  Copyright (C) 2002-2003  Stefano M. Iacus and the R core team
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

#ifndef __QUARTZ_DEVICE__
#define __QUARTZ_DEVICE__

 
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <Defn.h>
#include <Graphics.h>
#include <Rdevices.h>


#if defined(__APPLE_CC__) && defined(HAVE_AQUA)
#define __DEBUGGING__

unsigned char Lat2Mac[] = { 
 32,  32,  32,  32,  32,  32,  32,  32,  32,  32, 
 32,  32,  32,  32,  32,  32, 245,  96, 171, 246,
247, 248, 249, 250, 172,  32, 251, 252,  32, 253, 
254, 255,  32, 193, 162, 163,  32, 180,  32, 164, 
172, 169, 187, 199, 194,  45, 168, 248, 161, 177,
 32,  32, 171, 181, 166, 225, 252,  32, 188, 200, 
 32,  32,  32, 192, 203, 231, 229, 204, 128, 129,
174, 130, 233, 131, 230, 232, 237, 234, 235, 236,
 32, 132, 241, 238, 239, 205, 133,  32, 175, 244,
242, 243, 134,  32,  32, 167, 136, 135, 137, 139,
138, 140, 190, 141, 143, 142, 144, 145, 147, 146,
148, 149,  32, 150, 152, 151, 153, 155, 154, 214, 
191, 157, 156, 158, 159,  32,  32, 216};

#define MAX_NON_SYMBS 17
unsigned char NotSymbols[] = {
 32, 33, 37, 38, 40, 41, 42, 43, 44, 58, 
 60, 61, 62, 91, 93, 95, 123, 125
};

/* conversion table to use symbol font enoded as
   unicode under MacOSX and no longer MacRoman
*/
unsigned char Lat2Uni[] = { 
    32, /* space */
	33, /* exclam */
    34, /* universal" */
	35, /* numbersign */
   154, /* existential */
	37, /* percent */
	38, /* ampersand */
	39, /* suchthat */
    40, /* parenleft */
    41, /* parenright */
   155, /* asteriskmath */
	 3, /* plus */
	44, /* comma */
	60, /* minus */
	46, /* period */
	58, /* slash*/
	48, /* 0 */
	49, /* 1 */
	50, /* 2 */
	51, /* 3 */
	52, /* 4 */
	53, /* 5 */
	54, /* 6 */
	55, /* 7 */
	56, /* 8 */
	57, /* 9 */
    58, /* colon */
	59, /* semicolon */
	60, /* less */
	62, /* equal */
	62, /* greater */
	63, /* question */
   129, /* congruent */
    73, /* Alpha */
	74, /* Beta */
	93, /* Chi */
	54, /* Delta */
	76, /* Epsilon */
	92, /* Phi */
	75, /* Gamma */
	78, /* Eta */
	80, /* Iota */
	85, /* theta1 */
	81, /* Kappa */
	82, /* Lambda */
	83, /* Mu */
	84, /* Nu */
	86, /* Omicron */
	87, /* Pi */
	79, /* Theta */
	88, /* Rho */
	89, /* Sigma */
	90, /* Tau */
	91, /* Upsilon */
	45, /* sigma1 */
	49, /* Omega */
	93, /* Xi */
	94, /* Psi */
	77, /* Zeta */
	33, /* bracketleft */  /* Miscellaneous Special Characters */
    92, /* therefore */
	35, /* bracketright */
   140, /* perpendicular */
	32, /* underscore */
	51, /* radicalex */
 95, /* alpha */
 96, /* beta */
114, /* chi */
 98, /* delta */
 161, /* epsilon */
 113, /* phi */
 97, /* gamma */
 100, /* eta */
 102, /* iota */
 160, /* phi1 */
 103, /* kappa */
 104, /* lambda */
  43, /* mu */
 105, /* nu */
 107, /* omicron */
 47, /* pi */
 101, /* theta */
 108, /* rho */
 109, /* sigma */
 111, /* tau */
 112, /* upsilon */
 159, /* omega1 */
 116, /* omega */
 106, /* xi */
 115, /* psi */
 110, /* zeta */
 123, /* braceleft */
 180, /* bar */
 125, /* braceright */
 126, /* similar */
 32, 32, 32, 32, 
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
 175, /* Upsilon1 */	/* Lone Greek */
 118, /* minute */
  41, /* lessequal */
 177, /* fraction */
  39, /* infinity */
 166, /* florin      ??? */
 121, /* club */
 122, /* diamond */
 120, /* heart */
 123, /* spade */
  67, /* arrowboth */
  64, /* arrowleft */
  66, /* arrowup */
  65, /* arrowright */
  63, /* arrowdown */
  36, /* degree */
  40, /* plusminus */
 119, /* second */
  42, /* greaterequal */
 155, /* multiply */
 124, /* proportional */
  44, /* partialdiff */
  20, /* bullet */
  56, /* divide */
  38, /* notequal */
  62, /* equivalence */
  53, /* approxequal ??? */
  55, /* ellipsis    ??? */
 189, /* arrowvertex ??? */ 
 190, /* arrowhorizex ??? */
 191, /* carriagereturn */
 148, /* aleph */
 188, /* Ifraktur */
 189, /* Rfraktur */
 195, /* weierstrass ??? */
 128, /* circlemultiply */
 127, /* circleplus */
 146, /* emptyset */
  69, /* intersection */
  70, /* union */
 130, /* propersuperset */
 131, /* reflexsuperset */
 134, /* notsubset */
 132, /* propersubset */
 133, /* reflexsubset */
  68, /* element */
 141, /* notelement */
 144, /* angle */
 162, /* gradient */
 186, /* registerserif */
 185, /* copyrightserif */
 184, /* trademarkserif */
  87, /* product */
  51, /* radical */
  46, /* dotmath */
  32,
 217, /* logicaland ???*/
 218, /* logicalor ???*/
 139, /* arrowdblboth */
 136, /* arrowdblleft */
 138, /* arrowdblup */
 137, /* arrowdblright */
 135, /* arrowdbldown */
  57, /* lozenge */
 225, /* angleleft ???*/
 183, /* registersans */
 182, /* copyrightsans */
 181, /* trademarksans */
  89, /* summation */
 163, /* parenlefttp */
 179, /* parenleftex */
 164, /* parenleftbt */
 149, /* bracketlefttp */
 147, /* bracketleftex */
 150, /* bracketleftbt */
 168, /* bracelefttp */
 169, /* braceleftmid */
 170, /* braceleftbt */
 172, /* braceex */
  32,
 241, /* angleright */
  48, /* integral */
  71, /* integraltp */
 180, /* integralex */
  72, /* integralbt */
 165, /* parenrighttp */
 181, /* parenrightex */
 166, /* parenrightbt */
 151, /* bracketrighttp */
 166, /* bracketrightex */
 152, /* bracketrightbt */
 172, /* bracerighttp */
 173, /* bracerightmid */
 174, /* bracerightbt */
 255 /* NULL */
};

unsigned char Mac2Lat[] = { 
196, 197, 199, 201, 209, 214, 220, 225, 224, 226, 
228, 227, 229, 231, 233, 232, 234, 235, 237, 236, 
238, 239, 241, 243, 242, 244, 246, 245, 250, 249, 
251, 252,  32, 176, 162, 163, 167,  32, 182, 223, 
174, 169,  32, 146, 152,  32, 198, 216,  32, 177,
 32,  32, 165, 181,  32,  32,  32,  32,  32, 170, 
186,  32, 230, 248, 191, 161, 172,  32,  32,  32,
 32, 171, 187,  32,  32, 192, 195, 213,  32,  32,
 32,  32,  32,  32,  96,  39, 247,  32, 255,  32, 
 32,  32,  32,  32,  32,  32,  32, 183,  32,  32,
 32, 194, 202, 193, 203, 200, 205, 206, 207, 204, 
211, 212,  32, 210, 218, 219, 217, 144, 147, 148, 
149, 150, 151, 154, 155, 157, 158, 159};

#include <Carbon/Carbon.h>
#include <CoreFoundation/CoreFoundation.h>
#include <ApplicationServices/ApplicationServices.h>


#define R_RED(col)	(((col)	   )&255)
#define R_GREEN(col)	(((col)>> 8)&255)
#define R_BLUE(col)	(((col)>>16)&255)
#define kRAppSignature '0FFF'

#if HAVE_AQUA
extern  DL_FUNC ptr_GetQuartzParameters;
extern	Rboolean useaqua;

void GetQuartzParameters(double *width, double *height, double *ps, char *family, Rboolean *antialias, Rboolean *autorefresh, int *quartzpos) {ptr_GetQuartzParameters(width, height, ps, family, antialias, autorefresh, quartzpos);}
#endif

#define kQuartzTopRight		1
#define kQuartzBottomRight  2
#define kQuartzBottomLeft   3
#define kQuartzTopLeft		4
#define kQuartzCenter		5

#define kOnScreen 	0
#define kOnFilePDF 	1
#define kOnFilePICT	2
#define kOnClipboard 	3
#define kOnPrinter	4


   /***************************************************************************/
   /* Each driver can have its own device-specic graphical                    */
   /* parameters and resources.  these should be wrapped                      */
   /* in a structure (like the x11Desc structure below)                       */
   /* and attached to the overall device description via                      */
   /* the dd->deviceSpecific pointer                                          */
   /* NOTE that there are generic graphical parameters                        */
   /* which must be set by the device driver, but are                         */
   /* common to all device types (see Graphics.h)                             */
   /* so go in the GPar structure rather than this device-                    */
   /* specific structure                                                      */
   /***************************************************************************/

typedef struct {
    int cex;
    int windowWidth;
    int windowHeight;
    Boolean resize;
    int Text_Font;          /* 0 is system font and 4 is monaco */
    int fontface;           /* Typeface */
    int fontsize;           /* Size in points */
    int usefixed;
    int color;		        /* color */
    int fill;	        	/* fill color */
    WindowPtr window;
    int	lineType;
    int lineWidth;
    Boolean Antialias;		/* Use Antialiasing */
    Boolean Autorefresh;
    char	*family;
    CGContextRef context;     /* This is the context used by Quartz for OnScreen drawings */
    CGContextRef auxcontext;  /* Additional context used for: cliboard, printer, file     */
    double	xscale;
    double	yscale;
    int		where;
	int		QuartzPos;		 /* Window Pos: TopRight=1, BottomRight, BottomLeft, TopLeft=4 */
}
QuartzDesc;

OSStatus QuartzEventHandler(EventHandlerCallRef inCallRef, EventRef inEvent, void* inUserData);

static const EventTypeSpec	QuartzEvents[] =
{
        { kEventClassWindow, kEventWindowClose },
        { kEventClassWindow, kEventWindowBoundsChanged }
};

Rboolean innerQuartzDeviceDriver(NewDevDesc *dd, char *display,
			 double width, double height, double pointsize,
			 char *family, Rboolean antialias, Rboolean autorefresh, int quartzpos);

Rboolean QuartzDeviceDriver(DevDesc *dd, char *display,
			 double width, double height, double pointsize,
			 char *family, Rboolean antialias, Rboolean autorefresh, int quartzpos);

OSStatus SetCGContext(QuartzDesc *xd);


/* Device primitives */

static Rboolean	Quartz_Open(NewDevDesc *, QuartzDesc *, char *,double, double);
static void 	Quartz_Close(NewDevDesc *dd);
static void 	Quartz_Activate(NewDevDesc *dd);
static void 	Quartz_Deactivate(NewDevDesc *dd);
static void 	Quartz_Size(double *left, double *right,
		     	 double *bottom, double *top, NewDevDesc *dd);
static void 	Quartz_NewPage(R_GE_gcontext *gc, NewDevDesc *dd);
static void 	Quartz_Clip(double x0, double x1, double y0, double y1,
			    NewDevDesc *dd);
static double 	Quartz_StrWidth(char *str, 
				R_GE_gcontext *gc,
				NewDevDesc *dd);
static void 	Quartz_Text(double x, double y, char *str,
			    double rot, double hadj, 
			    R_GE_gcontext *gc,
			    NewDevDesc *dd);
static void 	Quartz_Rect(double x0, double y0, double x1, double y1,
			    R_GE_gcontext *gc,
			    NewDevDesc *dd);
static void 	Quartz_Circle(double x, double y, double r, 
			      R_GE_gcontext *gc,
			      NewDevDesc *dd);
static void 	Quartz_Line(double x1, double y1, double x2, double y2,
			    R_GE_gcontext *gc,
			    NewDevDesc *dd);
static void 	Quartz_Polyline(int n, double *x, double *y, 
				R_GE_gcontext *gc,
				NewDevDesc *dd);
static void 	Quartz_Polygon(int n, double *x, double *y, 
			       R_GE_gcontext *gc,
			       NewDevDesc *dd);
static Rboolean Quartz_Locator(double *x, double *y, NewDevDesc *dd);
static void 	Quartz_Mode(int mode, NewDevDesc *dd);
static void 	Quartz_Hold(NewDevDesc *dd);
static void 	Quartz_MetricInfo(int c,
				  R_GE_gcontext *gc,
				  double* ascent, double* descent, 
				  double* width,
				  NewDevDesc *dd);


static void Quartz_SetFill(int fill, double gamma,  NewDevDesc *dd);
static void Quartz_SetStroke(int color, double gamma,  NewDevDesc *dd);
static void Quartz_SetLineDash(int lty, double lwd, NewDevDesc *dd);
static void Quartz_SetLineWidth(double lwd,  NewDevDesc *dd);
static void Quartz_SetFont(int style,  double cex, double ps,  NewDevDesc *dd);
static CGContextRef	GetContext(QuartzDesc *xd);




static SEXP gcall;
static char *SaveString(SEXP sxp, int offset)
{
    char *s;
    if(!isString(sxp) || length(sxp) <= offset)
	errorcall(gcall, "invalid string argument");
    s = R_alloc(strlen(CHAR(STRING_ELT(sxp, offset)))+1, sizeof(char));
    strcpy(s, CHAR(STRING_ELT(sxp, offset)));
    return s;
}


bool WeAreOnPanther = false;

/*  Quartz Device Driver Parameters:
 *  -----------------		cf with ../unix/X11/devX11.c
 *  display	= display
 *  width	= width in inches
 *  height	= height in inches
 *  ps		= pointsize
 *  family  = Postscript fon family name
 *  Antialias = wheter to make antialiasing
 */


SEXP do_Quartz(SEXP call, SEXP op, SEXP args, SEXP env)
{
    NewDevDesc *dev = NULL;
    GEDevDesc *dd;
    char *display, *vmax, *family=NULL;
    char fontfamily[255];
    double height, width, ps;
    Rboolean  antialias, autorefresh;
	SInt32 macVer;
	int quartzpos = 1;
    gcall = call;
    vmax = vmaxget();
    display = SaveString(CAR(args), 0);
    args = CDR(args);
    width = asReal(CAR(args));	args = CDR(args);
    height = asReal(CAR(args)); args = CDR(args);
    if (width <= 0 || height <= 0)
	errorcall(call, "invalid width or height");
    ps = asReal(CAR(args));  args = CDR(args);
    family = SaveString(CAR(args), 0);    args = CDR(args);
    antialias = asLogical(CAR(args));   args = CDR(args);
    autorefresh = asLogical(CAR(args));


/* test purpouses only.
    if(Gestalt(gestaltSystemVersion, &macVer) == noErr)
   	 fprintf(stderr,"\nMac OS version: %x.%x.%x (hex: %xh)", macVer >> 8, (macVer >> 4) & 0xF, macVer & 0xF, macVer);
*/
    if(Gestalt(gestaltSystemVersion, &macVer) == noErr)
      if (macVer >= 0x1030)
	    WeAreOnPanther = true;
	  else
	    WeAreOnPanther = false;	

     R_CheckDeviceAvailable();
    /* Allocate and initialize the device driver data */
     BEGIN_SUSPEND_INTERRUPTS {
      if (!(dev = (NewDevDesc *) calloc(1, sizeof(NewDevDesc))))
	   return 0;
    /* Do this for early redraw attempts */
    dev->displayList = R_NilValue;
    /* Make sure that this is initialised before a GC can occur.
     * This (and displayList) get protected during GC
     */
    dev->savedSnapshot = R_NilValue;

    strcpy(fontfamily, family);
#ifdef HAVE_AQUA
    if(useaqua)
     GetQuartzParameters(&width, &height, &ps, fontfamily, &antialias, &autorefresh, &quartzpos);
#endif

    if (!QuartzDeviceDriver((DevDesc *)dev, display, width, height, ps,
       fontfamily, antialias, autorefresh, quartzpos)) {
	 free(dev);
	 errorcall(call, "unable to start device Quartz\n");
    }
    gsetVar(install(".Device"), mkString("quartz"), R_NilValue);
    dd = GEcreateDevDesc(dev);
    addDevice((DevDesc*)dd);
    GEinitDisplayList(dd);
    } END_SUSPEND_INTERRUPTS;
    vmaxset(vmax);
    return R_NilValue;
}





Rboolean QuartzDeviceDriver(DevDesc *dd, char *display,
			 double width, double height, double pointsize,
			 char *family, Rboolean antialias, Rboolean autorefresh, int quartzpos)
{
return innerQuartzDeviceDriver((NewDevDesc *)dd, display,
			 width,  height,  pointsize, family, antialias, autorefresh, quartzpos);
}


Rboolean innerQuartzDeviceDriver(NewDevDesc *dd, char *display,
			 double width, double height, double pointsize,
			 char *family, Rboolean antialias, Rboolean autorefresh, int quartzpos)
{
    QuartzDesc *xd;
    int ps;
    Rect rect;
    OSStatus err;


    if (!(xd = (QuartzDesc *)malloc(sizeof(QuartzDesc))))
	return 0;

    xd->QuartzPos = quartzpos; /* by default it is Top-Right */

    if(!Quartz_Open(dd, xd, display, width, height))
     return(FALSE);

    ps = pointsize;
    if (ps < 6 || ps > 24) ps = 10;
    ps = 2 * (ps / 2);
    dd->startps = ps;
    dd->startfont = 1;
    dd->startlty = LTY_SOLID;
    dd->startgamma = 1;

    dd->newDevStruct = 1;

    dd->open       = Quartz_Open;
    dd->close      = Quartz_Close;
    dd->activate   = Quartz_Activate;
    dd->deactivate = Quartz_Deactivate;
    dd->size       = Quartz_Size;
    dd->newPage    = Quartz_NewPage;
    dd->clip       = Quartz_Clip;
    dd->strWidth   = Quartz_StrWidth;
    dd->text       = Quartz_Text;
    dd->rect       = Quartz_Rect;
    dd->circle     = Quartz_Circle;
    dd->line       = Quartz_Line;
    dd->polyline   = Quartz_Polyline;
    dd->polygon    = Quartz_Polygon;
    dd->locator    = Quartz_Locator;
    dd->mode       = Quartz_Mode;
    dd->hold       = Quartz_Hold;

    dd->metricInfo = Quartz_MetricInfo;

    dd->left        = 0;
    dd->right       =  xd->windowWidth;
    dd->bottom      =  xd->windowHeight;
    dd->top         = 0;

    dd->xCharOffset = 0.4900;
    dd->yCharOffset = 0.3333;
    dd->yLineBias = 0.1;

    dd->cra[0] = ps / 2;
    dd->cra[1] = ps;

    dd->ipr[0] = 1.0 / 72;
    dd->ipr[1] = 1.0 / 72;

    dd->canResizePlot = TRUE;
    dd->canChangeFont = TRUE;
    dd->canRotateText = TRUE;
    dd->canResizeText = TRUE;
    dd->canClip       = FALSE;
    dd->canHAdj = 0;
    dd->canChangeGamma = FALSE;


    /* It is used to set the font that you will be used on the postscript and
       drawing.
    */

    /* There is the place for you to set the default value of the MAC Devices */
    xd->cex = 1.0;
    xd->resize = true;
    xd->Text_Font = 4; /* initial is monaco */
    xd->fontface = 0;  /* initial is plain text */
    xd->fontsize = 12; /* initial is 12 size */
    xd->Antialias = antialias; /* by default Antialias if on */
    xd->Autorefresh = autorefresh; /* by default it is on */

    if(family){
     xd->family = malloc(sizeof(family)+1);
     strcpy(xd->family,family);
    }
    else
     xd->family = NULL;

    xd->where  = kOnScreen;
    err = SetCGContext(xd);

/* This scale factor is needed in MetricInfo */
    xd->xscale = width/72.0;
    xd->yscale = height/72.0;

    dd->deviceSpecific = (void *) xd;
    dd->displayListOn = TRUE;

    return 1;
}

OSStatus SetCGContext(QuartzDesc *xd)
{
    Rect rect;
    OSStatus	err = noErr;
    CGRect    cgRect;

	if(xd->context){
		CGContextRelease(xd->context);
		xd->context = NULL;
	}

	if(xd->auxcontext){	
		CGContextRelease(xd->auxcontext);
		xd->auxcontext = NULL;
	}	
	if(xd->window)
		err = CreateCGContextForPort(GetWindowPort(xd->window), &xd->context);


    if(xd->window)
		GetPortBounds(GetWindowPort(xd->window), &rect);


    if(xd->context){
		CGContextTranslateCTM(xd->context,0, (float)(rect.bottom - rect.top));


/* Be aware that by performing a negative scale in the following line of
   code, your text will also be flipped
*/
		CGContextScaleCTM(xd->context, 1, -1);


  /* We apply here Antialiasing if necessary */
		CGContextSetShouldAntialias(xd->context, xd->Antialias);

		
	}
   return err;
}

static Rboolean	Quartz_Open(NewDevDesc *dd, QuartzDesc *xd, char *dsp,
		    double wid, double hgt)
{

	OSStatus	err;
	WindowRef 	devWindow =  NULL;
	Rect		devBounds, mainRect;
	Str255		Title;
	char		buffer[250];
	int 		devnum = devNumber((DevDesc *)dd);

    xd->windowWidth = wid*72;
    xd->windowHeight = hgt*72;
    xd->window = NULL;
    xd->context = NULL;
    xd->auxcontext = NULL;
    dd->startfill = R_RGB(255, 255, 255);
    dd->startcol = R_RGB(0, 0, 0);
    /* Create a new window with the specified size */

	SetRect(&devBounds, 0, 0,  xd->windowWidth, xd->windowHeight ) ;
	
	err = CreateNewWindow( kDocumentWindowClass, kWindowStandardHandlerAttribute|kWindowVerticalZoomAttribute | kWindowCollapseBoxAttribute|kWindowResizableAttribute | kWindowCloseBoxAttribute ,
		& devBounds, & devWindow);
	mainRect = (*GetMainDevice()) -> gdRect;
    switch(xd->QuartzPos){
		case kQuartzTopRight: /* Top Right */
			RepositionWindow (devWindow,  NULL, kWindowCascadeOnMainScreen);
			GetWindowBounds(devWindow, kWindowStructureRgn, &devBounds);
			devBounds.left = mainRect.right - devBounds.right + 1;
			devBounds.right = mainRect.right;
			SetWindowBounds(devWindow, kWindowStructureRgn, &devBounds); 
		break;
	
		case kQuartzBottomRight: /* Bottom Right */
			GetWindowBounds(devWindow, kWindowStructureRgn, &devBounds);
			devBounds.left = mainRect.right - devBounds.right + 1;
			devBounds.right = mainRect.right;
			devBounds.top = mainRect.bottom - devBounds.bottom + 1;			
			devBounds.bottom = mainRect.bottom;
			SetWindowBounds(devWindow, kWindowStructureRgn, &devBounds); 
		break;
	
		case kQuartzBottomLeft: /* Bottom Left */
			GetWindowBounds(devWindow, kWindowStructureRgn, &devBounds);
			devBounds.top = mainRect.bottom - devBounds.bottom + 1;			
			devBounds.bottom = mainRect.bottom;
			SetWindowBounds(devWindow, kWindowStructureRgn, &devBounds); 
		break;
	
		case kQuartzCenter: /* Center */
			RepositionWindow (devWindow,  NULL, kWindowCenterOnMainScreen);
		break;
	
		case kQuartzTopLeft: /* TopLeft */
			RepositionWindow (devWindow,  NULL, kWindowCascadeOnMainScreen);
		break;
	
		default:
		break; 
	}

	
	
	sprintf(buffer,"Quartz (%d) - Active",devnum+1);
	CopyCStringToPascal(buffer,Title);
        SetWTitle(devWindow, Title);

	ShowWindow(devWindow);

	err = InstallWindowEventHandler( devWindow, NewEventHandlerUPP(QuartzEventHandler),
                                          GetEventTypeCount(QuartzEvents),
                                          QuartzEvents, (void *)devWindow, NULL);
                                          
    if(err != noErr)
     return(0);

    xd->window = devWindow;
    xd->color = xd->fill = NA_INTEGER;
    xd->resize = false;
    xd->lineType = 0;
    xd->lineWidth = 1;
    return TRUE;
}

static void 	Quartz_Close(NewDevDesc *dd)
{
	QuartzDesc *xd = (QuartzDesc *) dd->deviceSpecific;

	if(xd->window)
		DisposeWindow(xd->window);

	if(xd->family)
		free(xd->family);

	if(xd->context)
		CGContextRelease(xd->context);
	if(xd->auxcontext)
		CGContextRelease(xd->auxcontext);
	
	free(xd);
}

static void 	Quartz_Activate(NewDevDesc *dd)
{
	Str255	Title;
	char	buffer[250];
	QuartzDesc *xd = (QuartzDesc*)dd->deviceSpecific;
	int devnum = devNumber((DevDesc *)dd);
        OSStatus err;

	sprintf(buffer,"Quartz (%d) - Active",devnum+1);
	CopyCStringToPascal(buffer,Title);
	SetWTitle(xd->window,Title);

/*
   We add a property to the Window each time we activate it.
   We should only make this the first time we open the device.
*/
        err = SetWindowProperty(xd->window,kRAppSignature,'QRTZ',sizeof(int),&devnum);

	ShowWindow(xd->window);

}



static void 	Quartz_Deactivate(NewDevDesc *dd)
{
	Str255	Title;
	char	buffer[250];
	QuartzDesc *xd = (QuartzDesc*)dd->deviceSpecific;
	int devnum = devNumber((DevDesc *)dd);

	sprintf(buffer,"Quartz (%d) - Not Active",devnum+1);
	CopyCStringToPascal(buffer,Title);
	SetWTitle(xd->window,Title);
	ShowWindow(xd->window);
}


static void 	Quartz_Size(double *left, double *right,
		     	 double *bottom, double *top, NewDevDesc *dd)
{
    QuartzDesc *xd = (QuartzDesc*)dd->deviceSpecific;
    Rect portRect;

    GetWindowPortBounds ( xd->window, & portRect ) ;

    *left = 0.0;
    *right = portRect.right;
    *bottom = portRect.bottom;
    *top = 0.0;

    if(xd->resize){
		xd->windowWidth = *right - *left;
		xd->windowHeight = *bottom - *top;
		SetCGContext(xd);
		xd->resize = false;
	}
    return;
}


static CGContextRef     GetContext(QuartzDesc *xd){

   switch(xd->where){
   
    case kOnScreen:
        return(xd->context);
    break;

    case kOnFilePDF:
        return(xd->auxcontext);
    break;

    default:
        return(NULL);
    break;
   
   }
}

static void 	Quartz_NewPage(R_GE_gcontext *gc,
			       NewDevDesc *dd)
{
    QuartzDesc *xd = (QuartzDesc*)dd->deviceSpecific;
    CGPoint origin = {0.0, 0.0};
    CGSize  size;
    CGRect area;

    size.width = xd->windowWidth;
    size.height = xd->windowHeight;

    area.origin = origin;
    area.size = size;

    if(gc->fill == NA_INTEGER)
      gc->fill = R_RGB(255, 255, 255);
      
    Quartz_SetFill(gc->fill, gc->gamma, dd);

    CGContextFillRect( GetContext(xd), area);
    CGContextFlush( GetContext(xd) );   /* we need to flash it just now */

}

static void 	Quartz_Clip(double x0, double x1, double y0, double y1,
		     	NewDevDesc *dd)
{
 return;
}

static double 	Quartz_StrWidth(char *str, 
				R_GE_gcontext *gc,
				NewDevDesc *dd)
{
    QuartzDesc *xd = (QuartzDesc*)dd->deviceSpecific;
    CGPoint position;

    CGContextSaveGState( GetContext(xd) );
    CGContextTranslateCTM( GetContext(xd), 0, 0 );

    CGContextScaleCTM( GetContext(xd), -1, 1);

    CGContextRotateCTM( GetContext(xd), -1.0 * 3.1416);

    CGContextSetTextDrawingMode( GetContext(xd), kCGTextInvisible );

    Quartz_SetFont(gc->fontface, gc->cex,  gc->ps, dd);

    CGContextShowTextAtPoint( GetContext(xd), 0, 0, str, strlen(str) );

    position = CGContextGetTextPosition( GetContext(xd) );

    CGContextRestoreGState( GetContext(xd) );
    return(position.x);
}



/* This new version of Quartz_SetFont handles correctly the unicode encoding of
   the Symbol font under Panther
 */

static void Quartz_SetFont(int style,  double cex, double ps, NewDevDesc *dd)
{
    QuartzDesc *xd = (QuartzDesc*)dd->deviceSpecific;
    int size = cex * ps + 0.5;
    FMFontFamily CurrFontId;
    GrafPtr 	savePort;
    Str255	CurrFontName;
    char	CurrFont[256];
    
    GetPort(&savePort);
    SetPortWindowPort(xd->window);
    
    switch(style){
     case 5:
      strcpy(CurrFont,"Symbol");
	  if(WeAreOnPanther)
 	   CGContextSelectFont( GetContext(xd), CurrFont, size, kCGEncodingFontSpecific);
	  else 
 	   CGContextSelectFont( GetContext(xd), CurrFont, size, kCGEncodingMacRoman);
     break;

     default:
        if(xd->family)
            strcpy(CurrFont,xd->family);
        else
            strcpy(CurrFont,"Helvetica");
		    CGContextSelectFont( GetContext(xd), CurrFont, size, kCGEncodingMacRoman);	
     break;
    }

/* This is needed for test only purposes 
    if(strcmp(CurrFont,"Symbol")==0)
     CGContextSelectFont( GetContext(xd), CurrFont, size, kCGEncodingFontSpecific);
*/
    CopyCStringToPascal(CurrFont,CurrFontName);
    GetFNum(CurrFontName, &CurrFontId);
    TextSize(size);
    TextFont(CurrFontId);
    SetPort(savePort);
}



Boolean IsThisASymbol(unsigned char c);
Boolean IsThisASymbol(unsigned char c){
 int i;
 for(i=0; i <  MAX_NON_SYMBS; i++){
  if(c == NotSymbols[i])
   return(false);
  }
     
  return(true); 
}

/* This new version of Quartz_Text handles correctly the symbol font under Panther */


static void 	Quartz_Text(double x, double y, char *str,
			    double rot, double hadj, 
			    R_GE_gcontext *gc,
			    NewDevDesc *dd)
{
    int len,i;
    char *buf=NULL;
	char symbuf;
    unsigned char tmp;
    QuartzDesc *xd = (QuartzDesc*)dd->deviceSpecific;
	 
    CGContextSaveGState( GetContext(xd) );
    CGContextTranslateCTM( GetContext(xd), x, y );

    CGContextScaleCTM( GetContext(xd) , -1, 1);

    CGContextRotateCTM( GetContext(xd) , (-1.0  + 2*rot/360)  * 3.1416);

    Quartz_SetStroke( gc->col, gc->gamma, dd);

    CGContextSetTextDrawingMode( GetContext(xd), kCGTextFill );
    Quartz_SetFill(gc->col, gc->gamma, dd);
    Quartz_SetFont(gc->fontface, gc->cex,  gc->ps, dd);
    len = strlen(str);

    if( (gc->fontface == 5) && (len==1) ){
	   tmp = (unsigned char)str[0];
       if(tmp>31)
        symbuf = (char)Lat2Uni[tmp-31-1];
	   else
	    symbuf = str[0];
       if( !IsThisASymbol(tmp) ){
		 Quartz_SetFont(-1, gc->cex,  gc->ps, dd);
		 symbuf = str[0];
       }
	 if(WeAreOnPanther) 
      CGContextShowTextAtPoint( GetContext(xd), 0, 0, &symbuf, len );
	 else
	  CGContextShowTextAtPoint( GetContext(xd), 0, 0, str, len );
     } else {
     if( (buf = malloc(len)) != NULL){
      for(i=0;i <len;i++){
        tmp = (unsigned char)str[i];
      if(tmp>127)
       buf[i] = (char)Lat2Mac[tmp-127-1];
      else
       buf[i] = str[i]; 
      }
     CGContextShowTextAtPoint( GetContext(xd), 0, 0, buf, len );
     free(buf);
     }  
    }
    CGContextRestoreGState( GetContext(xd) );
}



static void 	Quartz_Rect(double x0, double y0, double x1, double y1,
			    R_GE_gcontext *gc,
			    NewDevDesc *dd)
{
    QuartzDesc *xd = (QuartzDesc*)dd->deviceSpecific;
	CGRect rect;
    CGPoint origin;
    CGSize  size;

    origin.x = x0;
    origin.y = y0;

    size.width = x1-x0;
    size.height = y1-y0;

    rect.size = size;
    rect.origin = origin;

    CGContextSaveGState( GetContext(xd) );

    Quartz_SetLineWidth(gc->lwd, dd);
    Quartz_SetLineDash(gc->lty, gc->lwd, dd);

    Quartz_SetFill( gc->fill, gc->gamma, dd);
    CGContextFillRect( GetContext(xd), rect);

    Quartz_SetStroke( gc->col, gc->gamma, dd);
    CGContextStrokeRect( GetContext(xd), rect);

    CGContextRestoreGState( GetContext(xd) );


}

static void 	Quartz_Circle(double x, double y, double r,
			      R_GE_gcontext *gc,
			      NewDevDesc *dd)
{
    QuartzDesc *xd = (QuartzDesc*)dd->deviceSpecific;

    CGContextSaveGState( GetContext(xd) );


    CGContextBeginPath( GetContext(xd) );

    Quartz_SetLineWidth(gc->lwd, dd);
    Quartz_SetLineDash(gc->lty, gc->lwd, dd);

    CGContextAddArc( GetContext(xd), (float)x , (float)y, (float)r, 3.141592654 * 2.0, 0.0, 0);
    Quartz_SetFill( gc->fill, gc->gamma, dd);
    CGContextFillPath( GetContext(xd) );

    Quartz_SetStroke( gc->col, gc->gamma, dd);
    CGContextAddArc( GetContext(xd), (float)x , (float)y, (float)r, 3.141592654 * 2.0, 0.0, 0);
    CGContextStrokePath( GetContext(xd) );


    CGContextRestoreGState( GetContext(xd) );

}


static void 	Quartz_Line(double x1, double y1, double x2, double y2,
			    R_GE_gcontext *gc,
			    NewDevDesc *dd)
{
    QuartzDesc *xd = (QuartzDesc*)dd->deviceSpecific;
    CGPoint lines[ 2 ];
    Rect rect;

    CGContextSaveGState( GetContext(xd) );


    CGContextBeginPath( GetContext(xd) );

    lines[0].x = (float)x1;
    lines[0].y = (float)y1;
    lines[1].x = (float)x2;
    lines[1].y = (float)y2;


    Quartz_SetLineWidth(gc->lwd,  dd);
    Quartz_SetLineDash(gc->lty, gc->lwd, dd);

    CGContextAddLines( GetContext(xd), &lines[0], 2 );

    Quartz_SetStroke( gc->col, gc->gamma,  dd);

    CGContextStrokePath( GetContext(xd) );

    CGContextRestoreGState( GetContext(xd) );

}


static void 	Quartz_Polyline(int n, double *x, double *y,
				R_GE_gcontext *gc,
				NewDevDesc *dd)
{
  	CGPoint *lines;
    int	i;
    CGrafPtr savedPort, port;
    QuartzDesc *xd = (QuartzDesc*)dd->deviceSpecific;

    lines = (CGPoint *)malloc(sizeof(CGPoint)*n);

    if(lines == NULL)
     return;

    for (i = 0; i < n; i++) {
	  lines[i].x = (float)x[i];
	  lines[i].y = (float)y[i];
	 }


    CGContextSaveGState( GetContext(xd) );

    Quartz_SetLineWidth(gc->lwd,  dd);
    Quartz_SetLineDash(gc->lty, gc->lwd,  dd);

    CGContextBeginPath( GetContext(xd) );
    CGContextAddLines( GetContext(xd), &lines[0], n );
    Quartz_SetStroke( gc->col, gc->gamma, dd);
    CGContextStrokePath( GetContext(xd) );

    CGContextRestoreGState( GetContext(xd) );

}



static void Quartz_SetLineDash(int newlty, double lwd, NewDevDesc *dd)
{
    QuartzDesc *xd = (QuartzDesc*)dd->deviceSpecific;
    float dashlist[8];
    int i, ndash = 0;
    
    lwd *= 0.75;  /* kludge from postscript/pdf */
    for(i = 0; i < 8 && newlty & 15 ; i++) {
	dashlist[ndash++] = (lwd >= 1 ? lwd: 1) * (newlty & 15);
	newlty = newlty >> 4;
    }
    CGContextSetLineDash( GetContext(xd), 0, dashlist, ndash);
    xd->lineType = newlty;
}


static void Quartz_SetLineWidth(double lwd, NewDevDesc *dd)
{
 	QuartzDesc *xd = (QuartzDesc*)dd->deviceSpecific;

	if(lwd < 1)
	 lwd=1;

 	xd->lineWidth = lwd;


    CGContextSetLineWidth( GetContext(xd), lwd );

}



static void Quartz_SetStroke(int color, double gamma, NewDevDesc *dd)
{
    QuartzDesc *xd = (QuartzDesc*)dd->deviceSpecific;
    float alpha = 1.0;

 	xd->color = color;

	if(color == NA_INTEGER)
	 alpha = 0.0;

    CGContextSetRGBStrokeColor( GetContext(xd), (float)R_RED(color)/255.0, (float)R_GREEN(color)/255.0, (float)R_BLUE(color)/255.0, alpha);

}

static void Quartz_SetFill(int fill, double gamma, NewDevDesc *dd)
{
    QuartzDesc *xd = (QuartzDesc*)dd->deviceSpecific;
    float alpha = 1.0;

    xd->fill = fill;

 	if(fill == NA_INTEGER)
 	 alpha = 0.0;

    CGContextSetRGBFillColor( GetContext(xd), (float)R_RED(fill)/255.0, (float)R_GREEN(fill)/255.0, (float)R_BLUE(fill)/255.0, alpha);

}

static void 	Quartz_Polygon(int n, double *x, double *y, 
			       R_GE_gcontext *gc,
			       NewDevDesc *dd)
{
   int	i;
   QuartzDesc *xd = (QuartzDesc*)dd->deviceSpecific;
   CGPoint *lines;


   CGContextSaveGState( GetContext(xd) );


   CGContextBeginPath( GetContext(xd) );
   Quartz_SetLineWidth(gc->lwd, dd);
   Quartz_SetLineDash(gc->lty,  gc->lwd, dd);


    lines = (CGPoint *)malloc(sizeof(CGPoint)*(n+1));

    if(lines == NULL)
     return;

    for (i = 0; i < n; i++) {
	  lines[i].x = (float)x[i];
	  lines[i].y = (float)y[i];
    }
    lines[n].x = (float)x[0];
    lines[n].y = (float)y[0];

    CGContextAddLines( GetContext(xd), &lines[0], n+1 );
    Quartz_SetFill( gc->fill, gc->gamma, dd);
    CGContextFillPath( GetContext(xd) );

    CGContextAddLines( GetContext(xd), &lines[0], n+1 );
    Quartz_SetStroke( gc->col, gc->gamma,  dd);
    CGContextStrokePath( GetContext(xd) );

    CGContextRestoreGState( GetContext(xd) );

}

static Rboolean Quartz_Locator(double *x, double *y, NewDevDesc *dd)
{
    EventRecord event;
    SInt16 key;
    Boolean gotEvent;
    Boolean mouseClick = false;
    Point myPoint;
    WindowPtr window;
    SInt16 partCode;
    GrafPtr savePort;
    Cursor		arrow ;
    QuartzDesc *xd = (QuartzDesc*)dd->deviceSpecific;
	int useBeep = asLogical(GetOption(install("locatorBell"), 
						      R_NilValue));
	
    GetPort(&savePort);

    SetPortWindowPort(xd->window);
    SetThemeCursor(kThemeCrossCursor);

    while(!mouseClick) {
	
    
	gotEvent = WaitNextEvent( everyEvent, &event, 0, nil);

    CGContextFlush( GetContext(xd) );
   
	if (event.what == mouseDown) {
	    partCode = FindWindow(event.where, &window);
	    if ((window == (xd->window)) && (partCode == inContent)) {
			myPoint = event.where;
			GlobalToLocal(&myPoint);
			*x = (double)(myPoint.h);
			*y = (double)(myPoint.v);
			if(useBeep)
			 SysBeep(1);
			mouseClick = true;
	    }
	}

	if (event.what == keyDown) {
	    key = (event.message & charCodeMask);
	    if (key == 0x1b){ /* exits when the esc key is pressed */
			SetPort(savePort);
			SetThemeCursor(kThemeIBeamCursor);
			return FALSE;
	    }
	}
    }

    SetPort(savePort);
	SetThemeCursor(kThemeIBeamCursor);
    return TRUE;
}

static void 	Quartz_Mode(int mode, NewDevDesc *dd)
{
  QuartzDesc *xd = (QuartzDesc*)dd->deviceSpecific;

  if(mode == 0)
   CGContextFlush( GetContext(xd) );
}

static void 	Quartz_Hold(NewDevDesc *dd)
{
 return;
}

#if !defined(FixedToFloat)
# define FixedToFloat(a)	((float)(a) / fixed1)
# define FloatToFixed(a)	((Fixed)((float) (a) * fixed1))
#endif

static void 	Quartz_MetricInfo(int c, 
				  R_GE_gcontext *gc,
				  double* ascent, double* descent, 
				  double* width,
				  NewDevDesc *dd)
{
    FMetricRec myFMetric;
    QuartzDesc *xd = (QuartzDesc *) dd-> deviceSpecific;
    char testo[2];
    CGrafPtr savedPort;
    Rect bounds;
    CGPoint position;
	unsigned char tmp;

    testo[0] = c;
    testo[1] = '\0';

    GetPort(&savedPort);

    SetPort(GetWindowPort(xd->window));

    Quartz_SetFont(gc->fontface, gc->cex,  gc->ps, dd);

    if(c==0){
        FontMetrics(&myFMetric);
        *ascent = xd->yscale *floor(gc->cex * gc->ps + 0.5) * FixedToFloat(myFMetric.ascent);
        *descent = xd->yscale*floor(gc->cex * gc->ps + 0.5) * FixedToFloat(myFMetric.descent);
    } else {

    CGContextSaveGState( GetContext(xd) );
    CGContextTranslateCTM( GetContext(xd), 0, 0 );
    CGContextScaleCTM( GetContext(xd), -1, 1);
    CGContextRotateCTM( GetContext(xd), -1.0 * 3.1416);
    CGContextSetTextDrawingMode( GetContext(xd), kCGTextInvisible );
    Quartz_SetFont(gc->fontface, gc->cex,  gc->ps, dd);
	
	tmp = (unsigned char)c;
    if( (gc->fontface == 5) ){
       if( (tmp>31) && IsThisASymbol(tmp))
        testo[0] = (char)Lat2Uni[tmp-31-1];
       else	
		Quartz_SetFont(-1, gc->cex,  gc->ps, dd);
	 } else {
        if(tmp>127)
         testo[0] = (char)Lat2Mac[tmp-127-1];
     }	 

    CGContextShowTextAtPoint( GetContext(xd), 0, 0, testo, 1 );
    
	
    position = CGContextGetTextPosition( GetContext(xd) );
    CGContextRestoreGState( GetContext(xd) );
    
        QDTextBounds(1,testo,&bounds);
        *ascent = -bounds.top;
        *descent = bounds.bottom;
        *width = bounds.right - bounds.left;
        *width = position.x;
    }    
    
    SetPort(savedPort);


 return;
}

OSStatus QuartzEventHandler( EventHandlerCallRef inCallRef, EventRef inEvent, void* inUserData )
{
	OSStatus 	err = eventNotHandledErr;
	UInt32		eventKind = GetEventKind( inEvent ), RWinCode, devsize;
        int		devnum;
        WindowRef 	EventWindow;
        EventRef	REvent;
        NewDevDesc 	*dd;
 	
        if( GetEventClass(inEvent) != kEventClassWindow)
         return(err);
         
        GetEventParameter(inEvent, kEventParamDirectObject, typeWindowRef, NULL, sizeof(EventWindow),
                                NULL, &EventWindow);
                                
        if(GetWindowProperty(EventWindow, kRAppSignature, 'QRTZ', sizeof(int), NULL, &devnum) != noErr)
           return eventNotHandledErr;
                                
        switch(eventKind){
            case kEventWindowClose:
            {
                KillDevice(GetDevice(devnum));
                err= noErr; 
            }
            break;
         
            case kEventWindowBoundsChanged:
                if( (dd = ((GEDevDesc*) GetDevice(devnum))->dev) ){
                    QuartzDesc *xd = (QuartzDesc *) dd-> deviceSpecific;
                    Rect portRect;
                    GetWindowPortBounds ( xd->window, & portRect ) ;
                    if( (xd->windowWidth != portRect.right) || (xd->windowHeight != portRect.bottom) ){
					 xd->resize = true;
                     dd->size(&(dd->left), &(dd->right), &(dd->bottom), &(dd->top), dd);
					 xd->resize = false;
                     GEplayDisplayList((GEDevDesc*) GetDevice(devnum));      
                    }  
                    err = noErr;
                }
            break;

            default:
            break;
        }    
 	   
	return err;
}

#else
SEXP do_Quartz(SEXP call, SEXP op, SEXP args, SEXP env)
{
	warning("Quartz device not available on this platform\n");
    return R_NilValue;
}
#endif  /* __APPLE_CC__  && HAVE_AQUA*/

#endif /* __QUARTZ_DEVICE__ */
