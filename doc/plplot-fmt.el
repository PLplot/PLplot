;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; 
;;; Elisp functions for use with the PLplot latexinfo document. 
;;; written by Maurice J. LeBrun (mjl@dino.ph.utexas.edu)
;;; Version:         $Revision$
;;; Last Modified:   $Date$
;;;
;;; These functions are the elisp counterparts to the macros and/or
;;; environments in plplot.sty.  Both must be present, since the latexinfo
;;; file is processed by both emacs and latex.
;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(require 'latexinfo)

;; Here we register the recognized keywords.
;; Their handlers are defined below.

(put 'name 'latexinfo-format 'plplot-format-name)
(put 'namend 'latexinfo-format 'plplot-format-namend)
(put 'argu 'latexinfo-format 'plplot-format-argu)
(put 'rou 'latexinfo-format 'plplot-format-rou)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; latexinfo-format-parse-multiargs
;;
;; This function parses a number of arguments in LaTeX fashion.
;; E.g. \foo{a}{b}{c}
;; It's up to the caller to determine what to do with them.
;; Returns in arguments ARE significant.
;;
;; Limitations: 
;;
;; - Only one level of brace nesting inside arguments is permitted.  So
;;   constructs like \foo{\code{x}}{\code{y}} are OK, but not any deeper
;;   than that.  Constructs like \foo{\code{x}, \code{y}} are also OK.
;;
;; - The open brace on multi-args must IMMEDIATELY follow the close brace
;;   from the previous arg.  You can skip to the next line after that
;;   (it helps readability with long args) without it affecting the
;;   output, as long as there are no trailing blanks after the end brace.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defun latexinfo-format-parse-multiargs ()
  (let ((start (1- (point)))
	next beg end
	args)
    (search-forward "{")
    (forward-char -1)
    (while (= (following-char) ?\{)
      (forward-char 1)
      (skip-chars-forward " \t\n")
      (setq beg (point))

; Here I check for one extra level of brace.

      (re-search-forward "[{}]")
      (while (= (preceding-char) ?\{)
	(re-search-forward "[{}]")
	(if (= (preceding-char) ?\{)
	    (progn
	      (message "Too many nested braces in argument, hoser")
	      (re-search-forward "}")
	      ))
	(re-search-forward "[{}]")
	)

      (setq next (point))
      (forward-char -1)
      (skip-chars-backward " \t\n")
      (setq end (point))
      (setq args (cons (if (> end beg) (buffer-substring beg end))
		       args))
      (goto-char next))
    (if (eolp) (forward-char 1))
    (setq latexinfo-command-end (point))
    (nreverse args)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; plplot-format-name
;;
;; Handle the \name{arg} macro.
;; `arg' is the name of the function and its arguments.
;;
;; This is used for setting off a function specification.  In the printed
;; document, a box is drawn around the function.  Note: I tried adding
;; indentation but it's hard to customize without massive elisp hacking.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defun plplot-format-name ()
  (let ((args (latexinfo-format-parse-multiargs)))
    (latexinfo-discard-command)
    (insert
"--------------------------------------------------------------------------\n")
    (insert (nth 0 args) "\n")
    (insert
"--------------------------------------------------------------------------\n")
    (goto-char latexinfo-command-start)
    ))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; plplot-format-rou
;;
;; Handle the \rou{arg} macro.
;; `arg' is the name of the function.
;;
;; This is used for referencing a function.  It uses a tt font (in the
;; printed doc) and refers the reader to its location in the API section.
;; In the info document, a hot link is set up for this.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defun plplot-format-rou ()
  (let ((args (latexinfo-format-parse-multiargs)))
    (latexinfo-discard-command)
    (insert "\\code{" (nth 0 args) "}")
    (insert " (\\pxref{" (nth 0 args) "})")
    (goto-char latexinfo-command-start)
    ))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; plplot-format-namend
;;
;; Handle the \namend macro.
;;
;; Just a nice way to terminate a function specification.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defun plplot-format-namend ()
  (latexinfo-discard-command)
  (insert
"--------------------------------------------------------------------------\n")
  )

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; plplot-format-argu
;;
;; Handle the \argu{arg1}{arg2} macro.
;; `arg1' is the name of the argument, its type, and input/output status.
;; `arg2' is a description of its effects.
;;
;; This is used for setting off the specification and explanation of
;; a function argument(s).  The explanation can be as long as you like,
;; even spanning multiple paragraphs or including enumerated lists.
;; Because of the trailing `\refill', be very careful that the last right
;; brace be the very last item on the line (no trailing spaces).
;; Otherwise some spurious blank lines are sometimes created.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defun plplot-format-argu ()
  (let ((args (latexinfo-format-parse-multiargs)))
    (latexinfo-discard-command)
    (insert (nth 0 args) ":\n\n")
    (insert (nth 1 args))
    (insert "\\refill\n")
    (goto-char latexinfo-command-start)
    ))
