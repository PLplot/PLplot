/* $Id$
   $Log$
   Revision 1.3  1993/01/23 06:12:47  mjl
   Preliminary work on new graphical interface (2.04-specific) for the Amiga.

 * Revision 1.2  1992/10/12  17:11:24  mjl
 * Amiga-specific mods, including ANSI-fication.
 *
 * Revision 1.1  1992/05/20  21:35:26  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

#include "plplot.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "plamiga.h"

void 
eventwait(void)
{
    for (;;) {
	/* Wait for message */
	Wait(1 << PLWindow->UserPort->mp_SigBit);
	if (procmess())
	    return;
    }
}

PLINT 
procmess(void)
{
    ULONG class;
    USHORT code;
    struct IntuiMessage *message;

    while (message = (struct IntuiMessage *) GetMsg(PLWindow->UserPort)) {
	class = message->Class;
	code = message->Code;
	ReplyMsg((struct Message *) message);
	if (eventhandler(class, code))
	    return ((PLINT) 1);
    }
    return ((PLINT) 0);
}

PLINT 
eventhandler(ULONG class, USHORT code)
{
    switch (class) {
    case CLOSEWINDOW:
	return ((PLINT) 1);
    case MENUPICK:
	menuselect(class, code);
	break;
    case SIZEVERIFY:
	break;
    case NEWSIZE:
	if (!(PLCurPrefs.WinType & PLCUST)) {
	    PLCurPrefs.WWidth = PLWindow->Width;
	    PLCurPrefs.WHeight = PLWindow->Height;
	}
	else {
	    PLCurPrefs.CWidth = PLWindow->Width;
	    PLCurPrefs.CHeight = PLWindow->Height;
	}
	setlimits();
	disablegads(0);
	disablemenus();
	remakeplot();
	enablemenus();
	enablegads();
	break;
    default:
	break;
    }

    return ((PLINT) 0);
}

struct IntuiText IText0ItemScrSubs[] =
{
    {0, 1, JAM1, 0, 0, NULL, (UBYTE *) "WorkBench", NULL},
    {0, 1, JAM1, 0, 0, NULL, (UBYTE *) "Custom", NULL}
};

struct MenuItem Menu0ItemScrSubs[] =
{
    {
	&Menu0ItemScrSubs[1],
	0, 0, 0, 0,
	ITEMTEXT | ITEMENABLED | HIGHCOMP | CHECKIT,
	0xFFFE,
	(APTR) & IText0ItemScrSubs[0],
	NULL,
	NULL,
	NULL,
	NULL
    },
    {
	NULL,
	0, 0, 0, 0,
	ITEMTEXT | ITEMENABLED | HIGHCOMP | CHECKIT,
	0xFFFD,
	(APTR) & IText0ItemScrSubs[1],
	NULL,
	NULL,
	NULL,
	NULL
    }
};

struct IntuiText IText0ItemPriSubs[] =
{
    {0, 1, JAM1, 0, 0, NULL, (UBYTE *) "Bitmap Dump", NULL},
    {0, 1, JAM1, 0, 0, NULL, (UBYTE *) "Full Page (Landscape)", NULL},
    {0, 1, JAM1, 0, 0, NULL, (UBYTE *) "Full Page (Portrait)", NULL}
};

struct MenuItem Menu0ItemPriSubs[] =
{
    {
	&Menu0ItemPriSubs[1],
	0, 0, 0, 0,
	ITEMTEXT | ITEMENABLED | HIGHCOMP,
	NULL,
	(APTR) & IText0ItemPriSubs[0],
	NULL,
	NULL,
	NULL,
	NULL
    },
    {
	&Menu0ItemPriSubs[2],
	0, 0, 0, 0,
	ITEMTEXT | ITEMENABLED | HIGHCOMP,
	NULL,
	(APTR) & IText0ItemPriSubs[1],
	NULL,
	NULL,
	NULL,
	NULL
    },
    {
	NULL,
	0, 0, 0, 0,
	ITEMTEXT | ITEMENABLED | HIGHCOMP,
	NULL,
	(APTR) & IText0ItemPriSubs[2],
	NULL,
	NULL,
	NULL,
	NULL
    }
};

