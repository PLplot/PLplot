/* $Id$
   $Log$
   Revision 1.2  1992/10/12 17:11:27  mjl
   Amiga-specific mods, including ANSI-fication.

 * Revision 1.1  1992/05/20  21:35:29  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

#include "plplot.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "plamiga.h"
#include <libraries/dosextens.h>

extern short fbuffer;
extern FILE *PlotFile;

/* Most of the IFF and preferences stuff borrowed from Tom Rokicki */
static union printerIO {
    struct IOStdReq ios;
    struct IODRPReq iodrp;
    struct IOPrtCmdReq iopc;
} *printerIO;

static struct PrinterData *PD;
static struct PrinterExtendedData *PED;
static struct MsgPort *replyport;
static struct RastPort rastport;
static struct BitMap Bitmap;
static short dummycolors[] =
{0x0000, 0x0fff};
static struct ColorMap dummyColorMap =
{NULL, NULL, 2, (APTR) & dummycolors};


static void 
doio(void)
{
    register struct IODRPReq *ioreq;

    ioreq = &printerIO->iodrp;
    SendIO((struct IORequest *) ioreq);
    while (1) {
	if (GetMsg(replyport) == NULL)
	    Wait(1L << replyport->mp_SigBit);
	else
	    break;
    }
}

void 
dmpport(long flags, int x, int y)
{
    register struct IODRPReq *ioreq;

    ioreq = &printerIO->iodrp;
    ioreq->io_Command = PRD_DUMPRPORT;
    ioreq->io_RastPort = &rastport;
    ioreq->io_ColorMap = &dummyColorMap;
    ioreq->io_Modes = 0;
    ioreq->io_SrcX = 0;
    ioreq->io_SrcY = 0;
    ioreq->io_SrcWidth = x;
    ioreq->io_SrcHeight = y;
    ioreq->io_DestCols = x;
    ioreq->io_DestRows = y;
    ioreq->io_Special = flags | SPECIAL_TRUSTME;
    doio();
}

static long bitmapx, bitmapy, wordsperrow;
static short *bitmap;

int 
mapinit(long bmapx, long bmapy)
{
    bitmapx = bmapx;
    bitmapy = bmapy;
    wordsperrow = (bmapx + 15) / 16;
    Bitmap.BytesPerRow = wordsperrow * 2;
    Bitmap.Rows = bmapy;
    /* Allocate memory for bitmap */
    if ((bitmap = (short *) calloc(wordsperrow * 2, (int) bmapy)) == NULL) {
	fprintf(stderr, "\nCan't allocate memory for bitmap dump.\n");
	return (1);
    }
    Bitmap.Planes[0] = (PLANEPTR) bitmap;
    return (0);
}

void 
mapfree(void)
{
    free((VOID *) bitmap);
}

void 
mapclear(void)
{
    memset((char *) bitmap, '\0', (int) (wordsperrow * (long) bitmapy * 2));
}

/* Function to draw the line in the bitmap */

void 
mapline(register int x1, register int y1, register int x2, register int y2)
{
    register short *p;
    register unsigned int b;
    register int t;
    int d;
    int nextrow;

    /* we always want to go left to right. */
    if (x1 > x2) {
	t = x1;
	x1 = x2;
	x2 = t;
	t = y1;
	y1 = y2;
	y2 = t;
    }
    p = bitmap + y1 * wordsperrow;
    /* we always want to go `up'. */
    if (y2 > y1)
	nextrow = wordsperrow;
    else {
	y2 = y1 * 2 - y2;
	nextrow = -wordsperrow;
    }
    b = 1L << (15 - (x1 & 15));
    p += (x1 >> 4);
    x2 -= x1;
    y2 -= y1;
    /* two routines, one for major in x, one for major in y */
    if (y2 > x2) {
	d = y2;
	t = x2 / 2;
	y1 = y2;
	while (y1 >= 0) {
	    *p |= b;
	    p += nextrow;
	    t += x2;
	    if (t >= d) {
		t -= d;
		b >>= 1;
		if (b == 0) {
		    b = 0x8000;
		    p++;
		}
	    }
	    y1--;
	}
    }
    else {
	d = x2;
	t = y2 / 2;
	x1 = x2;
	while (x1 >= 0) {
	    *p |= b;
	    b >>= 1;
	    if (b == 0) {
		b = 0x8000;
		p++;
	    }
	    t += y2;
	    if (t >= d) {
		t -= d;
		p += nextrow;
	    }
	    x1--;
	}
    }
}

