/*

  This software is provided under the LGPL in March 2009 by the
  Cluster Science Centre
  QSAS team,
  Imperial College, London

  Copyright (C) 2009  Imperial College, London

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
;

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
void plD_tidy_qtwidget(PLStream *);
#endif

// Declaration of the generic interface functions

void plD_line_qt(PLStream *, short, short, short, short);
void plD_polyline_qt(PLStream *, short*, short*, PLINT);
void plD_bop_qt(PLStream *){}
void plD_tidy_qt(PLStream *){}
void plD_state_qt(PLStream *, PLINT);
void plD_esc_qt(PLStream *, PLINT, void*);

///////////// Generic Qt driver class /////////////////

QtPLDriver::QtPLDriver(PLINT i_iWidth, PLINT i_iHeight)
{
	m_dWidth=i_iWidth;
	m_dHeight=i_iHeight;
	downscale=1.;
}

QtPLDriver::~QtPLDriver()
{}

void QtPLDriver::drawLine(short x1, short y1, short x2, short y2)
{
	QLineF line(	(PLFLT)x1*downscale,
			m_dHeight-(PLFLT)y1*downscale,
			(PLFLT)x2*downscale,
			m_dHeight-(PLFLT)y2*downscale
		   );

	m_painterP->drawLine(line);
}

void QtPLDriver::drawPolyline(short * x, short * y, PLINT npts)
{
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
	QPointF * polygon=new QPointF[npts];
	for(int i=0; i<npts; ++i)
	{
		polygon[i].setX((PLFLT)x[i]*downscale);
		polygon[i].setY(m_dHeight-(PLFLT)y[i]*downscale);
	}
	m_painterP->drawPolygon(polygon, npts);
	delete[] polygon;
}

void QtPLDriver::setColor(int r, int g, int b)
{
	QPen p=m_painterP->pen();
	p.setColor(QColor(r, g, b));
	m_painterP->setPen(p);
	
	QBrush B=m_painterP->brush();
	B.setColor(QColor(r, g, b));
	m_painterP->setBrush(B);
}

void QtPLDriver::setWidth(PLINT w)
{
	QPen p=m_painterP->pen();
	p.setWidth(w);
	m_painterP->setPen(p);
}

void QtPLDriver::setDashed(PLINT nms, PLINT* mark, PLINT* space)
{
	// 72 dpi
	// => 72/25400 = 0.00283 dot for 1 micron
	QVector<qreal> vect;
	for(int i=0; i<nms; ++i)
	{
		vect << (PLFLT)mark[i]*0.00283;
		vect << (PLFLT)space[i]*0.00283;
	}
	QPen p=m_painterP->pen();
	p.setDashPattern(vect);
	m_painterP->setPen(p);
}

void QtPLDriver::setSolid()
{
	QPen p=m_painterP->pen();
	p.setStyle(Qt::SolidLine);
	m_painterP->setPen(p);
}

//////////// Buffered driver ///////////////////////

QtPLBufferedDriver::QtPLBufferedDriver(PLINT i_iWidth, PLINT i_iHeight):
	QtPLDriver(i_iWidth, i_iHeight)
{}

QtPLBufferedDriver::~QtPLBufferedDriver()
{
	clearBuffer();
}

void QtPLBufferedDriver::clearBuffer()
{
	for(QLinkedList<BufferElement>::iterator i=m_listBuffer.begin(); i!=m_listBuffer.end(); ++i)

	{
		switch(i->Element)
		{
			case POLYLINE:
			case POLYGON:
				delete[] i->Data.PolylineStruct.x;
				delete[] i->Data.PolylineStruct.y;
				break;
                
			case SET_DASHED:
				delete[] i->Data.PolylineStruct.x;
				break;
            
			default:
				break;
		}
	}
    
	m_listBuffer.clear();
}

void QtPLBufferedDriver::drawLine(short x1, short y1, short x2, short y2)
{
	BufferElement el;
	el.Element=LINE;
	el.Data.LineStruct.x1=(PLFLT)x1*downscale;
	el.Data.LineStruct.y1=m_dHeight-(PLFLT)(y1*downscale);
	el.Data.LineStruct.x2=(PLFLT)x2*downscale;
	el.Data.LineStruct.y2=m_dHeight-(PLFLT)(y2*downscale);
    
	m_listBuffer.append(el);
}

void QtPLBufferedDriver::drawPolyline(short * x, short * y, PLINT npts)
{
	BufferElement el;
	el.Element=POLYLINE;
	el.Data.PolylineStruct.npts=(PLINT)npts;
	el.Data.PolylineStruct.x=new PLFLT[npts];
	el.Data.PolylineStruct.y=new PLFLT[npts];
	for(int i=0; i<npts; ++i)
	{
		el.Data.PolylineStruct.x[i]=(PLFLT)(x[i])*downscale;
		el.Data.PolylineStruct.y[i]=m_dHeight-(PLFLT)((y[i])*downscale);
	}
    
	m_listBuffer.append(el);
}

void QtPLBufferedDriver::drawPolygon(short * x, short * y, PLINT npts)
{
	BufferElement el;
	el.Element=POLYGON;
	el.Data.PolylineStruct.npts=(PLINT)npts;
	el.Data.PolylineStruct.x=new PLFLT[npts];
	el.Data.PolylineStruct.y=new PLFLT[npts];
	for(int i=0; i<npts; ++i)
	{
		el.Data.PolylineStruct.x[i]=(PLFLT)(x[i])*downscale;
		el.Data.PolylineStruct.y[i]=m_dHeight-(PLFLT)((y[i])*downscale);
	}
	if(el.Data.PolylineStruct.x[0]==el.Data.PolylineStruct.x[npts-1] && el.Data.PolylineStruct.y[0]==el.Data.PolylineStruct.y[npts-1])
	{
		--el.Data.PolylineStruct.npts;
	}
    
	m_listBuffer.append(el);
}

void QtPLBufferedDriver::setColor(int r, int g, int b)
{
	BufferElement el;
	el.Element=SET_COLOUR;
	el.Data.ColourStruct.R=r;
	el.Data.ColourStruct.G=g;
	el.Data.ColourStruct.B=b;
    
	m_listBuffer.append(el);
}

void QtPLBufferedDriver::setWidth(PLINT w)
{
	BufferElement el;
	el.Element=SET_WIDTH;
	el.Data.intParam=w;
	m_listBuffer.append(el);
}

void QtPLBufferedDriver::setDashed(PLINT nms, PLINT* mark, PLINT* space)
{
	BufferElement el;
	el.Element=SET_DASHED;
	el.Data.PolylineStruct.npts=2*nms;
	el.Data.PolylineStruct.x=new PLFLT[2*nms];
	for(int i=0; i<nms; i++)
	{
		el.Data.PolylineStruct.x[2*i]=(PLFLT)mark[i]*0.00283;
		el.Data.PolylineStruct.x[2*i+1]=(PLFLT)space[i]*0.00283;
	}
    
	m_listBuffer.append(el);
}

void QtPLBufferedDriver::setSolid()
{
	BufferElement el;
	el.Element=SET_SOLID;
	m_listBuffer.append(el);
}

// Draw the content of the buffer
void QtPLBufferedDriver::doPlot(QPainter* i_painterP, double x_fact, double y_fact, double x_offset, double y_offset)
{
	QLineF line;
	QPointF * polyline;
	PLINT npts;
	QVector<qreal> vect;
	
	if(i_painterP==NULL) i_painterP=m_painterP;
	
	QPen p=i_painterP->pen();
	QBrush b=i_painterP->brush();
	b.setStyle(Qt::SolidPattern);
	i_painterP->setBrush(b);
	
	i_painterP->fillRect(x_offset, y_offset, m_dWidth*x_fact, m_dHeight*y_fact, QBrush(Qt::black));
	
	if(m_listBuffer.empty())
	{
		return;
	}
	for(QLinkedList<BufferElement>::const_iterator i=m_listBuffer.begin(); i!=m_listBuffer.end(); ++i)
	{
		switch(i->Element)
		{
			case LINE:
				line=QLineF(i->Data.LineStruct.x1*x_fact+x_offset, i->Data.LineStruct.y1*y_fact+y_offset, i->Data.LineStruct.x2*x_fact+x_offset, i->Data.LineStruct.y2*y_fact+y_offset);
				i_painterP->drawLine(line);
				break;
            
			case POLYLINE:
				npts=i->Data.PolylineStruct.npts;
				polyline=new QPointF[npts];
				for(int j=0; j<npts; ++j)
				{
					polyline[j].setX(i->Data.PolylineStruct.x[j]*x_fact+x_offset);
					polyline[j].setY(i->Data.PolylineStruct.y[j]*y_fact+y_offset);
				}
				i_painterP->drawPolyline(polyline, npts);
				delete[] polyline;
				break;
                
			case POLYGON:
				npts=i->Data.PolylineStruct.npts;
				polyline=new QPointF[npts];
				for(int j=0; j<npts; ++j)
				{
					polyline[j].setX(i->Data.PolylineStruct.x[j]*x_fact+x_offset);
					polyline[j].setY(i->Data.PolylineStruct.y[j]*y_fact+y_offset);
				}
				i_painterP->drawPolygon(polyline, npts);

				delete[] polyline;
				break;
                
			case SET_WIDTH:
				
				p.setWidthF(i->Data.intParam);
				i_painterP->setPen(p);
				break;
                
			case SET_COLOUR:
				p.setColor(QColor(i->Data.ColourStruct.R, i->Data.ColourStruct.G, i->Data.ColourStruct.B));
				i_painterP->setPen(p);
				b.setColor(QColor(i->Data.ColourStruct.R, i->Data.ColourStruct.G, i->Data.ColourStruct.B));
				i_painterP->setBrush(b);
				break;
                
			case SET_DASHED:
				vect.clear();
				for(int j=0; j<i->Data.PolylineStruct.npts; j++)
				{
					if(i->Data.PolylineStruct.x[j]>=1.) //TODO remove that when bug #228554 of Qt is solved on Windows...
						vect << i->Data.PolylineStruct.x[j];
					else
						vect <<1.;
				}
				p.setDashPattern(vect);
				i_painterP->setPen(p);
				break;
                
			case SET_SOLID:
				p.setStyle(Qt::SolidLine);
				i_painterP->setPen(p);
				break;
			
			case SET_SMOOTH:
				i_painterP->setRenderHint(QPainter::Antialiasing, i->Data.intParam);
				break;
                
			default:
				break;
		}
	}
}

void QtPLBufferedDriver::getPlotParameters(double & io_dXFact, double & io_dYFact, double & io_dXOffset, double & io_dYOffset)
{
	io_dXFact=1.;
	io_dYFact=1.;
	io_dXOffset=0.;
	io_dYOffset=0.;
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
#if defined(PLD_qtwidget)
	if(widget==NULL) widget=dynamic_cast<QtPLTabWidget*>((QWidget *) pls->dev);
#endif
	if(widget==NULL) return;
	
	widget->setColor(pls->curcolor.r, pls->curcolor.g, pls->curcolor.b);
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
#if defined(PLD_qtwidget)
	if(widget==NULL) widget=dynamic_cast<QtPLTabWidget*>((QWidget *) pls->dev);
#endif
	if(widget==NULL) return;
	
	widget->setColor(pls->curcolor.r, pls->curcolor.g, pls->curcolor.b);
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
#if defined(PLD_qtwidget)
	if(widget==NULL) widget=dynamic_cast<QtPLTabWidget*>((QWidget *) pls->dev);
#endif
	if(widget==NULL) return;
		    
	switch(op)
	{
		case PLESC_DASH:
			widget->setDashed(pls->nms, pls->mark, pls->space);
			widget->setColor(pls->curcolor.r, pls->curcolor.g, pls->curcolor.b);
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
			widget->setColor(pls->curcolor.r, pls->curcolor.g, pls->curcolor.b);
			widget->drawPolygon(xa, ya, pls->dev_npts);
            
			delete[] xa;
			delete[] ya;
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
#if defined(PLD_qtwidget)
	if(widget==NULL) widget=dynamic_cast<QtPLTabWidget*>((QWidget *) pls->dev);
#endif
	if(widget==NULL) return;
    
	switch(op)
	{
		case PLSTATE_WIDTH:
			widget->setWidth(pls->width);
			break;
       
		case PLSTATE_COLOR0:
			widget->setColor(pls->curcolor.r, pls->curcolor.g, pls->curcolor.b);
			break;
			
		case PLSTATE_COLOR1:
			widget->setColor(pls->curcolor.r, pls->curcolor.g, pls->curcolor.b);
			break;

            
			default: break;
	}
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
	/* Stream setup */
	pls->color = 1;
	pls->plbuf_write=0;
	pls->dev_fill0 = 1;
	pls->dev_fill1 = 0;
	pls->dev_dash=1;
	pls->dev_flush=1;
	pls->dev_clear=1;
	pls->termin=0;
	pls->page = 0;
	
	// Initialised with the default (A4) size
	pls->dev=new QtRasterDevice;
	
	// Shamelessly copied on the Cairo stuff :)
	if (pls->xlength <= 0 || pls->ylength <= 0)
	{
		pls->xlength = ((QtRasterDevice*)(pls->dev))->m_dWidth;
		pls->ylength = ((QtRasterDevice*)(pls->dev))->m_dHeight;
	}
	
	if (pls->xlength > pls->ylength)
		((QtRasterDevice*)(pls->dev))->downscale = (PLFLT)pls->xlength/(PLFLT)(PIXELS_X-1);
	else
		((QtRasterDevice*)(pls->dev))->downscale = (PLFLT)pls->ylength/(PLFLT)PIXELS_Y;
	
	plP_setphy((PLINT) 0, (PLINT) (pls->xlength / ((QtRasterDevice*)(pls->dev))->downscale), (PLINT) 0, (PLINT) (pls->ylength / ((QtRasterDevice*)(pls->dev))->downscale));
	
	plP_setpxl(DPI/25.4/((QtRasterDevice*)(pls->dev))->downscale, DPI/25.4/((QtRasterDevice*)(pls->dev))->downscale);
	
	/* Initialize family file info */
	plFamInit(pls);
	
	plOpenFile(pls);
}

