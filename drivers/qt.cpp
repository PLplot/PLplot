/*

  This software is provided under the LGPL in March 2009 by the
  Cluster Science Centre
  QSAS team,
  Imperial College, London

  Copyright (C) 2009  Imperial College, London
  Copyright (C) 2009  Alan W. Irwin

  This is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Lesser Public License as published
  by the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

  This software is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  To received a copy of the GNU Library General Public License
  write to the Free Software Foundation, Inc., 
  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
  
  History:

  
  March 2009:  v1.00
  Initial release.
  
*/


#include "qt.h"

// global variables initialised in init(), used in tidy()
// QApplication* app=NULL;
int argc; // argc and argv have to exist when tidy() is used, thus they are made global
char ** argv;
int appCounter=0; // to be rigorous, all uses should be placed between mutexes

// Drivers declaration
PLDLLIMPEXP_DRIVER const char* plD_DEVICE_INFO_qt = 
#if defined(PLD_bmpqt)
  "bmpqt:Qt Windows bitmap driver:0:qt:66:bmpqt\n"
#endif
#if defined(PLD_jpgqt)
  "jpgqt:Qt jpg driver:0:qt:67:jpgqt\n"
#endif
#if defined(PLD_pngqt)
  "pngqt:Qt png driver:0:qt:68:pngqt\n"
#endif
#if defined(PLD_ppmqt)
  "ppmqt:Qt ppm driver:0:qt:69:ppmqt\n"
#endif
#if defined(PLD_tiffqt)
  "tiffqt:Qt tiff driver:0:qt:70:tiffqt\n"
#endif
#if defined(PLD_svgqt) && QT_VERSION >= 0x040300
  "svgqt:Qt SVG driver:0:qt:71:svgqt\n"
#endif
#if defined(PLD_qtwidget)
  "qtwidget:Qt Widget:1:qt:72:qtwidget\n"
#endif
#if defined(PLD_epsqt)
  "epsqt:Qt EPS driver:0:qt:73:epsqt\n"
#endif
#if defined(PLD_pdfqt)
  "pdfqt:Qt PDF driver:0:qt:74:pdfqt\n"
#endif
#if defined(PLD_extqt)
  "extqt:External Qt driver:0:qt:75:extqt\n"
#endif
;

MasterHandler handler;

bool initQtApp(bool isGUI)
{
	QtPLDriver::mutex.lock();
	bool res=false;
	++appCounter;
	if(qApp==NULL && appCounter==1)
	{
		argc=1;
		argv=new char*[2];
		argv[0]=new char[10];
		argv[1]=new char[1];
		snprintf(argv[0], 10, "qt_driver");
		argv[1][0]='\0';
		new QApplication(argc, argv, isGUI);
		res=true;
	}
	QtPLDriver::mutex.unlock();
	return res;
}

void closeQtApp()
{
	QtPLDriver::mutex.lock();
	--appCounter;
	if(qApp!=NULL && appCounter==0)
	{
		delete qApp;
		delete[] argv[0];
		delete[] argv[1];
		delete[] argv;
		argv=NULL;
	}
	QtPLDriver::mutex.unlock();
}

/*---------------------------------------------------------------------
  qt_family_check ()
  
  support function to help supress more than one page if family file
  output not specified by the user  (e.g., with the -fam command-line option).
  Adapted directly from svg.c
  ---------------------------------------------------------------------*/
static int already_warned = 0;
static int qt_family_check(PLStream *pls)
{
	if (pls->family || pls->page == 1) 
    {
		return 0;
    }
	else 
    {
		if(! already_warned)
		{
			already_warned = 1;
			plwarn("All pages after the first skipped because family file output not specified.\n");
		}
		return 1;
    }
}

// Declaration of the driver-specific interface functions
#if defined (PLD_bmpqt) || defined(PLD_jpgqt) || defined (PLD_pngqt) || defined(PLD_ppmqt) || defined(PLD_tiffqt)
void plD_init_rasterqt(PLStream *);
void plD_eop_rasterqt(PLStream *);
#endif

#if defined (PLD_bmpqt)
void plD_dispatch_init_bmpqt(PLDispatchTable *pdt);
void plD_bop_bmpqt(PLStream *);
#endif

#if defined (PLD_jpgqt)
void plD_dispatch_init_jpgqt(PLDispatchTable *pdt);
void plD_bop_jpgqt(PLStream *);
#endif

#if defined (PLD_pngqt)
void plD_dispatch_init_pngqt(PLDispatchTable *pdt);
void plD_bop_pngqt(PLStream *);
#endif

#if defined (PLD_ppmqt)
void plD_dispatch_init_ppmqt(PLDispatchTable *pdt);
void plD_bop_ppmqt(PLStream *);
#endif

#if defined (PLD_tiffqt)
void plD_dispatch_init_tiffqt(PLDispatchTable *pdt);
void plD_bop_tiffqt(PLStream *);
#endif

#if defined(PLD_svgqt) && QT_VERSION >= 0x040300
void plD_dispatch_init_svgqt(PLDispatchTable *pdt);
void plD_init_svgqt(PLStream *);
void plD_bop_svgqt(PLStream *);
void plD_eop_svgqt(PLStream *);
#endif

#if defined(PLD_epsqt) || defined(PLD_pdfqt)
void plD_init_epspdfqt(PLStream *);
void plD_bop_epspdfqt_helper(PLStream *, int ifeps);
void plD_eop_epspdfqt(PLStream *);
#endif
#if defined(PLD_epsqt)
void plD_dispatch_init_epsqt(PLDispatchTable *pdt);
void plD_bop_epsqt(PLStream *);
#endif
#if defined(PLD_pdfqt)
void plD_dispatch_init_pdfqt(PLDispatchTable *pdt);
void plD_bop_pdfqt(PLStream *);
#endif

#if defined(PLD_qtwidget)
void plD_dispatch_init_qtwidget(PLDispatchTable *pdt);
void plD_init_qtwidget(PLStream *);
void plD_eop_qtwidget(PLStream *);
#endif

#if defined(PLD_extqt)
void plD_dispatch_init_extqt(PLDispatchTable *pdt);
void plD_init_extqt(PLStream *);
void plD_eop_extqt(PLStream *);
#endif

// Declaration of the generic interface functions

void plD_line_qt(PLStream *, short, short, short, short);
void plD_polyline_qt(PLStream *, short*, short*, PLINT);
void plD_bop_qt(PLStream *){}
void plD_tidy_qt(PLStream *);
void plD_state_qt(PLStream *, PLINT);
void plD_esc_qt(PLStream *, PLINT, void*);

///////////// Generic Qt driver class /////////////////

QMutex QtPLDriver::mutex;

QtPLDriver::QtPLDriver(PLINT i_iWidth, PLINT i_iHeight)
{
	m_dWidth=i_iWidth;
	m_dHeight=i_iHeight;
}

QtPLDriver::~QtPLDriver()
{
}

void QtPLDriver::drawLine(short x1, short y1, short x2, short y2)
{
	if(!m_painterP->isActive()) return;
	QLineF line(	(PLFLT)x1*downscale,
			m_dHeight-(PLFLT)y1*downscale,
			(PLFLT)x2*downscale,
			m_dHeight-(PLFLT)y2*downscale
		   );

	m_painterP->drawLine(line);
}

void QtPLDriver::drawPolyline(short * x, short * y, PLINT npts)
{
	if(!m_painterP->isActive()) return;
	QPointF * polyline=new QPointF[npts];
	for(int i=0; i<npts; ++i)
	{
		polyline[i].setX((PLFLT)x[i]*downscale);
		polyline[i].setY(m_dHeight-(PLFLT)y[i]*downscale);
	}
	m_painterP->drawPolyline(polyline, npts);
	delete[] polyline;
}

