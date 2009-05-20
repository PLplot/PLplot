#!/usr/bin/python
#
# A simple PLplot/PyQt example
#
# Copyright (C) 2009 Hazen Babcock
#
# This file is part of PLplot.
#
# PLplot is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Library Public License as published
# by the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# PLplot is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public License
# along with PLplot; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
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

