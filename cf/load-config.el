;;---------------------------------------------------------------------------;;
;; $Id$
;; Maurice LeBrun
;; Nov 13 2000
;;---------------------------------------------------------------------------;;
;; @> Emacs function to support per-project configuration files used for
;; @> customizing emacs modes.
;;
;; Here is an example usage for your emacs startup (strip the first 3
;; characters & copy to your .emacs or equivalent):
;; 
;; (defun my-c++-mode-hook ()
;;   (load-config-file)
;;   )
;; (add-hook 'c++-mode-hook 'my-c++-mode-hook)
;; (setq auto-mode-alist
;;       (append '(("\\.C$"  . c++-mode)
;; 		("\\.cc$" . c++-mode)
;; 		("\\.hh$" . c++-mode)
;; 		) auto-mode-alist))
;; 
;; ;; Note for now you must use hacked version of tcl mode to get comment
;; ;; indentation support. 
;; (autoload 'tcl-mode   "tcl-mjl" nil t)
;; (setq auto-mode-alist
;;       (append '(("\\.tcl$"  . tcl-mode)
;; 		("\\.itcl$" . tcl-mode)
;; 		("\\.itk$"  . tcl-mode)
;; 		("\\.ith$"  . tcl-mode)
;; 		("\\.itm$"  . tcl-mode)
;; 		) auto-mode-alist))
;; 
;; (defun my-tcl-mode-hook ()
;;   (setq tcl-indent-level 4)
;;   (setq tcl-continued-indent-level 4)
;;   (setq tcl-comment-indent-level 0)
;;   ;; project-specific overrides can go here
;;   (load-config-file)
;;   )
;; (add-hook 'tcl-mode-hook 'my-tcl-mode-hook)
;;---------------------------------------------------------------------------;;

(defun load-config-file ()
  "Recursively ascends directory tree looking for a file named config.el and
loads it if found.  First checks \".\", then \"..\", then \"../config\", and
so on from there."

  (interactive)
  (setq origdir (expand-file-name "."))
  (setq homedir (expand-file-name "~"))
  (setq curdir origdir)
  ;(progn (display-message 'message curdir) (sleep-for 1))
  (catch 'found
    (if (file-exists-p "config.el")
	(load-file "config.el")
      (while (not (or (string-equal curdir homedir)
		      (string-equal curdir "/")))
	(cd "..")
	(setq curdir (expand-file-name "."))
	;(progn (display-message 'message curdir) (sleep-for 1))

	(if (file-exists-p "config.el")
	    (progn
	      (load-file "config.el")
	      (throw 'found t)))

	(if (file-exists-p "config/config.el")
	    (progn
	      (load-file "config/config.el")
	      (throw 'found t)))
	)

      ))

  (cd origdir)
)
