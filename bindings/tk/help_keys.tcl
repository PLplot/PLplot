#----------------------------------------------------------------------------
# help_keys w
#
# Create a top-level window that displays info for On keys.. help item.
# Maurice LeBrun, IFS
# Adapted from mkStyles.tcl from widget demo.
#
# Arguments:
#    w -	Name to use for new top-level window.
#----------------------------------------------------------------------------

proc help_keys {{w .help_keys}} {
    catch {destroy $w}
    toplevel $w
    dpos $w
    wm title $w "Help on keys"
    wm iconname $w "help_keys"
    normal_text_setup $w 

    insertWithTags $w.t {\
When a plframe widget has the input focus, keyboard input is relayed to
its remote TK driver.  The default actions of the keyboard handler are as
follows:

"Q"				Terminate program
<Return> or <Page Down>		Advance to the next page

The user code can supply its own keyboard handler, and thereby add to
or modify these actions (this is in fact what is done by plrender).

} normal     
    insertWithTags $w.t {
plrender} bold
    insertWithTags $w.t {\
 supports the following additional set of actions:

<Backspace>
<Delete>	Back page
<Page up>

+<num><Return>	Seek forward <num> pages.
-<num><Return>	Seek backward <num> pages.

<num><Return>	Seek to page <num>.

Both <Backspace> and <Delete> are recognized for a back-page since the
target system may use either as its erase key.  <Page Up> is present on
some keyboards (different from keypad key).  
} normal
    $w.t configure -state disabled
    $w.t mark set insert 0.0
}