struct IntuiText IText0[] =
{
    {0, 1, JAM1, 0, 0, NULL, (UBYTE *) "Save Configuration", NULL},
    {0, 1, JAM1, 0, 0, NULL, (UBYTE *) "Reset", NULL},
    {0, 1, JAM1, 0, 0, NULL, (UBYTE *) "Maintain Plot Aspect", NULL},
    {0, 1, JAM1, 0, 0, NULL, (UBYTE *) "Redraw Enabled", NULL},
    {0, 1, JAM1, 0, 0, NULL, (UBYTE *) "Select Screen Type", NULL},
    {0, 1, JAM1, 0, 0, NULL, (UBYTE *) "Print", NULL},
    {0, 1, JAM1, 0, 0, NULL, (UBYTE *) "Save Bitmap as IFF file", NULL}
};

struct MenuItem Menu0Items[] =
{
    {
	&Menu0Items[1],
	0, 0, 0, 0,
	ITEMTEXT | ITEMENABLED | HIGHCOMP,
	0,
	(APTR) & IText0[0],
	NULL,
	NULL,
	NULL,
	NULL
    },
    {
	&Menu0Items[2],
	0, 0, 0, 0,
	ITEMTEXT | ITEMENABLED | HIGHCOMP,
	0,
	(APTR) & IText0[1],
	NULL,
	NULL,
	NULL,
	NULL
    },
    {
	&Menu0Items[3],
	0, 0, 0, 0,
	ITEMTEXT | ITEMENABLED | HIGHCOMP | CHECKIT | MENUTOGGLE,
	0,
	(APTR) & IText0[2],
	NULL,
	NULL,
	NULL,
	NULL
    },
    {
	&Menu0Items[4],
	0, 0, 0, 0,
	ITEMTEXT | ITEMENABLED | HIGHCOMP | CHECKIT | MENUTOGGLE,
	0,
	(APTR) & IText0[3],
	NULL,
	NULL,
	NULL,
	NULL
    },
    {
	&Menu0Items[5],
	0, 0, 0, 0,
	ITEMTEXT | ITEMENABLED | HIGHCOMP,
	0,
	(APTR) & IText0[4],
	NULL,
	NULL,
	&Menu0ItemScrSubs[0],
	NULL
    },
    {
	&Menu0Items[6],
	0, 0, 0, 0,
	ITEMTEXT | ITEMENABLED | HIGHCOMP,
	0,
	(APTR) & IText0[5],
	NULL,
	NULL,
	&Menu0ItemPriSubs[0],
	NULL
    },
    {
	NULL,
	0, 0, 0, 0,
	ITEMTEXT | ITEMENABLED | HIGHCOMP,
	0,
	(APTR) & IText0[6],
	NULL,
	NULL,
	NULL,
	NULL
    }
};

struct IntuiText IText1Item2Subs[] =
{
    {0, 1, JAM1, 0, 0, NULL, (UBYTE *) " 2", NULL},
    {0, 1, JAM1, 0, 0, NULL, (UBYTE *) " 4", NULL},
    {0, 1, JAM1, 0, 0, NULL, (UBYTE *) " 8", NULL},
    {0, 1, JAM1, 0, 0, NULL, (UBYTE *) "16", NULL}
};

struct MenuItem Menu1Item2Subs[] =
{
    {
	&Menu1Item2Subs[1],
	0, 0, 0, 0,
	ITEMTEXT | ITEMENABLED | HIGHCOMP | CHECKIT,
	0xFFFE,
	(APTR) & IText1Item2Subs[0],
	NULL,
	NULL,
	NULL,
	NULL
    },
    {
	&Menu1Item2Subs[2],
	0, 0, 0, 0,
	ITEMTEXT | ITEMENABLED | HIGHCOMP | CHECKIT,
	0xFFFD,
	(APTR) & IText1Item2Subs[1],
	NULL,
	NULL,
	NULL,
	NULL
    },
    {
	&Menu1Item2Subs[3],
	0, 0, 0, 0,
	ITEMTEXT | ITEMENABLED | HIGHCOMP | CHECKIT,
	0xFFFB,
	(APTR) & IText1Item2Subs[2],
	NULL,
	NULL,
	NULL,
	NULL
    },
    {
	NULL,
	0, 0, 0, 0,
	ITEMTEXT | ITEMENABLED | HIGHCOMP | CHECKIT,
	0xFFF7,
	(APTR) & IText1Item2Subs[3],
	NULL,
	NULL,
	NULL,
	NULL
    }
};

