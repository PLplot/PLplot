#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QString>
#include "plstream.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow( QWidget *parent = nullptr );
    ~MainWindow( void );
    PLFLT x[101], y[101];
    PLFLT xscale, yscale, xoff, yoff, xs[6], ys[6];
    int fontset;
    char *f_name;
    long w1, h1;
    QString picture;

private slots:
    void memqt( void );
    void memqtTransp( void );
    void pngqt( void );
    void mycase1( void );
    void imagebackground( void );
    void mycase( void );
    void opaque( void );

private:
    Ui::MainWindow *ui;
    QGraphicsScene scene;
    plstream *pls;
    void plot1( void );
    void plot2( void );
    void plot3( void );
    void plplot_commands( void );
    void initialize_memqt_buffer( unsigned char *buf, int buf_length, PLINT r, PLINT g, PLINT b, PLFLT alpha );
};
#endif // MAINWINDOW_H