int 
openprinter(void)
{
    replyport = (struct MsgPort *) CreatePort("PLPlot.PIO", 0L);
    if (replyport == NULL) {
	fprintf(stderr, "Couldn't open reply port for printer dump.\n");
	return (1);
    }

    printerIO = (union printerIO *)
	AllocMem((long) sizeof(union printerIO),
		 (long) (MEMF_CLEAR | MEMF_PUBLIC));
    if (printerIO == NULL) {
	DeletePort(replyport);
	fprintf(stderr, "Couldn't create IO request block for printer dump.\n");
	return (1);
    }
    printerIO->ios.io_Message.mn_Node.ln_Type = NT_MESSAGE;
    printerIO->ios.io_Message.mn_Length = sizeof(union printerIO);
    printerIO->ios.io_Message.mn_ReplyPort = replyport;

    if (OpenDevice("printer.device", 0L, (struct IORequest *) printerIO, 0L) != NULL) {
	FreeMem((VOID *) printerIO, (long) sizeof(union printerIO));
	DeletePort(replyport);
	fprintf(stderr, "Error opening printer device.\n");
	return (1);
    }

    PD = (struct PrinterData *) printerIO->iodrp.io_Device;
    PED = &PD->pd_SegmentData->ps_PED;
    return (0);
}

void 
closeprinter(void)
{
    CloseDevice((struct IORequest *) printerIO);
    FreeMem((VOID *) printerIO, (long) sizeof(union printerIO));
    DeletePort(replyport);
}

int 
queryprint(long *bmapx, long *bmapy, long *bmapxmax, long *bmapymax, 
	   long *xdpi, long *ydpi)
{
    int mode;
    Bitmap.BytesPerRow = 0;
    Bitmap.Rows = 0;
    Bitmap.Depth = 1;
    rastport.BitMap = &Bitmap;
    dmpport((long) (SPECIAL_NOPRINT), 0, 0);

    *bmapxmax = PED->ped_MaxXDots;
    *bmapymax = 95 * PED->ped_YDotsInch / 10;	/* 9.5" to prevent problems */

    *xdpi = PED->ped_XDotsInch;
    *ydpi = PED->ped_YDotsInch;

    if (PD->pd_Preferences.PrintFlags & BOUNDED_DIMENSIONS) {
	*bmapx = PD->pd_Preferences.PrintMaxWidth * PED->ped_XDotsInch / 10;
	*bmapy = PD->pd_Preferences.PrintMaxHeight * PED->ped_YDotsInch / 10;
	mode = 1;
    }
    else if (PD->pd_Preferences.PrintFlags & ABSOLUTE_DIMENSIONS) {
	*bmapx = PD->pd_Preferences.PrintMaxWidth * PED->ped_XDotsInch / 10;
	*bmapy = PD->pd_Preferences.PrintMaxHeight * PED->ped_YDotsInch / 10;
	mode = 0;
    }
    else if (PD->pd_Preferences.PrintFlags & PIXEL_DIMENSIONS) {
	*bmapx = PD->pd_Preferences.PrintMaxWidth;
	*bmapy = PD->pd_Preferences.PrintMaxHeight;
	mode = 0;
    }
    else {
	*bmapx = *bmapxmax;
	*bmapy = *bmapymax;
	mode = 1;
    }

    PD->pd_Preferences.PrintImage = IMAGE_NEGATIVE;
    PD->pd_Preferences.PrintShade = SHADE_BW;
    PD->pd_Preferences.PrintThreshold = 7;
    PD->pd_Preferences.PrintAspect = ASPECT_HORIZ;
    PD->pd_Preferences.PrintFlags = INTEGER_SCALING;
    return (mode);
}

