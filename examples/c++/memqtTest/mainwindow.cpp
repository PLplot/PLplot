
//      This test program was built to show ways how to se  memqt and uses the plplot x01 example code
//       x01
//     the Plot_using_pngqt does not use memqt driver it uses instead pngqt driver saves it to a file and then renders the file for viewing
//     it is here for output comparasion.
//     the actions memqtwithbg and mycase needes an image for background in the directory where the program runs with a name aurora.png by default
// one can invoke the program  memqtTest backgroundpicture
//     memqt action expressely sets a buffer and fills it with the desired background for the ones that want to use this driver in a non qt program
//
//    the action mycase one does not set any buffer but instead defines a QImage with the appropriated size ans sends its memory pointer to the drive.
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
    connect(ui->actionwithanimagebackground,SIGNAL(triggered()),this,SLOT(memqtwithbg()));
    connect(ui->actionPlot_using_pngqt,SIGNAL(triggered()),this,SLOT(pngqt()));
    connect(ui->actionmycase,SIGNAL(triggered()),this,SLOT(mycase()));
    connect(ui->actionmycase1,SIGNAL(triggered()),this,SLOT(mycase1()));
    connect(ui->actionNormalBackgroud_all_others_are_alhpa,SIGNAL(triggered()),this,SLOT(normal()));
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
void MainWindow::normal()
{
    this->setWindowTitle("**now displaying the result from memqt driver**");
    QRect geo = this->ui->graphicsView->geometry();
    w1 = (long) geo.width();
    h1 = (long) geo.height();
    PLINT digmax;


    unsigned char *buf; //= new uchar[4*w1*h1];

    // buf = (unsigned char *) calloc((4 * w1 * h1), sizeof(unsigned char));
    buf = (unsigned char *) malloc(4 * w1 * h1);

   //mimic the effect of calling     plscolbga(0, 0, 0, 1.);

    for (int i=0; i<4*w1*h1;i=i+4){
        buf[i]=0;
        buf[i+1]=0;
        buf[i+2]=0;
        buf[i+3]=(int) ( 1. * 255 ); //alpha

    }
    plsmema(w1, h1, buf);
//    plscolbga(5, 50, 50, .5);


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


void MainWindow::memqt()
{
    this->setWindowTitle("**now displaying the result from memqt driver**");
    QRect geo = this->ui->graphicsView->geometry();
    w1 = (long) geo.width();
    h1 = (long) geo.height();
    PLINT digmax;


    unsigned char *buf; //= new uchar[4*w1*h1];

    // buf = (unsigned char *) calloc((4 * w1 * h1), sizeof(unsigned char));
    buf = (unsigned char *) malloc(4 * w1 * h1);

   //mimic the effect of calling     plscolbga(5, 50, 50, .5);

    for (int i=0; i<4*w1*h1;i=i+4){
        buf[i]=5;
        buf[i+1]=50;
        buf[i+2]=50;
        buf[i+3]=(int) ( .5 * 255 ); //alpha

    }
    plsmema(w1, h1, buf);
//    plscolbga(5, 50, 50, .5);


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


    plscolbga(5,50,50,0.5);

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
void MainWindow::memqtwithbg()
{
    this->setWindowTitle("**now displaying the result from memqt driver with background**");
    QRect geo = this->ui->graphicsView->geometry();
    w1 = (long) geo.width();
    h1 = (long) geo.height();
    PLINT digmax;


    unsigned char *buf;
    // buf = (unsigned char *) calloc((4 * w1 * h1), sizeof(unsigned char));
    QImage image2 = QImage(picture);
    image2=image2.rgbSwapped();
    w1=image2.width();
    h1=image2.height();
//    buf = (unsigned char *) malloc(4 * w1 * h1);

    buf=image2.scanLine(0);


    plsmema(w1, h1, buf);



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

}

void MainWindow::mycase1()
{
    this->setWindowTitle("**now displaying the result from memqt driver with mycase**");
    QRect geo = this->ui->graphicsView->geometry();
    w1 = (long) geo.width();
    h1 = (long) geo.height();
    PLINT digmax;

    QImage image = QImage(w1,h1,QImage::Format_ARGB32);

  //mimic the effect of calling     plscolbga(5, 50, 50, .5);
    image.fill(QColor(50,50,5,(int) ( .5 * 255 )));
  //  image.fill(QColor(5,50,50,(int) ( .5 * 255 ))); This should be the natural  usage
  // however for some strange speciall needs as qt drivers are also based on  QImage::Format_ARGB32 the bytes are swaped
  // (twice otherwise it would not work) for example  let's look at de code plD_init_memqt the RGB is expressely swapped
  /*  for ( i = 0; i < ( pls->xlength * pls->ylength ); i++ )
    {
        qt_mem[2] = input_mem[0]; // R
        qt_mem[1] = input_mem[1]; // G
        qt_mem[0] = input_mem[2]; // B
        if ( pls->dev_mem_alpha == 1 )
        {
            qt_mem[3]  = input_mem[3];
            input_mem += 4;
        }
        else
        {
            input_mem += 3;
        }
        qt_mem += 4;
    }
   */
   // at the end of the driver  plD_eop_memqt is again swaped back
 /*
    for ( i = 0; i < ( pls->xlength * pls->ylength ); i++ )
    {
        memory[0] = qt_mem[2];           // R
        memory[1] = qt_mem[1];           // G
        memory[2] = qt_mem[0];           // B
        if ( pls->dev_mem_alpha == 1 )
        {
            memory[3] = qt_mem[3];
            memory   += 4;
        }
        else
        {
            memory += 3;
        }
        qt_mem += 4;
    }

   */
 // hard to understand why it has been done this way but that imposes the need
 // to explicily call     image.rgbSwapped(); to obtain the the derirable collors.



    plsmema(w1, h1, image.scanLine(0));
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
    image = image.rgbSwapped(); //hard to understand why the drivres do a RGB swapp
    QPixmap *renderer = new QPixmap(QPixmap::fromImage(image));


    scene.clear();
    this->ui->graphicsView->setScene(&scene);
    scene.addPixmap(*renderer);
    this->ui->graphicsView->setScene(&scene);
    delete renderer;

}
void MainWindow::mycase()
{
    this->setWindowTitle("**now displaying the result from memqt driver with mycase**");
    QRect geo = this->ui->graphicsView->geometry();
    w1 = (long) geo.width();
    h1 = (long) geo.height();
    PLINT digmax;



    // buf = (unsigned char *) calloc((4 * w1 * h1), sizeof(unsigned char));
    QImage image = QImage(picture);
    image=image.rgbSwapped();
    w1=image.width();
    h1=image.height();
   //mimic the effect of calling     plscolbga(5, 50, 50, .5);

    plsmema(w1, h1, image.scanLine(0));
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
    image = image.rgbSwapped();
    QPixmap *renderer = new QPixmap(QPixmap::fromImage(image));


    scene.clear();
    this->ui->graphicsView->setScene(&scene);
    scene.addPixmap(*renderer);
    this->ui->graphicsView->setScene(&scene);
    delete renderer;

}


