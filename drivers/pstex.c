/* $Id$

	PLplot pstex (Postscript/LaTeX) device driver.
*/

#include "plplot/plDevs.h"

#ifdef PLD_pstex

#include "plplot/plplotP.h"
#include "plplot/drivers.h"
#include "plplot/ps.h"

/*--------------------------------------------------------------------------*\
 * plD_init_pstex()
 *
 * Initialize device (terminal).
\*--------------------------------------------------------------------------*/

static void parse_str(const char *str, char *dest);
static void proc_str (PLStream *pls, EscText *args);
static long cur_pos;
static FILE *fp;

void
plD_init_pstex(PLStream *pls)
{
  char ofile[80];
  PLFLT rot_scale;

  pls->dev_text = 1; /* want to draw text */
  plD_init_psc(pls); /* init postscript driver-- FIXME: add color/no color to pstex */

  /* open latex output file */
  strncpy(ofile, pls->FileName, 80);
  strcat(ofile, "_t");
  fp = fopen(ofile, "w");

  /* Shouldn't these be set? I have to calculate rot_scale instead */
  /* fprintf(stderr,"%f %f\n", pls->aspdev, pls->aspori); */

  rot_scale = ((int)(pls->diorot) % 2) ? (float)YSIZE/(float)XSIZE : 1.;

  fprintf(fp,"\\begin{picture}(0,0)(0,0)%
\\includegraphics[scale=%f,clip]{%s}%
\\end{picture}%
\\setlength{\\unitlength}{%fbp}%
\\begingroup\\makeatletter\\ifx\\SetFigFont\\undefined%
\\gdef\\SetFigFont#1#2#3#4#5{%
\\reset@font\\fontsize{#1}{#2pt}%
\\fontfamily{#3}\\fontseries{#4}\\fontshape{#5}%
\\selectfont}%
\\fi\\endgroup%\n", 
	  rot_scale, pls->FileName, 72./25.4/pls->xpmm);

  cur_pos = ftell(fp);
  fprintf(fp,"\\begin{picture}(xxxxxx,xxxxxx)(xxxxxx,xxxxxx)%\n");
}

void
plD_esc_pstex(PLStream *pls, PLINT op, void *ptr)
{
  switch (op) {
  case PLESC_HAS_TEXT:
    proc_str(pls, ptr);
    break;
  default:
    plD_esc_ps(pls, op, ptr);
  }
}

void
plD_bop_pstex(PLStream *pls)
{
  plD_bop_ps(pls);
  plGetFam(pls);
}

void
plD_tidy_pstex(PLStream *pls)
{
  PSDev *dev = (PSDev *) pls->dev;
  float rot_scale, scale;

  plD_tidy_ps(pls);
  
  scale = pls->xpmm * 25.4 / 72.;
  rot_scale = ((int)(pls->diorot) % 2) ? (float)YSIZE/(float)XSIZE : 1.;

  fprintf(fp,"\\end{picture}\n");

  fseek(fp, cur_pos, SEEK_SET);
  fprintf(fp,"\\begin{picture}(%d,%d)(%d,%d)%\n%%",
	  ROUND((dev->urx - dev->llx) * scale * rot_scale),
	  ROUND((dev->ury - dev->lly) * scale * rot_scale),
	  ROUND((dev->llx - XOFFSET) * scale * rot_scale),
	  ROUND((dev->lly - YOFFSET) * scale * rot_scale));

  fclose(fp);
}

void
proc_str (PLStream *pls, EscText *args)
{

  PLFLT *t = args->xform;
  PLFLT a1, alpha, ft_ht, angle;
  char cptr[256], jst, ref;
  PSDev *dev = (PSDev *) pls->dev;

  /* font height */
  ft_ht = 1.6 /*!*/ * pls->chrht * 72.0/25.4; /* ft_ht in points. ht is in mm */

  /* calculate baseline text angle */
  angle = pls->diorot * 90.;
  a1 = 180. * acos(t[0]) / M_PI;
  if (t[1] <= 0.)
    alpha = a1 - angle - 90.;
  else
    alpha = 360. - a1 - angle - 90.;

  /* parse string for format (escape) characters */
  parse_str(args->string, cptr);

  /* Reference point (center baseline of string, not latex character reference point). 
     If base = 0, it is aligned with the center of the text box
     If base = 1, it is aligned with the baseline of the text box
     If base = 2, it is aligned with the top of the text box
     Currently plplot only uses base=0 */

  if (args->base == 2) /* not supported by plplot */
    ref = 't';
  else if (args->base == 1)
    ref = 'b';
  else
    ref = 'c';

  /* Text justification.  Left, center and right justification, which
     are the more common options, are supported; variable justification is
     only approximate, based on plplot computation of it's string lenght */
  
  if (args->just == 0.5)
    jst = 'c';
  else if (args->just == 1.)
    jst = 'r';
  else {
    jst = 'l';
    args->x = args->refx; /* use hints provided by plplot */
    args->y = args->refy;
  }

  plRotPhy(pls->diorot+1, dev->xmin, dev->ymin, dev->xmax, dev->ymax,
	   &(args->x), &(args->y));
#ifdef DEBUG
  fprintf(fp,"\\put(%d,%d){\\circle{10}}\n",
	  args->x, args->y);
#endif
  fprintf(fp,"\\put(%d,%d){\\rotatebox{%.1f}{\\makebox(0,0)[%c%c]{\\SetFigFont{%.1f}{12}",
	  args->x, args->y, alpha, jst, ref, ft_ht);

  /* font family, serie and shape. Currently not supported by plplot */
  fprintf(fp,"{\\familydefault}{\\mddefault}{\\updefault}");

  /* font color. */
  fprintf(fp,"\\special{ps: gsave %.3f %.3f %.3f setrgbcolor}{%s}%
\\special{ps: grestore}}}}\n",
	  pls->curcolor.r /255., pls->curcolor.g /255.,
	  pls->curcolor.b /255., cptr);

  /* keep ps driver happy -- needed for background and orientation.
     arghhh! can't calculate it, as I only have the string reference
     point, not its extent!
     The solution will be to use the current latex font size in pt?
     Also need to take into account rotation and justification!
     Quick (and final?) *hack*, ASSUME that no more than a char height
     extents after/before the string reference point. */

  dev->llx = MIN(dev->llx, args->x - ft_ht/2.*10.);
  dev->lly = MIN(dev->lly, args->y - ft_ht/2.*10.);
  dev->urx = MAX(dev->urx, args->x + ft_ht/2.*10.);
  dev->ury = MAX(dev->ury, args->y + ft_ht/2.*10.); 

}

