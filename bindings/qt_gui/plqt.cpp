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

// Global variables for Qt driver.
PLDLLIMPEXP_QT_DATA(int) vectorize = 0;
PLDLLIMPEXP_QT_DATA(MasterHandler) handler;

// Master Device Handler for multiple streams
// Only handles multiple Qt devices
MasterHandler::MasterHandler():QObject()
{
  masterDevice=NULL;
}

bool MasterHandler::isMasterDevice(QtPLDriver* d)
{
  return d==masterDevice;
}

void MasterHandler::setMasterDevice(QtPLDriver* d)
{
  masterDevice=d;
}

void MasterHandler::DeviceChangedPage(QtPLDriver* d)
{
  if(d==masterDevice)
    {
      emit MasterChangedPage();
    }
}

void MasterHandler::DeviceClosed(QtPLDriver* d)
{
  if(d==masterDevice)
    {
      emit MasterClosed();
      masterDevice=NULL;
    }
}

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
// 	if(!m_painterP->isActive()) return;

	QRectF rect(0., 0., 0., 0.);
	QRectF bounding;
	QPicture tempPic;
	QPainter tempPainter(&tempPic);
	tempPainter.setFont(p->font());
	
	if(vectorize)
	{
		bounding=tempPainter.boundingRect(rect, Qt::AlignHCenter|Qt::AlignVCenter|Qt::TextDontClip, text);
		
		QPen savePen=tempPainter.pen();
		QPen pen=savePen;
		pen.setStyle(Qt::NoPen);
		tempPainter.setPen(pen);
		
		double offset=QFontMetrics(tempPainter.font(), &tempPic).boundingRect(text).top(); // Distance between the baseline and the top of the bounding box
		
		QPainterPath path;
		path.addText(QPointF(bounding.left(), bounding.top()-offset), tempPainter.font(), text);
		tempPainter.drawPath(path);
		tempPainter.setPen(pen);
	}
	else
	{
		tempPainter.drawText(rect, Qt::AlignHCenter|Qt::AlignVCenter|Qt::TextDontClip, text, &bounding);
	}
	
	tempPainter.end();
	
	p->drawPicture((int)(xOffset+bounding.width()/2.), -yOffset, tempPic);
	
	xOffset+=bounding.width();
}

QPicture QtPLDriver::getTextPicture(PLUNICODE fci, PLUNICODE* text, int len, PLFLT chrht)
{
	char plplotEsc;
	plgesc( &plplotEsc );
	
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
				currentString.append(QString(QChar(text[i])));
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
	PLUNICODE fci;
	plgfci( &fci );
	QPicture picText=getTextPicture(fci, txt->unicode_array, txt->unicode_array_len, pls->chrht);
	picDpi=picText.logicalDpiY();

	m_painterP->setClipping(true);
	m_painterP->setClipRect(QRect((int)(pls->clpxmi*downscale), (int)(m_dHeight-pls->clpyma*downscale), (int)((pls->clpxma-pls->clpxmi)*downscale), (int)((pls->clpyma-pls->clpymi)*downscale)), Qt::ReplaceClip);

	rotation -= pls->diorot * M_PI / 2.0;
	m_painterP->translate(txt->x*downscale, m_dHeight-txt->y*downscale);
	QMatrix rotShearMatrix(cos(rotation)*stride, -sin(rotation)*stride, cos(rotation)*sin(shear)+sin(rotation)*cos(shear), -sin(rotation)*sin(shear)+cos(rotation)*cos(shear), 0., 0.);

	m_painterP->setWorldMatrix(rotShearMatrix, true);

	m_painterP->translate(-txt->just*xOffset*m_painterP->device()->logicalDpiY()/picDpi, 0.);

	m_painterP->drawPicture(0, 0, picText);

	m_painterP->setWorldMatrix(QMatrix());
	m_painterP->setClipping(false);
}

void QtPLDriver::setColor(int r, int g, int b, double alpha)
{
	if(!m_painterP->isActive()) return;

	QPen p=m_painterP->pen();
	p.setColor(QColor(r, g, b, (int)(alpha*255)));
	m_painterP->setPen(p);
	
	QBrush B=m_painterP->brush();
	B.setColor(QColor(r, g, b, (int)(alpha*255)));
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
	delete m_painterP;
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
	delete m_painterP;
}

void QtSVGDevice::definePlotName(const char* fileName)
{
	setFileName(QString(fileName));
	setResolution(POINTS_PER_INCH);
	setSize(QSize((int)m_dWidth, (int)m_dHeight));
#if QT_VERSION >=  0x040500
        setViewBox(QRect(0, 0, (int)m_dWidth, (int)m_dHeight));
#endif

	m_painterP=new QPainter(this);
	m_painterP->fillRect(0, 0, (int)m_dWidth, (int)m_dHeight, QBrush(Qt::black));
}