void QtPLDriver::drawPolygon(short * x, short * y, PLINT npts)
{
	if(!m_painterP->isActive()) return;
	QPointF * polygon=new QPointF[npts];
	for(int i=0; i<npts; ++i)
	{
		polygon[i].setX((PLFLT)x[i]*downscale);
		polygon[i].setY(m_dHeight-(PLFLT)y[i]*downscale);
	}
	m_painterP->drawPolygon(polygon, npts);
	delete[] polygon;
}


QFont QtPLDriver::getFont(PLUNICODE unicode)
{
	// Get new font parameters
	unsigned char fontFamily, fontStyle, fontWeight;

	plP_fci2hex(unicode, &fontFamily, PL_FCI_FAMILY);
	plP_fci2hex(unicode, &fontStyle, PL_FCI_STYLE);
	plP_fci2hex(unicode, &fontWeight, PL_FCI_WEIGHT);
			
	QFont f;

	f.setPointSizeF(currentFontSize*currentFontScale<4 ? 4 : currentFontSize*currentFontScale);

	switch(fontFamily) {
	  case 1:
		f.setStyleHint(QFont::Serif);
// 		f.setFamily("Times");
		break;
	  case 2:
		f.setStyleHint(QFont::TypeWriter);
// 		f.setFamily("Courier");
		break;
	  case 0: case 3: case 4:default:
		f.setStyleHint(QFont::SansSerif);
// 		f.setFamily("Helvetica");
		break;
	}
	f.setFamily(""); // no family name, forcing Qt to find an appropriate font by itself

	if(fontStyle) f.setItalic(true);
	if(fontWeight) f.setWeight(QFont::Bold);
	else f.setWeight(QFont::Normal);
			
	f.setUnderline(underlined);
	f.setOverline(overlined);

	return f;
}

void QtPLDriver::drawTextInPicture(QPainter* p, const QString& text)
{
	if(!m_painterP->isActive()) return;

	QPicture tempPic;
	QPainter tempPainter(&tempPic);
	tempPainter.setFont(p->font());
	QRectF rect(0., 0., 0., 0.);
	QRectF bounding;
	tempPainter.drawText(rect, Qt::AlignHCenter|Qt::AlignVCenter|Qt::TextDontClip, text, &bounding);

// 	tempPainter.drawLine(bounding.left()+1, bounding.bottom(), bounding.right()-1, bounding.bottom());
// tempPainter.drawLine(bounding.left()+1, bounding.top(), bounding.right()-1, bounding.top());

// 	bounding.adjust(-0.5, bounding.height(), -0.5, -bounding.height()/5.); // Empiric adjustment of the true bounding box

	tempPainter.end();

	p->drawPicture(xOffset+bounding.width()/2., -yOffset, tempPic);

	xOffset+=bounding.width();
}

QPicture QtPLDriver::getTextPicture(PLUNICODE* text, int len, int chrht)
{
	char plplotEsc;
	plgesc( &plplotEsc );
	PLUNICODE fci;
	plgfci( &fci );
	
	double old_fontScale;
  
	QPicture res;
	QPainter p(&res);
	
	QString currentString;

	yOffset=0.;
	xOffset=0.;

	// Scaling factor of 1.6 determined empirically to make all qt results
	// have the same character size as cairo results (taking into account
	// the slightly different actual glyph sizes for the different
        // default fonts found by cairo and qt).
	currentFontSize=chrht*POINTS_PER_INCH/25.4*1.45;
	currentFontScale=1.;
	underlined=false;
	overlined=false;
	
	p.setFont(getFont(fci));

	int i=0;
	while(i < len)
	{
		if(text[i]<PL_FCI_MARK) // Not a font change
		{
			if(text[i]!=(PLUNICODE)plplotEsc)
			{
				currentString.append(QString((QChar*)&(text[i]), 1));
				++i;
				continue;
			}
			++i; // Now analyse the escaped character
			switch(text[i])
			{
				case 'd':
					drawTextInPicture(&p, currentString);
					
					currentString.clear();
					old_fontScale=currentFontScale;
					if( yOffset>0.0 )
						currentFontScale *= 1.25;  /* Subscript scaling parameter */
					else
						currentFontScale *= 0.8;  /* Subscript scaling parameter */

					yOffset -= currentFontSize * old_fontScale / 2.;
					
					p.setFont(getFont(fci));
					break;
				case 'u':
					drawTextInPicture(&p, currentString);
					
					currentString.clear();
					if( yOffset<0.0 )
						currentFontScale *= 1.25;  /* Subscript scaling parameter */
					else
						currentFontScale *= 0.8;  /* Subscript scaling parameter */

					yOffset += currentFontSize * currentFontScale / 2.;
					p.setFont(getFont(fci));
					break;
				case '-':
					drawTextInPicture(&p, currentString);
					
					currentString.clear();
					underlined=!underlined;
					p.setFont(getFont(fci));
					break;
				case '+':
					drawTextInPicture(&p, currentString);

					currentString.clear();
					overlined=!overlined;
					p.setFont(getFont(fci));
					break;
				case '#':
					currentString.append(QString((QChar*)&(text[i]), 1));
					break;
				default :
					std::cout << "unknown escape char " << ((QChar)text[i]).toLatin1() << std::endl;
					break;
			}
		}
		else // Font change
		{
			drawTextInPicture(&p, currentString);

			currentString.clear();
			fci=text[i];
			p.setFont(getFont(fci));
		}
		++i;
	}
	drawTextInPicture(&p, currentString);
	
	p.end();
	return res;
}

void QtPLDriver::drawText(PLStream* pls, EscText* txt)
{
	if(!m_painterP->isActive()) return;

	/* Check that we got unicode, warning message and return if not */
	if( txt->unicode_array_len == 0 ) {
		printf( "Non unicode string passed to a Qt driver, ignoring\n" );
		return;
	}
	
	/* Check that unicode string isn't longer then the max we allow */
	if( txt->unicode_array_len >= 500 ) {
		printf( "Sorry, the Qt drivers only handle strings of length < %d\n", 500 );
		return;
	}

	PLFLT rotation, shear, stride;
	plRotationShear( txt->xform, &rotation, &shear, &stride);
	
	double picDpi;
	QPicture picText=getTextPicture(txt->unicode_array, txt->unicode_array_len, pls->chrht);
	picDpi=picText.logicalDpiY();

	m_painterP->setClipping(true);
	m_painterP->setClipRect(QRect(pls->clpxmi*downscale, m_dHeight-pls->clpyma*downscale, (pls->clpxma-pls->clpxmi)*downscale, (pls->clpyma-pls->clpymi)*downscale), Qt::ReplaceClip);

	rotation -= pls->diorot * M_PI / 2.0;
	m_painterP->translate(txt->x*downscale, m_dHeight-txt->y*downscale);
	QMatrix rotShearMatrix(cos(rotation)*stride, -sin(rotation)*stride, cos(rotation)*sin(shear)+sin(rotation)*cos(shear), -sin(rotation)*sin(shear)+cos(rotation)*cos(shear), 0., 0.);

	m_painterP->setWorldMatrix(rotShearMatrix, true);

	m_painterP->translate(-txt->just*xOffset*m_painterP->device()->logicalDpiY()/picDpi, 0.);

	m_painterP->drawPicture(0., 0., picText);

	m_painterP->setWorldMatrix(QMatrix());
	m_painterP->setClipping(false);
}

void QtPLDriver::setColor(int r, int g, int b, double alpha)
{
	if(!m_painterP->isActive()) return;

	QPen p=m_painterP->pen();
	p.setColor(QColor(r, g, b, alpha*255));
	m_painterP->setPen(p);
	
	QBrush B=m_painterP->brush();
	B.setColor(QColor(r, g, b, alpha*255));
	B.setStyle(Qt::SolidPattern);
	m_painterP->setBrush(B);
}

void QtPLDriver::setWidth(PLINT w)
{
	if(!m_painterP->isActive()) return;

	QPen p=m_painterP->pen();
	p.setWidth(w);
	m_painterP->setPen(p);
}