struct IntuiText IText1[] =
{
    {0, 1, JAM1, 0, 0, NULL, (UBYTE *) "Interlaced", NULL},
    {0, 1, JAM1, 0, 0, NULL, (UBYTE *) "High Resolution", NULL},
    {0, 1, JAM1, 0, 0, NULL, (UBYTE *) "Number of Colors", NULL},
    {0, 1, JAM1, 0, 0, NULL, (UBYTE *) "Set Color Palette", NULL}
};


struct MenuItem Menu1Items[] =
{
    {
	&Menu1Items[1],
	0, 0, 0, 0,
	ITEMTEXT | ITEMENABLED | HIGHCOMP | CHECKIT | MENUTOGGLE,
	0,
	(APTR) & IText1[0],
	NULL,
	NULL,
	NULL,
	NULL
    },
    {
	&Menu1Items[2],
	0, 0, 0, 0,
	ITEMTEXT | ITEMENABLED | HIGHCOMP | CHECKIT | MENUTOGGLE,
	0,
	(APTR) & IText1[1],
	NULL,
	NULL,
	NULL,
	NULL
    },
    {
	&Menu1Items[3],
	0, 0, 0, 0,
	ITEMTEXT | ITEMENABLED | HIGHCOMP,
	0,
	(APTR) & IText1[2],
	NULL,
	NULL,
	&Menu1Item2Subs[0],
	NULL
    },
    {
	NULL,
	0, 0, 0, 0,
	ITEMTEXT | ITEMENABLED | HIGHCOMP,
	0,
	(APTR) & IText1[3],
	NULL,
	NULL,
	NULL,
	NULL
    }
};

struct Menu Menu[] =
{
    {
	&Menu[1],
	0, 0, 0, 0,
	MENUENABLED,
	"PLPlot",
	&Menu0Items[0]
    },
    {
	NULL,
	0, 0, 0, 0,
	MENUENABLED,
	"Screen Format",
	&Menu1Items[0]
    }
};

enum menus {
    PLMCONTROL, PLMSCREEN
};

/* Control menu items */
enum menu0items {
    PLMSAVECONFIG, PLMRECONFIG, PLMASPECT, PLMBUFF,
    PLMSCREENTYPE, PLMSCRDUMP, PLMSIFF
};
enum menu0itemscrsubs {
    PLMWBENCH, PLMCUSTOM
};
enum menu0itemprisubs {
    PLMPRWIN, PLMPRPLAN, PLMPRPPOR
};

/* Custom screen menu items */
enum menu1items {
    PLMLACE, PLMHIRES, PLMCOLORS, PLMCOLREQ
};

/* Number color subitems */
enum menu1item2subs {
    PLMC2, PLMC4, PLMC8, PLMC16
};