void 
ejectpage(void)
{
    printerIO->ios.io_Command = PRD_RAWWRITE;
    printerIO->ios.io_Data = (APTR) "\014";
    printerIO->ios.io_Length = 1;
    doio();
}

/* If type = 0 dump the plplot window to the printer with full preferences
   support, i.e. color, shading, threshold, etc.  Otherwise we override many
   of the preferences selections, create a full page black and white
   bitmap and dump it to the printer. */

void 
screendump(PLINT type)
{
    register struct IODRPReq *ioreq;
    long bmapx, bmapy;
    long cxy, x1, y1;
    long dwidth, dheight, xlas, ylas, xnew, ynew;
    int penwid = 1;

    if (type != 0 && fbuffer == 0)
	return;
    if (openprinter())		/* return if error */
	return;

    ioreq = &printerIO->iodrp;

    if (type == 0) {
	ioreq->io_Command = PRD_DUMPRPORT;
	ioreq->io_RastPort = PLWRPort;
	ioreq->io_ColorMap = PLScreen->ViewPort.ColorMap;
	ioreq->io_Modes = PLScreen->ViewPort.Modes;
	ioreq->io_SrcX = XOffset;
	ioreq->io_SrcY = YOffset;
	ioreq->io_SrcWidth = PLWidth;
	ioreq->io_SrcHeight = PLHeight;
	ioreq->io_DestCols = 0;
	ioreq->io_DestRows = 0;
	ioreq->io_Special = SPECIAL_FULLROWS | SPECIAL_FULLCOLS |
	    SPECIAL_TRUSTME;
	doio();
    }
    else {
	/* get bmapx and bmapy (the others are dummy variables) */
	queryprint(&bmapx, &bmapy, &dwidth, &dheight, &dwidth, &dheight);

	/* Set up map */
	if (mapinit(bmapx, bmapy)) {
	    closeprinter();
	    return;
	}

	prepupdate();

	dwidth = bmapx - 2;
	dheight = bmapy - 2;
	while (!getpoint(&cxy, &x1, &y1)) {
	    if (cxy == PENU) {
		if (type == 1) {
		    xlas = ((long) dheight * (long) x1) / InitPLWidth;
		    ylas = ((long) dwidth * (long) y1) / InitPLHeight;
		}
		else {
		    xlas = ((long) dwidth * (long) x1) / InitPLWidth;
		    ylas = ((long) dheight * (long) y1) / InitPLHeight;
		}
	    }
	    else if (cxy == PEND) {
		if (type == 1) {
		    xnew = ((long) dheight * (long) x1) / InitPLWidth;
		    ynew = ((long) dwidth * (long) y1) / InitPLHeight;
		    switch (penwid) {
		    case 3:
		      mapline(ylas, xlas, ynew, xnew);
		    case 2:
		      mapline(ylas + 2, xlas + 2, ynew + 2, xnew + 2);
		    case 1:
		    default:
		      mapline(ylas + 1, xlas + 1, ynew + 1, xnew + 1);
		    }
		}
		else {
		    xnew = ((long) dwidth * (long) x1) / InitPLWidth;
		    ynew = ((long) dheight * (long) y1) / InitPLHeight;
		    switch (penwid) {
		    case 3:
		      mapline(xlas, dheight - ylas, xnew, dheight - ynew);
		    case 2:
		      mapline(xlas + 2, dheight - ylas + 2, xnew + 1, dheight - ynew + 2);
		    case 1:
		    default:
		      mapline(xlas + 1, dheight - ylas + 1, xnew + 1, dheight - ynew + 1);
		    }
		}
		xlas = xnew;
		ylas = ynew;
	    }
	    else if (cxy == SPEN) {
		;
	    }
	    else if (cxy == PWID) {
		penwid = x1;
	    }
	}
	finiupdate();

	dmpport(0L, bmapx, bmapy);
	mapfree();
    }
    closeprinter();
}

