## Copyright (C) 1998, 1999, 2000 Joao Cardoso.
## 
## This program is free software; you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by the
## Free Software Foundation; either version 2 of the License, or (at your
## option) any later version.
## 
## This program is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
##
## This file is part of plplot_octave.
## It is based on the corresponding demo function of PLplot.

1;

## Shows two views of the world map.

function ix19c
  ## Parse and process command line arguments
  
  ## (void) plparseopts( &argc, argv, PL_PARSE_FULL );
  
  ## Longitude (x) and latitude (y)
  
  miny = -70;
  maxy = 80;
  
  plinit();
  
  ## Cartesian plots
  ## Most of world
  
  minx = -170;
  maxx = minx + 360;
 
  ## Setup a custom latitude and longitude-based scaling function.
  plslabelfunc( @geolocation_labeler, [] );

  plcol0( 1 );
  plenv( minx, maxx, miny, maxy, 1, 70 );
  plmap( [], "usaglobe", minx, maxx, miny, maxy );

  ## The Americas

  minx = 190;
  maxx = 340;
  
  plcol0( 1 );
  plenv( minx, maxx, miny, maxy, 1, 70 );
  plmap( [], "usaglobe", minx, maxx, miny, maxy );
 
  ## Clear the labeling function
  plslabelfunc( [], [] );
  
  ## Polar, Northern hemisphere
 
  minx = 0;
  maxx = 360;

  plenv( -75., 75., -75., 75., 1, -1 );
  plmap( @mapform19, "globe", minx, maxx, miny, maxy );

  pllsty( 2 );
  plmeridians( @mapform19, 10.0, 10.0, 0.0, 360.0, -10.0, 80.0 );
  
  ## Polar, Northern hemisphere, this time with a PLplot-wide transform
  
  minx = 0;
  maxx = 360;
 
  plstransform( @map_transform, [] );

  pllsty( 1 );
  plenv( -75., 75., -75., 75., 1, -1 );
  ## No need to set the map transform here as the global transform will be
  ## used.
  plmap( [], "globe", minx, maxx, miny, maxy );
  
  pllsty( 2 );
  plmeridians( [], 10.0, 10.0, 0.0, 360.0, -10.0, 80.0 );
  
  ## Show Baltimore, MD on the map
  plcol0( 2 );
  plssym( 0.0, 2.0 );
  x = -76.6125;
  y = 39.2902778;
  plpoin( x, y, 18 );
  plssym( 0.0, 1.0 );
  plptex( -76.6125, 43.0, 0.0, 0.0, 0.0, "Baltimore, MD" );
  
  ## For C, this is how the global transform is cleared
  plstransform( [], [] );
  
  plend1();
endfunction

function [xt, yt] = map_transform( x, y, data )
    radius = 90.0 - y;
    xt    = radius .* cos( x * pi / 180.0 );
    yt    = radius .* sin( x * pi / 180.0 );
endfunction

##--------------------------------------------------------------------------
## mapform19
##
## Defines specific coordinate transformation for example 19.
## Not to be confused with mapform in src/plmap.c.
## x[], y[] are the coordinates to be plotted.
##--------------------------------------------------------------------------

function [xp, yp] = mapform19( x, y )
  [xp, yp] = map_transform( x, y, [] );
endfunction

## "Normalize" longitude values so that they always fall between -180.0 and
## 180.0
function norm = normalize_longitude( lon )
  if ( lon >= -180.0 && lon <= 180.0 )
    norm = lon;
  else
    times = floor( ( abs( lon ) + 180.0 ) / 360.0 );
    if ( lon < 0.0 )
      norm = lon + 360.0 * times;
    else
      norm = lon - 360.0 * times;
    endif
  endif
endfunction

## A custom axis labeling function for longitudes and latitudes.
function label = geolocation_labeler( axis, value, data )

  global PL_X_AXIS PL_Y_AXIS
  if ( axis == PL_Y_AXIS )
    label_val = value;
    if ( label_val > 0.0 )
      direction_label = " N";
    elseif ( label_val < 0.0 )
      direction_label = " S";
    else
      direction_label = "Eq";
    endif
  elseif ( axis == PL_X_AXIS )
    label_val = normalize_longitude( value );
    if ( label_val > 0.0 )
      direction_label = " E";
    elseif ( label_val < 0.0 )
      direction_label = " W";
    else
      direction_label = "";
    endif
  endif
  if ( axis == PL_Y_AXIS && value == 0.0 )
    ## A special case for the equator
    label = sprintf( "%s", direction_label );
  else
    label = sprintf( "%.0f%s", abs( label_val ), direction_label );
  endif
endfunction

ix19c
