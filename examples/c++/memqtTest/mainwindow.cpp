// This test program is designed to show how to use the memqt device where the basic
// calls to PLplot routines are taken from examples/c++/x01.cc.
//
// Further comments about this code:
// * imagebackground() demonstrates how to display an image background with the memqt device.
// * opaque() demonstrates how to display an opaque background with the memqt device.
// * memqt() demonstrates how to display a semi-transparent background with the memqt device.
// * pngqt() demonstrates how to display a semi-transparent background with the pngqt device.
//   (In this case, the results are saved to a file, and then that file rendered for
//   comparison with the results from memqt().)
// * mycase() is a variant of imagebackground() that does not set a buffer.  Instead, it
//   defines a QImage of the appropriated size and uses a memory pointer to that QImage.
// * mycase1() is a variant of memqt() that does not set a buffer.  Instead, it
//   defines a QImage of the appropriated size and uses a memory pointer to that QImage.
//
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QImage>
#include <cmath>

MainWindow::MainWindow( QWidget *parent )
    : QMainWindow( parent )
    , ui( new Ui::MainWindow )
{
    ui->setupUi( this );
    connect( ui->actionimagebackground, SIGNAL(triggered()), this, SLOT(imagebackground()) );
    connect( ui->actionopaque_background, SIGNAL(triggered()), this, SLOT(opaque()) );
    connect( ui->actionmemqt, SIGNAL(triggered()), this, SLOT(memqt()) );
    connect( ui->actionpngqt, SIGNAL(triggered()), this, SLOT(pngqt()) );
    connect( ui->actionmycase, SIGNAL(triggered()), this, SLOT(mycase()) );
    connect( ui->actionmycase1, SIGNAL(triggered()), this, SLOT(mycase1()) );
    fontset = 1;
    f_name  = NULL;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::plot1( void )
{
    int   i;
    PLFLT xmin, xmax, ymin, ymax;

    for ( i = 0; i < 60; i++ )
    {
        x[i] = xoff + xscale * ( i + 1 ) / 60.0;
        y[i] = yoff + yscale * pow( x[i], 2. );
    }

    xmin = x[0];
    xmax = x[59];
    ymin = y[0];
    ymax = y[59];

    for ( i = 0; i < 6; i++ )
    {
        xs[i] = x[i * 10 + 3];
        ys[i] = y[i * 10 + 3];
    }

    // Set up the viewport and window using PLENV. The range in X is
    // 0.0 to 6.0, and the range in Y is 0.0 to 30.0. The axes are
    // scaled separately (just = 0), and we just draw a labelled
    // box (axis = 0).
    //
    plcol0( 1 );
    plenv( xmin, xmax, ymin, ymax, 0, 0 );
    plcol0( 2 );
    pllab( "(x)", "(y)", "#frPLplot Example 1 - y=x#u2" );

    // Plot the data points

    plcol0( 4 );
    plpoin( 6, xs, ys, 9 );

    // Draw the line through the data

    plcol0( 3 );
    plline( 60, x, y );
}

void MainWindow::plot2( void )
{
    int i;

    // Set up the viewport and window using PLENV. The range in X is -2.0 to
    // 10.0, and the range in Y is -0.4 to 2.0. The axes are scaled separately
    // (just = 0), and we draw a box with axes (axis = 1).
    //
    plcol0( 1 );
    plenv( -2.0, 10.0, -0.4, 1.2, 0, 1 );
    plcol0( 2 );
    pllab( "(x)", "sin(x)/x", "#frPLplot Example 1 - Sinc Function" );

    // Fill up the arrays

    for ( i = 0; i < 100; i++ )
    {
        x[i] = ( i - 19.0 ) / 6.0;
        y[i] = 1.0;
        if ( x[i] != 0.0 )
            y[i] = sin( x[i] ) / x[i];
    }

    // Draw the line

    plcol0( 3 );
    plwidth( 2 );
    plline( 100, x, y );
    plwidth( 1 );
}

void MainWindow::plot3( void )
{
    PLINT space0 = 0, mark0 = 0, space1 = 1500, mark1 = 1500;
    int   i;

    // For the final graph we wish to override the default tick intervals, and
    // so do not use plenv().
    //
    pladv( 0 );

    // Use standard viewport, and define X range from 0 to 360 degrees, Y range
    // from -1.2 to 1.2.
    //
    plvsta();
    plwind( 0.0, 360.0, -1.2, 1.2 );

    // Draw a box with ticks spaced 60 degrees apart in X, and 0.2 in Y.

    plcol0( 1 );
    plbox( "bcnst", 60.0, 2, "bcnstv", 0.2, 2 );

    // Superimpose a dashed line grid, with 1.5 mm marks and spaces.
    // plstyl expects a pointer!
    //
    plstyl( 1, &mark1, &space1 );
    plcol0( 2 );
    plbox( "g", 30.0, 0, "g", 0.2, 0 );
    plstyl( 0, &mark0, &space0 );

    plcol0( 3 );
    pllab( "Angle (degrees)", "sine", "#frPLplot Example 1 - Sine function" );

    for ( i = 0; i < 101; i++ )
    {
        x[i] = 3.6 * i;
        y[i] = sin( x[i] * M_PI / 180.0 );
    }

    plcol0( 4 );
    plline( 101, x, y );
}

void MainWindow::initialize_memqt_buffer( unsigned char *buf, int buf_length, PLINT r, PLINT g, PLINT b, PLFLT alpha )
{
    int i;
    for ( int i = 0; i < buf_length; i = i + 4 )
    {
        buf[i]     = r;
        buf[i + 1] = g;
        buf[i + 2] = b;
        buf[i + 3] = (int) ( alpha * 255. );
    }
}

void MainWindow::opaque()
{
    this->setWindowTitle( "**Now displaying the result from the memqt device with opaque black background**" );
    QRect geo = this->ui->graphicsView->geometry();
    w1 = (long) geo.width();
    h1 = (long) geo.height();
    PLINT         digmax;

    unsigned char *buf; //= new uchar[4*w1*h1];

    // buf = (unsigned char *) calloc((4 * w1 * h1), sizeof(unsigned char));
    buf = (unsigned char *) malloc( 4 * w1 * h1 );

    // Use opaque black background for this case.
    plscolbga( 0, 0, 0, 1. );

    plsmema( w1, h1, buf );

    plspage( 0., 0., w1, h1, 0, 0 );

    plsdev( "memqt" );

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

    plend();
    QImage  image = QImage( buf, w1, h1, QImage::Format_ARGB32 );
    image = image.rgbSwapped();
    QPixmap *renderer = new QPixmap( QPixmap::fromImage( image ) );

    scene.clear();
    this->ui->graphicsView->setScene( &scene );
    scene.addPixmap( *renderer );
    this->ui->graphicsView->setScene( &scene );
    delete renderer;

    free( buf );
}

void MainWindow::memqt()
{
    this->setWindowTitle( "**Now displaying the result from the memqt device with blue semi-transparent background**" );
    QRect geo = this->ui->graphicsView->geometry();
    w1 = (long) geo.width();
    h1 = (long) geo.height();
    PLINT         digmax;

    unsigned char *buf; //= new uchar[4*w1*h1];

    // buf = (unsigned char *) calloc((4 * w1 * h1), sizeof(unsigned char));
    buf = (unsigned char *) malloc( 4 * w1 * h1 );
    // N.B. buf must be intialized to avoid memory management issues.  Furthermore, the
    // memqt device *always* specifies a background colour which is applied
    // on top of this initialization.  Therefore,
    // initialize buf to a transparent image so as not to interfere with
    // whatever PLplot RGBA background colour that is specified later.
    initialize_memqt_buffer( buf, 4 * w1 * h1, 0, 0, 0, 0. );

    // The default PLplot opaque black background works fine, but for
    // this case specify a blue semi-transparent background instead.
    plscolbga( 0, 0, 255, 0.5 );

    plsmema( w1, h1, buf );

    plspage( 0., 0., w1, h1, 0, 0 );

    plsdev( "memqt" );

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

    plend();
    QImage  image = QImage( buf, w1, h1, QImage::Format_ARGB32 );
    image = image.rgbSwapped();
    QPixmap *renderer = new QPixmap( QPixmap::fromImage( image ) );

    scene.clear();
    this->ui->graphicsView->setScene( &scene );
    scene.addPixmap( *renderer );
    this->ui->graphicsView->setScene( &scene );
    delete renderer;

    free( buf );
}


void MainWindow::pngqt()
{
    this->setWindowTitle( "**Now displaying the result from the pngqt device with blue semi-transparent background**" );
    PLINT digmax;
    QRect geo = this->ui->graphicsView->geometry();
    w1 = (long) geo.width();
    h1 = (long) geo.height();

    // Use blue semi-transparent background for this case.
    plscolbga( 0, 0, 255, 0.5 );

    plspage( 0., 0., w1, h1, 0, 0 );

    plsdev( "pngqt" );

    plsfnam( "lixo.png" );

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

    QPixmap *renderer = new QPixmap( "lixo.png" );
    scene.clear();
    scene.addPixmap( *renderer );
    this->ui->graphicsView->setScene( &scene );
    delete renderer;
}
void MainWindow::imagebackground()
{
    this->setWindowTitle( "**Now displaying the result from the memqt device with image background**" );
    QRect geo = this->ui->graphicsView->geometry();
    w1 = (long) geo.width();
    h1 = (long) geo.height();
    PLINT         digmax;

    unsigned char *buf;
    // buf = (unsigned char *) calloc((4 * w1 * h1), sizeof(unsigned char));
    QImage        image2 = QImage( picture );
    image2 = image2.rgbSwapped();
    w1     = image2.width();
    h1     = image2.height();

    // Emphasize the plot by applying a gray semi-transparent layer on top of the background image.
    plscolbga( 100, 100, 100, 0.5 );

    buf = image2.scanLine( 0 );

    plsmema( w1, h1, buf );

    plspage( 0., 0., w1, h1, 0, 0 );

    plsdev( "memqt" );

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

    plend();
    QImage  image = QImage( buf, w1, h1, QImage::Format_ARGB32 );
    image = image.rgbSwapped();
    QPixmap *renderer = new QPixmap( QPixmap::fromImage( image ) );

    scene.clear();
    this->ui->graphicsView->setScene( &scene );
    scene.addPixmap( *renderer );
    this->ui->graphicsView->setScene( &scene );
    delete renderer;
}

void MainWindow::mycase1()
{
    this->setWindowTitle( "**Now displaying the result from the memqt device with mycase1**" );
    QRect geo = this->ui->graphicsView->geometry();
    w1 = (long) geo.width();
    h1 = (long) geo.height();
    PLINT  digmax;

    QImage image = QImage( w1, h1, QImage::Format_ARGB32 );

    // Leave PLplot free to do whatever it likes with the background by initializing
    // the Qt5 image to a completely transparent image.

    image.fill( QColor( 0, 0, 0, (int) ( 0.0 * 255 ) ) );
    // The above call should be the natural usage but for unknown
    // reasons for the QImage::Format_ARGB32 used by plD_init_memqt,
    // the RBG bytes are swapped, i.e., in that code we have
    //   qt_mem[2] = input_mem[0]; // R
    //   qt_mem[1] = input_mem[1]; // G
    //   qt_mem[0] = input_mem[2]; // B
    // and for plD_eop_memqt they are swapped back again, i.e., in
    // that code we have
    //   memory[0] = qt_mem[2];           // R
    //   memory[1] = qt_mem[1];           // G
    //   memory[2] = qt_mem[0];           // B
    // This RGB swapping by the memqt device imposes the need (at
    // least when R, G, or B are different from each other which is
    // not the case above) to explicitly call image.rgbSwapped();
    // below to obtain the the correct RGB colours.

    // Use blue semi-transparent background for this case.
    plscolbga( 0, 0, 255, 0.5 );

    plsmema( w1, h1, image.scanLine( 0 ) );
    plspage( 0., 0., w1, h1, 0, 0 );

    plsdev( "memqt" );

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

    plend();
    // It is hard to understand why memqt does this RGB swap, but this
    // compensates for it when R, G, and B are different from each other, see comment above.
    image = image.rgbSwapped();
    QPixmap *renderer = new QPixmap( QPixmap::fromImage( image ) );

    scene.clear();
    this->ui->graphicsView->setScene( &scene );
    scene.addPixmap( *renderer );
    this->ui->graphicsView->setScene( &scene );
    delete renderer;
}
void MainWindow::mycase()
{
    this->setWindowTitle( "**Now displaying the result from the memqt device with mycase**" );
    QRect geo = this->ui->graphicsView->geometry();
    w1 = (long) geo.width();
    h1 = (long) geo.height();
    PLINT digmax;

    // buf = (unsigned char *) calloc((4 * w1 * h1), sizeof(unsigned char));
    QImage image = QImage( picture );
    image = image.rgbSwapped();
    w1    = image.width();
    h1    = image.height();

    // Emphasize the background by specifying a completely transparent PLplot background
    // rather than the default opaque black (no image background at all) or
    // light-grey semi-transparent background (plot emphasized over background).
    plscolbga( 0, 0, 0, 0.0 );

    plsmema( w1, h1, image.scanLine( 0 ) );
    plspage( 0., 0., w1, h1, 0, 0 );

    plsdev( "memqt" );

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

    plend();
    image = image.rgbSwapped();
    QPixmap *renderer = new QPixmap( QPixmap::fromImage( image ) );

    scene.clear();
    this->ui->graphicsView->setScene( &scene );
    scene.addPixmap( *renderer );
    this->ui->graphicsView->setScene( &scene );
    delete renderer;
}