/* prepupdate() flushes and rewinds the plot buffer file.  This should always
   be called before attempting to read the buffer file. */

void 
prepupdate(void)
{
/*    plgfile(PlotFile); */
    if (fbuffer) {
	fflush(PlotFile);
	rewind(PlotFile);
    }
}

/* Use getpoint to read the next command in the plot buffer file. */
/* Returns 0 if okay or 1 if end of file. */

int 
getpoint(long *com, long *x, long *y)
{
    short csh, xsh, ysh;

/*    plgfile(PlotFile); */
    if (!fread((char *) &csh, sizeof(short), 1, PlotFile))
	return (1);

    *com = csh;
    if (csh == PENU || csh == PEND) {
	fread((char *) &xsh, sizeof(short), 1, PlotFile);
	fread((char *) &ysh, sizeof(short), 1, PlotFile);
	*x = xsh;
	*y = ysh;
    }
    else if (csh == SPEN || csh == PWID) {
	fread((char *) &xsh, sizeof(short), 1, PlotFile);
	*x = xsh;
    }

    return (0);
}

void 
finiupdate(void)
{
/*    plgfile(PlotFile); */
    fseek(PlotFile, 0L, 2);
}

static long iffpos;
static short curbyte;
static short curcount;
static short runcount;
static FILE *OutFile;

struct BitMapHeader {
    UWORD w, h;
    WORD x, y;
    UBYTE nPlanes;
    UBYTE masking;
    UBYTE compression;
    UBYTE pad1;
    UWORD transparentColor;
    UBYTE xAspect, yAspect;
    WORD pageWidth, pageHeight;
} bmhd;

static void 
iffobyte(register int b)
{
    putc(b, OutFile);
    iffpos++;
}
char outchunk[256];

static void 
iffoutbyte(register int b)
{
    register int i;

    if (b == curbyte && runcount < 125) {
	runcount++;
    }
    else {
	if (runcount > 2) {
	    if (curcount > 0) {
		iffobyte(curcount - 1);
		for (i = 0; i < curcount; i++)
		    iffobyte(outchunk[i]);
		curcount = 0;
	    }
	    iffobyte(256 - runcount + 1);
	    iffobyte(curbyte);
	}
	else {
	    while (runcount > 0) {
		outchunk[curcount++] = curbyte;
		runcount--;
	    }
	    if (curcount > 110) {
		iffobyte(curcount - 1);
		for (i = 0; i < curcount; i++)
		    iffobyte(outchunk[i]);
		curcount = 0;
	    }
	}
	curbyte = b;
	runcount = 1;
    }
}

static void 
finishrow(void)
{
    register int i;

    if (runcount <= 2) {
	while (runcount > 0) {
	    outchunk[curcount++] = curbyte;
	    runcount--;
	}
    }
    if (curcount > 0) {
	iffobyte(curcount - 1);
	for (i = 0; i < curcount; i++)
	    iffobyte(outchunk[i]);
	curcount = 0;
    }
    if (runcount > 0) {
	iffobyte(256 - runcount + 1);
	iffobyte(curbyte);
	curbyte = -1;
	runcount = 0;
    }
}

#define BADFLAGS    (SPRITES|VP_HIDE|GENLOCK_AUDIO|GENLOCK_VIDEO)
#define FLAGMASK    (~BADFLAGS)
#define CAMGMASK    (FLAGMASK & 0x0000FFFFL)