void QtSVGDevice::savePlot()
{
	m_painterP->end();
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
	delete m_painterP;
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
	m_painterP->fillRect(0, 0, (int)m_dWidth, (int)m_dHeight, QBrush(Qt::black));
}

void QtEPSDevice::savePlot()
{
	m_painterP->end();
}
#endif

#if defined (PLD_qtwidget) || defined(PLD_extqt)
QtPLWidget::QtPLWidget(int i_iWidth, int i_iHeight, QWidget* parent):
	 QWidget(parent), QtPLDriver(i_iWidth, i_iHeight)
{
	m_painterP=new QPainter;
	
	m_dAspectRatio=m_dWidth/m_dHeight;
	
	m_pixPixmap=NULL;
	m_iOldSize=0;
	pageNumber=0;
	resize(i_iWidth, i_iHeight);
}

QtPLWidget::~QtPLWidget()
{
	clearBuffer();
	delete m_pixPixmap;
}

void QtPLWidget::clearWidget()
{
	clearBuffer();
	m_bAwaitingRedraw=true;
	update();
}

void QtPLWidget::clearBuffer()
{
	for(QLinkedList<BufferElement>::iterator i=m_listBuffer.begin(); i!=m_listBuffer.end(); ++i)
	{
		switch(i->Element)
		{
			case POLYLINE:
			case POLYGON:
				delete[] i->Data.PolylineStruct->x;
				delete[] i->Data.PolylineStruct->y;
				delete i->Data.PolylineStruct;
				break;
				
			case LINE:
				delete i->Data.LineStruct;
				break;
            
			case SET_COLOUR:
				delete i->Data.ColourStruct;
				break;
				
			case TEXT:
				delete[] i->Data.TextStruct->text;
				delete i->Data.TextStruct;
				break;
			default:
				break;
		}
	}
    
	m_listBuffer.clear();
}

void QtPLWidget::drawLine(short x1, short y1, short x2, short y2)
{
	BufferElement el;
	el.Element=LINE;
	el.Data.LineStruct=new struct LineStruct_;
	el.Data.LineStruct->x1=(PLFLT)x1*downscale;
	el.Data.LineStruct->y1=m_dHeight-(PLFLT)y1*downscale;
	el.Data.LineStruct->x2=(PLFLT)x2*downscale;
	el.Data.LineStruct->y2=m_dHeight-(PLFLT)y2*downscale;

	m_listBuffer.append(el);
}

void QtPLWidget::drawPolyline(short * x, short * y, PLINT npts)
{
	BufferElement el;
	el.Element=POLYLINE;
	el.Data.PolylineStruct=new struct PolylineStruct_;
	el.Data.PolylineStruct->npts=(PLINT)npts;
	el.Data.PolylineStruct->x=new PLFLT[npts];
	el.Data.PolylineStruct->y=new PLFLT[npts];
	for(int i=0; i<npts; ++i)
	{
		el.Data.PolylineStruct->x[i]=(PLFLT)x[i]*downscale;
		el.Data.PolylineStruct->y[i]=m_dHeight-(PLFLT)y[i]*downscale;
	}
    
	m_listBuffer.append(el);
}

void QtPLWidget::drawPolygon(short * x, short * y, PLINT npts)
{
	BufferElement el;
	el.Element=POLYGON;
	el.Data.PolylineStruct=new struct PolylineStruct_;
	el.Data.PolylineStruct->npts=(PLINT)npts;
	el.Data.PolylineStruct->x=new PLFLT[npts];
	el.Data.PolylineStruct->y=new PLFLT[npts];
	for(int i=0; i<npts; ++i)
	{
		el.Data.PolylineStruct->x[i]=(PLFLT)x[i]*downscale;
		el.Data.PolylineStruct->y[i]=m_dHeight-(PLFLT)y[i]*downscale;
	}
    
	m_listBuffer.append(el);
}

void QtPLWidget::setColor(int r, int g, int b, double alpha)
{
	BufferElement el;
	el.Element=SET_COLOUR;
	el.Data.ColourStruct=new struct ColourStruct_;
	el.Data.ColourStruct->R=r;
	el.Data.ColourStruct->G=g;
	el.Data.ColourStruct->B=b;
	el.Data.ColourStruct->A=alpha*255.;

	m_listBuffer.append(el);
}

void QtPLWidget::setSolid()
{
	BufferElement el;
	el.Element=SET_SOLID;
	m_listBuffer.append(el);
}

void QtPLWidget::setWidth(PLINT w)
{
	BufferElement el;
	el.Element=SET_WIDTH;
	el.Data.intParam=w;
	m_listBuffer.append(el);
}