void QtPLDriver::setDashed(PLINT nms, PLINT* mark, PLINT* space)
{
	if(!m_painterP->isActive()) return;

	QVector<qreal> vect;
	for(int i=0; i<nms; ++i)
	{
		vect << (PLFLT)mark[i]*m_painterP->device()->logicalDpiX()/25400.;
		vect << (PLFLT)space[i]*m_painterP->device()->logicalDpiX()/25400.;
	}
	QPen p=m_painterP->pen();
	p.setDashPattern(vect);
	m_painterP->setPen(p);
}

void QtPLDriver::setSolid()
{
	if(!m_painterP->isActive()) return;

	QPen p=m_painterP->pen();
	p.setStyle(Qt::SolidLine);
	m_painterP->setPen(p);
}

// Generic driver interface

void plD_line_qt(PLStream * pls, short x1a, short y1a, short x2a, short y2a)
{
	QtPLDriver * widget=NULL;
	
	// We have to dynamic_cast to make sure the good virtual functions are called
#if defined (PLD_bmpqt) || defined(PLD_jpgqt) || defined (PLD_pngqt) || defined(PLD_ppmqt) || defined(PLD_tiffqt)
	if(widget==NULL) widget=dynamic_cast<QtRasterDevice*>((QtPLDriver *) pls->dev);
	if(widget!=NULL && qt_family_check(pls)) {return;} 
#endif
#if defined(PLD_svgqt) && QT_VERSION >= 0x040300
	if(widget==NULL) widget=dynamic_cast<QtSVGDevice*>((QtPLDriver *) pls->dev);
	if(widget!=NULL && qt_family_check(pls)) {return;} 
#endif
#if defined(PLD_epsqt) || defined(PLD_pdfqt)
	if(widget==NULL) widget=dynamic_cast<QtEPSDevice*>((QtPLDriver *) pls->dev);
	if(widget!=NULL && qt_family_check(pls)) {return;} 
#endif
#if defined(PLD_qtwidget) || defined(PLD_extqt)
	if(widget==NULL) widget=dynamic_cast<QtPLWidget*>((QWidget *) pls->dev);
#endif
	if(widget==NULL) return;
	
	widget->setColor(pls->curcolor.r, pls->curcolor.g, pls->curcolor.b, pls->curcolor.a);
	widget->drawLine(x1a, y1a, x2a, y2a);
}

void plD_polyline_qt(PLStream *pls, short *xa, short *ya, PLINT npts)
{
	QtPLDriver * widget=NULL;

#if defined (PLD_bmpqt) || defined(PLD_jpgqt) || defined (PLD_pngqt) || defined(PLD_ppmqt) || defined(PLD_tiffqt)
	if(widget==NULL) widget=dynamic_cast<QtRasterDevice*>((QtPLDriver *) pls->dev);
	if(widget!=NULL && qt_family_check(pls)) {return;} 
#endif
#if defined(PLD_svgqt) && QT_VERSION >= 0x040300
	if(widget==NULL) widget=dynamic_cast<QtSVGDevice*>((QtPLDriver *) pls->dev);
	if(widget!=NULL && qt_family_check(pls)) {return;} 
#endif
#if defined(PLD_epsqt) || defined(PLD_pdfqt)
	if(widget==NULL) widget=dynamic_cast<QtEPSDevice*>((QtPLDriver *) pls->dev);
	if(widget!=NULL && qt_family_check(pls)) {return;} 
#endif
#if defined(PLD_qtwidget) || defined(PLD_extqt)
	if(widget==NULL) widget=dynamic_cast<QtPLWidget*>((QWidget *) pls->dev);
#endif
	if(widget==NULL) return;
	
	widget->setColor(pls->curcolor.r, pls->curcolor.g, pls->curcolor.b, pls->curcolor.a);
	widget->drawPolyline(xa, ya, npts);
}

void plD_esc_qt(PLStream * pls, PLINT op, void* ptr)
{
	short *xa, *ya;
	PLINT i, j;
	QtPLDriver * widget=NULL;

#if defined (PLD_bmpqt) || defined(PLD_jpgqt) || defined (PLD_pngqt) || defined(PLD_ppmqt) || defined(PLD_tiffqt)
	if(widget==NULL) widget=dynamic_cast<QtRasterDevice*>((QtPLDriver *) pls->dev);
	if(widget!=NULL && qt_family_check(pls)) {return;} 
#endif
#if defined(PLD_svgqt) && QT_VERSION >= 0x040300
	if(widget==NULL) widget=dynamic_cast<QtSVGDevice*>((QtPLDriver *) pls->dev);
	if(widget!=NULL && qt_family_check(pls)) {return;} 
#endif
#if defined(PLD_epsqt) || defined(PLD_pdfqt)
	if(widget==NULL) widget=dynamic_cast<QtEPSDevice*>((QtPLDriver *) pls->dev);
	if(widget!=NULL && qt_family_check(pls)) {return;} 
#endif
#if defined(PLD_qtwidget) || defined(PLD_extqt)
	if(widget==NULL) widget=dynamic_cast<QtPLWidget*>((QWidget *) pls->dev);
#endif
	if(widget==NULL) return;
		    
	switch(op)
	{
		case PLESC_DASH:
			widget->setDashed(pls->nms, pls->mark, pls->space);
			widget->setColor(pls->curcolor.r, pls->curcolor.g, pls->curcolor.b, pls->curcolor.a);
			widget->drawPolyline(pls->dev_x, pls->dev_y, pls->dev_npts);
			widget->setSolid();
			break;
        
		case PLESC_FILL:
			xa=new short[pls->dev_npts];
			ya=new short[pls->dev_npts];
            
			for (i = 0; i < pls->dev_npts; i++)
			{
				xa[i] = pls->dev_x[i];
				ya[i] = pls->dev_y[i];
			}
			widget->setColor(pls->curcolor.r, pls->curcolor.g, pls->curcolor.b, pls->curcolor.a);
			widget->drawPolygon(xa, ya, pls->dev_npts);
            
			delete[] xa;
			delete[] ya;
			break;
      
    case PLESC_HAS_TEXT:
      /*$$ call the generic ProcessString function
      ProcessString( pls, (EscText *)ptr ); */
		widget->setColor(pls->curcolor.r, pls->curcolor.g, pls->curcolor.b, pls->curcolor.a);
		widget->drawText(pls, (EscText *)ptr);
      break;        
    
		default: break;
	}
}

void plD_state_qt(PLStream * pls, PLINT op)
{
	QtPLDriver * widget=NULL;
#if defined (PLD_bmpqt) || defined(PLD_jpgqt) || defined (PLD_pngqt) || defined(PLD_ppmqt) || defined(PLD_tiffqt)
	if(widget==NULL) widget=dynamic_cast<QtRasterDevice*>((QtPLDriver *) pls->dev);
	if(widget!=NULL && qt_family_check(pls)) {return;} 
#endif
#if defined(PLD_svgqt) && QT_VERSION >= 0x040300
	if(widget==NULL) widget=dynamic_cast<QtSVGDevice*>((QtPLDriver *) pls->dev);
	if(widget!=NULL && qt_family_check(pls)) {return;} 
#endif
#if defined(PLD_epsqt) || defined(PLD_pdfqt)
	if(widget==NULL) widget=dynamic_cast<QtEPSDevice*>((QtPLDriver *) pls->dev);
	if(widget!=NULL && qt_family_check(pls)) {return;} 
#endif
#if defined(PLD_qtwidget) || defined(PLD_extqt)
	if(widget==NULL) widget=dynamic_cast<QtPLWidget*>((QWidget *) pls->dev);
#endif
	if(widget==NULL) return;
    
	switch(op)
	{
		case PLSTATE_WIDTH:
			widget->setWidth(pls->width);
			break;
       
		case PLSTATE_COLOR0:
			widget->setColor(pls->curcolor.r, pls->curcolor.g, pls->curcolor.b, pls->curcolor.a);
			break;
			
		case PLSTATE_COLOR1:
			widget->setColor(pls->curcolor.r, pls->curcolor.g, pls->curcolor.b, pls->curcolor.a);
			break;

            
			default: break;
	}
}