/* Routine to write out color bitmap. */
void 
saveiff(void)
{
    long int formlen = 0, formpos, bmhdlen, camglen, camgbod;
    long int bodylen = 0, bodypos, cmaplen;
    long int numcolors, rowlen, rowoff, coloff;
    short int i, j, k;
    UWORD *coltable;
    UBYTE *byteptr, rgb[3];
    struct BitMap *PLBitMap;
    char *filename;
    FILE *IFFFile;
    char *plfilereq();

    filename = plfilereq();
    if (!filename)
	return;

    if ((IFFFile = fopen(filename, "w+")) == NULL) {
	fprintf(stderr, "Can't open %s\n", filename);
	return;
    }

    iffpos = 0;
    curbyte = -1;
    curcount = 0;
    runcount = 0;
    OutFile = IFFFile;

    bmhdlen = sizeof(struct BitMapHeader);
    camglen = 4;
    PLBitMap = &PLScreen->BitMap;

    fwrite("FORM", sizeof(char), 4, IFFFile);
    formpos = ftell(IFFFile);
    fwrite((char *) &formlen, sizeof(long), 1, IFFFile);

    fwrite("ILBM", sizeof(char), 4, IFFFile);
    formlen += 4;

    fwrite("BMHD", sizeof(char), 4, IFFFile);
    formlen += 4;
    fwrite((char *) &bmhdlen, sizeof(long), 1, IFFFile);
    formlen += 4;
    bmhd.w = 16 * ((PLWidth + 15) / 16);
    bmhd.h = PLHeight;
    bmhd.x = PLWindow->LeftEdge + XOffset;
    bmhd.y = PLWindow->TopEdge + YOffset;
    bmhd.nPlanes = PLBitMap->Depth;
    bmhd.masking = 0;
    bmhd.compression = 1;
    bmhd.pad1 = 0;
    bmhd.transparentColor = 0;
    bmhd.xAspect = 10;
    bmhd.yAspect = 11;
    bmhd.pageWidth = PLScreen->Width;
    bmhd.pageHeight = PLScreen->Height;
    fwrite((char *) &bmhd, bmhdlen, 1, IFFFile);
    formlen += bmhdlen;

    fwrite("CAMG", sizeof(char), 4, IFFFile);
    formlen += 4;
    fwrite((char *) &camglen, sizeof(long), 1, IFFFile);
    formlen += 4;
    camgbod = PLScreen->ViewPort.Modes & CAMGMASK;
    fwrite((char *) &camgbod, sizeof(long), 1, IFFFile);
    formlen += 4;

    fwrite("CMAP", sizeof(char), 4, IFFFile);
    formlen += 4;
    numcolors = 1;
    for (i = PLBitMap->Depth; i > 0; i--)
	numcolors *= 2;
    cmaplen = numcolors * 3;
    fwrite((char *) &cmaplen, sizeof(long), 1, IFFFile);
    formlen += 4;
    coltable = (UWORD *) PLCMap->ColorTable;
    for (i = 0; i < numcolors; i++) {
	rgb[0] = (*coltable >> 4) & 0xf0;
	rgb[1] = *coltable & 0xf0;
	rgb[2] = (*coltable << 4) & 0xf0;
	fwrite((char *) rgb, sizeof(UBYTE), 3, IFFFile);
	formlen += 3;
	coltable++;
    }

    fwrite("BODY", sizeof(char), 4, IFFFile);
    formlen += 4;
    bodypos = ftell(IFFFile);
    fwrite((char *) &bodylen, sizeof(long), 1, IFFFile);
    formlen += 4;
    rowlen = ((PLWidth + 15) / 16);
    rowlen *= 2;
    rowoff = (PLWindow->TopEdge + YOffset) * PLBitMap->BytesPerRow;
    coloff = (PLWindow->LeftEdge + XOffset) / 8;
    for (i = 0; i < PLHeight; i++) {
	for (j = 0; j < PLBitMap->Depth; j++) {
	    byteptr = (UBYTE *) PLBitMap->Planes[j] + rowoff + coloff;
	    for (k = 0; k < rowlen; k++)
		iffoutbyte(*byteptr++ & 255);
	    finishrow();
	}
	rowoff += PLBitMap->BytesPerRow;
    }
    if (iffpos & 1)
	iffobyte(0);
    bodylen = iffpos;
    formlen += iffpos;
    fflush(IFFFile);
    fseek(IFFFile, formpos, 0);
    fwrite((char *) &formlen, sizeof(long), 1, IFFFile);
    fseek(IFFFile, bodypos, 0);
    fwrite((char *) &bodylen, sizeof(long), 1, IFFFile);
    fclose(IFFFile);
}

