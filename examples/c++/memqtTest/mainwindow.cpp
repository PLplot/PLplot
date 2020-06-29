
//      This test program was built to show how to use of memqt and uses the plplot x01 example code
//       x01
//

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QImage>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->action_testmemqt,SIGNAL(triggered()),this,SLOT(memqt()));
    connect(ui->actionPlot_using_pngqt,SIGNAL(triggered()),this,SLOT(pngqt()));
    fontset = 1;
    f_name = NULL;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::plot1(void)
{
    int i;
    PLFLT xmin, xmax, ymin, ymax;

    for (i = 0; i < 60; i++) {
        x[i] = xoff + xscale * (i + 1) / 60.0;
        y[i] = yoff + yscale * pow(x[i], 2.);
    }

    xmin = x[0];
    xmax = x[59];
    ymin = y[0];
    ymax = y[59];

    for (i = 0; i < 6; i++) {
        xs[i] = x[i * 10 + 3];
        ys[i] = y[i * 10 + 3];
    }

    // Set up the viewport and window using PLENV. The range in X is
    // 0.0 to 6.0, and the range in Y is 0.0 to 30.0. The axes are
    // scaled separately (just = 0), and we just draw a labelled
    // box (axis = 0).
    //
    plcol0(1);
    plenv(xmin, xmax, ymin, ymax, 0, 0);
    plcol0(2);
    pllab("(x)", "(y)", "#frPLplot Example 1 - y=x#u2");

    // Plot the data points

    plcol0(4);
    plpoin(6, xs, ys, 9);

    // Draw the line through the data

    plcol0(3);
    plline(60, x, y);

}

//--------------------------------------------------------------------------


void MainWindow::plot2(void)
{
    int i;

    // Set up the viewport and window using PLENV. The range in X is -2.0 to
    // 10.0, and the range in Y is -0.4 to 2.0. The axes are scaled separately
    // (just = 0), and we draw a box with axes (axis = 1).
    //
    plcol0(1);
    plenv(-2.0, 10.0, -0.4, 1.2, 0, 1);
    plcol0(2);
    pllab("(x)", "sin(x)/x", "#frPLplot Example 1 - Sinc Function");

    // Fill up the arrays

    for (i = 0; i < 100; i++) {
        x[i] = (i - 19.0) / 6.0;
        y[i] = 1.0;
        if (x[i] != 0.0)
            y[i] = sin(x[i]) / x[i];
    }

    // Draw the line

    plcol0(3);
    plwidth(2);
    plline(100, x, y);
    plwidth(1);
}

//--------------------------------------------------------------------------


void MainWindow::plot3(void)
{
    PLINT space0 = 0, mark0 = 0, space1 = 1500, mark1 = 1500;
    int i;

    // For the final graph we wish to override the default tick intervals, and
    // so do not use plenv().
    //
    pladv(0);

    // Use standard viewport, and define X range from 0 to 360 degrees, Y range
    // from -1.2 to 1.2.
    //
    plvsta();
    plwind(0.0, 360.0, -1.2, 1.2);

    // Draw a box with ticks spaced 60 degrees apart in X, and 0.2 in Y.

    plcol0(1);
    plbox("bcnst", 60.0, 2, "bcnstv", 0.2, 2);

    // Superimpose a dashed line grid, with 1.5 mm marks and spaces.
    // plstyl expects a pointer!
    //
    plstyl(1, &mark1, &space1);
    plcol0(2);
    plbox("g", 30.0, 0, "g", 0.2, 0);
    plstyl(0, &mark0, &space0);

    plcol0(3);
    pllab("Angle (degrees)", "sine", "#frPLplot Example 1 - Sine function");

    for (i = 0; i < 101; i++) {
        x[i] = 3.6 * i;
        y[i] = sin(x[i] * M_PI / 180.0);
    }

    plcol0(4);
    plline(101, x, y);
}


void MainWindow::memqt()
{
    this->setWindowTitle("**now displaying the result from memqt driver**");
    QRect geo = this->ui->graphicsView->geometry();
    w1 = (long) geo.width();
    h1 = (long) geo.height();
    PLINT digmax;


    unsigned char *buf; //= new uchar[4*w1*h1];

    buf = (unsigned char *) calloc((4 * w1 * h1), sizeof(unsigned char));

    plsmema(w1, h1, buf);
    //plscolbga(5, 50, 50, .5);
    plscolbga(0,0,0,1.);


    plspage(0., 0., w1, h1, 0, 0);


    plsdev("memqt");



    // Initialize plplot
    // Divide page into 2x2 plots
    // Note: calling plstar replaces separate calls to plssub and plinit
    plstar(2, 2);

    // Select font set as per input flag

    if (fontset)
        plfontld(1);
    else
        plfontld(0);

    // Set up the data
    // Original case

    xscale = 6.;
    yscale = 1.;
    xoff = 0.;
    yoff = 0.;

    // Do a plot

    plot1();

    // Set up the data

    xscale = 1.;
    yscale = 0.0014;
    yoff = 0.0185;

    // Do a plot

    digmax = 5;
    plsyax(digmax, 0);

    plot1();

    plot2();

    plot3();

    plend();
    QImage image = QImage(buf, w1, h1, QImage::Format_ARGB32);
    image = image.rgbSwapped();
    QPixmap *renderer = new QPixmap(QPixmap::fromImage(image));


    scene.clear();
    this->ui->graphicsView->setScene(&scene);
    scene.addPixmap(*renderer);
    this->ui->graphicsView->setScene(&scene);
    delete renderer;

    free(buf);
}


void MainWindow::pngqt()
{
    this->setWindowTitle("now displaying the result from pngqt driver");
    PLINT digmax;
    QRect geo = this->ui->graphicsView->geometry();
    w1 = (long) geo.width();
    h1 = (long) geo.height();


    //plscolbga(5,50,50,0.5);
    plscolbga(0,0,0,1.);

    plspage(0.,0.,w1,h1,0,0);

    plsdev("pngqt");

    plsfnam("lixo.png");

    // Initialize plplot
    // Divide page into 2x2 plots
    // Note: calling plstar replaces separate calls to plssub and plinit
    plstar( 2, 2 );

    // Select font set as per input flag

    if ( fontset )
        plfontld( 1 );
    else
        plfontld( 0 );

    // Set up the data
    // Original case

    xscale = 6.;
    yscale = 1.;
    xoff   = 0.;
    yoff   = 0.;

    // Do a plot

    plot1();

    // Set up the data

    xscale = 1.;
    yscale = 0.0014;
    yoff   = 0.0185;

    // Do a plot

    digmax = 5;
    plsyax( digmax, 0 );

    plot1();

    plot2();

    plot3();

    //
    // Show how to save a plot:
    // Open a new device, make it current, copy parameters,
    // and replay the plot buffer
    //




    // Don't forget to call plend() to finish off!


    plend();

    QPixmap *renderer = new QPixmap("lixo.png");
    scene.clear();
    scene.addPixmap(*renderer);
    this->ui->graphicsView->setScene(&scene);
    delete renderer;
}
