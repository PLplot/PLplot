
A cookbook for using the cmake mode with emacs
----------------------------------------------

There is a cmake mode available for emacs. On some systems (the latest 
Debian unstable for instance) this may be automatically installed along
with cmake. If not, then the following cookbook should work.

1) Get the cmake-mode.el file from the cmake site
   http://www.cmake.org/CMakeDocs/cmake-mode.el . Note the current file
   has a couple of lines from diff / patch in (starting with >>> or <<<) 
   which need removing.

2) Install cmake-mode.el in a suitable location in your system / home
   directory. 

3) Add the following to your .emacs file to automatically load cmake
   mode for .cmake and CMakeLists.txt files.

----

;--------------------
; Enable CMake mode |
;--------------------
(setq load-path (cons (expand-file-name "~/lib/site-lisp") load-path))
(autoload 'cmake-mode "CMAKE" "Major mode for editing CMake listfiles")
(setq auto-mode-alist
      (append '(("CMakeLists\\.txt\\'" . cmake-mode)
                ("\\.cmake\\'" . cmake-mode))
              auto-mode-alist))

----

  You will need to alter the directory path depending on where you 
  installed cmake-mode.el

4) Check the file works by loading one of the plplot cmake files into
   emacs. You should see CMAKE at the bottom of the screen as the major 
   mode.

5) For my version of emacs this does not autoload the helper function 
   for unscreamifying the buffer (i.e. all cmake commands in lower case).
   Typing "M-x load-library cmake-mode" should fix this.

   The two most useful functions are 
   "M-x unscreamify-buffer" which will put all cmake commands into 
   lower case
   "M-x indent-region" which will indent the selected region 
   correctly

