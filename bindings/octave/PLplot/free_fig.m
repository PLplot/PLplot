function n = free_fig

  ## n = free_fig
  ##
  ## return the number of an unused plot figure
  
  global __pl
  __pl_init;

  if (any(__pl.open == 0))
    n = min(find(__pl.open == 0))-1;
  else
    n = length(__pl.open);
  endif
  
endfunction
	