void QtPLWidget::drawText(PLStream* pls, EscText* txt)
{
	BufferElement el;
	el.Element=TEXT;
	el.Data.TextStruct=new struct TextStruct_;
	el.Data.TextStruct->x=txt->x*downscale;
	el.Data.TextStruct->y=m_dHeight-txt->y*downscale;
	el.Data.TextStruct->clipxmin=pls->clpxmi*downscale;
	el.Data.TextStruct->clipymin=m_dHeight-pls->clpymi*downscale;
	el.Data.TextStruct->clipxmax=pls->clpxma*downscale;
	el.Data.TextStruct->clipymax=m_dHeight-pls->clpyma*downscale;
	PLUNICODE fci;
	plgfci(&fci);
	el.Data.TextStruct->fci=fci;
	PLFLT rotation, shear, stride;
	plRotationShear( txt->xform, &rotation, &shear, &stride);
	rotation -= pls->diorot * M_PI / 2.0;
	el.Data.TextStruct->rotation=rotation;
	el.Data.TextStruct->shear=shear;
	el.Data.TextStruct->stride=stride;
	el.Data.TextStruct->just=txt->just;
	el.Data.TextStruct->text=new PLUNICODE[txt->unicode_array_len];
	memcpy(el.Data.TextStruct->text, txt->unicode_array, txt->unicode_array_len*sizeof(PLUNICODE));
	el.Data.TextStruct->len=txt->unicode_array_len;
	el.Data.TextStruct->chrht=pls->chrht;
	
	m_listBuffer.append(el);

}

void QtPLWidget::drawTextInPicture(QPainter* p, const QString& text)
{
	QRectF rect(0., 0., 0., 0.);
	QRectF bounding;
	QPicture tempPic;
	QPainter tempPainter(&tempPic);
	tempPainter.setFont(p->font());
	
	if(vectorize)
	{
		bounding=tempPainter.boundingRect(rect, Qt::AlignHCenter|Qt::AlignVCenter|Qt::TextDontClip, text);
		
		QPen savePen=tempPainter.pen();
		QPen pen=savePen;
		pen.setStyle(Qt::NoPen);
		tempPainter.setPen(pen);
		
		double offset=QFontMetrics(tempPainter.font(), &tempPic).boundingRect(text).top(); // Distance between the baseline and the top of the bounding box
		
		QPainterPath path;
		path.addText(QPointF(bounding.left(), bounding.top()-offset), tempPainter.font(), text);
		tempPainter.drawPath(path);
		tempPainter.setPen(pen);
	}
	else
	{
		tempPainter.drawText(rect, Qt::AlignHCenter|Qt::AlignVCenter|Qt::TextDontClip, text, &bounding);
		
	}
	
	tempPainter.end();
	
	p->drawPicture((int)(xOffset+bounding.width()/2.), -yOffset, tempPic);
	xOffset+=bounding.width();
}

void QtPLWidget::renderText(QPainter* p, struct TextStruct_* s, double x_fact, double x_offset, double y_fact, double y_offset)
{
	if(s->len<=0 || s->len>=500) return;
	QPicture picText=getTextPicture(s->fci, s->text, s->len, s->chrht*y_fact);
	
	double picDpi=picText.logicalDpiY();

	p->setClipping(true);
	p->setClipRect(QRectF(s->clipxmin*x_fact+x_offset, s->clipymax*y_fact+y_offset, (s->clipxmax-s->clipxmin)*x_fact, (-s->clipymax+s->clipymin)*y_fact), Qt::ReplaceClip);
	
	p->translate(s->x*x_fact+x_offset, s->y*y_fact+y_offset);
	QMatrix rotShearMatrix(cos(s->rotation)*s->stride, -sin(s->rotation)*s->stride, cos(s->rotation)*sin(s->shear)+sin(s->rotation)*cos(s->shear), -sin(s->rotation)*sin(s->shear)+cos(s->rotation)*cos(s->shear), 0.,0.);
	p->setWorldMatrix(rotShearMatrix, true);

	p->translate(-s->just*xOffset*p->device()->logicalDpiY()/picDpi, 0.);
	
	p->drawPicture(0., 0., picText);
	
	p->setWorldMatrix(QMatrix());

	p->setClipping(false);
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
	delete m_pixPixmap;
	m_pixPixmap=NULL;
}

