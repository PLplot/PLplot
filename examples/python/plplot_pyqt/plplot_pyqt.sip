
%Module plplot_pyqt 0

%Import QtCore/QtCoremod.sip
%Import QtGui/QtGuimod.sip

class QtPLDriver {

%TypeHeaderCode
#include "plplot/my_qt.h"
%End

  public:
    QtPLDriver(int i_iWidth=QT_DEFAULT_X, int i_iHeight=QT_DEFAULT_Y);
    virtual ~QtPLDriver();
};

class QtPLWidget: QWidget, QtPLDriver {

%TypeHeaderCode
#include "plplot/my_qt.h"
%End
    
  public:
    QtPLWidget(int i_iWidth=QT_DEFAULT_X, int i_iHeight=QT_DEFAULT_Y, QWidget* parent=0);
    virtual ~QtPLWidget();

    void clearWidget();

  protected:
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void closeEvent(QCloseEvent *event);
    void nextPage();
    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *);
    void getPlotParameters(double &io_dXFact, double &io_dYFact, double &io_dXOffset, double &io_dYOffset);



};

class QtExtWidget: QtPLWidget {
    
%TypeHeaderCode
#include "plplot/my_qt.h"
%End

  public:
    QtExtWidget(int i_iWidth=QT_DEFAULT_X, int i_iHeight=QT_DEFAULT_Y, QWidget* parent=0);
    virtual ~QtExtWidget();

    void captureMousePlotCoords(double *x, double *y);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

  protected:
    void paintEvent(QPaintEvent *event);
};

void plsetqtdev(QtExtWidget* widget);
void plfreeqtdev();