/*
 *   Code we steal to write a black and white IFF file.
 */
static struct iffhead {
    char formname[4];
    long formlen;		/* fill me in */
    char ilbmname[4];
    char bmhdname[4];
    long bmhdlen;
    short w, h;			/* fill me in */
    long dummy0;
    char numplanes, masking, compression, pad1;
    short tc;
    char xas, yas;
    short pw, ph;
    char cmapname[4];
    long cmaplen;
    char r0, g0, b0, r1, g1, b1;
    char bodyname[4];
    long bodylen;		/* fill me in */
} iffhead = { {'F','O','R','M'}, 0, {'I','L','B','M'}, {'B','M','H','D'}, 20,
      0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 320, 200, {'C','M','A','P'}, 6, 240, 240,
      240, 0, 0, 0, {'B','O','D','Y'}, 0 } ;

/*
 *   Finally we get into the nitty gritty of writing the stupid file.
 */

void 
iffwritefile(PLINT xdpi, PLINT ydpi, FILE *File)
{
    register int i, j;
    register short *p;

    p = bitmap;
    OutFile = File;
    fwrite((char *) &iffhead, 1, sizeof(struct iffhead), OutFile);
    iffpos = 0;
    curbyte = -1;
    curcount = 0;
    runcount = 0;
    for (j = 0; j < bitmapy; j++) {
	for (i = wordsperrow; i; i--, p++) {
	    iffoutbyte((*p >> 8) & 255);
	    iffoutbyte(*p & 255);
	}
	finishrow();
    }
    if (iffpos & 1)
	iffobyte(0);
    fseek(OutFile, 0L, 0);
    iffhead.w = bitmapx;
    iffhead.h = bitmapy;
    iffhead.pw = bitmapx;
    iffhead.ph = bitmapy;
    iffhead.formlen = iffpos + sizeof(struct iffhead) - 8;
    iffhead.bodylen = iffpos;
    iffhead.xas = xdpi;
    iffhead.yas = ydpi;
    fwrite((char *) &iffhead, 1, sizeof(struct iffhead), OutFile);
}

/* Use 1.3 palette program to set colors. */
/* Note this comes with 2.0 as 'sys:tools/colors' */

void 
plcolreq(void)
{
    short i;
    extern PLINT MaxColors;
    struct FileLock *lock;
    struct Process *thisproc;
    APTR winptr;

    /* This ensures that a requester won't appear if device doesn't exist */
    thisproc = (struct Process *) FindTask(0);
    winptr = thisproc->pr_WindowPtr;
    thisproc->pr_WindowPtr = (APTR) - 1;	/* No window */

    /* Check to see if palette is in tools: or sys:tools */
    /* Note this has been renamed to 'sys:tools/colors' in 2.0 */

    if (lock = (struct FileLock *) Lock("sys:tools/colors", ACCESS_READ)) {
	Execute("sys:tools/colors", 0, 0);
	UnLock((BPTR) lock);
    }
    else if (lock = (struct FileLock *) Lock("tools:palette", ACCESS_READ)) {
	Execute("tools:palette", 0, 0);
	UnLock((BPTR) lock);
    }
    /* Can't find palette so just return */
    else {
	thisproc->pr_WindowPtr = winptr;
	return;
    }

    /* Restore window pointer */
    thisproc->pr_WindowPtr = winptr;

    /* Save new colors in PLCurPrefs. */
    for (i = 0; i < MaxColors; i++)
	PLCurPrefs.Color[i] = GetRGB4(PLCMap, i);

}

#define PLOKGAD    0
#define PLCAGAD    1
#define PLSTGAD    2

static UBYTE Gadget3SIBuff[80];
static struct StringInfo Gadget3SInfo =
{
    Gadget3SIBuff,		/* buffer where text will be edited */
    NULL,			/* optional undo buffer */
    0,				/* character position in buffer */
    80,				/* maximum number of characters to allow */
    0,				/* first displayed character buffer position */
    0, 0, 0, 0, 0,		/* Intuition initialized and maintained variables */
    0,				/* Rastport of gadget */
    0,				/* initial value for integer gadgets */
    NULL			/* alternate keymap (fill in if you set the flag) */
};