void plD_tidy_qt(PLStream * pls)
{
	QtPLDriver * widget=NULL;
	#if defined (PLD_bmpqt) || defined(PLD_jpgqt) || defined (PLD_pngqt) || defined(PLD_ppmqt) || defined(PLD_tiffqt)
	if(widget==NULL) widget=dynamic_cast<QtRasterDevice*>((QtPLDriver *) pls->dev);
#endif
#if defined(PLD_svgqt) && QT_VERSION >= 0x040300
	if(widget==NULL) widget=dynamic_cast<QtSVGDevice*>((QtPLDriver *) pls->dev);
#endif
#if defined(PLD_epsqt) || defined(PLD_pdfqt)
	if(widget==NULL) widget=dynamic_cast<QtEPSDevice*>((QtPLDriver *) pls->dev);
#endif
#if defined(PLD_qtwidget) || defined(PLD_extqt)
	if(widget==NULL) widget=dynamic_cast<QtPLWidget*>((QtPLWidget *) pls->dev);
#endif
	
	if(widget!=NULL)
	{
		handler.DeviceClosed(widget);
		delete widget;
		pls->dev=NULL;
	}
	
	closeQtApp();
}

////////////////// Raster driver-specific definitions: class and interface functions /////////

#if defined (PLD_bmpqt) || defined(PLD_jpgqt) || defined (PLD_pngqt) || defined(PLD_ppmqt) || defined(PLD_tiffqt)
QtRasterDevice::QtRasterDevice(int i_iWidth, int i_iHeight):
	QtPLDriver(i_iWidth, i_iHeight),
	QImage(i_iWidth, i_iHeight, QImage::Format_RGB32)
{
	// Painter initialised in the constructor contrary
	// to buffered drivers, which paint only in doPlot().
	m_painterP=new QPainter(this);
	QBrush b=m_painterP->brush();
	b.setStyle(Qt::SolidPattern);
	m_painterP->setBrush(b);
	m_painterP->setRenderHint(QPainter::Antialiasing, true);

	// Let's fill the background
	m_painterP->fillRect(0, 0, width(), height(), QBrush(Qt::black));

// 	fontScalingFactor=1.;
}

QtRasterDevice::~QtRasterDevice()
{
	if(m_painterP!=NULL) delete m_painterP;
}

void QtRasterDevice::definePlotName(const char* fileName, const char* format)
{
	// Avoid buffer overflows
	strncpy(this->format, format, 4);
	this->format[4]='\0';
	
	this->fileName=QString(fileName);
}

void QtRasterDevice::savePlot()
{
	m_painterP->end();
	save(fileName, format, 85);

	m_painterP->begin(this);
	m_painterP->setRenderHint(QPainter::Antialiasing, true);
	QBrush b=m_painterP->brush();
	b.setStyle(Qt::SolidPattern);
	m_painterP->setBrush(b);
	m_painterP->fillRect(0, 0, width(), height(), QBrush(Qt::black));
}

void plD_init_rasterqt(PLStream * pls)
{
        double dpi;
	/* Stream setup */
	pls->color = 1;
	pls->plbuf_write=0;
	pls->dev_fill0 = 1;
	pls->dev_fill1 = 0;
        /* Let the PLplot core handle dashed lines since
	 * the driver results for this capability have a number of issues.
	pls->dev_dash=1; */
	pls->dev_dash=0;
	pls->dev_flush=1;
	pls->dev_clear=1;
	pls->termin=0;
	pls->page = 0;
	pls->dev_text = 1; // want to draw text
	pls->dev_unicode = 1; // want unicode 
  
	bool isMaster=initQtApp(/*false*/true); // Is it still a problem?
	
	if (pls->xdpi <=0.)
	  dpi = DEFAULT_DPI;
	else
	  dpi = pls->xdpi;
	// For raster debug
	// std::cout << dpi << std::endl;

	// Shamelessly copied on the Cairo stuff :)
	if (pls->xlength <= 0 || pls->ylength <= 0)
	{
		pls->dev=new QtRasterDevice;
		pls->xlength = ((QtRasterDevice*)(pls->dev))->m_dWidth;
		pls->ylength = ((QtRasterDevice*)(pls->dev))->m_dHeight;
	}
	else
	{
		pls->dev=new QtRasterDevice(pls->xlength, pls->ylength);
	}

	if(isMaster) handler.setMasterDevice((QtRasterDevice*)(pls->dev));

	if (pls->xlength > pls->ylength)
		((QtRasterDevice*)(pls->dev))->downscale = (PLFLT)pls->xlength/(PLFLT)(PIXELS_X-1);
	else
		((QtRasterDevice*)(pls->dev))->downscale = (PLFLT)pls->ylength/(PLFLT)PIXELS_Y;
	
	plP_setphy((PLINT) 0, (PLINT) (pls->xlength / ((QtRasterDevice*)(pls->dev))->downscale), (PLINT) 0, (PLINT) (pls->ylength / ((QtRasterDevice*)(pls->dev))->downscale));
	
	plP_setpxl(dpi/25.4/((QtRasterDevice*)(pls->dev))->downscale, dpi/25.4/((QtRasterDevice*)(pls->dev))->downscale);
	
	((QtRasterDevice*)(pls->dev))->setResolution(dpi);

	/* Initialize family file info */
	plFamInit(pls);
	
	plOpenFile(pls);
}

void plD_eop_rasterqt(PLStream *pls)
{
	if(qt_family_check(pls)) {return;} 
	((QtRasterDevice *)pls->dev)->savePlot();
	handler.DeviceChangedPage((QtRasterDevice *)pls->dev);
}

#endif

#if defined(PLD_bmpqt)
void plD_dispatch_init_bmpqt(PLDispatchTable *pdt)
{
#ifndef ENABLE_DYNDRIVERS
	pdt->pl_MenuStr  = "Qt Windows bitmap Driver";
	pdt->pl_DevName  = "bmpqt";
#endif
	pdt->pl_type     = plDevType_FileOriented;
	pdt->pl_seq      = 66;
	pdt->pl_init     = (plD_init_fp)     plD_init_rasterqt;
	pdt->pl_line     = (plD_line_fp)     plD_line_qt;
	pdt->pl_polyline = (plD_polyline_fp) plD_polyline_qt;
	pdt->pl_eop      = (plD_eop_fp)      plD_eop_rasterqt;
	pdt->pl_bop      = (plD_bop_fp)      plD_bop_bmpqt;
	pdt->pl_tidy     = (plD_tidy_fp)     plD_tidy_qt;
	pdt->pl_state    = (plD_state_fp)    plD_state_qt;
	pdt->pl_esc      = (plD_esc_fp)      plD_esc_qt;
}

void plD_bop_bmpqt(PLStream *pls)
{
	/* Plot familying stuff. Not really understood, just copying gd.c */
	plGetFam(pls);

	pls->famadv = 1;
	pls->page++;
	if(qt_family_check(pls)) {return;} 
	((QtRasterDevice *)pls->dev)->definePlotName(pls->FileName, "BMP");
}
#endif

#if defined(PLD_jpgqt)
void plD_dispatch_init_jpgqt(PLDispatchTable *pdt)
{
#ifndef ENABLE_DYNDRIVERS
	pdt->pl_MenuStr  = "Qt jpg Driver";
	pdt->pl_DevName  = "jpgqt";
#endif
	pdt->pl_type     = plDevType_FileOriented;
	pdt->pl_seq      = 67;
	pdt->pl_init     = (plD_init_fp)     plD_init_rasterqt;
	pdt->pl_line     = (plD_line_fp)     plD_line_qt;
	pdt->pl_polyline = (plD_polyline_fp) plD_polyline_qt;
	pdt->pl_eop      = (plD_eop_fp)      plD_eop_rasterqt;
	pdt->pl_bop      = (plD_bop_fp)      plD_bop_jpgqt;
	pdt->pl_tidy     = (plD_tidy_fp)     plD_tidy_qt;
	pdt->pl_state    = (plD_state_fp)    plD_state_qt;
	pdt->pl_esc      = (plD_esc_fp)      plD_esc_qt;
}

