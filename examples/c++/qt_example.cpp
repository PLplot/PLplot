/*

  This software is provided under the LGPL in March 2009 by the
  Cluster Science Centre
  QSAS team,
  Imperial College, London

  Copyright (C) 2009  Imperial College, London

  This is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Lesser Public License as published
  by the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

  This software is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  To received a copy of the GNU Library General Public License
  write to the Free Software Foundation, Inc., 
  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
  
*/

#include "qt_PlotWindow.h"

int main(int argc, char** argv)
{
    int res;
    
    // Deep copy of the arguments before QApplication alters them
    int Argc=argc;
    char** Argv;

    Argv=new char*[argc];
    for(int i=0; i<Argc; ++i)
    {
        int len=strlen(argv[i])+1;
        Argv[i]=new char[len];
        strncpy(Argv[i], argv[i], len);
    }
    
	QApplication a( argc, argv );
	PlotWindow* win=new PlotWindow(Argc, Argv);
	a.setActiveWindow( win );
	win->setVisible(true);

	res=a.exec();

    for(int i=0; i<Argc; ++i)
    {
        delete[] Argv[i];
    }
    delete[] Argv;
    
    return res;
}