static struct Gadget Gadget3 =
{
    NULL,			/* next gadget */
    11, 30,			/* origin XY of hit box relative to window TopLeft */
    158, 9,			/* hit box width and height */
    NULL,			/* gadget flags */
    RELVERIFY + ENDGADGET + STRINGCENTER,	/* activation flags */
    STRGADGET + REQGADGET,	/* gadget type flags */
    NULL,			/* gadget border or image to be rendered */
    NULL,			/* alternate imagery for selection */
    NULL,			/* first IntuiText structure */
    NULL,			/* gadget mutual-exclude long word */
    (APTR) & Gadget3SInfo,	/* SpecialInfo structure */
    PLSTGAD,			/* user-definable data */
    NULL			/* pointer to user-definable data */
};

static SHORT BorderVectors1[] =
{
    0, 0,
    61, 0,
    61, 16,
    0, 16,
    0, 0
};
static struct Border Border1 =
{
    -1, -1,			/* XY origin relative to container TopLeft */
    1, 0, JAM1,			/* front pen, back pen and drawmode */
    5,				/* number of XY vectors */
    BorderVectors1,		/* pointer to XY vectors */
    NULL			/* next border in list */
};

static struct IntuiText IText1 =
{
    3, 0, JAM2,			/* front and back text pens, drawmode and fill byte */
    7, 4,			/* XY origin relative to container TopLeft */
    NULL,			/* font pointer or NULL for default */
    "CANCEL",			/* pointer to text */
    NULL			/* next IntuiText structure */
};

static struct Gadget Gadget2 =
{
    &Gadget3,			/* next gadget */
    110, 65,			/* origin XY of hit box relative to window TopLeft */
    60, 15,			/* hit box width and height */
    NULL,			/* gadget flags */
    RELVERIFY + ENDGADGET,	/* activation flags */
    BOOLGADGET + REQGADGET,	/* gadget type flags */
    (APTR) & Border1,		/* gadget border or image to be rendered */
    NULL,			/* alternate imagery for selection */
    &IText1,			/* first IntuiText structure */
    NULL,			/* gadget mutual-exclude long word */
    NULL,			/* SpecialInfo structure */
    PLCAGAD,			/* user-definable data */
    NULL			/* pointer to user-definable data */
};

static SHORT BorderVectors2[] =
{
    0, 0,
    61, 0,
    61, 16,
    0, 16,
    0, 0
};
static struct Border Border2 =
{
    -1, -1,			/* XY origin relative to container TopLeft */
    1, 0, JAM1,			/* front pen, back pen and drawmode */
    5,				/* number of XY vectors */
    BorderVectors2,		/* pointer to XY vectors */
    NULL			/* next border in list */
};

static struct IntuiText IText2 =
{
    3, 0, JAM2,			/* front and back text pens, drawmode and fill byte */
    24, 4,			/* XY origin relative to container TopLeft */
    NULL,			/* font pointer or NULL for default */
    "OK",			/* pointer to text */
    NULL			/* next IntuiText structure */
};

static struct Gadget Gadget1 =
{
    &Gadget2,			/* next gadget */
    10, 65,			/* origin XY of hit box relative to window TopLeft */
    60, 15,			/* hit box width and height */
    NULL,			/* gadget flags */
    RELVERIFY + ENDGADGET,	/* activation flags */
    BOOLGADGET + REQGADGET,	/* gadget type flags */
    (APTR) & Border2,		/* gadget border or image to be rendered */
    NULL,			/* alternate imagery for selection */
    &IText2,			/* first IntuiText structure */
    NULL,			/* gadget mutual-exclude long word */
    NULL,			/* SpecialInfo structure */
    PLOKGAD,			/* user-definable data */
    NULL			/* pointer to user-definable data */
};

#define GadgetList1 Gadget1