void QtPLWidget::paintEvent( QPaintEvent * )
{
	double x_fact, y_fact, x_offset(0.), y_offset(0.); //Parameters to scale and center the plot on the widget
	
	getPlotParameters(x_fact, y_fact, x_offset, y_offset);
	if(m_bAwaitingRedraw || m_pixPixmap==NULL || m_listBuffer.size()!=m_iOldSize) // If must regenerate image, draw it in the pixmap
	{
		delete m_pixPixmap;
		m_pixPixmap=new QPixmap(width(), height());
		QPainter* painter=new QPainter;
		painter->begin(m_pixPixmap);
		
		// Draw the margins and the background
		painter->fillRect(0, 0, width(), height(), QBrush(Qt::white));
		painter->fillRect(0, 0, width(), height(), QBrush(Qt::gray, Qt::Dense4Pattern));
		painter->fillRect((int)x_offset, (int)y_offset, (int)floor(x_fact*m_dWidth+0.5), (int)floor(y_fact*m_dHeight+0.5), QBrush(Qt::black));
		
		// Draw the plot
		doPlot(painter, x_fact, y_fact, x_offset, y_offset);
		painter->end();
		
		m_bAwaitingRedraw=false;
		m_iOldSize=m_listBuffer.size();
		
		delete painter;
	}
	
	// draw the current pixmap
	m_painterP->begin(this);
	
	m_painterP->drawPixmap(0, 0, *m_pixPixmap);
	
	m_painterP->end();
}

void QtPLWidget::doPlot(QPainter* p, double x_fact, double y_fact, double x_offset, double y_offset)
{
	QLineF line;
	QPointF * polyline;
	PLINT npts;
	QVector<qreal> vect;
	QPen pen=p->pen();
	QBrush brush=p->brush();
	brush.setStyle(Qt::SolidPattern);
	p->setBrush(brush);
	p->setRenderHints(QPainter::Antialiasing, true);
	
	if(m_listBuffer.empty())
	{
		p->fillRect(0, 0, (int)(x_fact+2.*x_offset), (int)(y_fact+2.*y_offset), QBrush());
		return;
	}
	// unrolls the buffer and draws each element accordingly
	for(QLinkedList<BufferElement>::const_iterator i=m_listBuffer.begin(); i!=m_listBuffer.end(); ++i)
	{
		switch(i->Element)
		{
			case LINE:
				line=QLineF(i->Data.LineStruct->x1*x_fact+x_offset, i->Data.LineStruct->y1*y_fact+y_offset, i->Data.LineStruct->x2*x_fact+x_offset, i->Data.LineStruct->y2*y_fact+y_offset);
				p->drawLine(line);
				break;
            
			case POLYLINE:
				npts=i->Data.PolylineStruct->npts;
				polyline=new QPointF[npts];
				for(int j=0; j<npts; ++j)
				{
					polyline[j].setX(i->Data.PolylineStruct->x[j]*x_fact+x_offset);
					polyline[j].setY(i->Data.PolylineStruct->y[j]*y_fact+y_offset);
				}
				p->drawPolyline(polyline, npts);
				delete[] polyline;
				break;
                
			case POLYGON:
				npts=i->Data.PolylineStruct->npts;
				polyline=new QPointF[npts];
				for(int j=0; j<npts; ++j)
				{
					polyline[j].setX(i->Data.PolylineStruct->x[j]*x_fact+x_offset);
					polyline[j].setY(i->Data.PolylineStruct->y[j]*y_fact+y_offset);
				}
				p->drawPolygon(polyline, npts);
				delete[] polyline;
				break;
                
			case TEXT:
				renderText(p, i->Data.TextStruct, x_fact, x_offset, y_fact, y_offset);
				break;
				
			case SET_WIDTH:
				pen.setWidth(i->Data.intParam);
				p->setPen(pen);
				break;

			case SET_COLOUR:
				pen.setColor(QColor(i->Data.ColourStruct->R, i->Data.ColourStruct->G, i->Data.ColourStruct->B, i->Data.ColourStruct->A));
				p->setPen(pen);
				brush.setColor(QColor(i->Data.ColourStruct->R, i->Data.ColourStruct->G, i->Data.ColourStruct->B, i->Data.ColourStruct->A));
				p->setBrush(brush);
				break;

			case SET_SOLID:
				pen.setStyle(Qt::SolidLine);
				p->setPen(pen);
				break;
			
			default:
				break;
		}
	}
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

#if defined(PLD_extqt)
QtExtWidget::QtExtWidget(int i_iWidth, int i_iHeight, QWidget* parent):
	QtPLWidget(i_iWidth, i_iHeight, parent)
{
	cursorParameters.isTracking=false;
	cursorParameters.cursor_x=-1.0;
	cursorParameters.cursor_y=-1.0;
	killed=false;
}

QtExtWidget::~QtExtWidget()
{
	killed=true;
	QCoreApplication::processEvents(QEventLoop::AllEvents, 10);
	delete m_pixPixmap;
	delete m_painterP;
// 	if(pic!=NULL) delete pic;
	m_pixPixmap=NULL;
	m_painterP=NULL;
// 	pic=NULL;
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

	p.drawLine((int)cursorParameters.cursor_x, 0, (int)cursorParameters.cursor_x, height());
	p.drawLine(0, (int)cursorParameters.cursor_y, width(), (int)cursorParameters.cursor_y);

	p.end();
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

#include "moc_files.h"
