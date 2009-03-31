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

/*
Interpretation of the -geometry XSIZExYSIZE option (or the third and fourth 
  parameters of plspage if those are specified by the user instead) for
  the various devices:
- the raster devices (bmpqt, jpgqt, pngqt, ppmqt, tiffqt):
  XSIZE and YSIZE define the x and y size in pixels
- qtwidget:
  XSIZE and YSIZE define the default x and y size of the widget in 
  pixels, as well as its aspect ratio, which is kept when the widget is
  resized.
- svgqt, epsqt, pdfqt:
  XSIZE and YSIZE define the x and y size in points (1/72 of inches).
  EPS and PDF files will be drawn in A4 pages for Qt versions before 4.4

Interpretation of the -dpi DPI option (or the first parameter of
  plspage if that is specified by the user instead).
  DPI is ignored for all but the raster devices.  For those devices 
  DPI should be set to the DPI value of the monitor being used to view
  the results if exact character sizes are desired.  Otherwise, DEFAULT_DPI
  (set to an average value for modern monitors) is used instead.
 */

#ifndef QT_H
#define QT_H

#include <iostream>
#include <QImage>
#include <QPainter>
#include <QLinkedList>
#include <QPrinter>
#include <QApplication>
#include <QWidget>
#include <QMouseEvent>
#include <QTabWidget>
#include <QMainWindow>
#include <QPicture>
#include <QMutex>

#include "plplotP.h"
#include "drivers.h"

// Used for devices (epsqt, pdfqt, svgqt) with known physical size in points.
#define POINTS_PER_INCH 72

// Average value of dots per inch assumed for modern monitors if the user
// does not specify a value appropriate to their monitor with plspage or
// the -dpi option.  Used only for devices with size specified in pixels 
// but not qtwidget since it has independent access to information (e.g.,
// delivered by X) about the DPI of the monitor.  So this value is only
// used for the raster devices (bmpqt, jpgqt, pngqt, ppmqt, tiffqt).
#define DEFAULT_DPI 72

// These values are in units of pixels (the raster devices and qtwidget)
// or points (epsqt, pdfqt, svgqt).  In the points case, this corresponds
// to the A4 paper size.
#define QT_DEFAULT_X 842
#define QT_DEFAULT_Y 595

class QtPLDriver;

// Master Device Handler for multiple streams
// Only handles multiple Qt devices
class MasterHandler:public QObject
{
	Q_OBJECT
	
	public:
		MasterHandler():QObject()
		{
			masterDevice=NULL;
		}

		~MasterHandler(){}

		bool isMasterDevice(QtPLDriver* d)
		{
			return d==masterDevice;
		}

		void setMasterDevice(QtPLDriver* d)
		{
			masterDevice=d;
		}
		
		void DeviceChangedPage(QtPLDriver* d)
		{
			if(d==masterDevice)
			{
				emit MasterChangedPage();
			}
		}

		void DeviceClosed(QtPLDriver* d)
		{
			if(d==masterDevice)
			{
				emit MasterClosed();
				masterDevice=NULL;
			}
		}
	signals:
		void MasterChangedPage();	

		void MasterClosed();

	protected:
		QtPLDriver* masterDevice;
};

// Basic class, offering the common interface to all Qt plplot devices
class QtPLDriver
{
	public:
		// Constructor, taking the device size as arguments
		QtPLDriver(PLINT i_iWidth=QT_DEFAULT_X, PLINT i_iHeight=QT_DEFAULT_Y);
		
		virtual ~QtPLDriver(); // does not delete emitter!
		
		// Draws a line from (x1, y1) to (x2, y2) in internal plplot coordinates
		virtual void drawLine(short x1, short y1, short x2, short y2);

		virtual void drawPolyline(short * x, short * y, PLINT npts);
			
		virtual void drawPolygon(short * x, short * y, PLINT npts);

		virtual void drawText(PLStream* pls, EscText* txt);

		virtual void setColor(int r, int g, int b, double alpha);

		virtual void setWidth(PLINT w);

		virtual void setDashed(PLINT nms, PLINT* mark, PLINT* space);

		// Set pen to draw solid strokes (called after drawing dashed strokes)
		virtual void setSolid();