void plD_eop_rasterqt(PLStream *pls)
{
	if(qt_family_check(pls)) {return;} 
	((QtRasterDevice *)pls->dev)->savePlot();
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
	setSize(QSize(m_dWidth, m_dHeight));
}

QtSVGDevice::~QtSVGDevice()
{
}

void QtSVGDevice::definePlotName(const char* fileName)
{
	setFileName(QString(fileName));
	setResolution(72);

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
	pls->dev_dash=1;
	pls->dev_flush=1;
	pls->dev_clear=1;
	pls->termin=0;
	pls->page = 0;
	
	pls->dev=new QtSVGDevice;
	
	if (pls->xlength <= 0 || pls->ylength <= 0)
	{
		pls->xlength = ((QtSVGDevice*)(pls->dev))->m_dWidth;
		pls->ylength = ((QtSVGDevice*)(pls->dev))->m_dHeight;
	}
	
	if (pls->xlength > pls->ylength)
		((QtSVGDevice*)(pls->dev))->downscale = (PLFLT)pls->xlength/(PLFLT)(PIXELS_X-1);
	else
		((QtSVGDevice*)(pls->dev))->downscale = (PLFLT)pls->ylength/(PLFLT)PIXELS_Y;
	
	plP_setphy((PLINT) 0, (PLINT) (pls->xlength / ((QtSVGDevice*)(pls->dev))->downscale), (PLINT) 0, (PLINT) (pls->ylength / ((QtSVGDevice*)(pls->dev))->downscale));
	
	plP_setpxl(DPI/25.4/((QtSVGDevice*)(pls->dev))->downscale, DPI/25.4/((QtSVGDevice*)(pls->dev))->downscale);

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
	delete ((QtSVGDevice *)pls->dev);
	
	pls->dev=new QtSVGDevice(s.width(), s.height());
	((QtSVGDevice *)pls->dev)->downscale=downscale;
}