static SHORT BorderVectors4[] =
{
    0, 0,
    161, 0,
    161, 12,
    0, 12,
    0, 0
};
static struct Border Border4 =
{
    9, 28,			/* XY origin relative to container TopLeft */
    3, 0, JAM1,			/* front pen, back pen and drawmode */
    5,				/* number of XY vectors */
    BorderVectors4,		/* pointer to XY vectors */
    NULL			/* next border in list */
};

static SHORT BorderVectors3[] =
{
    0, 0,
    179, 0,
    179, 99,
    0, 99,
    0, 0
};
static struct Border Border3 =
{
    0, 0,			/* XY origin relative to container TopLeft */
    3, 0, JAM1,			/* front pen, back pen and drawmode */
    5,				/* number of XY vectors */
    BorderVectors3,		/* pointer to XY vectors */
    &Border4			/* next border in list */
};

#define BorderList1 Border3

static struct IntuiText IText3 =
{
    3, 0, JAM2,			/* front and back text pens, drawmode and fill byte */
    41, 17,			/* XY origin relative to container TopLeft */
    NULL,			/* font pointer or NULL for default */
    "IFF File Name",		/* pointer to text */
    NULL			/* next IntuiText structure */
};

#define IntuiTextList1 IText3

static struct Requester PLFileReq =
{
    NULL,			/* previous requester (filled in by Intuition) */
    5, 15,			/* requester XY origin relative to TopLeft of window */
    180, 100,			/* requester width and height */
    0, 0,			/* relative to these mouse offsets if POINTREL is set */
    &GadgetList1,		/* gadget list */
    &BorderList1,		/* box's border */
    &IntuiTextList1,		/* requester text */
    NULL,			/* requester flags */
    0,				/* back-plane fill pen */
    NULL,			/* leave these alone */
    NULL,			/* custom bitmap if PREDRAWN is set */
    NULL			/* leave this alone */
};

/* end of PowerWindows source generation */

char *
plfilereq(void)
{
    ULONG oldFlags;
    USHORT gadid;
    char *file;
    struct IntuiMessage *message;

    oldFlags = PLWindow->IDCMPFlags;
    ModifyIDCMP(PLWindow, GADGETUP);
    Request(&PLFileReq, PLWindow);
    Wait(1 << PLWindow->UserPort->mp_SigBit);
    message = (struct IntuiMessage *) GetMsg(PLWindow->UserPort);
    gadid = ((struct Gadget *) message->IAddress)->GadgetID;
    ReplyMsg((struct Message *) message);
    switch (gadid) {
    case PLOKGAD:
	if (Gadget3SIBuff[0] == NULL)
	    file = NULL;
	else
	    file = (char *) Gadget3SIBuff;
	break;
    case PLCAGAD:
	file = NULL;
	break;
    case PLSTGAD:
	if (Gadget3SIBuff[0] == NULL)
	    file = NULL;
	else
	    file = (char *) Gadget3SIBuff;
	break;
    default:
	break;
    }
    ModifyIDCMP(PLWindow, oldFlags);
    return (file);
}

/* This routine disables all the gadgets attached to PLWindow.
   It's useful when saving the window as an IFF file or when dumping it
   to the printer. (We don't want the user moving the window around or
   resizing it during those periods.). We always disable the window sizing
   and close gadgets. If flag is zero we disable the dragging gadget also. */

void 
disablegads(PLINT flag)
{
    int type;
    struct Gadget *gadget;

    gadget = PLWindow->FirstGadget;
    while (gadget) {
	type = gadget->GadgetType & 0x00ff;
	if (type == SIZING || type == CLOSE || (type == WDRAGGING && !flag))
	    OffGadget(gadget, PLWindow, 0);
	gadget = gadget->NextGadget;
    }
}

void 
enablegads(void)
{
    struct Gadget *gadget;

    gadget = PLWindow->FirstGadget;
    while (gadget) {
	OnGadget(gadget, PLWindow, 0);
	gadget = gadget->NextGadget;
    }
    RefreshWindowFrame(PLWindow);
}