		// Conversion factor from internal plplot coordinates to device coordinates
		double downscale;
		double m_dWidth, m_dHeight;
		
		static QMutex mutex; // All-purpose mutex

	protected:
	
		// Returns font with the good size for a QPicture's resolution
		QFont getFont(PLUNICODE code);
		// Draws text in a QPicture using a sub-QPicture (!), updates the current xOffset
		void drawTextInPicture(QPainter*, const QString&);
		// Gets the QPicture displaying text, with the base chrht char height
		QPicture getTextPicture(PLUNICODE* text, int len, int chrht);
		
		// Text-related variables
		bool underlined;
		bool overlined;
		double currentFontScale;
		double currentFontSize;
		double yOffset;
		double xOffset;
		
// 		double fontScalingFactor;// To have a nice font size on qtwidget
		
		QPainter* m_painterP;
};

#if defined (PLD_bmpqt) || defined(PLD_jpgqt) || defined (PLD_pngqt) || defined(PLD_ppmqt) || defined(PLD_tiffqt)
// Driver painting whatever raster format Qt can save
class QtRasterDevice: public QtPLDriver, public QImage
{
	public:
		QtRasterDevice(int i_iWidth=QT_DEFAULT_X,
			    int i_iHeight=QT_DEFAULT_Y);
		
		virtual ~QtRasterDevice();
		
		void definePlotName(const char* fileName, const char* format);
		
		void savePlot();
		
		virtual void setResolution(double dotsPerInch)
		{
			setDotsPerMeterX((dotsPerInch/25.4)*1000.);
			setDotsPerMeterY((dotsPerInch/25.4)*1000.);
		}
		
	protected:
		char format[5];
		QString fileName;
		
};
#endif

#if defined(PLD_svgqt) && QT_VERSION >= 0x040300
#include <QSvgGenerator>
// Driver painting on an SVG device
class QtSVGDevice: public QtPLDriver, public QSvgGenerator
{
	public:
		QtSVGDevice(int i_iWidth=QT_DEFAULT_X,
			    int i_iHeight=QT_DEFAULT_Y);

		virtual ~QtSVGDevice();

		void definePlotName(const char* fileName);

		void savePlot();
	protected:
};
#endif

#if defined (PLD_epsqt) || defined (PLD_pdfqt)
// Driver painting on an EPS or PDF device, uses QPrinter
// A (possibly dummy) QApplication must be declared before use
class QtEPSDevice: public QtPLDriver, public QPrinter
{
	public:
#if QT_VERSION < 0x040400
		QtEPSDevice(int i_iWidth=-1, int i_iHeight=-1);
#else
		QtEPSDevice(int i_iWidth=QT_DEFAULT_X, int i_iHeight=QT_DEFAULT_Y);
#endif
		
		virtual ~QtEPSDevice();
	
		void definePlotName(const char* fileName, int ifeps);
	
		void savePlot();
			
	protected:

};
#endif

#if defined (PLD_qtwidget)

// This widget allows to use plplot as a plotting engine in a Qt Application
// The aspect ratio of the plotted data is constant, so gray strips are used
// to delimit the page when the widget aspect ratio is not the one of the plotted page
class QtPLWidget: public QWidget, public QtPLDriver
{
	Q_OBJECT

	public:
		// Parameters are the actual size of the page, NOT the size of the widget
		// Call QWidget::resize for that
		QtPLWidget(int i_iWidth=QT_DEFAULT_X, int i_iHeight=QT_DEFAULT_Y, QWidget* parent=0);
		
		virtual ~QtPLWidget();
		
		void clearWidget();
		
		int pageNumber;

	protected:
		
		void resizeEvent(QResizeEvent*);
		void paintEvent(QPaintEvent*);
		
		// Used to center the plot on the page
		void getPlotParameters(double & io_dXFact, double & io_dYFact, double & io_dXOffset, double & io_dYOffset);
		
		double m_dAspectRatio;
		QPixmap * m_pixPixmap; // to avoid redrawing everything
		bool m_bAwaitingRedraw;
		int m_iOldSize;
		QPicture* pic;

	protected slots:
		void mouseReleaseEvent ( QMouseEvent * event );
		void closeEvent(QCloseEvent* event);
		void nextPage();
};

#endif

#endif