void plD_bop_jpgqt(PLStream *pls)
{
	/* Plot familying stuff. Not really understood, just copying gd.c */
	plGetFam(pls);

	pls->famadv = 1;
	pls->page++;
	if(qt_family_check(pls)) {return;} 
	((QtRasterDevice *)pls->dev)->definePlotName(pls->FileName, "JPG");
}
#endif

#if defined(PLD_pngqt)
void plD_dispatch_init_pngqt(PLDispatchTable *pdt)
{
#ifndef ENABLE_DYNDRIVERS
	pdt->pl_MenuStr  = "Qt png Driver";
	pdt->pl_DevName  = "pngqt";
#endif
	pdt->pl_type     = plDevType_FileOriented;
	pdt->pl_seq      = 68;
	pdt->pl_init     = (plD_init_fp)     plD_init_rasterqt;
	pdt->pl_line     = (plD_line_fp)     plD_line_qt;
	pdt->pl_polyline = (plD_polyline_fp) plD_polyline_qt;
	pdt->pl_eop      = (plD_eop_fp)      plD_eop_rasterqt;
	pdt->pl_bop      = (plD_bop_fp)      plD_bop_pngqt;
	pdt->pl_tidy     = (plD_tidy_fp)     plD_tidy_qt;
	pdt->pl_state    = (plD_state_fp)    plD_state_qt;
	pdt->pl_esc      = (plD_esc_fp)      plD_esc_qt;
}

void plD_bop_pngqt(PLStream *pls)
{
	/* Plot familying stuff. Not really understood, just copying gd.c */
	plGetFam(pls);

	pls->famadv = 1;
	pls->page++;
	if(qt_family_check(pls)) {return;} 
	((QtRasterDevice *)pls->dev)->definePlotName(pls->FileName, "PNG");
}
#endif

#if defined(PLD_ppmqt)
void plD_dispatch_init_ppmqt(PLDispatchTable *pdt)
{
#ifndef ENABLE_DYNDRIVERS
	pdt->pl_MenuStr  = "Qt ppm Driver";
	pdt->pl_DevName  = "ppmqt";
#endif
	pdt->pl_type     = plDevType_FileOriented;
	pdt->pl_seq      = 69;
	pdt->pl_init     = (plD_init_fp)     plD_init_rasterqt;
	pdt->pl_line     = (plD_line_fp)     plD_line_qt;
	pdt->pl_polyline = (plD_polyline_fp) plD_polyline_qt;
	pdt->pl_eop      = (plD_eop_fp)      plD_eop_rasterqt;
	pdt->pl_bop      = (plD_bop_fp)      plD_bop_ppmqt;
	pdt->pl_tidy     = (plD_tidy_fp)     plD_tidy_qt;
	pdt->pl_state    = (plD_state_fp)    plD_state_qt;
	pdt->pl_esc      = (plD_esc_fp)      plD_esc_qt;
}

void plD_bop_ppmqt(PLStream *pls)
{
	/* Plot familying stuff. Not really understood, just copying gd.c */
	plGetFam(pls);

	pls->famadv = 1;
	pls->page++;
	if(qt_family_check(pls)) {return;} 
	((QtRasterDevice *)pls->dev)->definePlotName(pls->FileName, "PPM");
}
#endif

#if defined(PLD_tiffqt)
void plD_dispatch_init_tiffqt(PLDispatchTable *pdt)
{
#ifndef ENABLE_DYNDRIVERS
	pdt->pl_MenuStr  = "Qt tiff Driver";
	pdt->pl_DevName  = "tiffqt";
#endif
	pdt->pl_type     = plDevType_FileOriented;
	pdt->pl_seq      = 70;
	pdt->pl_init     = (plD_init_fp)     plD_init_rasterqt;
	pdt->pl_line     = (plD_line_fp)     plD_line_qt;
	pdt->pl_polyline = (plD_polyline_fp) plD_polyline_qt;
	pdt->pl_eop      = (plD_eop_fp)      plD_eop_rasterqt;
	pdt->pl_bop      = (plD_bop_fp)      plD_bop_tiffqt;
	pdt->pl_tidy     = (plD_tidy_fp)     plD_tidy_qt;
	pdt->pl_state    = (plD_state_fp)    plD_state_qt;
	pdt->pl_esc      = (plD_esc_fp)      plD_esc_qt;
}

void plD_bop_tiffqt(PLStream *pls)
{
	/* Plot familying stuff. Not really understood, just copying gd.c */
	plGetFam(pls);

	pls->famadv = 1;
	pls->page++;
	if(qt_family_check(pls)) {return;} 
	((QtRasterDevice *)pls->dev)->definePlotName(pls->FileName, "TIFF");
}
#endif


#if defined(PLD_svgqt) && QT_VERSION >= 0x040300
QtSVGDevice::QtSVGDevice(int i_iWidth, int i_iHeight):
	QtPLDriver(i_iWidth, i_iHeight)
{
	m_painterP=NULL;

// 	fontScalingFactor=1.;
}

QtSVGDevice::~QtSVGDevice()
{
	if(m_painterP!=NULL) delete m_painterP;
}

void QtSVGDevice::definePlotName(const char* fileName)
{
	setFileName(QString(fileName));
	setResolution(POINTS_PER_INCH);
	setSize(QSize(m_dWidth, m_dHeight));
#if QT_VERSION >=  0x040500
        setViewBox(QRect(0, 0, (int)m_dWidth, (int)m_dHeight));
#endif

	m_painterP=new QPainter(this);
	m_painterP->fillRect(0, 0, m_dWidth, m_dHeight, QBrush(Qt::black));
}

void QtSVGDevice::savePlot()
{
	m_painterP->end();
}

void plD_dispatch_init_svgqt(PLDispatchTable *pdt)
{
#ifndef ENABLE_DYNDRIVERS
	pdt->pl_MenuStr  = "Qt SVG Driver";
	pdt->pl_DevName  = "svgqt";
#endif
	pdt->pl_type     = plDevType_FileOriented;
	pdt->pl_seq      = 71;
	pdt->pl_init     = (plD_init_fp)     plD_init_svgqt;
	pdt->pl_line     = (plD_line_fp)     plD_line_qt;
	pdt->pl_polyline = (plD_polyline_fp) plD_polyline_qt;
	pdt->pl_eop      = (plD_eop_fp)      plD_eop_svgqt;
	pdt->pl_bop      = (plD_bop_fp)      plD_bop_svgqt;
	pdt->pl_tidy     = (plD_tidy_fp)     plD_tidy_qt;
	pdt->pl_state    = (plD_state_fp)    plD_state_qt;
	pdt->pl_esc      = (plD_esc_fp)      plD_esc_qt;
}

void plD_init_svgqt(PLStream * pls)
{
	/* Stream setup */
	pls->color = 1;
	pls->plbuf_write=0;
	pls->dev_fill0 = 1;
	pls->dev_fill1 = 0;
        /* Let the PLplot core handle dashed lines since
	 * the driver results for this capability have a number of issues.
	pls->dev_dash=1; */
	pls->dev_dash=0;
	pls->dev_flush=1;
	pls->dev_clear=1;
	pls->termin=0;
	pls->page = 0;
	pls->dev_text = 1; // want to draw text
	pls->dev_unicode = 1; // want unicode 
	
	bool isMaster=initQtApp(/*false*/ true); // Is is still a problem?
	
	if (pls->xlength <= 0 || pls->ylength <= 0)
	{
		pls->dev=new QtSVGDevice;
		pls->xlength = ((QtSVGDevice*)(pls->dev))->m_dWidth;
		pls->ylength = ((QtSVGDevice*)(pls->dev))->m_dHeight;
	}
	else
	{
		pls->dev=new QtSVGDevice(pls->xlength, pls->ylength);
	}

	if(isMaster) handler.setMasterDevice((QtSVGDevice*)(pls->dev));

	if (pls->xlength > pls->ylength)
		((QtSVGDevice*)(pls->dev))->downscale = (PLFLT)pls->xlength/(PLFLT)(PIXELS_X-1);
	else
		((QtSVGDevice*)(pls->dev))->downscale = (PLFLT)pls->ylength/(PLFLT)PIXELS_Y;
	
	plP_setphy((PLINT) 0, (PLINT) (pls->xlength / ((QtSVGDevice*)(pls->dev))->downscale), (PLINT) 0, (PLINT) (pls->ylength / ((QtSVGDevice*)(pls->dev))->downscale));
	
	plP_setpxl(POINTS_PER_INCH/25.4/((QtSVGDevice*)(pls->dev))->downscale, POINTS_PER_INCH/25.4/((QtSVGDevice*)(pls->dev))->downscale);

	/* Initialize family file info */
	plFamInit(pls);
  
	plOpenFile(pls);
}

