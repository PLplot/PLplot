;;---------------------------------------------------------------------------;;
;; $Id$
;; Maurice LeBrun
;; Nov 13 2000
;;---------------------------------------------------------------------------;;
;; @> Customizations for emacs modes appropriate for plplot
;; @> To be used with "load-config-file" function.
;;---------------------------------------------------------------------------;;

(defun local-c++-mode-setup ()
  (setq c-basic-offset 4)
  (setq c-offsets-alist
	(append '((access-label . -2)
		  (comment-intro . -)
		  (inline-open . 0)
		  (substatement-open . 0)
		  ) c-offsets-alist))
  )

;; Note the tcl-comment-indent-level is currently only supported by mjl's
;; hacked Tcl-mode 1.50 ("tcl-mjl.el").

(defun local-tcl-mode-setup ()
  (setq tcl-indent-level 4)
  (setq tcl-continued-indent-level 4)
  (setq tcl-comment-indent-level -4)
  )

(cond 
 ((equal mode-name "C")
  (local-c++-mode-setup))
 ((equal mode-name "C++")
  (local-c++-mode-setup))
 ((equal mode-name "Tcl")
  (local-tcl-mode-setup))
 )