void
parse_str(const char *str, char *dest)
{
  int n, opened = 0, raised = 0, overline = 0, underline = 0, fontset = 0, math = 0;
  char *tp = dest;
  char esc;
  char raise_lower[] = "\\raisebox{%.2fex}{";

  plgesc(&esc);

  while (*str) {
    if (*str != esc) {
      *tp++ = *str++;
      continue;
    }
    str++;

    switch (*str++) {

    case 'u': /* up one level */
      if (raised < 0) {
	*tp++ = '}';
	opened--;
      } else {
	n = sprintf(tp, raise_lower, 0.6);
	tp += n; opened++;
      }
      raised++;
      break;

    case 'd': /* down one level */
      if (raised > 0) {
	*tp++ = '}';
	opened--;
      } else {
	n = sprintf(tp, raise_lower, -0.6);
	tp += n; opened++;
      }
      raised--;
      break;

    case 'b': /* backspace */
      n = sprintf(tp,"\\hspace{-1em}");
      tp += n;
      break;
			 
    case '+': /* toggles overline mode. Side effect, enter math mode. */
      if (overline) {
	if (--math)
	  *tp++ = '}';
	else {
	  n = sprintf(tp, "}$");
	  tp += n;
	}
	overline--; opened--;
      } else {
	if (!math)
	  *tp++ = '$';

	n = sprintf(tp, "\\overline{");
	tp += n; overline++; opened++; math++;
      }
      break;

    case '-': /* toggles underline mode. Side effect, enter math mode. */
      if (underline) {
	if (--math)
	  *tp++ = '}';
	else {
	  n = sprintf(tp, "}$");
	  tp += n;
	}
	underline--; opened--;
      } else {
	if (!math)
	  *tp++ = '$';

	n = sprintf(tp, "\\underline{");		
	tp += n; underline++; opened++; math++;
      }
      break;

    case 'g': /* greek letter corresponding to roman letter x */
      str++;
      break;

    case '(': /* Hershey symbol number (nnn) (any number of digits) FIXME ???*/
      while (*str++ != ')');
      break;

    case 'f': /* switch font */
	  
      switch (*str++) {
      case 'n': /* Normal */
	while (fontset--) {
	  *tp++ = '}';
	  opened--;
	}

	if (math) {
	  *tp++ = '$';
	  math = 0;
	}		  

	n = sprintf(tp, "\\normalfont ");
	tp += n;
	break;

      case 'r': /* Roman */
	if (math)
	  n = sprintf(tp, "\\mathrm{");
	else
	  n = sprintf(tp, "\\textrm{");

	tp += n; opened++; fontset++;
	break;

      case 'i': /* Italic */
	if (math)
	  n = sprintf(tp, "\\mathit{");
	else
	  n = sprintf(tp, "\\textit{");

	tp += n; opened++; fontset++;
	break;

      case 's': /* Script */
	if (!math)
	  *tp++ = '$';

	n = sprintf(tp, "\\mathcal{");
	tp += n; opened++; fontset++; math++;
	break;

      default:
      }

    default:
      if (*str == esc)
	*tp++ = esc;
    }
  }

  while(opened--)
    *tp++ = '}';
  *tp = '\0';

}

#else
int 
pldummy_pstex()
{
  return 0;
}

#endif				/* PLD_pstexdev */
