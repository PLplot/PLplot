function out =__pl_pltext(str)

### str =__pl_pltext(str)
### convert (almost) an iso-8859-1 string `str' to a plplot accentuated string
### PRELIMINARY

  in = "ÀÁÂÃÈÉÊÌÍÒÓôÕÙÚÛàáâãèéêìíòóôõùúûçÇ";
  tr = ["#u`#d#bA"; "#u'#d#bA"; "A#b#u^#d"; "A#b#u~#d";
	"#u`#d#bE"; "#u'#d#bE"; "E#b#u^#d";
	"I#b#u`#d"; "I#b#u'#d";
	"#u`#d#bO"; "#u'#d#bO"; "#u^#d#bO"; "#u~#d#bO";
	"#u`#d#bU"; "#u'#d#bU"; "#u^#d#bU";
	"#u`#d#ba"; "#u'#d#ba"; "#u^#d#ba"; "a#b#u~#d";
	"#u`#d#be"; "#u'#d#be"; "#u^#d#be";
	"#u`#d#bi"; "#u'#d#bi";
	"#u`#d#bo"; "#u'#d#bo"; "#u^#d#bo"; "#u~#d#bo";
	"#u`#d#bu"; "#u'#d#bu"; "#u^#d#bu"; 
	"#d,#u#bc"; "#d,#u#bC"];


  out = "";
  for k=1:rows(str);
    t = "";
    for i=1:length(str(k,:))
      ix = find(str(k,i) == in);
      if (isempty(ix))
	t = [t, str(k,i)];
      else
	t = [t, tr(ix,:)]
      endif
    endfor
    out = [out; t];
  endfor

  
endfunction
