function p16

  t = automatic_replot;
  automatic_replot = 0;

  multiplot(1, 2);
  p1;
  
  subwindow(1, 2);
  [x, y, z] = rosenbrock;
  plcolormap('default');
  shade(x, y, log(z));
  automatic_replot = t;

endfunction