#endif

#if defined (PLD_epsqt) || defined(PLD_pdfqt)
QtEPSDevice::QtEPSDevice()
{
	setPageSize(QPrinter::A4);
	setResolution(DPI);
	setColorMode(QPrinter::Color);
	setOrientation(QPrinter::Landscape);
			
	m_dWidth=pageRect().width();
	m_dHeight=pageRect().height();
}

QtEPSDevice::~QtEPSDevice()
{
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
	pls->dev_dash=1;
	pls->dev_flush=1;
	pls->dev_clear=1;
	pls->termin=0;
	pls->page = 0;
	
	int argc=0;
	char argv[]={'\0'};
	// QPrinter devices won't create if there is no QApplication declared...
	QApplication * app=new QApplication(argc, (char**)&argv);
	pls->dev=new QtEPSDevice;
	delete app;
	
	if (pls->xlength <= 0 || pls->ylength <= 0)
	{
		pls->xlength = ((QtEPSDevice*)(pls->dev))->m_dWidth;
		pls->ylength = ((QtEPSDevice*)(pls->dev))->m_dHeight;
	}
	
	if (pls->xlength > pls->ylength)
		((QtEPSDevice*)(pls->dev))->downscale = (PLFLT)pls->xlength/(PLFLT)(PIXELS_X-1);
	else
		((QtEPSDevice*)(pls->dev))->downscale = (PLFLT)pls->ylength/(PLFLT)PIXELS_Y;
	
	plP_setphy((PLINT) 0, (PLINT) (pls->xlength / ((QtEPSDevice*)(pls->dev))->downscale), (PLINT) 0, (PLINT) (pls->ylength / ((QtEPSDevice*)(pls->dev))->downscale));
	
	plP_setpxl(DPI/25.4/((QtEPSDevice*)(pls->dev))->downscale, DPI/25.4/((QtEPSDevice*)(pls->dev))->downscale);
	
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
	
	((QtEPSDevice *)pls->dev)->savePlot();
	// Once saved, we have to create a new device with the same properties
	// to be able to plot another page.
	downscale=((QtEPSDevice *)pls->dev)->downscale;

	delete ((QtEPSDevice *)pls->dev);
	
	QApplication * app=new QApplication(argc, (char**)&argv);
	pls->dev=new QtEPSDevice;
	((QtEPSDevice *)pls->dev)->downscale=downscale;
	delete app;
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

#if defined (PLD_qtwidget)

QtPLWidget::QtPLWidget(int i_iWidth, int i_iHeight, QWidget* parent):
	 QWidget(parent), QtPLBufferedDriver(i_iWidth, i_iHeight)
{
	m_painterP=new QPainter;
	
	m_dAspectRatio=(double)i_iWidth/(double)i_iHeight;
	
	cursorParameters.isTracking=false;
	m_pixPixmap=NULL;
	m_iOldSize=0;
	
	resize(i_iWidth, i_iHeight);

}

QtPLWidget::~QtPLWidget()
{
	clearBuffer();
	if(m_pixPixmap!=NULL) delete m_pixPixmap;
}

void QtPLWidget::setSmooth(bool b)
{
	BufferElement el;
	el.Element=SET_SMOOTH;
	el.Data.intParam=b;
	m_listBuffer.append(el);
}

void QtPLWidget::clearWidget()
{
	clearBuffer();
	m_bAwaitingRedraw=true;
	update();
}

void QtPLWidget::captureMousePlotCoords(double * xi, double* yi, double * xf, double * yf)
{
	setMouseTracking(true);
	cursorParameters.isTracking=true;
	cursorParameters.cursor_start_x=
		cursorParameters.cursor_start_y=
		cursorParameters.cursor_end_x=
		cursorParameters.cursor_end_y=-1.;
	cursorParameters.step=1;
	do
	{
		QCoreApplication::processEvents(QEventLoop::AllEvents, 10);
        
	} while(cursorParameters.isTracking);
    
	PLFLT a,b;
	PLINT c;
	plcalc_world(cursorParameters.cursor_start_x, 1.-cursorParameters.cursor_start_y, &a, &b, &c);
	*xi=a;
	*yi=b;
	plcalc_world(cursorParameters.cursor_end_x, 1.-cursorParameters.cursor_end_y, &a, &b, &c);
	*xf=a;
	*yf=b;
    
}

void QtPLWidget::captureMouseDeviceCoords(double * xi, double* yi, double * xf, double * yf)
{
	setMouseTracking(true);
	cursorParameters.isTracking=true;
	cursorParameters.cursor_start_x=
			cursorParameters.cursor_start_y=
			cursorParameters.cursor_end_x=
			cursorParameters.cursor_end_y=-1.;
	cursorParameters.step=1;
	do
	{
		QCoreApplication::processEvents(QEventLoop::AllEvents, 10);
        
	} while(cursorParameters.isTracking);
    
	*xi=cursorParameters.cursor_start_x;
	*yi=cursorParameters.cursor_start_y;
	*xf=cursorParameters.cursor_end_x;
	*yf=cursorParameters.cursor_end_y;
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
	double x_fact, y_fact, x_offset(0.), y_offset(0.); //Parameters to scale and center the plot on the widget
	getPlotParameters(x_fact, y_fact, x_offset, y_offset);
	
	// If actual redraw, not just adding the cursor acquisition traces
	if(m_bAwaitingRedraw || m_pixPixmap==NULL || m_listBuffer.size()!=m_iOldSize  )
	{
		if(m_pixPixmap!=NULL) delete m_pixPixmap;
		m_pixPixmap=new QPixmap(width(), height());
		QPainter * painter=new QPainter;
		painter->begin(m_pixPixmap);
		painter->fillRect(0, 0, width(), height(), QBrush(Qt::white));
		painter->fillRect(0, 0, width(), height(), QBrush(Qt::gray, Qt::Dense4Pattern));
		painter->fillRect((int)x_offset, (int)y_offset, (int)floor(x_fact*m_dWidth+0.5), (int)floor(y_fact*m_dHeight+0.5), QBrush(Qt::black));
		painter->setRenderHint(QPainter::Antialiasing, true);
		doPlot(painter, x_fact, y_fact, x_offset, y_offset);
		painter->end();
		m_bAwaitingRedraw=false;
		m_iOldSize=m_listBuffer.size();
	}
	
	m_painterP->begin(this);
	
	// repaint plot
	m_painterP->drawPixmap(0, 0, *m_pixPixmap);

	// now paint the cursor tracking patterns
	if(cursorParameters.isTracking)
	{
		QPen p=m_painterP->pen();
		p.setColor(Qt::white);
		if(cursorParameters.step==1)
		{
			m_painterP->setPen(p);
			m_painterP->drawLine((int)(cursorParameters.cursor_start_x*x_fact*m_dWidth+x_offset), 0, (int)(cursorParameters.cursor_start_x*x_fact*m_dWidth+x_offset), height());
			m_painterP->drawLine(0, (int)(cursorParameters.cursor_start_y*y_fact*m_dHeight+y_offset), width(), (int)(cursorParameters.cursor_start_y*y_fact*m_dHeight+y_offset));
		}
		else
		{
			p.setStyle(Qt::DotLine);
			m_painterP->setPen(p);
			m_painterP->drawLine((int)(cursorParameters.cursor_start_x*x_fact*m_dWidth+x_offset), 0, (int)(cursorParameters.cursor_start_x*x_fact*m_dWidth+x_offset), height());
			m_painterP->drawLine(0, (int)(cursorParameters.cursor_start_y*y_fact*m_dHeight+y_offset), width(), (int)(cursorParameters.cursor_start_y*y_fact*m_dHeight+y_offset));
			p.setStyle(Qt::SolidLine);
			m_painterP->setPen(p);
			m_painterP->drawLine((int)(cursorParameters.cursor_end_x*x_fact*m_dWidth+x_offset), 0, (int)(cursorParameters.cursor_end_x*x_fact*m_dWidth+x_offset), height());
			m_painterP->drawLine(0, (int)(cursorParameters.cursor_end_y*y_fact*m_dHeight+y_offset), width(), (int)(cursorParameters.cursor_end_y*y_fact*m_dHeight+y_offset));
		}
	}
    
	m_painterP->end();
}

void QtPLWidget::mousePressEvent(QMouseEvent* event)
{
	if(!cursorParameters.isTracking) return;
    
	double x_fact, y_fact, x_offset, y_offset; //Parameters to scale and center the plot on the widget
    
	getPlotParameters(x_fact, y_fact, x_offset, y_offset);
	
	PLFLT X=(PLFLT)(event->x()-x_offset)/(m_dWidth*x_fact);
	PLFLT Y=(PLFLT)(event->y()-y_offset)/(m_dHeight*y_fact);
	
	if(cursorParameters.step==1)
	{
		cursorParameters.cursor_start_x=X;
		cursorParameters.cursor_start_y=Y;
		cursorParameters.step=2; // First step of selection done, going to the next one
		update();
	}
}

void QtPLWidget::mouseReleaseEvent(QMouseEvent* event)
{
	double x_fact, y_fact, x_offset, y_offset; //Parameters to scale and center the plot on the widget
    
	getPlotParameters(x_fact, y_fact, x_offset, y_offset);
	
	PLFLT X=(PLFLT)(event->x()-x_offset)/(m_dWidth*x_fact);
	PLFLT Y=(PLFLT)(event->y()-y_offset)/(m_dHeight*y_fact);
    
	if(cursorParameters.step!=1)
	{
		cursorParameters.cursor_end_x=X;
		cursorParameters.cursor_end_y=Y;
		cursorParameters.isTracking=false;
		setMouseTracking(false);
		update();
	}
}

void QtPLWidget::mouseMoveEvent(QMouseEvent* event)
{
	this->activateWindow();
	this->raise();
	
	if(!cursorParameters.isTracking) return;
    
	double x_fact, y_fact, x_offset, y_offset; //Parameters to scale and center the plot on the widget
    
	getPlotParameters(x_fact, y_fact, x_offset, y_offset);
	
	PLFLT X=(PLFLT)(event->x()-x_offset)/(m_dWidth*x_fact);
	PLFLT Y=(PLFLT)(event->y()-y_offset)/(m_dHeight*y_fact);
    
	if(cursorParameters.step==1)
	{
		cursorParameters.cursor_start_x=X;
		cursorParameters.cursor_start_y=Y;
	}
	else
	{
		cursorParameters.cursor_end_x=X;
		cursorParameters.cursor_end_y=Y;
	}

	update();
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

void QtPLTabWidget::newTab()
{
	QtPLWidget * plotWidget=new QtPLWidget;
	plotWidget->downscale=downscale;
	plotWidget->m_dWidth=m_dWidth;
	plotWidget->m_dHeight=m_dHeight;
	addTab(plotWidget, QString("page %1").arg(count()+1));
	currentWidget=plotWidget;
}


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
	pdt->pl_tidy     = (plD_tidy_fp)     plD_tidy_qtwidget;
	pdt->pl_state    = (plD_state_fp)    plD_state_qt;
	pdt->pl_esc      = (plD_esc_fp)      plD_esc_qt;
}

// global variables initialised in init(), used in tidy()
QApplication* app;
int argc=1; // argc and argv have to exist when tidy() is used, thus they are made global
char argv[20];

void plD_init_qtwidget(PLStream * pls)
{
	PLINT w, h;
	argv[0]='\0';
	app=new QApplication(argc, (char**)&argv);
	QMainWindow * mw=new QMainWindow;
	QtPLTabWidget* tabWidget=new QtPLTabWidget;

	plsc->dev = (void*)tabWidget;
	plsc->xlength = tabWidget->m_dWidth;
	plsc->ylength = tabWidget->m_dHeight;
	
	if (plsc->xlength > plsc->ylength)
		tabWidget->downscale = (PLFLT)plsc->xlength/(PLFLT)(PIXELS_X-1);
	else
		tabWidget->downscale = (PLFLT)plsc->ylength/(PLFLT)PIXELS_Y;
	
	plP_setphy((PLINT) 0, (PLINT) (plsc->xlength / tabWidget->downscale), (PLINT) 0, (PLINT) (plsc->ylength / tabWidget->downscale));
	
	plP_setpxl(DPI/25.4/tabWidget->downscale, DPI/25.4/tabWidget->downscale);

	pls->color = 1;		/* Is a color device */
	pls->plbuf_write=0;
	pls->dev_fill0 = 1;	/* Handle solid fills */
	pls->dev_fill1 = 0;
	pls->dev_dash=1;
	pls->dev_flush=1;
	pls->dev_clear=1;
	pls->termin=1;
	
	mw->setCentralWidget(tabWidget);
	mw->setVisible(true);
	mw->setWindowTitle("plplot");
	mw->resize(plsc->xlength, plsc->ylength);
	app->setActiveWindow(mw);
	
}

void plD_eop_qtwidget(PLStream *pls)
{
	QtPLTabWidget* tabWidget=((QtPLTabWidget*)pls->dev);
	tabWidget->currentWidget=NULL;
}

void plD_tidy_qtwidget(PLStream *pls)
{
    app->exec();
}
#endif

#endif
