function fig_raise(fig)

  ## fig_raise([figure])
  ##
  ## raise current (or "figure") plot window  

  if (nargin == 0)
    [id driver intp] = figure;
  else
    [id driver intp] = figure(fig);
  endif
    
  if (strcmp(driver, "tk"))
    tk_cmd(intp,"raise .",0);
    figure(id)
  else
    error("fig_raise can only be used with the tk driver.\n");
  endif

endfunction