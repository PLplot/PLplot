//      Holds system header includes, prototypes of TK driver
//      utility functions, and definition of the state structure.
//
//      This file is set up so that the determined user can get access
//      to the tk driver data without too much hassle.  The only drawback
//      is that plplotP.h must be included to get all of the necessary
//      system header files and configuration settings.
//

#ifndef __PLTKD_H__
#define __PLTKD_H__

#include "plplotP.h"
#include "pltk.h"
#include "pdf.h"

#include <tk.h>
#ifdef PLD_dp
#include <dp.h>
#endif

// typedef the type pid_t - a dummy for the moment - for MS Windows
// platforms other than those (e.g., MinGW) that typedef it already.
#if defined ( _WIN32 ) && !defined ( __MINGW32__ )
typedef unsigned int pid_t;
#endif

// One of these holds the TK driver state information

typedef struct
{
    Tk_Window    w;                     // Main window
    Tcl_Interp   *interp;               // Interpreter
    PLINT        xold, yold;            // Coordinates of last point plotted
    unsigned     width, height;         // Current window dimensions
    int          exit_eventloop;        // Break out of event loop
    int          pass_thru;             // Skip normal error termination
    char         *cmdbuf;               // Command buffer
    size_t       cmdbuf_len;            // and its length
    PLiodev      *iodev;                // I/O device info
    const char   *updatecmd;            // Name of update command
    pid_t        child_pid;             // PID for child process
    int          instr;                 // Instruction timer
    int          max_instr;             // Limit before issuing an update
    int          locate_mode;           // Set while in locate mode
    PLGraphicsIn gin;                   // Graphics input structure
} TkDev;

#endif  // __PLTKD_H__
