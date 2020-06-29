#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <plplot.h>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    PLFLT        x[101], y[101];
    PLFLT        xscale, yscale, xoff, yoff, xs[6], ys[6];
    PLGraphicsIn gin;
    int          locate_mode;
    int          test_xor;
    int          fontset ;
    char         *f_name ;
    long w1,h1;


private slots:
    void memqt();
    void pngqt();


private:
    Ui::MainWindow *ui;
    QGraphicsScene scene;
    void plot1(int );
    void plot2();
    void plot3();
    void plot();


};
#endif // MAINWINDOW_H
