
function strm = __pl_init

  global __pl

  if (!exist("__pl") || !struct_contains (__pl,"inited"))
    figure(0);
  endif

  strm = plgstrm+1;
endfunction