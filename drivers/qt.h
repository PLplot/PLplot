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
  
*/

#ifndef QT_H
#define QT_H

#include <iostream>
#include <QImage>
#include <QPainter>
#include <QLinkedList>
#include <QSvgGenerator>
#include <QPrinter>
#include <QApplication>
#include <QWidget>
#include <QMouseEvent>

#include "plplotP.h"
#include "drivers.h"

#define DPI 72
#define QT_DEFAULT_X 842
#define QT_DEFAULT_Y 598

typedef enum ElementType_
{
 LINE,
 POLYLINE,
 POLYGON,
 SET_WIDTH,
 SET_COLOUR,
 SET_DASHED,
 SET_SOLID,
 SET_SMOOTH
} ElementType;

class BufferElement
{
	public:
        
		ElementType Element;
    
		union DataType
		{
			struct LineStruct_
			{
				PLFLT x1;
				PLFLT x2;
				PLFLT y1;
				PLFLT y2;
			} LineStruct;
        
			struct PolylineStruct_
			{
				PLINT npts;
				PLFLT* x;
				PLFLT* y;
			} PolylineStruct;
        
			struct ColourStruct_
			{
				PLINT R, G, B;
			} ColourStruct;
        
			PLINT intParam;
			PLFLT fltParam;
		} Data;
};


class QtPLDriver
{
	public:
		QtPLDriver(PLINT i_iWidth=QT_DEFAULT_X, PLINT i_iHeight=QT_DEFAULT_Y);
		
		virtual ~QtPLDriver();
			
		virtual void drawLine(short x1, short y1, short x2, short y2);
			
		virtual void drawPolyline(short * x, short * y, PLINT npts);
			
		virtual void drawPolygon(short * x, short * y, PLINT npts);

		virtual void setColor(int r, int g, int b);

		virtual void setWidth(PLINT w);

		virtual void setDashed(PLINT nms, PLINT* mark, PLINT* space);

		virtual void setSolid();
		
		virtual void savePlot(char* fileName);
		
		double downscale;
		double m_dWidth, m_dHeight;
	protected:
		
		QPainter* m_painterP;
};

class QtPLBufferedDriver: public QtPLDriver
{
	public:
		QtPLBufferedDriver(PLINT i_iWidth=QT_DEFAULT_X, PLINT i_iHeight=QT_DEFAULT_Y);

		virtual ~QtPLBufferedDriver();
			
		virtual void drawLine(short x1, short y1, short x2, short y2);
			
		virtual void drawPolyline(short * x, short * y, PLINT npts);
			
		virtual void drawPolygon(short * x, short * y, PLINT npts);

		virtual void setColor(int r, int g, int b);

		virtual void setWidth(PLINT r);

		virtual void setDashed(PLINT nms, PLINT* mark, PLINT* space);

		virtual void setSolid();
		
		virtual void savePlot(char* fileName);
		
	protected:
		
		virtual void doPlot(QPainter* p=NULL, double x_fact=1., double y_fact=1., double x_offset=0., double y_offset=0.);
		
		virtual void getPlotParameters(double & io_dXFact, double & io_dYFact, double & io_dXOffset, double & io_dYOffset);
		
		void clearBuffer();
		
		QLinkedList<BufferElement> m_listBuffer;
};

#if defined (PLD_rasterqt)
class QtRasterDevice: public QtPLDriver, public QImage
{
	public:
		QtRasterDevice(int i_iWidth=QT_DEFAULT_X,
			    int i_iHeight=QT_DEFAULT_Y);
		
		virtual ~QtRasterDevice(){}
		
		QString getFileName(char* fileName);
		
		void savePlot(char* fileName);
		
	protected:
		int pageCounter;
		
};
#endif

#if defined (PLD_svgqt)
class QtSVGDevice: public QtPLBufferedDriver, public QSvgGenerator
{
	public:
		QtSVGDevice(int i_iWidth=QT_DEFAULT_X,
			    int i_iHeight=QT_DEFAULT_Y);

		virtual ~QtSVGDevice();
		
		QString getFileName(char* fileName);

		void savePlot(char* fileName);
			
		int pageCounter;
	protected:
};
#endif

#if defined (PLD_epspdfqt)
class QtEPSDevice: public QtPLBufferedDriver, public QPrinter
{
	public:
		QtEPSDevice();
		
		virtual ~QtEPSDevice();
		
		QString getFileName(char* fileName);
			 
		void savePlot(char* fileName);
			
		int pageCounter;
	protected:

};
#endif

#if defined (PLD_qtwidget)

class QtPLWidget: public QWidget, public QtPLBufferedDriver
{
	Q_OBJECT

	public:
		// Parameters are the actual size of the page, NOT the size of the widget
		// Call QWidget::resize for that
		QtPLWidget(int i_iWidth=QT_DEFAULT_X, int i_iHeight=QT_DEFAULT_Y, QWidget* parent=0);
		
		virtual ~QtPLWidget();
		
		void setSmooth(bool);
		
		void clearWidget();
	
	public slots:
		
		void captureMousePlotCoords(double* xi, double* yi, double* xf, double* yf);
		void captureMouseDeviceCoords(double* xi, double* yi, double* xf, double* yf);
		
	protected slots:
		
		void resizeEvent(QResizeEvent*);
		void paintEvent(QPaintEvent*);
		void mousePressEvent(QMouseEvent*);
		void mouseReleaseEvent(QMouseEvent*);
		void mouseMoveEvent(QMouseEvent*);
	
	protected:
		
		void getPlotParameters(double & io_dXFact, double & io_dYFact, double & io_dXOffset, double & io_dYOffset);
		
		struct
		{
			bool isTracking;
			int step; //1 for first part, 2 for second part
			double cursor_start_x, cursor_start_y;
			double cursor_end_x, cursor_end_y;
		} cursorParameters;
		
		double m_dAspectRatio;
		QPixmap * m_pixPixmap; // to avoid redrawing everything
		bool m_bAwaitingRedraw;
		int m_iOldSize;
};

void plsqtdev(QtPLWidget *plotdev);
#endif

#endif
