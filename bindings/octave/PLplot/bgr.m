## map = bgr(n)
##
## return a colormap of size 'n' going from blue to green to red.
## Good for coloring magnitude colored surfaces.
## Without arguments, map has lenght 64

function map = bgr(n)

  if (nargin != 1)
    n = 64;
  endif

  i = fix(linspace (1, n, 5));

  rg = i(1):i(2);
  r(rg) = 0;
  g(rg) = linspace(0,1,length(rg));
  b(rg) = 1;
  
  rg = i(2):i(3);
  r(rg) = 0;
  g(rg) = 1;
  b(rg) = linspace(1,0,length(rg));
  
  rg = i(3):i(4);
  r(rg) = linspace(0,1,length(rg));
  g(rg) = 1;
  b(rg) = 0;

  rg = i(4):i(5);
  r(rg) = 1;
  g(rg) = linspace(1,0,length(rg));
  b(rg) = 0;

  map = [r', g', b'];
endfunction