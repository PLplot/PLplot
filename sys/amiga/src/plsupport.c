/* $Id$
   $Log$
   Revision 1.4  1993/03/15 21:34:29  mjl
   Reorganization and update of Amiga drivers.  Window driver now uses Amiga
   OS 2.0 capabilities.

 * Revision 1.3  1993/01/23  06:12:48  mjl
 * Preliminary work on new graphical interface (2.04-specific) for the Amiga.
 *
 * Revision 1.2  1992/10/12  17:11:27  mjl
 * Amiga-specific mods, including ANSI-fication.
 *
 * Revision 1.1  1992/05/20  21:35:29  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

#include "plplot.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "plamiga.h"

#include <exec/memory.h>
#include <devices/printer.h>
#include <devices/prtbase.h>
#include <libraries/dosextens.h>
#include <proto/exec.h>

/*----------------------------------------------------------------------*\
* Bitmap builders
\*----------------------------------------------------------------------*/

static struct BitMap Bitmap;
static long bitmapx, bitmapy, wordsperrow;
static short *bitmap;

/* Functions */

int  mapinit	(long, long);
void mapline	(register int, register int, register int, register int);
void mapfree	(void);
void mapclear	(void);

/*----------------------------------------------------------------------*\
* mapinit
* 
* Allocate bitmap.
\*----------------------------------------------------------------------*/

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

/*----------------------------------------------------------------------*\
* mapline
* 
* Draw a line in the bitmap.
\*----------------------------------------------------------------------*/

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

/*----------------------------------------------------------------------*\
* mapfree  -- deallocate bitmap.
* mapclear -- 0-fill bitmap.
\*----------------------------------------------------------------------*/

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

/*----------------------------------------------------------------------*\
* Printer utility routines
\*----------------------------------------------------------------------*/

static union printerIO {
    struct IOStdReq ios;
    struct IODRPReq iodrp;
    struct IOPrtCmdReq iopc;
} *printerIO;

static struct PrinterData *PD;
static struct PrinterExtendedData *PED;
static struct MsgPort *replyport;
static struct RastPort rastport;

static short dummycolors[] =
{0x0000, 0x0fff};
static struct ColorMap dummyColorMap =
{NULL, NULL, 2, (APTR) & dummycolors};

/* Functions */

static void doio	(void);

int  queryprint		(long *, long *, long *, long *, long *, long *);
int  openprinter	(void);
void closeprinter	(void);
void ejectpage		(void);
void dmpport		(long, int, int);
void screendump		(PLINT);

/*----------------------------------------------------------------------*\
* queryprint
* 
* Get preferences printer settings.
\*----------------------------------------------------------------------*/

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

/*----------------------------------------------------------------------*\
* openprinter
* 
* Set up for printer dump.
\*----------------------------------------------------------------------*/

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

/*----------------------------------------------------------------------*\
* closeprinter -- Free resources used in printer dump.
* ejectpage    -- Send a page eject command to printer.
\*----------------------------------------------------------------------*/

void 
closeprinter(void)
{
    CloseDevice((struct IORequest *) printerIO);
    FreeMem((VOID *) printerIO, (long) sizeof(union printerIO));
    DeletePort(replyport);
}

void 
ejectpage(void)
{
    printerIO->ios.io_Command = PRD_RAWWRITE;
    printerIO->ios.io_Data = (APTR) "\014";
    printerIO->ios.io_Length = 1;
    doio();
}

/*----------------------------------------------------------------------*\
* doio
* 
* Send off an i/o request.
\*----------------------------------------------------------------------*/

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

/*----------------------------------------------------------------------*\
* dmpport
* 
* Dump a bitmap to the printer.
\*----------------------------------------------------------------------*/

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

/*----------------------------------------------------------------------*\
* screendump
*
* If type = 0 dump the plplot window to the printer with full preferences
* support, i.e. color, shading, threshold, etc.  Otherwise we override many
* of the preferences selections, create a full page black and white bitmap
* and dump it to the printer.
\*----------------------------------------------------------------------*/

void 
screendump(PLINT type)
{
    register struct IODRPReq *ioreq;
    long bmapx, bmapy;
    long dwidth, dheight;

    if (type != 0)		/* originally also had  && fbuffer == 0 */
	return;
    if (openprinter())		/* return if error */
	return;

    ioreq = &printerIO->iodrp;

/* INDENT OFF */

    if (type == 0) {
	ioreq->io_Command	= PRD_DUMPRPORT;
	ioreq->io_RastPort	= pla->WRPort;
	ioreq->io_ColorMap	= pla->screen->ViewPort.ColorMap;
	ioreq->io_Modes		= pla->screen->ViewPort.Modes;
	ioreq->io_SrcX		= pla->xoffset;
	ioreq->io_SrcY		= pla->yoffset;
	ioreq->io_SrcWidth	= pla->cur_width;
	ioreq->io_SrcHeight	= pla->cur_height;
	ioreq->io_DestCols	= 0;
	ioreq->io_DestRows	= 0;
	ioreq->io_Special	= SPECIAL_FULLROWS | SPECIAL_FULLCOLS |
	                          SPECIAL_TRUSTME;
/* INDENT ON */

	doio();
    }
#if 0
    else {

/* get bmapx and bmapy (the others are dummy variables) */

	queryprint(&bmapx, &bmapy, &dwidth, &dheight, &dwidth, &dheight);

/* Set up map */

	if (mapinit(bmapx, bmapy)) {
	    closeprinter();
	    return;
	}

	dwidth = bmapx - 2;
	dheight = bmapy - 2;

/* PUT PLOT BUFFER READ HERE */
/* NEED TO CALL MAPLINE AS NECESSARY -- SEE OLD CODE */

	dmpport(0L, bmapx, bmapy);
	mapfree();
    }
#endif
    closeprinter();
}