void plD_bop_svgqt(PLStream *pls)
{
	/* Plot familying stuff. Not really understood, just copying gd.c */
	plGetFam(pls);
	
	pls->famadv = 1;
	pls->page++;
	if(qt_family_check(pls)) {return;} 
	((QtSVGDevice *)pls->dev)->definePlotName(pls->FileName);
}

void plD_eop_svgqt(PLStream *pls)
{
	double downscale;
	QSize s;
	
	((QtSVGDevice *)pls->dev)->savePlot();
	// Once saved, we have to create a new device with the same properties
	// to be able to plot another page.
	downscale=((QtSVGDevice *)pls->dev)->downscale;
	s=((QtSVGDevice *)pls->dev)->size();
	bool isMaster=(handler.isMasterDevice((QtSVGDevice *)pls->dev));
	delete ((QtSVGDevice *)pls->dev);
	
	pls->dev=new QtSVGDevice(s.width(), s.height());
	((QtSVGDevice *)pls->dev)->downscale=downscale;
	
	if(isMaster) handler.setMasterDevice((QtSVGDevice *)pls->dev);
	handler.DeviceChangedPage((QtSVGDevice *)pls->dev);
}

#endif

#if defined (PLD_epsqt) || defined(PLD_pdfqt)
QtEPSDevice::QtEPSDevice(int i_iWidth, int i_iHeight)
{
#if QT_VERSION < 0x040400
	setPageSize(QPrinter::A4);
#else
	setFullPage(true);
	setPaperSize(QSizeF(i_iHeight, i_iWidth), QPrinter::Point);
#endif
	setResolution(POINTS_PER_INCH);
	setColorMode(QPrinter::Color);
	setOrientation(QPrinter::Landscape);
	setPrintProgram(QString("lpr"));
	
	if(i_iWidth<=0 || i_iHeight<=0)
	{
		m_dWidth=pageRect().width();
		m_dHeight=pageRect().height();
	}
	else
	{
		m_dWidth=i_iWidth;
		m_dHeight=i_iHeight;
	}
	m_painterP=NULL;

// 	fontScalingFactor=1.;
}

QtEPSDevice::~QtEPSDevice()
{
	if(m_painterP!=NULL) delete m_painterP;
}

void QtEPSDevice::definePlotName(const char* fileName, int ifeps)
{
	setOutputFileName(QString(fileName));
	if(ifeps)
	{
		setOutputFormat(QPrinter::PostScriptFormat);
	}
	else
	{
		setOutputFormat(QPrinter::PdfFormat);
	}
				
	m_painterP=new QPainter(this);
	m_painterP->fillRect(0, 0, m_dWidth, m_dHeight, QBrush(Qt::black));
}

void QtEPSDevice::savePlot()
{
	m_painterP->end();
}

#if defined(PLD_epsqt)
void plD_dispatch_init_epsqt(PLDispatchTable *pdt)
{
#ifndef ENABLE_DYNDRIVERS
	pdt->pl_MenuStr  = "Qt EPS Driver";
	pdt->pl_DevName  = "epsqt";
#endif
	pdt->pl_type     = plDevType_FileOriented;
	pdt->pl_seq      = 73;
	pdt->pl_init     = (plD_init_fp)     plD_init_epspdfqt;
	pdt->pl_line     = (plD_line_fp)     plD_line_qt;
	pdt->pl_polyline = (plD_polyline_fp) plD_polyline_qt;
	pdt->pl_eop      = (plD_eop_fp)      plD_eop_epspdfqt;
	pdt->pl_bop      = (plD_bop_fp)      plD_bop_epsqt;
	pdt->pl_tidy     = (plD_tidy_fp)     plD_tidy_qt;
	pdt->pl_state    = (plD_state_fp)    plD_state_qt;
	pdt->pl_esc      = (plD_esc_fp)      plD_esc_qt;
}
#endif

#if defined(PLD_pdfqt)
void plD_dispatch_init_pdfqt(PLDispatchTable *pdt)
{
#ifndef ENABLE_DYNDRIVERS
	pdt->pl_MenuStr  = "Qt PDF Driver";
	pdt->pl_DevName  = "pdfqt";
#endif
	pdt->pl_type     = plDevType_FileOriented;
	pdt->pl_seq      = 74;
	pdt->pl_init     = (plD_init_fp)     plD_init_epspdfqt;
	pdt->pl_line     = (plD_line_fp)     plD_line_qt;
	pdt->pl_polyline = (plD_polyline_fp) plD_polyline_qt;
	pdt->pl_eop      = (plD_eop_fp)      plD_eop_epspdfqt;
	pdt->pl_bop      = (plD_bop_fp)      plD_bop_pdfqt;
	pdt->pl_tidy     = (plD_tidy_fp)     plD_tidy_qt;
	pdt->pl_state    = (plD_state_fp)    plD_state_qt;
	pdt->pl_esc      = (plD_esc_fp)      plD_esc_qt;
}
#endif

void plD_init_epspdfqt(PLStream * pls)
{
	/* Stream setup */
	pls->color = 1;
	pls->plbuf_write=0;
	pls->dev_fill0 = 1;
	pls->dev_fill1 = 0;
        /* Let the PLplot core handle dashed lines since
	 * the driver results for this capability have a number of issues.
	pls->dev_dash=1; */
	pls->dev_dash=0;
	pls->dev_flush=1;
	pls->dev_clear=1;
	pls->termin=0;
	pls->page = 0;
	pls->dev_text = 1; // want to draw text
	pls->dev_unicode = 1; // want unicode 
	
	// QPrinter devices won't create if there is no QApplication declared...
	bool isMaster=initQtApp(/*false*/true); //Is it still a problem?
	
	if (pls->xlength <= 0 || pls->ylength <= 0)
	{
		pls->dev=new QtEPSDevice;
		pls->xlength = ((QtEPSDevice*)(pls->dev))->m_dWidth;
		pls->ylength = ((QtEPSDevice*)(pls->dev))->m_dHeight;
	}
	else
	{
		pls->dev=new QtEPSDevice(pls->xlength, pls->ylength);
	}

	if(isMaster) handler.setMasterDevice((QtEPSDevice*)(pls->dev));

	if (pls->xlength > pls->ylength)
		((QtEPSDevice*)(pls->dev))->downscale = (PLFLT)pls->xlength/(PLFLT)(PIXELS_X-1);
	else
		((QtEPSDevice*)(pls->dev))->downscale = (PLFLT)pls->ylength/(PLFLT)PIXELS_Y;
	
	plP_setphy((PLINT) 0, (PLINT) (pls->xlength / ((QtEPSDevice*)(pls->dev))->downscale), (PLINT) 0, (PLINT) (pls->ylength / ((QtEPSDevice*)(pls->dev))->downscale));
	
	plP_setpxl(POINTS_PER_INCH/25.4/((QtEPSDevice*)(pls->dev))->downscale, POINTS_PER_INCH/25.4/((QtEPSDevice*)(pls->dev))->downscale);
	
	/* Initialize family file info */
	plFamInit(pls);
	
	plOpenFile(pls);
}

