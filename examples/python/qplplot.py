import qt
from qt import QWidget 
import pl
import pyqt_pl
from Numeric import array

class qplplot(QWidget):

   def __init__(self,parent=None,name=None  ):
      qt.QWidget.__init__(self,parent,name)
      print " QWidget OK"
#      self.setMouseTracking (1 )
      self.is_created=0
      self.count=1
      print " ??????"
      self.ipls=pyqt_pl.plpartialInitXw()
      print " ??????"
      print  self.ipls     
      self.isactive=0

   def __getattr__(self,attr):
      print attr
      attr="pl"+attr
      print "pl+++",attr
      self.__attr = getattr(pl,attr)
      if(self.isactive==0):
        return self.message
      print "AAA",self.__attr ,"AAA"
      return self.__wrap
      
   def __wrap(self, *args):
      print " wrap "
      
      if(hasattr(pl,"semaphore")):
       if(pl.owner is not self): 
           pl.semaphore.acquire()
           pl.owner=self
      self.set_stream()
      self.__attr(*args)
      if(hasattr(pl,"semaphore")):
       pl.semaphore.release()

   def resize(self, w,h):
      qt.QWidget.resize(self,w,h)
      print " OK "

   def attiva(self):
      self.isactive=1
      self.installEventFilter(self)
      self.show()   
  
   def message(self,*args):
        print "active this widget first "

   def mousePressEvent(self, qmev):
      self.xstart=qmev.x()
      self.ystart=qmev.y()
      
   def mouseMoveEvent (self,  qmev ) :
         print "MOVE IN PYTHON"
         if(hasattr(pl,"semaphore")):
      	  if(pl.owner is not self): 
            pl.semaphore.acquire()
            pl.owner=self
         self.set_stream()


         if(qmev.state()==1):
           x=qmev.x()
           y=qmev.y()
           (xa,ya)=pl.plPixel2U(self.xstart,self.ystart )
           (xb,yb)=pl.plPixel2U(x, y)
           pl.plmoveimage(xb-xa,yb-ya)
           pl.plNoBufferNoPixmap()
           pl.plRestoreWrite2BufferPixmap()

            
         else:
           pl.plNoBufferNoPixmap()

           (xu,yu)=pl.plPixel2U(qmev.x(), qmev.y())
           (xmin,xmax,ymin,ymax)=pl.plPgvpw()
           pl.plline( [xmin,xmax],[yu,yu])
           pl.plline( [xu,xu],[ymin, ymax])

         
           pl.plRestoreWrite2BufferPixmap()


         
         if(hasattr(pl,"semaphore")):
           pl.semaphore.release()

        
   def set_stream(self):
      pl.plsstrm(self.ipls)

   def resizeQUIET(self):
      if(hasattr(pl,"semaphore")):
        if(pl.owner is not self): 
           pl.semaphore.acquire()
           pl.owner=self
      pl.plsstrm(self.ipls)
      pyqt_pl.plresize(self.width(), self.height())
      if(hasattr(pl,"semaphore")):
        pl.semaphore.release()

   def exposeQUIET(self):
      if(hasattr(pl,"semaphore")):
        if(pl.owner is not self): 
           pl.semaphore.acquire()
           pl.owner=self
      pl.plsstrm(self.ipls)
      pyqt_pl.plexpose()
      if(hasattr(pl,"semaphore")):
        pl.semaphore.release()

   def eventFilter(self,ob,ev):

      if(hasattr(pl,"semaphore")):
        if(pl.owner is not self): 
           pl.semaphore.acquire()
           pl.owner=self
      pl.plsstrm(self.ipls)

      print " intercettato event ",  ev.type() 
      
      if(self.count==1):
        self.Init()
        self.is_created=1
        self.count=2
  
      if(self.is_created):
        print " EVENTO = ",ev.type() , " ", qt.QEvent.Resize
        if(ev.type()==qt.QEvent.Paint):
	  pyqt_pl.plexpose()
          timer =  qt.QTimer( self )
	  self.connect( timer, qt.SIGNAL("timeout()"), self.exposeQUIET)
	  timer.start( 0, 1)
          return 1
        elif(ev.type()==qt.QEvent.Resize):
          print " RESIZE "
          self.resizeQUIET()  
          return 1
        else:
          if(ev.type()==12) :
	   pyqt_pl.plexpose();
      if(hasattr(pl,"semaphore")):
        pl.semaphore.release()
  
      return 0

   def Init(self):
    print " init  \n"

    if(hasattr(pl,"semaphore")):
      if(pl.owner is not self): 
           pl.semaphore.acquire()
           pl.owner=self
    pl.plsdev("xwin")
    pl.plsxwin(self.winId())
    pl.plspause(0)
    pl.plinit()
    pl.plbop()
    if(hasattr(pl,"semaphore")):
     pl.semaphore.release()