void 
MakePLMenu(void)
{
    short mtx, mledge;
    short itx, itxlen, itedge, ichk;
    short stx, stxlen, stedge, schk;
    short fheight;
    char *string;
    struct Menu *menu;
    struct MenuItem *item, *subitem;
    struct IntuiText *itext;

    fheight = PLScreen->Font->ta_YSize;

    if (PLCurPrefs.WinType & PLCUST)
	Menu[0].NextMenu = &Menu[1];
    else
	Menu[0].NextMenu = NULL;

    mledge = 0;
    for (menu = Menu; menu != NULL; menu = menu->NextMenu) {
	string = (char *) menu->MenuName;
	mtx = (short) TextLength(PLSRPort, string, (long) strlen(string));
	/* Leave two character space between menus */
	mtx += (2 * mtx) / strlen(string);
	menu->LeftEdge = mledge;
	menu->Width = mtx;
	mledge += mtx;

	/* Find length of longest menuitem in this menu */
	itxlen = 0;
	ichk = 0;
	for (item = menu->FirstItem; item != NULL; item = item->NextItem) {
	    itext = (struct IntuiText *) item->ItemFill;
	    string = (char *) itext->IText;
	    itx = (short) TextLength(PLSRPort, string, (long) strlen(string));
	    itxlen = MAX(itxlen, itx);
	    if (item->Flags & CHECKIT)
		ichk = 1;
	}
	itedge = 0;
	for (item = menu->FirstItem; item != NULL; item = item->NextItem) {
	    item->Width = itxlen;
	    item->LeftEdge = 0;
	    item->TopEdge = itedge;
	    item->Height = fheight;
	    if (ichk) {
		item->Width += CHECKWIDTH;
		itext = (struct IntuiText *) item->ItemFill;
		itext->LeftEdge = CHECKWIDTH;
	    }
	    itedge += fheight;
	    stxlen = 0;
	    schk = 0;
	    for (subitem = item->SubItem; subitem != NULL; subitem = subitem->NextItem) {
		itext = (struct IntuiText *) subitem->ItemFill;
		string = (char *) itext->IText;
		stx = (short) TextLength(PLSRPort, string, (long) strlen(string));
		stxlen = MAX(stxlen, stx);
		if (subitem->Flags & CHECKIT)
		    schk = 1;
	    }
	    stedge = 0;
	    for (subitem = item->SubItem; subitem != NULL; subitem = subitem->NextItem) {
		subitem->Width = stxlen;
		subitem->LeftEdge = item->Width / 2;
		subitem->TopEdge = stedge;
		subitem->Height = fheight;
		if (schk) {
		    subitem->Width += CHECKWIDTH;
		    itext = (struct IntuiText *) subitem->ItemFill;
		    itext->LeftEdge = CHECKWIDTH;
		}
		stedge += fheight;
	    }
	}
    }

    if (PLCurPrefs.WinType & PLCUST) {
	Menu0ItemScrSubs[PLMCUSTOM].Flags |= CHECKED;
	Menu0ItemScrSubs[PLMWBENCH].Flags &= ~CHECKED;
	Menu1Items[PLMLACE].Flags |= ITEMENABLED;
	Menu1Items[PLMHIRES].Flags |= ITEMENABLED;
	Menu1Items[PLMCOLORS].Flags |= ITEMENABLED;
    }
    else {
	Menu0ItemScrSubs[PLMCUSTOM].Flags &= ~CHECKED;
	Menu0ItemScrSubs[PLMWBENCH].Flags |= CHECKED;
    }

    Menu1Item2Subs[PLMC2].Flags &= ~CHECKED;
    Menu1Item2Subs[PLMC4].Flags &= ~CHECKED;
    Menu1Item2Subs[PLMC8].Flags &= ~CHECKED;
    Menu1Item2Subs[PLMC16].Flags &= ~CHECKED;
    Menu1Item2Subs[PLCurPrefs.Depth - 1].Flags |= CHECKED;

    if (PLCurPrefs.ScrType & PLLACE)
	Menu1Items[PLMLACE].Flags |= CHECKED;
    else
	Menu1Items[PLMLACE].Flags &= ~CHECKED;

    if (PLCurPrefs.ScrType & PLHIRES)
	Menu1Items[PLMHIRES].Flags |= CHECKED;
    else
	Menu1Items[PLMHIRES].Flags &= ~CHECKED;

    if (PLCurPrefs.WinType & PLASP)
	Menu0Items[PLMASPECT].Flags |= CHECKED;
    else
	Menu0Items[PLMASPECT].Flags &= ~CHECKED;

    if (PLCurPrefs.WinType & PLBUFF)
	Menu0Items[PLMBUFF].Flags |= CHECKED;
    else
	Menu0Items[PLMBUFF].Flags &= ~CHECKED;

    SetMenuStrip(PLWindow, Menu);
}

void 
disablemenus(void)
{
    OffMenu(PLWindow, PLMCONTROL | SHIFTITEM(NOITEM));
    OffMenu(PLWindow, PLMSCREEN | SHIFTITEM(NOITEM));
}

void 
enablemenus(void)
{
    OnMenu(PLWindow, PLMCONTROL | SHIFTITEM(NOITEM));
    OnMenu(PLWindow, PLMSCREEN | SHIFTITEM(NOITEM));
}