void plD_bop_epspdfqt_helper(PLStream *pls, int ifeps)
{
	/* Plot familying stuff. Not really understood, just copying gd.c */
	plGetFam(pls);

	pls->famadv = 1;
	pls->page++;
	if(qt_family_check(pls)) {return;}
	((QtEPSDevice *)pls->dev)->definePlotName(pls->FileName, ifeps);
}

void plD_eop_epspdfqt(PLStream *pls)
{
	double downscale;

	int argc=0;
	char argv[]={'\0'};
	if(qt_family_check(pls)) {return;} 
	((QtEPSDevice *)pls->dev)->savePlot();
	// Once saved, we have to create a new device with the same properties
	// to be able to plot another page.
	downscale=((QtEPSDevice *)pls->dev)->downscale;
	bool isMaster=handler.isMasterDevice((QtEPSDevice *)pls->dev);
	delete ((QtEPSDevice *)pls->dev);

	pls->dev=new QtEPSDevice;
	((QtEPSDevice *)pls->dev)->downscale=downscale;

	if(isMaster) handler.setMasterDevice((QtEPSDevice *)pls->dev);
	handler.DeviceChangedPage((QtEPSDevice *)pls->dev);
}

#if defined(PLD_epsqt)
void plD_bop_epsqt(PLStream *pls)
{
  plD_bop_epspdfqt_helper(pls, 1);
}
#endif

#if defined(PLD_pdfqt)
void plD_bop_pdfqt(PLStream *pls)
{
  plD_bop_epspdfqt_helper(pls, 0);
}
#endif

#endif

#if defined (PLD_qtwidget) || defined(PLD_extqt)

QtPLWidget::QtPLWidget(int i_iWidth, int i_iHeight, QWidget* parent):
	 QWidget(parent), QtPLDriver(i_iWidth, i_iHeight)
{
	m_painterP=new QPainter;
	
	m_dAspectRatio=(double)i_iWidth/(double)i_iHeight;
	m_pixPixmap=NULL;
	m_iOldSize=0;
	
	pic=new QPicture;
	m_painterP->begin(pic);
	m_painterP->setRenderHint(QPainter::Antialiasing, true);
	pageNumber=1;
	
}

QtPLWidget::~QtPLWidget()
{
	if(m_pixPixmap!=NULL) delete m_pixPixmap;
	if(m_painterP!=NULL) delete m_painterP;
	if(pic!=NULL) delete pic;
	m_pixPixmap=NULL;
	m_painterP=NULL;
	pic=NULL;
}

void QtPLWidget::clearWidget()
{
	if(m_painterP->isActive()) m_painterP->end();
	delete pic;
	pic=new QPicture;
	m_painterP->begin(pic);
	m_painterP->setRenderHint(QPainter::Antialiasing, true);
	m_bAwaitingRedraw=true;
	update();
}

void QtPLWidget::mouseReleaseEvent ( QMouseEvent * event )
{
	if(event->button()==Qt::RightButton)
	{
		handler.DeviceChangedPage(this);
	}
}

void QtPLWidget::keyPressEvent(QKeyEvent* event)
{
	if(event->key()==Qt::Key_Enter || event->key()==Qt::Key_Return)
	{
		handler.DeviceChangedPage(this);
	}
}

void QtPLWidget::closeEvent(QCloseEvent* event)
{
	handler.DeviceClosed(this);
	event->ignore();
}

void QtPLWidget::nextPage()
{
	clearWidget();
	pageNumber++;
}

void QtPLWidget::resizeEvent( QResizeEvent * )
{
	m_bAwaitingRedraw=true;
	if(m_pixPixmap!=NULL)
	{
		delete m_pixPixmap;
		m_pixPixmap=NULL;
	}
}

void QtPLWidget::paintEvent( QPaintEvent * )
{
	if(m_painterP->isActive()) m_painterP->end();
	double x_fact, y_fact, x_offset(0.), y_offset(0.); //Parameters to scale and center the plot on the widget
	getPlotParameters(x_fact, y_fact, x_offset, y_offset);
	QPainter * painter=new QPainter;

	if(m_bAwaitingRedraw || m_pixPixmap==NULL)
	{
		if(m_pixPixmap!=NULL) delete m_pixPixmap;
		m_pixPixmap=new QPixmap(width(), height());
		painter->begin(m_pixPixmap);
		painter->fillRect(0, 0, width(), height(), QBrush(Qt::white));
		painter->fillRect(0, 0, width(), height(), QBrush(Qt::gray, Qt::Dense4Pattern));
		painter->fillRect((int)x_offset, (int)y_offset, (int)floor(x_fact*m_dWidth+0.5), (int)floor(y_fact*m_dHeight+0.5), QBrush(Qt::black));
		painter->setRenderHint(QPainter::Antialiasing, true);
		painter->setWorldMatrix(QMatrix(x_fact, 0., 0., y_fact, x_offset, y_offset));
		pic->play(painter);
		painter->end();
		m_bAwaitingRedraw=false;
	}
	
	painter->begin(this);

	painter->drawPixmap(0, 0, *m_pixPixmap);

	painter->end();
	delete painter;
}

void QtPLWidget::getPlotParameters(double & io_dXFact, double & io_dYFact, double & io_dXOffset, double & io_dYOffset)
{
	double w=(double)width();
	double h=(double)height();

	if(w/h>m_dAspectRatio) //Too wide, h is the limitating factor
	{
		io_dYFact=h/m_dHeight;
		io_dXFact=h*m_dAspectRatio/m_dWidth;
		io_dYOffset=0.;
		io_dXOffset=(w-io_dXFact*m_dWidth)/2.;
	}
	else
	{
		io_dXFact=w/m_dWidth;
		io_dYFact=w/m_dAspectRatio/m_dHeight;
		io_dXOffset=0.;
		io_dYOffset=(h-io_dYFact*m_dHeight)/2.;
	}
}
#endif

#if defined (PLD_qtwidget)
void plD_dispatch_init_qtwidget(PLDispatchTable *pdt)
{
#ifndef ENABLE_DYNDRIVERS
	pdt->pl_MenuStr  = "Qt Widget";
	pdt->pl_DevName  = "qtwidget";
#endif
	pdt->pl_type     = plDevType_Interactive;
	pdt->pl_seq      = 72;
	pdt->pl_init     = (plD_init_fp)     plD_init_qtwidget;
	pdt->pl_line     = (plD_line_fp)     plD_line_qt;
	pdt->pl_polyline = (plD_polyline_fp) plD_polyline_qt;
	pdt->pl_eop      = (plD_eop_fp)      plD_eop_qtwidget;
	pdt->pl_bop      = (plD_bop_fp)      plD_bop_qt;
	pdt->pl_tidy     = (plD_tidy_fp)     plD_tidy_qt;
	pdt->pl_state    = (plD_state_fp)    plD_state_qt;
	pdt->pl_esc      = (plD_esc_fp)      plD_esc_qt;
}

void plD_init_qtwidget(PLStream * pls)
{
	PLINT w, h;
	bool isMaster=initQtApp(true);
	QtPLWidget* widget;

	if (pls->xlength <= 0 || pls->ylength <= 0)
	{
		widget=new QtPLWidget;
		pls->dev=(void*) widget;
		pls->xlength = (int)widget->m_dWidth;
		pls->ylength = (int)widget->m_dHeight;
	}
	else
	{
		widget=new QtPLWidget(pls->xlength, pls->ylength);
		pls->dev=(void*) widget;
	}

	if(isMaster) handler.setMasterDevice(widget);	

	if (plsc->xlength > plsc->ylength)
		widget->downscale = (PLFLT)plsc->xlength/(PLFLT)(PIXELS_X-1);
	else
		widget->downscale = (PLFLT)plsc->ylength/(PLFLT)PIXELS_Y;
	
	plP_setphy((PLINT) 0, (PLINT) (plsc->xlength / widget->downscale), (PLINT) 0, (PLINT) (plsc->ylength / widget->downscale));
	
	QPicture temp;
	QPainter tempPainter(&temp);
	
	plP_setpxl(temp.logicalDpiX()/25.4/widget->downscale, temp.logicalDpiY()/25.4/widget->downscale);

	pls->color = 1;		/* Is a color device */
	pls->plbuf_write=0;
	pls->dev_fill0 = 1;	/* Handle solid fills */
	pls->dev_fill1 = 0;
        /* Let the PLplot core handle dashed lines since
	 * the driver results for this capability have a number of issues.
	pls->dev_dash=1; */
	pls->dev_dash=0;
	pls->dev_flush=1;
	pls->dev_clear=1;
// 	pls->termin=1;
	pls->dev_text = 1; // want to draw text
	pls->dev_unicode = 1; // want unicode 
	
	widget->setVisible(true);
	widget->resize(plsc->xlength, plsc->ylength);

	qApp->connect(&handler, SIGNAL(MasterChangedPage()), widget, SLOT(nextPage()));
	qApp->connect(&handler, SIGNAL(MasterClosed()), widget, SLOT(close()));
	
}