/*----------------------------------------------------------------------*\
* IFF file manipulators
\*----------------------------------------------------------------------*/

#define BADFLAGS    (SPRITES|VP_HIDE|GENLOCK_AUDIO|GENLOCK_VIDEO)
#define FLAGMASK    (~BADFLAGS)
#define CAMGMASK    (FLAGMASK & 0x0000FFFFL)

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

/* Header for a B&W IFF image file. */

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
    U_CHAR r0, g0, b0, r1, g1, b1;
    char bodyname[4];
    long bodylen;		/* fill me in */
} iffhead = { {'F','O','R','M'}, 0, {'I','L','B','M'}, {'B','M','H','D'}, 20,
      0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 320, 200, {'C','M','A','P'}, 6, 240, 240,
      240, 0, 0, 0, {'B','O','D','Y'}, 0 } ;

static long iffpos;
static short curbyte;
static short curcount;
static short runcount;
static FILE *OutFile;

char outchunk[256];

/* Function prototypes */

static void iffobyte	(register int);
static void iffoutbyte	(register int);
static void finishrow	(void);

void iffwritefile	(PLINT, PLINT, FILE *);
int  saveiff		(char *);

/*----------------------------------------------------------------------*\
* saveiff
*
* Writes out color bitmap.
\*----------------------------------------------------------------------*/

int
saveiff(char *filename)
{
    long int formlen = 0, formpos, bmhdlen, camglen, camgbod;
    long int bodylen = 0, bodypos, cmaplen;
    long int numcolors, rowlen, rowoff, coloff;
    short int i, j, k;
    UWORD *coltable;
    UBYTE *byteptr, rgb[3];
    struct BitMap *PLBitMap;
    FILE *IFFFile;

    if ((IFFFile = fopen(filename, "w+")) == NULL) 
	return (1);

    iffpos = 0;
    curbyte = -1;
    curcount = 0;
    runcount = 0;
    OutFile = IFFFile;

    bmhdlen = sizeof(struct BitMapHeader);
    camglen = 4;
    PLBitMap = &pla->screen->BitMap;

    fwrite("FORM", sizeof(char), 4, IFFFile);
    formpos = ftell(IFFFile);
    fwrite((char *) &formlen, sizeof(long), 1, IFFFile);

    fwrite("ILBM", sizeof(char), 4, IFFFile);
    formlen += 4;

    fwrite("BMHD", sizeof(char), 4, IFFFile);
    formlen += 4;
    fwrite((char *) &bmhdlen, sizeof(long), 1, IFFFile);
    formlen += 4;
    bmhd.w = 16 * ((pla->cur_width + 15) / 16);
    bmhd.h = pla->cur_height;
    bmhd.x = pla->window->LeftEdge + pla->xoffset;
    bmhd.y = pla->window->TopEdge + pla->yoffset;
    bmhd.nPlanes = PLBitMap->Depth;
    bmhd.masking = 0;
    bmhd.compression = 1;
    bmhd.pad1 = 0;
    bmhd.transparentColor = 0;
    bmhd.xAspect = 10;
    bmhd.yAspect = 11;
    bmhd.pageWidth = pla->screen->Width;
    bmhd.pageHeight = pla->screen->Height;
    fwrite((char *) &bmhd, bmhdlen, 1, IFFFile);
    formlen += bmhdlen;

    fwrite("CAMG", sizeof(char), 4, IFFFile);
    formlen += 4;
    fwrite((char *) &camglen, sizeof(long), 1, IFFFile);
    formlen += 4;
    camgbod = pla->screen->ViewPort.Modes & CAMGMASK;
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
    coltable = (UWORD *) pla->CMap->ColorTable;
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
    rowlen = ((pla->cur_width + 15) / 16);
    rowlen *= 2;
    rowoff = (pla->window->TopEdge + pla->yoffset) * PLBitMap->BytesPerRow;
    coloff = (pla->window->LeftEdge + pla->xoffset) / 8;
    for (i = 0; i < pla->cur_height; i++) {
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
    return (0);
}

/*----------------------------------------------------------------------*\
* iffwritefile
*
* Writes out B&W bitmap.
\*----------------------------------------------------------------------*/

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

/*----------------------------------------------------------------------*\
* IFF utility routines
\*----------------------------------------------------------------------*/

static void 
iffobyte(register int b)
{
    putc(b, OutFile);
    iffpos++;
}

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

