;; Modify the following line to be the name of this file, and
;; ut the in your ~/.emacs (without the semi-colons).
;; (load-file "/usr2/ai/nesc/.emacs")

;;; LaTeXinfo
(setq latexinfo-formats-directory
      (expand-file-name (getenv "LATEXINFO")))

(setq load-path 
      (cons
       latexinfo-formats-directory
       load-path))

(autoload 'get-latexinfo-node "get-node.el"
	  "Get help on a LaTeXinfo topic" t)

(define-key help-map "g" 'get-latexinfo-node)

(autoload 'latexinfo-format-buffer "latexinfo"
	  "Format a buffer for LaTeXinfo" t)

(autoload 'nodify-file "nodify"
	  "Create nodes structure for a LaTeXinfo file." t)




(find-file "plplot.tex")
(latexinfo-format-buffer t)
(save-some-buffers t)
(kill-emacs 0)
