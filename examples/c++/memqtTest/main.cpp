#include "mainwindow.h"

#include <QApplication>

int main( int argc, char *argv[] )
{
    QApplication a( argc, argv );
    MainWindow   w;
    QString      picture;
    picture = "aurora.png";
    if ( argc > 1 )
        picture = QString( argv[1] );
    w.picture = picture;
    w.show();
    return a.exec();
}
