function n = free_fig

  ## n = free_fig
  ##
  ## return the number of an unused plot figure
  
  global __pl_inited  __pl

  if(! exist("__pl"))
    n = 0;
  else
    if (any(__pl.open == 0))
      n = min(find(__pl.open == 0))-1;
    else
      n = length(__pl.open);
    endif
  endif
  
endfunction
	

