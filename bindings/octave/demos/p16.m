function p16

  multiplot(1, 2);
  p1;
  
  subwindow(1, 2);
  [x, y, z] = rosenbrock;
  shade(x, y, log(z));

endfunction