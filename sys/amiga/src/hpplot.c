/* $Id$
   $Log$
   Revision 1.1  1992/05/20 21:35:24  furnish
   Initial checkin of the whole PLPLOT project.

*/

/* This file contains drivers for the HP7475A plotter */

#include "plplot.h"
#include <stdio.h>
#include <string.h>

#define PLTX       10299
#define PLTY        7649

static FILE *OutDev;
static PLINT orient;
static PLINT select=0;
static char FileName[80];

void hp7475setup(xdpi, ydpi, xwid, ywid)
PLINT xwid, ywid;
PLFLT xdpi, ydpi;
{
}

void hp7475select(ori, name)
PLINT ori;
char *name;
{
   orient = ori;
   strncpy(FileName,name,sizeof(FileName)-1);
   FileName[sizeof(FileName)-1] = '\0';
   select = 1;
}

/* Set up device specific stuff and initialize the device */
/* If orient is 0 set up for landscape, otherwise portrait. */
void hp7475init()
{
   char line[80];

   if(!select) {
      printf("Landscape or portrait orientation? (0 or 1) ");
      fgets(line,sizeof(line),stdin);
      if(sscanf(line,"%d",&orient) != 1)
         orient = 0;
   }

   /* setpxl() sets the dots/mm in the x and y directions */
   setpxl((PLFLT)40.,(PLFLT)40.);         /* 40 dots/mm or 1016 dots/inch */

   /* setphy() sets the device coordinates. These are integer */
   /* values. Set up for landscape orientation (long axis of page in the */
   /* x direction). Origin is in the lower left hand corner. */
   if(!orient)
      setphy(0,PLTX,0,PLTY);
   else
      setphy(0,PLTY,0,PLTX);

   /* Set default pen color using scol(color). */
   /* Any default pen color can be used but a black pen is probably best. */
   scol(1);

   /* Set default pen width using swid(width) */
   swid(1);

   /* Set device interaction mode using smod(mode). Set mode to 0 for */
   /* a noninteractive device, Unless you are writing your */
   /* own Amiga screen driver mode should be 0. */
   smod(0);

   /* Well that's all the information plplot needs. Let's prompt for a */
   /* graphics file name. */
   for(;;) {
      if(!select) {
         printf("Enter graphics file name. ");
         fgets(line,sizeof(line),stdin);
         if(sscanf(line,"%s",FileName)!=1)
            continue;
      }

      if (!(OutDev = fopen(FileName,"w"))) {
         fprintf(stderr,"Can't open %s.\n",FileName);
         select = 0;
      }
      else
         break;
   }
   select = 0;
   fprintf(OutDev,"IN; ");
}

/* Sets to text mode */
void hp7475text()
{
   /* None of the built in fonts are supported yet. */
}

/* Sets to graphics mode */
void hp7475graph()
{
   /* We're always in graphics mode with this device. */
}

/* Clears the page */
void hp7475clear()
{
   /* On the HP plotter eject the page. */
   fprintf(OutDev,"PG; ");
}

static PLINT xlast, ylast;

void hp7475page()
{
   xlast = -100000; ylast = -100000;
}

void hp7475width(width)
PLINT width;
{
}

/* Change the pen color */
void hp7475color(color)
PLINT color;
{
   if(color<1 || color>8)
      fprintf(stderr,"Invalid pen selection.\n");
   else
      fprintf(OutDev,"SP %d;",color);
}


/* Draws a line from (x1,y1) to (x2,y2) */
void hp7475line(x1,y1,x2,y2)
PLINT x1,y1,x2,y2;
{

   /* If starting point of this line is the same as the ending point of */
   /* the previous line then don't raise the pen. (This really speeds up */
   /* plotting and reduces the size of the file. */
   if(!orient) {
      if(x1 == xlast && y1 == ylast)
         fprintf(OutDev," %d %d",x2,y2);
      else
         fprintf(OutDev,"\nPU %d %d PD %d %d",x1,y1,x2,y2);
   }
   else {
      if(x1 == xlast && y1 == ylast)
         fprintf(OutDev," %d %d",PLTX-y2,x2);
      else
         fprintf(OutDev,"\nPU %d %d PD %d %d",PLTX-y1,x1,PLTX-y2,x2);
   }

   xlast = x2;
   ylast = y2;
}

/* Cleanup and close file. */
void hp7475tidy()
{
   fprintf(OutDev,"\nSP0");
   fclose(OutDev);
}

void hp7475esc(op, ptr)
int op;
char *ptr;
{
}