void plD_eop_qtwidget(PLStream *pls)
{
	QtPLWidget* widget=((QtPLWidget*)pls->dev);
	int currentPage=widget->pageNumber;
	while(currentPage==widget->pageNumber && handler.isMasterDevice(widget))
	{
		qApp->processEvents(QEventLoop::WaitForMoreEvents);
	}
}

#endif

#if defined(PLD_extqt)
QtExtWidget::QtExtWidget(int i_iWidth, int i_iHeight, QWidget* parent):
	QtPLWidget(i_iWidth, i_iHeight, parent)
{
	killed=false;
}

QtExtWidget::~QtExtWidget()
{
	killed=true;
	QCoreApplication::processEvents(QEventLoop::AllEvents, 10);
	if(m_pixPixmap!=NULL) delete m_pixPixmap;
	if(m_painterP!=NULL) delete m_painterP;
	if(pic!=NULL) delete pic;
	m_pixPixmap=NULL;
	m_painterP=NULL;
	pic=NULL;
}

void QtExtWidget::captureMousePlotCoords(PLFLT* x, PLFLT* y)
{
	setMouseTracking(true);
	cursorParameters.isTracking=true;
	cursorParameters.cursor_x=
	cursorParameters.cursor_y=-1.;
	do
	{
		QCoreApplication::processEvents(QEventLoop::AllEvents, 10);
	} while(cursorParameters.isTracking && !killed);
    
	*x=cursorParameters.cursor_x;
	*y=cursorParameters.cursor_y;
}

void QtExtWidget::mouseMoveEvent(QMouseEvent* event)
{
	if(!cursorParameters.isTracking) return;

	double x_fact, y_fact, x_offset, y_offset; //Parameters to scale and center the plot on the widget
    
	getPlotParameters(x_fact, y_fact, x_offset, y_offset);
    
	cursorParameters.cursor_x=(PLFLT)event->x();
	cursorParameters.cursor_y=(PLFLT)event->y();

	double ratio_x;
	double ratio_y;
	ratio_x=(cursorParameters.cursor_x-x_offset)/(width()-2.*x_offset);
	ratio_y=(cursorParameters.cursor_y-y_offset)/(height()-2.*y_offset);

	PLFLT a,b;
	PLINT c;
	plcalc_world(ratio_x, 1.-ratio_y, &a, &b, &c);

	if(c<0)
	{
		cursorParameters.cursor_x=-1.;
		cursorParameters.cursor_y=-1.;
	}

	update();
}

void QtExtWidget::mousePressEvent(QMouseEvent* event)
{
}

void QtExtWidget::mouseReleaseEvent(QMouseEvent* event)
{	
	if(!cursorParameters.isTracking) return;

 	double x_fact, y_fact, x_offset, y_offset; //Parameters to scale and center the plot on the widget
	
	getPlotParameters(x_fact, y_fact, x_offset, y_offset);

	cursorParameters.cursor_x=(PLFLT)event->x();
	cursorParameters.cursor_y=(PLFLT)event->y();
	cursorParameters.isTracking=false;
	setMouseTracking(false);

	double ratio_x;
	double ratio_y;
	ratio_x=(cursorParameters.cursor_x-x_offset)/(width()-2.*x_offset);
	ratio_y=(cursorParameters.cursor_y-y_offset)/(height()-2.*y_offset);

	PLFLT a,b;
	PLINT c;
	plcalc_world(ratio_x, 1.-ratio_y, &a, &b, &c);

	if(c<0)
	{
		cursorParameters.cursor_x=-1.;
		cursorParameters.cursor_y=-1.;
	}
	else
	{
		cursorParameters.cursor_x=a;
		cursorParameters.cursor_y=b;
	}

	update();
}

void QtExtWidget::paintEvent(QPaintEvent* event)
{
	QtPLWidget::paintEvent(event);

	if(!cursorParameters.isTracking || cursorParameters.cursor_x<0) return;

	QPainter p(this);

	p.setPen(QPen(Qt::white));

	p.drawLine(cursorParameters.cursor_x, 0., cursorParameters.cursor_x, height());
	p.drawLine(0., cursorParameters.cursor_y, width(), cursorParameters.cursor_y);

	p.end();
}

void plD_dispatch_init_extqt(PLDispatchTable *pdt)
{
#ifndef ENABLE_DYNDRIVERS
	pdt->pl_MenuStr  = "External Qt Widget";
	pdt->pl_DevName  = "extqt";
#endif
	pdt->pl_type     = plDevType_Interactive;
	pdt->pl_seq      = 75;
	pdt->pl_init     = (plD_init_fp)     plD_init_extqt;
	pdt->pl_line     = (plD_line_fp)     plD_line_qt;
	pdt->pl_polyline = (plD_polyline_fp) plD_polyline_qt;
	pdt->pl_eop      = (plD_eop_fp)      plD_eop_extqt;
	pdt->pl_bop      = (plD_bop_fp)      plD_bop_qt;
	pdt->pl_tidy     = (plD_tidy_fp)     plD_tidy_qt;
	pdt->pl_state    = (plD_state_fp)    plD_state_qt;
	pdt->pl_esc      = (plD_esc_fp)      plD_esc_qt;
}

void plD_init_extqt(PLStream * pls)
{
	if(pls->dev==NULL/* || pls->xlength <= 0 || pls->ylength <= 0*/)
	{
		printf("Error: use plsetqtdev to set up the Qt device before calling plinit()\n");
		return;
	}

	QtExtWidget* widget=(QtExtWidget*)(pls->dev);

	if (widget->m_dWidth > widget->m_dHeight)
		widget->downscale = (PLFLT)widget->m_dWidth/(PLFLT)(PIXELS_X-1);
	else
		widget->downscale = (PLFLT)widget->m_dHeight/(PLFLT)PIXELS_Y;
	
	plP_setphy((PLINT) 0, (PLINT) (widget->m_dWidth / widget->downscale), (PLINT) 0, (PLINT) (widget->m_dHeight / widget->downscale));
	
	QPicture temp;
	QPainter tempPainter(&temp);
	
	plP_setpxl(temp.logicalDpiX()/25.4/widget->downscale, temp.logicalDpiY()/25.4/widget->downscale);

	pls->color = 1;		/* Is a color device */
	pls->plbuf_write=0;
	pls->dev_fill0 = 1;	/* Handle solid fills */
	pls->dev_fill1 = 0;
        /* Let the PLplot core handle dashed lines since
	 * the driver results for this capability have a number of issues.
	pls->dev_dash=1; */
	pls->dev_dash=0;
	pls->dev_flush=1;
	pls->dev_clear=1;
	pls->dev_text = 1; // want to draw text
	pls->dev_unicode = 1; // want unicode 

	
}

void plD_eop_extqt(PLStream *pls)
{

}

void plsetqtdev(QtExtWidget* widget)
{
	plsc->dev = (void*)widget;
}

void plfreeqtdev()
{
	delete ((QtExtWidget*)plsc->dev);
	plsc->dev=NULL;
}
#endif