void 
menuselect(ULONG class, USHORT code)
{
    PLINT ItemNumber, MenuNumber, SubNumber;
    PLINT remake = 0;
    struct MenuItem *Item;

    if (class == VANILLAKEY);
    else if (class == MENUPICK) {
	while (code != MENUNULL) {
	    Item = ItemAddress(Menu, code);
	    MenuNumber = MENUNUM(code);
	    ItemNumber = ITEMNUM(code);
	    SubNumber = SUBNUM(code);
	    if (MenuNumber == PLMCONTROL) {
		if (ItemNumber == PLMSAVECONFIG) {
		    if (!(PLCurPrefs.WinType & PLCUST)) {
			PLCurPrefs.WXPos = PLWindow->LeftEdge;
			PLCurPrefs.WYPos = PLWindow->TopEdge;
		    }
		    else {
			PLCurPrefs.CXPos = PLWindow->LeftEdge;
			PLCurPrefs.CYPos = PLWindow->TopEdge;
		    }
		    SetPLDefs();
		}
		else if (ItemNumber == PLMRECONFIG) {
		    RestorePrefs();
		    remake = 1;
		}
		else if (ItemNumber == PLMASPECT) {
		    if (Item->Flags & CHECKED) {
			PLCurPrefs.WinType |= PLASP;
		    }
		    else {
			PLCurPrefs.WinType &= ~PLASP;
		    }
		    setlimits();
		    disablegads(0);
		    disablemenus();
		    remakeplot();
		    enablemenus();
		    enablegads();
		}
		else if (ItemNumber == PLMBUFF) {
		    if (Item->Flags & CHECKED) {
			PLCurPrefs.WinType |= PLBUFF;
		    }
		    else {
			PLCurPrefs.WinType &= ~PLBUFF;
		    }
		}
		else if (ItemNumber == PLMSCREENTYPE) {
		    if (SubNumber == PLMWBENCH) {
			if (!(PLCurPrefs.WinType & PLCUST))
			    goto reselect;
			PLCurPrefs.CXPos = PLWindow->LeftEdge;
			PLCurPrefs.CYPos = PLWindow->TopEdge;
			PLCurPrefs.WinType &= ~PLCUST;
		    }
		    else if (SubNumber == PLMCUSTOM) {
			if (PLCurPrefs.WinType & PLCUST)
			    goto reselect;
			/* Save wbench window position */
			PLCurPrefs.WXPos = PLWindow->LeftEdge;
			PLCurPrefs.WYPos = PLWindow->TopEdge;
			PLCurPrefs.WinType |= PLCUST;
		    }
		    remake = 1;
		}
		else if (ItemNumber == PLMSCRDUMP) {
		    /* Disable system gadgets */
		    disablegads(SubNumber);
		    disablemenus();
		    screendump(SubNumber);
		    enablemenus();
		    enablegads();
		}
		else if (ItemNumber == PLMSIFF) {
		    disablegads(0);
		    disablemenus();
		    saveiff();
		    enablemenus();
		    enablegads();
		}
	    }
	    else if (MenuNumber == PLMSCREEN) {
		if (ItemNumber == PLMLACE) {
		    if (Item->Flags & CHECKED)
			PLCurPrefs.ScrType |= PLLACE;
		    else
			PLCurPrefs.ScrType &= ~PLLACE;
		    PLCurPrefs.CWidth = 0;
		    PLCurPrefs.CHeight = 0;
		    remake = 1;
		}
		else if (ItemNumber == PLMHIRES) {
		    if (Item->Flags & CHECKED)
			PLCurPrefs.ScrType |= PLHIRES;
		    else
			PLCurPrefs.ScrType &= ~PLHIRES;
		    PLCurPrefs.CWidth = 0;
		    PLCurPrefs.CHeight = 0;
		    remake = 1;
		}
		else if (ItemNumber == PLMCOLORS) {
		    if (SubNumber == PLMC2) {
			if (PLCurPrefs.Depth == 1)
			    goto reselect;
			PLCurPrefs.Depth = 1;
		    }
		    else if (SubNumber == PLMC4) {
			if (PLCurPrefs.Depth == 2)
			    goto reselect;
			PLCurPrefs.Depth = 2;
		    }
		    else if (SubNumber == PLMC8) {
			if (PLCurPrefs.Depth == 3)
			    goto reselect;
			PLCurPrefs.Depth = 3;
		    }
		    else if (SubNumber == PLMC16) {
			if (PLCurPrefs.Depth == 4)
			    goto reselect;
			PLCurPrefs.Depth = 4;
		    }
		    PLCurPrefs.CXPos = PLWindow->LeftEdge;
		    PLCurPrefs.CYPos = PLWindow->TopEdge;
		    remake = 1;
		}
		else if (ItemNumber == PLMCOLREQ) {
		    disablegads(1);
		    disablemenus();
		    plcolreq();
		    enablemenus();
		    enablegads();
		}
	    }
	  reselect:
	    code = Item->NextSelect;
	}
    }
    if (remake) {
	ClosePLWind();
	OpenPLWind();
	setlimits();
	disablegads(0);
	disablemenus();
	remakeplot();
	enablemenus();
	enablegads();
    }
}
