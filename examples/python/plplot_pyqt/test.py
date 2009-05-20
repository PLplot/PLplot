#!/usr/bin/python
#
# PLplot / PyQt test.
#
# Hazen 5/09
#

import sys
from PyQt4 import QtCore, QtGui
import plplot
import plplot_pyqt

class QPlot(QtGui.QMainWindow):
    def __init__(self):
        print "init"
        QtGui.QMainWindow.__init__(self, None)

        self.plot = plplot_pyqt.QtExtWidget(800, 800, self)
        self.setCentralWidget(self.plot)

        plplot_pyqt.plsetqtdev(self.plot)
        plplot.plsdev("extqt")
        plplot.plinit()

        self.resize(400,400)

    def cleanup(self):
        plplot.plend()
        plplot_pyqt.plfreeqtdev()

    def paintEvent(self, event):
        plplot.pladv(0)
        plplot.plenv(0, 10, 0, 10, 0, 0)
        plplot.pllab("X", "Y", "X vs Y")
        self.plot.show()


app = QtGui.QApplication(sys.argv)
plot = QPlot()
plot.show()
app.exec_()
plot.cleanup()

