/*
 * This file was automatically generated for octave by wrap_octave on
 * Sun Mar  4 18:47:43 2001
 * from plplot_octave.h.
 */

#include "plplot_octave.h"


#include "oct.h"

#ifndef __GNUC__
#include <alloca.h>
#endif

inline int max(int a, int b) { return a >= b ? a : b; }
inline int min(int a, int b) { return a >= b ? a : b; }

//
// Function to get the total length (rows*columns) of an octave object of
// arbitrary type.
// Arguments:
// 1) The octave object.
//
// If the object is a scalar, the array length is 1.
//
static int
_arraylen(const octave_value &o_obj)
{
  return max(o_obj.rows(),1) * max(o_obj.columns(),1); // Return the size.
				// max is necessary because sometimes
				// rows() or columns() return -1 or 0 for
				// scalars.
}

//
// Function to get the number of dimensions of an object.
//
static int
_n_dims(const octave_value &o_obj)
{
  if (max(o_obj.columns(),1) > 1)
    return 2;
				// max is necessary because sometimes
				// rows() or columns() return -1 or 0 for
				// scalars.
  else if (max(o_obj.rows(),1) > 1)
    return 1;
  else
    return 0;
}

//
// Return the n'th dimension of an object.  Dimension 0 is the 1st dimension.
//
static inline int
_dim(const octave_value &o_obj, int dim_idx)
{
  if (dim_idx == 0)
    return max(o_obj.rows(), 1);
				// max is necessary because sometimes
				// rows() or columns() return -1 or 0 for
				// scalars.
  else if (dim_idx == 1)
    return max(o_obj.columns(), 1);
  else
    return 1;
}

//
// The following function converts an array of doubles into some other
// numeric type.  Arguments:
// 1) Where to store the result.  The type is determined from the type of
//    this pointer.
// 2) A vector of doubles to convert.
// 3) The number of doubles.
//
template <class FLOAT>
static inline void
_cvt_double_to(FLOAT *out_arr, double *in_arr, unsigned n_el)
{
  while (n_el-- > 0)
    *out_arr++ = (FLOAT)(*in_arr++);
}

template void _cvt_double_to(int *, double *, unsigned);
template void _cvt_double_to(unsigned *, double *, unsigned);
template void _cvt_double_to(short *, double *, unsigned);
template void _cvt_double_to(unsigned short *, double *, unsigned);
template void _cvt_double_to(float *, double *, unsigned);
				// Instantiate our templates.  Octave uses
				// manual template instantiation.

//
// Convert an array of some other type into an array of doubles.  Arguments:
// 1) The array of objects of other type.
// 2) The output array of doubles.
// 3) The number of elements to convert.
//
template <class FLOAT>
static inline void
_cvt_to_double(FLOAT *arr, double *d_arr, unsigned n_el)
{
  while (n_el-- > 0)
    *d_arr++ = double(*arr++);
}

template void _cvt_to_double(int *, double *, unsigned);
template void _cvt_to_double(unsigned *, double *, unsigned);
template void _cvt_to_double(short *, double *, unsigned);
template void _cvt_to_double(unsigned short *, double *, unsigned);
template void _cvt_to_double(float *, double *, unsigned);
				// Instantiate our templates.  Octave uses
				// manual template instantiation.


/*
 * Check to see if the vectorizing dimensions on an input argument are
 * ok.  Arguments:
 * 1) The input argument.
 * 2) The number of vectorizing dimensions we have so far.  This is updated
 *    if we add more vectorizing dimensions.
 * 3) An array containing the existing vectorizing dimensions.
 * 4) The number of explicitly declared dimensions, i.e., 0 if this was
 *    declared as a scalar, 1 if a vector.  We vectorize only the dimensions
 *    higher than the explicitly declared ones.
 * 5) A value which is set to 0 if this argument is not vectorized.  This
 *    value is left unaltered if the argument is vectorized.
 *
 * Returns 0 if there was a problem, 1 if the dimensions were ok.
 */
int
_check_input_vectorize(const octave_value &arg,
                       int *n_vec_dim,
		       int _d[2],
		       int explicit_dims,
		       int *vec_stride)
{
  int v_idx;

  int n_dims = _n_dims(arg);

  if (n_dims > explicit_dims)	/* Any additional dimensions? */
  {
    if (*n_vec_dim == 0)	/* No vectorizing dimensions seen yet? */
    {				/* This defines the vectorizing dimensions. */
      *n_vec_dim = n_dims - explicit_dims; /* Remember the # of dims. */
      for (v_idx = 0; v_idx < 2-explicit_dims; ++v_idx)
        _d[v_idx] = _dim(arg, v_idx+explicit_dims); /* Remember this dim. */
    } 
    else			/* Already had some vectorizing dimensions. */
    {				/* These must match exactly. */
      for (v_idx = 0; v_idx < 2-explicit_dims; ++v_idx)
        if (_d[v_idx] != _dim(arg, v_idx+explicit_dims)) /* Wrong size? */
          return 0;		/* Error! */
    }
  }  
/*  else if (n_dims < explicit_dims) */ /* Too few dimensions? */
/*    return 0; */ /* We don't do this check because there's no way to
		    * distinguish between a vector and a 3x1 matrix. */
  else
    *vec_stride = 0;		/* Vectorization not required. */

  return 1;
}

/*
 * Same thing except for modify variables.  Arguments:
 * 1) The input argument.
 * 2) The number of vectorizing dimensions we have so far.
 * 3) An array containing the existing vectorizing dimensions.
 * 4) The number of explicitly declared dimensions, i.e., 0 if this was
 *    declared as a scalar, 1 if a vector.  We vectorize only the dimensions
 *    higher than the explicitly declared ones.
 * 5) A flag indicating whether this is the first modify variable.  This
 *    flag is passed by reference and updated by this subroutine.
 *
 * The vectorizing dimensions of modify arguments must exactly match those
 * specified for input variables.  The difference between this subroutine
 * and _check_input_vectorize is that only the first modify variable may
 * specify additional vectorizing dimensions.
 *
 * Returns 0 if there was a problem, 1 if the dimensions were ok.
 */
int
_check_modify_vectorize(const octave_value &arg,
		        int *n_vec_dim,
		        int _d[2],
		        int explicit_dims,
			int *first_modify_flag)
{
  int v_idx;

  int n_dims = _n_dims(arg);

  if (n_dims > explicit_dims)	/* Any additional dimensions? */
  {
    if (*n_vec_dim == 0 && *first_modify_flag)	/* No vectorizing dimensions seen yet? */
    {				/* This defines the vectorizing dimensions. */
      *n_vec_dim = n_dims - explicit_dims; /* Remember the # of dims. */
      for (v_idx = 0; v_idx < 2-explicit_dims; ++v_idx)
        _d[v_idx] = _dim(arg, v_idx+explicit_dims); /* Remember this dim. */
    } 
    else			/* Already had some vectorizing dimensions. */
    {				/* These must match exactly. */
      for (v_idx = 0; v_idx < 2-explicit_dims; ++v_idx)
        if (_d[v_idx] != _dim(arg, v_idx+explicit_dims)) /* Wrong size? */
          return 0;		/* Error! */
    }
  }  
/*  else if (n_dims < explicit_dims) */ /* Too few dimensions? */
/*    return 0; */ /* We don't do this check because there's no way to
		    * distinguish between a vector and a 3x1 matrix. */

  *first_modify_flag = 0;	/* Next modify variable will not be first. */
  return 1;
}

octave_value_list _wrap_pl_setcontlabelformat(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 2)
  {
    print_usage("pl_setcontlabelformat");
    return retval;
  }

  int _arg_lexp;
  int _arg_sigdig;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument lexp"); return retval; }
  _arg_lexp = (int)args(1).double_value();

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument sigdig"); return retval; }
  _arg_sigdig = (int)args(2).double_value();

    c_pl_setcontlabelformat(_arg_lexp, _arg_sigdig);
  return retval;
}

octave_value_list _wrap_pl_setcontlabelparam(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 4)
  {
    print_usage("pl_setcontlabelparam");
    return retval;
  }

  float _arg_offset;
  float _arg_size;
  float _arg_spacing;
  int _arg_active;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument offset"); return retval; }
  _arg_offset = (float)args(1).double_value();

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument size"); return retval; }
  _arg_size = (float)args(2).double_value();

  if (_n_dims(args(3)) > 0)
    { error("dimension mismatch in argument spacing"); return retval; }
  _arg_spacing = (float)args(3).double_value();

  if (_n_dims(args(4)) > 0)
    { error("dimension mismatch in argument active"); return retval; }
  _arg_active = (int)args(4).double_value();

    c_pl_setcontlabelparam(_arg_offset, _arg_size, _arg_spacing, _arg_active);
  return retval;
}

octave_value_list _wrap_pladv(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 1)
  {
    print_usage("pladv");
    return retval;
  }

  int _arg_page;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument page"); return retval; }
  _arg_page = (int)args(1).double_value();

    c_pladv(_arg_page);
  return retval;
}

octave_value_list _wrap_plaxes(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 8)
  {
    print_usage("plaxes");
    return retval;
  }

  float _arg_x0;
  float _arg_y0;
  char * _arg_xopt;
  float _arg_xtick;
  int _arg_nxsub;
  char * _arg_yopt;
  float _arg_ytick;
  int _arg_nysub;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument x0"); return retval; }
  _arg_x0 = (float)args(1).double_value();

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument y0"); return retval; }
  _arg_y0 = (float)args(2).double_value();

  if (0 > 0)
    { error("dimension mismatch in argument xopt"); return retval; }
  string _xopt = args(3).string_value();
  _xopt += '\0';
  _arg_xopt = (char *)_xopt.data();

  if (_n_dims(args(4)) > 0)
    { error("dimension mismatch in argument xtick"); return retval; }
  _arg_xtick = (float)args(4).double_value();

  if (_n_dims(args(5)) > 0)
    { error("dimension mismatch in argument nxsub"); return retval; }
  _arg_nxsub = (int)args(5).double_value();

  if (0 > 0)
    { error("dimension mismatch in argument yopt"); return retval; }
  string _yopt = args(6).string_value();
  _yopt += '\0';
  _arg_yopt = (char *)_yopt.data();

  if (_n_dims(args(7)) > 0)
    { error("dimension mismatch in argument ytick"); return retval; }
  _arg_ytick = (float)args(7).double_value();

  if (_n_dims(args(8)) > 0)
    { error("dimension mismatch in argument nysub"); return retval; }
  _arg_nysub = (int)args(8).double_value();

    c_plaxes(_arg_x0, _arg_y0, _arg_xopt, _arg_xtick, _arg_nxsub, _arg_yopt, _arg_ytick, _arg_nysub);
  return retval;
}

octave_value_list _wrap_plbin(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 3)
  {
    print_usage("plbin");
    return retval;
  }

  int _arg_nbin;
  float *_arg_x;
  float *_arg_y;
  int _arg_center;
  _arg_nbin = (_dim(args(2), 0));
  if (  (int)(_dim(args(1), 0)) != (int)(_arg_nbin) ||
      _n_dims(args(1)) > 1)
    { error("dimension mismatch in argument x"); return retval; }
  Matrix _x = args(1).matrix_value();
  _arg_x = (float *)alloca(_x.dim1() * _x.dim2() * sizeof (float));
  _cvt_double_to(_arg_x, &_x(0,0), _x.dim1()*_x.dim2());

  if (_n_dims(args(2)) > 1)
    { error("dimension mismatch in argument y"); return retval; }
  Matrix _y = args(2).matrix_value();
  _arg_y = (float *)alloca(_y.dim1() * _y.dim2() * sizeof (float));
  _cvt_double_to(_arg_y, &_y(0,0), _y.dim1()*_y.dim2());

  if (_n_dims(args(3)) > 0)
    { error("dimension mismatch in argument center"); return retval; }
  _arg_center = (int)args(3).double_value();

    c_plbin(_arg_nbin, _arg_x, _arg_y, _arg_center);
  return retval;
}

octave_value_list _wrap_plbop(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 0)
  {
    print_usage("plbop");
    return retval;
  }

    c_plbop();
  return retval;
}

octave_value_list _wrap_plbox(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 6)
  {
    print_usage("plbox");
    return retval;
  }

  char * _arg_xopt;
  float _arg_xtick;
  int _arg_nxsub;
  char * _arg_yopt;
  float _arg_ytick;
  int _arg_nysub;
  if (0 > 0)
    { error("dimension mismatch in argument xopt"); return retval; }
  string _xopt = args(1).string_value();
  _xopt += '\0';
  _arg_xopt = (char *)_xopt.data();

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument xtick"); return retval; }
  _arg_xtick = (float)args(2).double_value();

  if (_n_dims(args(3)) > 0)
    { error("dimension mismatch in argument nxsub"); return retval; }
  _arg_nxsub = (int)args(3).double_value();

  if (0 > 0)
    { error("dimension mismatch in argument yopt"); return retval; }
  string _yopt = args(4).string_value();
  _yopt += '\0';
  _arg_yopt = (char *)_yopt.data();

  if (_n_dims(args(5)) > 0)
    { error("dimension mismatch in argument ytick"); return retval; }
  _arg_ytick = (float)args(5).double_value();

  if (_n_dims(args(6)) > 0)
    { error("dimension mismatch in argument nysub"); return retval; }
  _arg_nysub = (int)args(6).double_value();

    c_plbox(_arg_xopt, _arg_xtick, _arg_nxsub, _arg_yopt, _arg_ytick, _arg_nysub);
  return retval;
}

octave_value_list _wrap_plbox3(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 12)
  {
    print_usage("plbox3");
    return retval;
  }

  char * _arg_xopt;
  char * _arg_xlabel;
  float _arg_xtick;
  int _arg_nsubx;
  char * _arg_yopt;
  char * _arg_ylabel;
  float _arg_ytick;
  int _arg_nsuby;
  char * _arg_zopt;
  char * _arg_zlabel;
  float _arg_ztick;
  int _arg_nsubz;
  if (0 > 0)
    { error("dimension mismatch in argument xopt"); return retval; }
  string _xopt = args(1).string_value();
  _xopt += '\0';
  _arg_xopt = (char *)_xopt.data();

  if (0 > 0)
    { error("dimension mismatch in argument xlabel"); return retval; }
  string _xlabel = args(2).string_value();
  _xlabel += '\0';
  _arg_xlabel = (char *)_xlabel.data();

  if (_n_dims(args(3)) > 0)
    { error("dimension mismatch in argument xtick"); return retval; }
  _arg_xtick = (float)args(3).double_value();

  if (_n_dims(args(4)) > 0)
    { error("dimension mismatch in argument nsubx"); return retval; }
  _arg_nsubx = (int)args(4).double_value();

  if (0 > 0)
    { error("dimension mismatch in argument yopt"); return retval; }
  string _yopt = args(5).string_value();
  _yopt += '\0';
  _arg_yopt = (char *)_yopt.data();

  if (0 > 0)
    { error("dimension mismatch in argument ylabel"); return retval; }
  string _ylabel = args(6).string_value();
  _ylabel += '\0';
  _arg_ylabel = (char *)_ylabel.data();

  if (_n_dims(args(7)) > 0)
    { error("dimension mismatch in argument ytick"); return retval; }
  _arg_ytick = (float)args(7).double_value();

  if (_n_dims(args(8)) > 0)
    { error("dimension mismatch in argument nsuby"); return retval; }
  _arg_nsuby = (int)args(8).double_value();

  if (0 > 0)
    { error("dimension mismatch in argument zopt"); return retval; }
  string _zopt = args(9).string_value();
  _zopt += '\0';
  _arg_zopt = (char *)_zopt.data();

  if (0 > 0)
    { error("dimension mismatch in argument zlabel"); return retval; }
  string _zlabel = args(10).string_value();
  _zlabel += '\0';
  _arg_zlabel = (char *)_zlabel.data();

  if (_n_dims(args(11)) > 0)
    { error("dimension mismatch in argument ztick"); return retval; }
  _arg_ztick = (float)args(11).double_value();

  if (_n_dims(args(12)) > 0)
    { error("dimension mismatch in argument nsubz"); return retval; }
  _arg_nsubz = (int)args(12).double_value();

    c_plbox3(_arg_xopt, _arg_xlabel, _arg_xtick, _arg_nsubx, _arg_yopt, _arg_ylabel, _arg_ytick, _arg_nsuby, _arg_zopt, _arg_zlabel, _arg_ztick, _arg_nsubz);
  return retval;
}

octave_value_list _wrap_c_plcol0(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 1)
  {
    print_usage("c_plcol0");
    return retval;
  }

  int _arg_icol0;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument icol0"); return retval; }
  _arg_icol0 = (int)args(1).double_value();

    c_plcol0(_arg_icol0);
  return retval;
}

octave_value_list _wrap_plcol1(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 1)
  {
    print_usage("plcol1");
    return retval;
  }

  float _arg_col1;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument col1"); return retval; }
  _arg_col1 = (float)args(1).double_value();

    c_plcol1(_arg_col1);
  return retval;
}

octave_value_list _wrap_plcpstrm(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 2)
  {
    print_usage("plcpstrm");
    return retval;
  }

  int _arg_iplsr;
  int _arg_flags;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument iplsr"); return retval; }
  _arg_iplsr = (int)args(1).double_value();

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument flags"); return retval; }
  _arg_flags = (int)args(2).double_value();

    c_plcpstrm(_arg_iplsr, _arg_flags);
  return retval;
}

octave_value_list _wrap_plend(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 0)
  {
    print_usage("plend");
    return retval;
  }

    c_plend();
  return retval;
}

octave_value_list _wrap_plend1(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 0)
  {
    print_usage("plend1");
    return retval;
  }

    c_plend1();
  return retval;
}

octave_value_list _wrap_plenv(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 6)
  {
    print_usage("plenv");
    return retval;
  }

  float _arg_xmin;
  float _arg_xmax;
  float _arg_ymin;
  float _arg_ymax;
  int _arg_just;
  int _arg_axis;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument xmin"); return retval; }
  _arg_xmin = (float)args(1).double_value();

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument xmax"); return retval; }
  _arg_xmax = (float)args(2).double_value();

  if (_n_dims(args(3)) > 0)
    { error("dimension mismatch in argument ymin"); return retval; }
  _arg_ymin = (float)args(3).double_value();

  if (_n_dims(args(4)) > 0)
    { error("dimension mismatch in argument ymax"); return retval; }
  _arg_ymax = (float)args(4).double_value();

  if (_n_dims(args(5)) > 0)
    { error("dimension mismatch in argument just"); return retval; }
  _arg_just = (int)args(5).double_value();

  if (_n_dims(args(6)) > 0)
    { error("dimension mismatch in argument axis"); return retval; }
  _arg_axis = (int)args(6).double_value();

    c_plenv(_arg_xmin, _arg_xmax, _arg_ymin, _arg_ymax, _arg_just, _arg_axis);
  return retval;
}

octave_value_list _wrap_pleop(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 0)
  {
    print_usage("pleop");
    return retval;
  }

    c_pleop();
  return retval;
}

octave_value_list _wrap_plerrx(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 3)
  {
    print_usage("plerrx");
    return retval;
  }

  int _arg_n;
  float *_arg_xmin;
  float *_arg_xmax;
  float *_arg_y;
  _arg_n = (_dim(args(3), 0));
  if (  (int)(_dim(args(1), 0)) != (int)(_arg_n) ||
      _n_dims(args(1)) > 1)
    { error("dimension mismatch in argument xmin"); return retval; }
  Matrix _xmin = args(1).matrix_value();
  _arg_xmin = (float *)alloca(_xmin.dim1() * _xmin.dim2() * sizeof (float));
  _cvt_double_to(_arg_xmin, &_xmin(0,0), _xmin.dim1()*_xmin.dim2());

  if (  (int)(_dim(args(2), 0)) != (int)(_arg_n) ||
      _n_dims(args(2)) > 1)
    { error("dimension mismatch in argument xmax"); return retval; }
  Matrix _xmax = args(2).matrix_value();
  _arg_xmax = (float *)alloca(_xmax.dim1() * _xmax.dim2() * sizeof (float));
  _cvt_double_to(_arg_xmax, &_xmax(0,0), _xmax.dim1()*_xmax.dim2());

  if (_n_dims(args(3)) > 1)
    { error("dimension mismatch in argument y"); return retval; }
  Matrix _y = args(3).matrix_value();
  _arg_y = (float *)alloca(_y.dim1() * _y.dim2() * sizeof (float));
  _cvt_double_to(_arg_y, &_y(0,0), _y.dim1()*_y.dim2());

    c_plerrx(_arg_n, _arg_xmin, _arg_xmax, _arg_y);
  return retval;
}

octave_value_list _wrap_plerry(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 3)
  {
    print_usage("plerry");
    return retval;
  }

  int _arg_n;
  float *_arg_x;
  float *_arg_ymin;
  float *_arg_ymax;
  _arg_n = (_dim(args(3), 0));
  if (  (int)(_dim(args(1), 0)) != (int)(_arg_n) ||
      _n_dims(args(1)) > 1)
    { error("dimension mismatch in argument x"); return retval; }
  Matrix _x = args(1).matrix_value();
  _arg_x = (float *)alloca(_x.dim1() * _x.dim2() * sizeof (float));
  _cvt_double_to(_arg_x, &_x(0,0), _x.dim1()*_x.dim2());

  if (  (int)(_dim(args(2), 0)) != (int)(_arg_n) ||
      _n_dims(args(2)) > 1)
    { error("dimension mismatch in argument ymin"); return retval; }
  Matrix _ymin = args(2).matrix_value();
  _arg_ymin = (float *)alloca(_ymin.dim1() * _ymin.dim2() * sizeof (float));
  _cvt_double_to(_arg_ymin, &_ymin(0,0), _ymin.dim1()*_ymin.dim2());

  if (_n_dims(args(3)) > 1)
    { error("dimension mismatch in argument ymax"); return retval; }
  Matrix _ymax = args(3).matrix_value();
  _arg_ymax = (float *)alloca(_ymax.dim1() * _ymax.dim2() * sizeof (float));
  _cvt_double_to(_arg_ymax, &_ymax(0,0), _ymax.dim1()*_ymax.dim2());

    c_plerry(_arg_n, _arg_x, _arg_ymin, _arg_ymax);
  return retval;
}

octave_value_list _wrap_plfamadv(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 0)
  {
    print_usage("plfamadv");
    return retval;
  }

    c_plfamadv();
  return retval;
}

octave_value_list _wrap_plfill(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 2)
  {
    print_usage("plfill");
    return retval;
  }

  int _arg_n;
  float *_arg_x;
  float *_arg_y;
  _arg_n = (_dim(args(2), 0));
  if (  (int)(_dim(args(1), 0)) != (int)(_arg_n) ||
      _n_dims(args(1)) > 1)
    { error("dimension mismatch in argument x"); return retval; }
  Matrix _x = args(1).matrix_value();
  _arg_x = (float *)alloca(_x.dim1() * _x.dim2() * sizeof (float));
  _cvt_double_to(_arg_x, &_x(0,0), _x.dim1()*_x.dim2());

  if (_n_dims(args(2)) > 1)
    { error("dimension mismatch in argument y"); return retval; }
  Matrix _y = args(2).matrix_value();
  _arg_y = (float *)alloca(_y.dim1() * _y.dim2() * sizeof (float));
  _cvt_double_to(_arg_y, &_y(0,0), _y.dim1()*_y.dim2());

    c_plfill(_arg_n, _arg_x, _arg_y);
  return retval;
}

octave_value_list _wrap_plfill3(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 3)
  {
    print_usage("plfill3");
    return retval;
  }

  int _arg_n;
  float *_arg_x;
  float *_arg_y;
  float *_arg_z;
  _arg_n = (_dim(args(3), 0));
  if (  (int)(_dim(args(1), 0)) != (int)(_arg_n) ||
      _n_dims(args(1)) > 1)
    { error("dimension mismatch in argument x"); return retval; }
  Matrix _x = args(1).matrix_value();
  _arg_x = (float *)alloca(_x.dim1() * _x.dim2() * sizeof (float));
  _cvt_double_to(_arg_x, &_x(0,0), _x.dim1()*_x.dim2());

  if (  (int)(_dim(args(2), 0)) != (int)(_arg_n) ||
      _n_dims(args(2)) > 1)
    { error("dimension mismatch in argument y"); return retval; }
  Matrix _y = args(2).matrix_value();
  _arg_y = (float *)alloca(_y.dim1() * _y.dim2() * sizeof (float));
  _cvt_double_to(_arg_y, &_y(0,0), _y.dim1()*_y.dim2());

  if (_n_dims(args(3)) > 1)
    { error("dimension mismatch in argument z"); return retval; }
  Matrix _z = args(3).matrix_value();
  _arg_z = (float *)alloca(_z.dim1() * _z.dim2() * sizeof (float));
  _cvt_double_to(_arg_z, &_z(0,0), _z.dim1()*_z.dim2());

    c_plfill3(_arg_n, _arg_x, _arg_y, _arg_z);
  return retval;
}

octave_value_list _wrap_plflush(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 0)
  {
    print_usage("plflush");
    return retval;
  }

    c_plflush();
  return retval;
}

octave_value_list _wrap_plfont(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 1)
  {
    print_usage("plfont");
    return retval;
  }

  int _arg_ifont;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument ifont"); return retval; }
  _arg_ifont = (int)args(1).double_value();

    c_plfont(_arg_ifont);
  return retval;
}

octave_value_list _wrap_plfontld(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 1)
  {
    print_usage("plfontld");
    return retval;
  }

  int _arg_fnt;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument fnt"); return retval; }
  _arg_fnt = (int)args(1).double_value();

    c_plfontld(_arg_fnt);
  return retval;
}

octave_value_list _wrap_plgchr(const octave_value_list &args, int nargout)
{
  octave_value_list retval(2, octave_value());
  if (args.length()-1 != 0 ||
      nargout != 2)
  {
    print_usage("plgchr");
    return retval;
  }

  float _arg_p_def;
  float _arg_p_ht;
    c_plgchr(&_arg_p_def, &_arg_p_ht);
  retval(0) = octave_value((double)_arg_p_def);
  retval(1) = octave_value((double)_arg_p_ht);
  return retval;
}

octave_value_list _wrap_plgcol0(const octave_value_list &args, int nargout)
{
  octave_value_list retval(3, octave_value());
  if (args.length()-1 != 1 ||
      nargout != 3)
  {
    print_usage("plgcol0");
    return retval;
  }

  int _d[2] = { 1,1 };
  int _vec_n = 0;
  int _vidx;
  int _vec_sz;
  int *_arg_icol0;
  int *_arg_r;
  int *_arg_g;
  int *_arg_b;
  int _vecstride_icol0 = 1;
  if (!_check_input_vectorize(args(1), &_vec_n, _d, 0, &_vecstride_icol0))
    { error("dimension mismatch in argument icol0"); return retval; }
  Matrix _icol0 = args(1).matrix_value();
  _arg_icol0 = (int *)alloca(_icol0.dim1() * _icol0.dim2() * sizeof (int));
  _cvt_double_to(_arg_icol0, &_icol0(0,0), _icol0.dim1()*_icol0.dim2());

  if (_d[0]  != 1 || _d[1]  != 1)
    retval(0) = octave_value(Matrix(_d[0] , _d[1] ));
  _arg_r = (int *)alloca(_d[0]  * _d[1]  * sizeof (int));
  if (_d[0]  != 1 || _d[1]  != 1)
    retval(1) = octave_value(Matrix(_d[0] , _d[1] ));
  _arg_g = (int *)alloca(_d[0]  * _d[1]  * sizeof (int));
  if (_d[0]  != 1 || _d[1]  != 1)
    retval(2) = octave_value(Matrix(_d[0] , _d[1] ));
  _arg_b = (int *)alloca(_d[0]  * _d[1]  * sizeof (int));
  _vec_sz = _d[0]*_d[1];
  for (_vidx = 0; _vidx < _vec_sz; ++_vidx) {
    c_plgcol0(_arg_icol0[_vecstride_icol0*_vidx], &_arg_r[_vidx], &_arg_g[_vidx], &_arg_b[_vidx]);
  }
  if (_arraylen(retval(0)) == 1)
    retval(0) = octave_value(double(*_arg_r));
  else
    _cvt_to_double(_arg_r, (double *)retval(0).matrix_value().data(), _arraylen(retval(0)));
  if (_arraylen(retval(1)) == 1)
    retval(1) = octave_value(double(*_arg_g));
  else
    _cvt_to_double(_arg_g, (double *)retval(1).matrix_value().data(), _arraylen(retval(1)));
  if (_arraylen(retval(2)) == 1)
    retval(2) = octave_value(double(*_arg_b));
  else
    _cvt_to_double(_arg_b, (double *)retval(2).matrix_value().data(), _arraylen(retval(2)));
  return retval;
}

octave_value_list _wrap_plgcolbg(const octave_value_list &args, int nargout)
{
  octave_value_list retval(3, octave_value());
  if (args.length()-1 != 0 ||
      nargout != 3)
  {
    print_usage("plgcolbg");
    return retval;
  }

  int _arg_r;
  int _arg_g;
  int _arg_b;
    c_plgcolbg(&_arg_r, &_arg_g, &_arg_b);
  retval(0) = octave_value((double)_arg_r);
  retval(1) = octave_value((double)_arg_g);
  retval(2) = octave_value((double)_arg_b);
  return retval;
}

octave_value_list _wrap_c_plgcompression(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 1)
  {
    print_usage("c_plgcompression");
    return retval;
  }

  int _arg_compression;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument compression"); return retval; }
  _arg_compression = (int)args(1).double_value();

    c_plgcompression(&_arg_compression);
  return retval;
}

octave_value_list _wrap_plgdev(const octave_value_list &args, int )
{
  octave_value_list retval(1, octave_value());
  if (args.length()-1 != 0)
  {
    print_usage("plgdev");
    return retval;
  }

  char *_arg_p_dev;
  retval(0) = octave_value(charMatrix(80, 1), true);
  _arg_p_dev = (char *)retval(0).char_matrix_value().data();
    c_plgdev(_arg_p_dev);
  return retval;
}

octave_value_list _wrap_plgdidev(const octave_value_list &args, int nargout)
{
  octave_value_list retval(4, octave_value());
  if (args.length()-1 != 0 ||
      nargout != 4)
  {
    print_usage("plgdidev");
    return retval;
  }

  float _arg_p_mar;
  float _arg_p_aspect;
  float _arg_p_jx;
  float _arg_p_jy;
    c_plgdidev(&_arg_p_mar, &_arg_p_aspect, &_arg_p_jx, &_arg_p_jy);
  retval(0) = octave_value((double)_arg_p_mar);
  retval(1) = octave_value((double)_arg_p_aspect);
  retval(2) = octave_value((double)_arg_p_jx);
  retval(3) = octave_value((double)_arg_p_jy);
  return retval;
}

octave_value_list _wrap_plgdiori(const octave_value_list &args, int )
{
  octave_value_list retval(1, octave_value());
  if (args.length()-1 != 0)
  {
    print_usage("plgdiori");
    return retval;
  }

  float _arg_p_rot;
    c_plgdiori(&_arg_p_rot);
  retval(0) = octave_value((double)_arg_p_rot);
  return retval;
}

octave_value_list _wrap_plgdiplt(const octave_value_list &args, int nargout)
{
  octave_value_list retval(4, octave_value());
  if (args.length()-1 != 0 ||
      nargout != 4)
  {
    print_usage("plgdiplt");
    return retval;
  }

  float _arg_p_xmin;
  float _arg_p_ymin;
  float _arg_p_xmax;
  float _arg_p_ymax;
    c_plgdiplt(&_arg_p_xmin, &_arg_p_ymin, &_arg_p_xmax, &_arg_p_ymax);
  retval(0) = octave_value((double)_arg_p_xmin);
  retval(1) = octave_value((double)_arg_p_ymin);
  retval(2) = octave_value((double)_arg_p_xmax);
  retval(3) = octave_value((double)_arg_p_ymax);
  return retval;
}

octave_value_list _wrap_plgfam(const octave_value_list &args, int nargout)
{
  octave_value_list retval(3, octave_value());
  if (args.length()-1 != 0 ||
      nargout != 3)
  {
    print_usage("plgfam");
    return retval;
  }

  int _arg_p_fam;
  int _arg_p_num;
  int _arg_p_bmax;
    c_plgfam(&_arg_p_fam, &_arg_p_num, &_arg_p_bmax);
  retval(0) = octave_value((double)_arg_p_fam);
  retval(1) = octave_value((double)_arg_p_num);
  retval(2) = octave_value((double)_arg_p_bmax);
  return retval;
}

octave_value_list _wrap_plgfnam(const octave_value_list &args, int )
{
  octave_value_list retval(1, octave_value());
  if (args.length()-1 != 0)
  {
    print_usage("plgfnam");
    return retval;
  }

  char *_arg_fnam;
  retval(0) = octave_value(charMatrix(80, 1), true);
  _arg_fnam = (char *)retval(0).char_matrix_value().data();
    c_plgfnam(_arg_fnam);
  return retval;
}

octave_value_list _wrap_plglevel(const octave_value_list &args, int )
{
  octave_value_list retval(1, octave_value());
  if (args.length()-1 != 0)
  {
    print_usage("plglevel");
    return retval;
  }

  int _arg_p_level;
    c_plglevel(&_arg_p_level);
  retval(0) = octave_value((double)_arg_p_level);
  return retval;
}

octave_value_list _wrap_plgpage(const octave_value_list &args, int nargout)
{
  octave_value_list retval(6, octave_value());
  if (args.length()-1 != 0 ||
      nargout != 6)
  {
    print_usage("plgpage");
    return retval;
  }

  float _arg_p_xp;
  float _arg_p_yp;
  int _arg_p_xleng;
  int _arg_p_yleng;
  int _arg_p_xoff;
  int _arg_p_yoff;
    c_plgpage(&_arg_p_xp, &_arg_p_yp, &_arg_p_xleng, &_arg_p_yleng, &_arg_p_xoff, &_arg_p_yoff);
  retval(0) = octave_value((double)_arg_p_xp);
  retval(1) = octave_value((double)_arg_p_yp);
  retval(2) = octave_value((double)_arg_p_xleng);
  retval(3) = octave_value((double)_arg_p_yleng);
  retval(4) = octave_value((double)_arg_p_xoff);
  retval(5) = octave_value((double)_arg_p_yoff);
  return retval;
}

octave_value_list _wrap_plgra(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 0)
  {
    print_usage("plgra");
    return retval;
  }

    c_plgra();
  return retval;
}

octave_value_list _wrap_plgspa(const octave_value_list &args, int nargout)
{
  octave_value_list retval(4, octave_value());
  if (args.length()-1 != 0 ||
      nargout != 4)
  {
    print_usage("plgspa");
    return retval;
  }

  float _arg_xmin;
  float _arg_xmax;
  float _arg_ymin;
  float _arg_ymax;
    c_plgspa(&_arg_xmin, &_arg_xmax, &_arg_ymin, &_arg_ymax);
  retval(0) = octave_value((double)_arg_xmin);
  retval(1) = octave_value((double)_arg_xmax);
  retval(2) = octave_value((double)_arg_ymin);
  retval(3) = octave_value((double)_arg_ymax);
  return retval;
}

octave_value_list _wrap_plgstrm(const octave_value_list &args, int )
{
  octave_value_list retval(1, octave_value());
  if (args.length()-1 != 0)
  {
    print_usage("plgstrm");
    return retval;
  }

  int _arg_p_strm;
    c_plgstrm(&_arg_p_strm);
  retval(0) = octave_value((double)_arg_p_strm);
  return retval;
}

octave_value_list _wrap_plgver(const octave_value_list &args, int )
{
  octave_value_list retval(1, octave_value());
  if (args.length()-1 != 0)
  {
    print_usage("plgver");
    return retval;
  }

  char *_arg_p_ver;
  retval(0) = octave_value(charMatrix(20, 1), true);
  _arg_p_ver = (char *)retval(0).char_matrix_value().data();
    c_plgver(_arg_p_ver);
  return retval;
}

octave_value_list _wrap_plgxax(const octave_value_list &args, int nargout)
{
  octave_value_list retval(2, octave_value());
  if (args.length()-1 != 0 ||
      nargout != 2)
  {
    print_usage("plgxax");
    return retval;
  }

  int _arg_p_digmax;
  int _arg_p_digits;
    c_plgxax(&_arg_p_digmax, &_arg_p_digits);
  retval(0) = octave_value((double)_arg_p_digmax);
  retval(1) = octave_value((double)_arg_p_digits);
  return retval;
}

octave_value_list _wrap_plgyax(const octave_value_list &args, int nargout)
{
  octave_value_list retval(2, octave_value());
  if (args.length()-1 != 0 ||
      nargout != 2)
  {
    print_usage("plgyax");
    return retval;
  }

  int _arg_p_digmax;
  int _arg_p_digits;
    c_plgyax(&_arg_p_digmax, &_arg_p_digits);
  retval(0) = octave_value((double)_arg_p_digmax);
  retval(1) = octave_value((double)_arg_p_digits);
  return retval;
}

octave_value_list _wrap_plgzax(const octave_value_list &args, int nargout)
{
  octave_value_list retval(2, octave_value());
  if (args.length()-1 != 0 ||
      nargout != 2)
  {
    print_usage("plgzax");
    return retval;
  }

  int _arg_p_digmax;
  int _arg_p_digits;
    c_plgzax(&_arg_p_digmax, &_arg_p_digits);
  retval(0) = octave_value((double)_arg_p_digmax);
  retval(1) = octave_value((double)_arg_p_digits);
  return retval;
}

octave_value_list _wrap_plhist(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 5)
  {
    print_usage("plhist");
    return retval;
  }

  int _arg_n;
  float *_arg_data;
  float _arg_datmin;
  float _arg_datmax;
  int _arg_nbin;
  int _arg_oldwin;
  _arg_n = (_dim(args(1), 0));
  if (_n_dims(args(1)) > 1)
    { error("dimension mismatch in argument data"); return retval; }
  Matrix _data = args(1).matrix_value();
  _arg_data = (float *)alloca(_data.dim1() * _data.dim2() * sizeof (float));
  _cvt_double_to(_arg_data, &_data(0,0), _data.dim1()*_data.dim2());

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument datmin"); return retval; }
  _arg_datmin = (float)args(2).double_value();

  if (_n_dims(args(3)) > 0)
    { error("dimension mismatch in argument datmax"); return retval; }
  _arg_datmax = (float)args(3).double_value();

  if (_n_dims(args(4)) > 0)
    { error("dimension mismatch in argument nbin"); return retval; }
  _arg_nbin = (int)args(4).double_value();

  if (_n_dims(args(5)) > 0)
    { error("dimension mismatch in argument oldwin"); return retval; }
  _arg_oldwin = (int)args(5).double_value();

    c_plhist(_arg_n, _arg_data, _arg_datmin, _arg_datmax, _arg_nbin, _arg_oldwin);
  return retval;
}

octave_value_list _wrap_plhls(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 3)
  {
    print_usage("plhls");
    return retval;
  }

  float _arg_h;
  float _arg_l;
  float _arg_s;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument h"); return retval; }
  _arg_h = (float)args(1).double_value();

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument l"); return retval; }
  _arg_l = (float)args(2).double_value();

  if (_n_dims(args(3)) > 0)
    { error("dimension mismatch in argument s"); return retval; }
  _arg_s = (float)args(3).double_value();

    c_plhls(_arg_h, _arg_l, _arg_s);
  return retval;
}

octave_value_list _wrap_plinit(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 0)
  {
    print_usage("plinit");
    return retval;
  }

    c_plinit();
  return retval;
}

octave_value_list _wrap_pljoin(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 4)
  {
    print_usage("pljoin");
    return retval;
  }

  float _arg_x1;
  float _arg_y1;
  float _arg_x2;
  float _arg_y2;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument x1"); return retval; }
  _arg_x1 = (float)args(1).double_value();

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument y1"); return retval; }
  _arg_y1 = (float)args(2).double_value();

  if (_n_dims(args(3)) > 0)
    { error("dimension mismatch in argument x2"); return retval; }
  _arg_x2 = (float)args(3).double_value();

  if (_n_dims(args(4)) > 0)
    { error("dimension mismatch in argument y2"); return retval; }
  _arg_y2 = (float)args(4).double_value();

    c_pljoin(_arg_x1, _arg_y1, _arg_x2, _arg_y2);
  return retval;
}

octave_value_list _wrap_pllab(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 3)
  {
    print_usage("pllab");
    return retval;
  }

  char * _arg_xlabel;
  char * _arg_ylabel;
  char * _arg_tlabel;
  if (0 > 0)
    { error("dimension mismatch in argument xlabel"); return retval; }
  string _xlabel = args(1).string_value();
  _xlabel += '\0';
  _arg_xlabel = (char *)_xlabel.data();

  if (0 > 0)
    { error("dimension mismatch in argument ylabel"); return retval; }
  string _ylabel = args(2).string_value();
  _ylabel += '\0';
  _arg_ylabel = (char *)_ylabel.data();

  if (0 > 0)
    { error("dimension mismatch in argument tlabel"); return retval; }
  string _tlabel = args(3).string_value();
  _tlabel += '\0';
  _arg_tlabel = (char *)_tlabel.data();

    c_pllab(_arg_xlabel, _arg_ylabel, _arg_tlabel);
  return retval;
}

octave_value_list _wrap_pllightsource(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 3)
  {
    print_usage("pllightsource");
    return retval;
  }

  float _arg_x;
  float _arg_y;
  float _arg_z;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument x"); return retval; }
  _arg_x = (float)args(1).double_value();

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument y"); return retval; }
  _arg_y = (float)args(2).double_value();

  if (_n_dims(args(3)) > 0)
    { error("dimension mismatch in argument z"); return retval; }
  _arg_z = (float)args(3).double_value();

    c_pllightsource(_arg_x, _arg_y, _arg_z);
  return retval;
}

octave_value_list _wrap_plline(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 2)
  {
    print_usage("plline");
    return retval;
  }

  int _arg_n;
  float *_arg_x;
  float *_arg_y;
  _arg_n = (_dim(args(2), 0));
  if (  (int)(_dim(args(1), 0)) != (int)(_arg_n) ||
      _n_dims(args(1)) > 1)
    { error("dimension mismatch in argument x"); return retval; }
  Matrix _x = args(1).matrix_value();
  _arg_x = (float *)alloca(_x.dim1() * _x.dim2() * sizeof (float));
  _cvt_double_to(_arg_x, &_x(0,0), _x.dim1()*_x.dim2());

  if (_n_dims(args(2)) > 1)
    { error("dimension mismatch in argument y"); return retval; }
  Matrix _y = args(2).matrix_value();
  _arg_y = (float *)alloca(_y.dim1() * _y.dim2() * sizeof (float));
  _cvt_double_to(_arg_y, &_y(0,0), _y.dim1()*_y.dim2());

    c_plline(_arg_n, _arg_x, _arg_y);
  return retval;
}

octave_value_list _wrap_plline3(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 3)
  {
    print_usage("plline3");
    return retval;
  }

  int _arg_n;
  float *_arg_x;
  float *_arg_y;
  float *_arg_z;
  _arg_n = (_dim(args(3), 0));
  if (  (int)(_dim(args(1), 0)) != (int)(_arg_n) ||
      _n_dims(args(1)) > 1)
    { error("dimension mismatch in argument x"); return retval; }
  Matrix _x = args(1).matrix_value();
  _arg_x = (float *)alloca(_x.dim1() * _x.dim2() * sizeof (float));
  _cvt_double_to(_arg_x, &_x(0,0), _x.dim1()*_x.dim2());

  if (  (int)(_dim(args(2), 0)) != (int)(_arg_n) ||
      _n_dims(args(2)) > 1)
    { error("dimension mismatch in argument y"); return retval; }
  Matrix _y = args(2).matrix_value();
  _arg_y = (float *)alloca(_y.dim1() * _y.dim2() * sizeof (float));
  _cvt_double_to(_arg_y, &_y(0,0), _y.dim1()*_y.dim2());

  if (_n_dims(args(3)) > 1)
    { error("dimension mismatch in argument z"); return retval; }
  Matrix _z = args(3).matrix_value();
  _arg_z = (float *)alloca(_z.dim1() * _z.dim2() * sizeof (float));
  _cvt_double_to(_arg_z, &_z(0,0), _z.dim1()*_z.dim2());

    c_plline3(_arg_n, _arg_x, _arg_y, _arg_z);
  return retval;
}

octave_value_list _wrap_pllsty(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 1)
  {
    print_usage("pllsty");
    return retval;
  }

  int _arg_lin;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument lin"); return retval; }
  _arg_lin = (int)args(1).double_value();

    c_pllsty(_arg_lin);
  return retval;
}

octave_value_list _wrap_plmkstrm(const octave_value_list &args, int )
{
  octave_value_list retval(1, octave_value());
  if (args.length()-1 != 0)
  {
    print_usage("plmkstrm");
    return retval;
  }

  int _arg_p_strm;
    c_plmkstrm(&_arg_p_strm);
  retval(0) = octave_value((double)_arg_p_strm);
  return retval;
}

octave_value_list _wrap_plmtex(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 5)
  {
    print_usage("plmtex");
    return retval;
  }

  char * _arg_side;
  float _arg_disp;
  float _arg_pos;
  float _arg_just;
  char * _arg_text;
  if (0 > 0)
    { error("dimension mismatch in argument side"); return retval; }
  string _side = args(1).string_value();
  _side += '\0';
  _arg_side = (char *)_side.data();

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument disp"); return retval; }
  _arg_disp = (float)args(2).double_value();

  if (_n_dims(args(3)) > 0)
    { error("dimension mismatch in argument pos"); return retval; }
  _arg_pos = (float)args(3).double_value();

  if (_n_dims(args(4)) > 0)
    { error("dimension mismatch in argument just"); return retval; }
  _arg_just = (float)args(4).double_value();

  if (0 > 0)
    { error("dimension mismatch in argument text"); return retval; }
  string _text = args(5).string_value();
  _text += '\0';
  _arg_text = (char *)_text.data();

    c_plmtex(_arg_side, _arg_disp, _arg_pos, _arg_just, _arg_text);
  return retval;
}

octave_value_list _wrap_plpat(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 2)
  {
    print_usage("plpat");
    return retval;
  }

  int _arg_nlin;
  int *_arg_inc;
  int *_arg_del;
  _arg_nlin = (_dim(args(2), 0));
  if (  (int)(_dim(args(1), 0)) != (int)(_arg_nlin) ||
      _n_dims(args(1)) > 1)
    { error("dimension mismatch in argument inc"); return retval; }
  Matrix _inc = args(1).matrix_value();
  _arg_inc = (int *)alloca(_inc.dim1() * _inc.dim2() * sizeof (int));
  _cvt_double_to(_arg_inc, &_inc(0,0), _inc.dim1()*_inc.dim2());

  if (_n_dims(args(2)) > 1)
    { error("dimension mismatch in argument del"); return retval; }
  Matrix _del = args(2).matrix_value();
  _arg_del = (int *)alloca(_del.dim1() * _del.dim2() * sizeof (int));
  _cvt_double_to(_arg_del, &_del(0,0), _del.dim1()*_del.dim2());

    c_plpat(_arg_nlin, _arg_inc, _arg_del);
  return retval;
}

octave_value_list _wrap_plpoin(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 3)
  {
    print_usage("plpoin");
    return retval;
  }

  int _arg_n;
  float *_arg_x;
  float *_arg_y;
  int _arg_code;
  _arg_n = (_dim(args(2), 0));
  if (  (int)(_dim(args(1), 0)) != (int)(_arg_n) ||
      _n_dims(args(1)) > 1)
    { error("dimension mismatch in argument x"); return retval; }
  Matrix _x = args(1).matrix_value();
  _arg_x = (float *)alloca(_x.dim1() * _x.dim2() * sizeof (float));
  _cvt_double_to(_arg_x, &_x(0,0), _x.dim1()*_x.dim2());

  if (_n_dims(args(2)) > 1)
    { error("dimension mismatch in argument y"); return retval; }
  Matrix _y = args(2).matrix_value();
  _arg_y = (float *)alloca(_y.dim1() * _y.dim2() * sizeof (float));
  _cvt_double_to(_arg_y, &_y(0,0), _y.dim1()*_y.dim2());

  if (_n_dims(args(3)) > 0)
    { error("dimension mismatch in argument code"); return retval; }
  _arg_code = (int)args(3).double_value();

    c_plpoin(_arg_n, _arg_x, _arg_y, _arg_code);
  return retval;
}

octave_value_list _wrap_plpoin3(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 4)
  {
    print_usage("plpoin3");
    return retval;
  }

  int _arg_n;
  float *_arg_x;
  float *_arg_y;
  float *_arg_z;
  int _arg_code;
  _arg_n = (_dim(args(3), 0));
  if (  (int)(_dim(args(1), 0)) != (int)(_arg_n) ||
      _n_dims(args(1)) > 1)
    { error("dimension mismatch in argument x"); return retval; }
  Matrix _x = args(1).matrix_value();
  _arg_x = (float *)alloca(_x.dim1() * _x.dim2() * sizeof (float));
  _cvt_double_to(_arg_x, &_x(0,0), _x.dim1()*_x.dim2());

  if (  (int)(_dim(args(2), 0)) != (int)(_arg_n) ||
      _n_dims(args(2)) > 1)
    { error("dimension mismatch in argument y"); return retval; }
  Matrix _y = args(2).matrix_value();
  _arg_y = (float *)alloca(_y.dim1() * _y.dim2() * sizeof (float));
  _cvt_double_to(_arg_y, &_y(0,0), _y.dim1()*_y.dim2());

  if (_n_dims(args(3)) > 1)
    { error("dimension mismatch in argument z"); return retval; }
  Matrix _z = args(3).matrix_value();
  _arg_z = (float *)alloca(_z.dim1() * _z.dim2() * sizeof (float));
  _cvt_double_to(_arg_z, &_z(0,0), _z.dim1()*_z.dim2());

  if (_n_dims(args(4)) > 0)
    { error("dimension mismatch in argument code"); return retval; }
  _arg_code = (int)args(4).double_value();

    c_plpoin3(_arg_n, _arg_x, _arg_y, _arg_z, _arg_code);
  return retval;
}

octave_value_list _wrap_plprec(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 2)
  {
    print_usage("plprec");
    return retval;
  }

  int _arg_setp;
  int _arg_prec;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument setp"); return retval; }
  _arg_setp = (int)args(1).double_value();

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument prec"); return retval; }
  _arg_prec = (int)args(2).double_value();

    c_plprec(_arg_setp, _arg_prec);
  return retval;
}

octave_value_list _wrap_plpsty(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 1)
  {
    print_usage("plpsty");
    return retval;
  }

  int _arg_patt;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument patt"); return retval; }
  _arg_patt = (int)args(1).double_value();

    c_plpsty(_arg_patt);
  return retval;
}

octave_value_list _wrap_plptex(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 6)
  {
    print_usage("plptex");
    return retval;
  }

  float _arg_x;
  float _arg_y;
  float _arg_dx;
  float _arg_dy;
  float _arg_just;
  char * _arg_text;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument x"); return retval; }
  _arg_x = (float)args(1).double_value();

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument y"); return retval; }
  _arg_y = (float)args(2).double_value();

  if (_n_dims(args(3)) > 0)
    { error("dimension mismatch in argument dx"); return retval; }
  _arg_dx = (float)args(3).double_value();

  if (_n_dims(args(4)) > 0)
    { error("dimension mismatch in argument dy"); return retval; }
  _arg_dy = (float)args(4).double_value();

  if (_n_dims(args(5)) > 0)
    { error("dimension mismatch in argument just"); return retval; }
  _arg_just = (float)args(5).double_value();

  if (0 > 0)
    { error("dimension mismatch in argument text"); return retval; }
  string _text = args(6).string_value();
  _text += '\0';
  _arg_text = (char *)_text.data();

    c_plptex(_arg_x, _arg_y, _arg_dx, _arg_dy, _arg_just, _arg_text);
  return retval;
}

octave_value_list _wrap_plreplot(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 0)
  {
    print_usage("plreplot");
    return retval;
  }

    c_plreplot();
  return retval;
}

octave_value_list _wrap_plrgb(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 3)
  {
    print_usage("plrgb");
    return retval;
  }

  float _arg_r;
  float _arg_g;
  float _arg_b;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument r"); return retval; }
  _arg_r = (float)args(1).double_value();

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument g"); return retval; }
  _arg_g = (float)args(2).double_value();

  if (_n_dims(args(3)) > 0)
    { error("dimension mismatch in argument b"); return retval; }
  _arg_b = (float)args(3).double_value();

    c_plrgb(_arg_r, _arg_g, _arg_b);
  return retval;
}

octave_value_list _wrap_plrgb1(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 3)
  {
    print_usage("plrgb1");
    return retval;
  }

  int _arg_r;
  int _arg_g;
  int _arg_b;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument r"); return retval; }
  _arg_r = (int)args(1).double_value();

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument g"); return retval; }
  _arg_g = (int)args(2).double_value();

  if (_n_dims(args(3)) > 0)
    { error("dimension mismatch in argument b"); return retval; }
  _arg_b = (int)args(3).double_value();

    c_plrgb1(_arg_r, _arg_g, _arg_b);
  return retval;
}

octave_value_list _wrap_plschr(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 2)
  {
    print_usage("plschr");
    return retval;
  }

  float _arg_def;
  float _arg_scale;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument def"); return retval; }
  _arg_def = (float)args(1).double_value();

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument scale"); return retval; }
  _arg_scale = (float)args(2).double_value();

    c_plschr(_arg_def, _arg_scale);
  return retval;
}

octave_value_list _wrap_plscmap0(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 3)
  {
    print_usage("plscmap0");
    return retval;
  }

  int *_arg_r;
  int *_arg_g;
  int *_arg_b;
  int _arg_ncol0;
  _arg_ncol0 = (_dim(args(3), 0));
  if (  (int)(_dim(args(1), 0)) != (int)(_arg_ncol0) ||
      _n_dims(args(1)) > 1)
    { error("dimension mismatch in argument r"); return retval; }
  Matrix _r = args(1).matrix_value();
  _arg_r = (int *)alloca(_r.dim1() * _r.dim2() * sizeof (int));
  _cvt_double_to(_arg_r, &_r(0,0), _r.dim1()*_r.dim2());

  if (  (int)(_dim(args(2), 0)) != (int)(_arg_ncol0) ||
      _n_dims(args(2)) > 1)
    { error("dimension mismatch in argument g"); return retval; }
  Matrix _g = args(2).matrix_value();
  _arg_g = (int *)alloca(_g.dim1() * _g.dim2() * sizeof (int));
  _cvt_double_to(_arg_g, &_g(0,0), _g.dim1()*_g.dim2());

  if (_n_dims(args(3)) > 1)
    { error("dimension mismatch in argument b"); return retval; }
  Matrix _b = args(3).matrix_value();
  _arg_b = (int *)alloca(_b.dim1() * _b.dim2() * sizeof (int));
  _cvt_double_to(_arg_b, &_b(0,0), _b.dim1()*_b.dim2());

    c_plscmap0(_arg_r, _arg_g, _arg_b, _arg_ncol0);
  return retval;
}

octave_value_list _wrap_plscmap0n(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 1)
  {
    print_usage("plscmap0n");
    return retval;
  }

  int _arg_ncol0;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument ncol0"); return retval; }
  _arg_ncol0 = (int)args(1).double_value();

    c_plscmap0n(_arg_ncol0);
  return retval;
}

octave_value_list _wrap_plscmap1(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 3)
  {
    print_usage("plscmap1");
    return retval;
  }

  int *_arg_r;
  int *_arg_g;
  int *_arg_b;
  int _arg_ncol1;
  _arg_ncol1 = (_dim(args(3), 0));
  if (  (int)(_dim(args(1), 0)) != (int)(_arg_ncol1) ||
      _n_dims(args(1)) > 1)
    { error("dimension mismatch in argument r"); return retval; }
  Matrix _r = args(1).matrix_value();
  _arg_r = (int *)alloca(_r.dim1() * _r.dim2() * sizeof (int));
  _cvt_double_to(_arg_r, &_r(0,0), _r.dim1()*_r.dim2());

  if (  (int)(_dim(args(2), 0)) != (int)(_arg_ncol1) ||
      _n_dims(args(2)) > 1)
    { error("dimension mismatch in argument g"); return retval; }
  Matrix _g = args(2).matrix_value();
  _arg_g = (int *)alloca(_g.dim1() * _g.dim2() * sizeof (int));
  _cvt_double_to(_arg_g, &_g(0,0), _g.dim1()*_g.dim2());

  if (_n_dims(args(3)) > 1)
    { error("dimension mismatch in argument b"); return retval; }
  Matrix _b = args(3).matrix_value();
  _arg_b = (int *)alloca(_b.dim1() * _b.dim2() * sizeof (int));
  _cvt_double_to(_arg_b, &_b(0,0), _b.dim1()*_b.dim2());

    c_plscmap1(_arg_r, _arg_g, _arg_b, _arg_ncol1);
  return retval;
}

octave_value_list _wrap_plscmap1l(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 6)
  {
    print_usage("plscmap1l");
    return retval;
  }

  int _arg_itype;
  int _arg_npts;
  float *_arg_intensity;
  float *_arg_coord1;
  float *_arg_coord2;
  float *_arg_coord3;
  int *_arg_rev;
  _arg_npts = (_dim(args(6), 0));
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument itype"); return retval; }
  _arg_itype = (int)args(1).double_value();

  if (  (int)(_dim(args(2), 0)) != (int)(_arg_npts) ||
      _n_dims(args(2)) > 1)
    { error("dimension mismatch in argument intensity"); return retval; }
  Matrix _intensity = args(2).matrix_value();
  _arg_intensity = (float *)alloca(_intensity.dim1() * _intensity.dim2() * sizeof (float));
  _cvt_double_to(_arg_intensity, &_intensity(0,0), _intensity.dim1()*_intensity.dim2());

  if (  (int)(_dim(args(3), 0)) != (int)(_arg_npts) ||
      _n_dims(args(3)) > 1)
    { error("dimension mismatch in argument coord1"); return retval; }
  Matrix _coord1 = args(3).matrix_value();
  _arg_coord1 = (float *)alloca(_coord1.dim1() * _coord1.dim2() * sizeof (float));
  _cvt_double_to(_arg_coord1, &_coord1(0,0), _coord1.dim1()*_coord1.dim2());

  if (  (int)(_dim(args(4), 0)) != (int)(_arg_npts) ||
      _n_dims(args(4)) > 1)
    { error("dimension mismatch in argument coord2"); return retval; }
  Matrix _coord2 = args(4).matrix_value();
  _arg_coord2 = (float *)alloca(_coord2.dim1() * _coord2.dim2() * sizeof (float));
  _cvt_double_to(_arg_coord2, &_coord2(0,0), _coord2.dim1()*_coord2.dim2());

  if (  (int)(_dim(args(5), 0)) != (int)(_arg_npts) ||
      _n_dims(args(5)) > 1)
    { error("dimension mismatch in argument coord3"); return retval; }
  Matrix _coord3 = args(5).matrix_value();
  _arg_coord3 = (float *)alloca(_coord3.dim1() * _coord3.dim2() * sizeof (float));
  _cvt_double_to(_arg_coord3, &_coord3(0,0), _coord3.dim1()*_coord3.dim2());

  if (_n_dims(args(6)) > 1)
    { error("dimension mismatch in argument rev"); return retval; }
  Matrix _rev = args(6).matrix_value();
  _arg_rev = (int *)alloca(_rev.dim1() * _rev.dim2() * sizeof (int));
  _cvt_double_to(_arg_rev, &_rev(0,0), _rev.dim1()*_rev.dim2());

    c_plscmap1l(_arg_itype, _arg_npts, _arg_intensity, _arg_coord1, _arg_coord2, _arg_coord3, _arg_rev);
  return retval;
}

octave_value_list _wrap_plscmap1n(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 1)
  {
    print_usage("plscmap1n");
    return retval;
  }

  int _arg_ncol1;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument ncol1"); return retval; }
  _arg_ncol1 = (int)args(1).double_value();

    c_plscmap1n(_arg_ncol1);
  return retval;
}

octave_value_list _wrap_plscol0(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 4)
  {
    print_usage("plscol0");
    return retval;
  }

  int _arg_icol0;
  int _arg_r;
  int _arg_g;
  int _arg_b;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument icol0"); return retval; }
  _arg_icol0 = (int)args(1).double_value();

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument r"); return retval; }
  _arg_r = (int)args(2).double_value();

  if (_n_dims(args(3)) > 0)
    { error("dimension mismatch in argument g"); return retval; }
  _arg_g = (int)args(3).double_value();

  if (_n_dims(args(4)) > 0)
    { error("dimension mismatch in argument b"); return retval; }
  _arg_b = (int)args(4).double_value();

    c_plscol0(_arg_icol0, _arg_r, _arg_g, _arg_b);
  return retval;
}

octave_value_list _wrap_plscolbg(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 3)
  {
    print_usage("plscolbg");
    return retval;
  }

  int _arg_r;
  int _arg_g;
  int _arg_b;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument r"); return retval; }
  _arg_r = (int)args(1).double_value();

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument g"); return retval; }
  _arg_g = (int)args(2).double_value();

  if (_n_dims(args(3)) > 0)
    { error("dimension mismatch in argument b"); return retval; }
  _arg_b = (int)args(3).double_value();

    c_plscolbg(_arg_r, _arg_g, _arg_b);
  return retval;
}

octave_value_list _wrap_plscolor(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 1)
  {
    print_usage("plscolor");
    return retval;
  }

  int _arg_color;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument color"); return retval; }
  _arg_color = (int)args(1).double_value();

    c_plscolor(_arg_color);
  return retval;
}

octave_value_list _wrap_c_plscompression(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 1)
  {
    print_usage("c_plscompression");
    return retval;
  }

  int _arg_compression;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument compression"); return retval; }
  _arg_compression = (int)args(1).double_value();

    c_plscompression(_arg_compression);
  return retval;
}

octave_value_list _wrap_plsdev(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 1)
  {
    print_usage("plsdev");
    return retval;
  }

  char * _arg_devname;
  if (0 > 0)
    { error("dimension mismatch in argument devname"); return retval; }
  string _devname = args(1).string_value();
  _devname += '\0';
  _arg_devname = (char *)_devname.data();

    c_plsdev(_arg_devname);
  return retval;
}

octave_value_list _wrap_plsdidev(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 4)
  {
    print_usage("plsdidev");
    return retval;
  }

  float _arg_mar;
  float _arg_aspect;
  float _arg_jx;
  float _arg_jy;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument mar"); return retval; }
  _arg_mar = (float)args(1).double_value();

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument aspect"); return retval; }
  _arg_aspect = (float)args(2).double_value();

  if (_n_dims(args(3)) > 0)
    { error("dimension mismatch in argument jx"); return retval; }
  _arg_jx = (float)args(3).double_value();

  if (_n_dims(args(4)) > 0)
    { error("dimension mismatch in argument jy"); return retval; }
  _arg_jy = (float)args(4).double_value();

    c_plsdidev(_arg_mar, _arg_aspect, _arg_jx, _arg_jy);
  return retval;
}

octave_value_list _wrap_plsdimap(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 6)
  {
    print_usage("plsdimap");
    return retval;
  }

  int _arg_dimxmin;
  int _arg_dimxmax;
  int _arg_dimymin;
  int _arg_dimymax;
  float _arg_dimxpmm;
  float _arg_dimypmm;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument dimxmin"); return retval; }
  _arg_dimxmin = (int)args(1).double_value();

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument dimxmax"); return retval; }
  _arg_dimxmax = (int)args(2).double_value();

  if (_n_dims(args(3)) > 0)
    { error("dimension mismatch in argument dimymin"); return retval; }
  _arg_dimymin = (int)args(3).double_value();

  if (_n_dims(args(4)) > 0)
    { error("dimension mismatch in argument dimymax"); return retval; }
  _arg_dimymax = (int)args(4).double_value();

  if (_n_dims(args(5)) > 0)
    { error("dimension mismatch in argument dimxpmm"); return retval; }
  _arg_dimxpmm = (float)args(5).double_value();

  if (_n_dims(args(6)) > 0)
    { error("dimension mismatch in argument dimypmm"); return retval; }
  _arg_dimypmm = (float)args(6).double_value();

    c_plsdimap(_arg_dimxmin, _arg_dimxmax, _arg_dimymin, _arg_dimymax, _arg_dimxpmm, _arg_dimypmm);
  return retval;
}

octave_value_list _wrap_plsdiori(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 1)
  {
    print_usage("plsdiori");
    return retval;
  }

  float _arg_rot;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument rot"); return retval; }
  _arg_rot = (float)args(1).double_value();

    c_plsdiori(_arg_rot);
  return retval;
}

octave_value_list _wrap_plsdiplt(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 4)
  {
    print_usage("plsdiplt");
    return retval;
  }

  float _arg_xmin;
  float _arg_ymin;
  float _arg_xmax;
  float _arg_ymax;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument xmin"); return retval; }
  _arg_xmin = (float)args(1).double_value();

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument ymin"); return retval; }
  _arg_ymin = (float)args(2).double_value();

  if (_n_dims(args(3)) > 0)
    { error("dimension mismatch in argument xmax"); return retval; }
  _arg_xmax = (float)args(3).double_value();

  if (_n_dims(args(4)) > 0)
    { error("dimension mismatch in argument ymax"); return retval; }
  _arg_ymax = (float)args(4).double_value();

    c_plsdiplt(_arg_xmin, _arg_ymin, _arg_xmax, _arg_ymax);
  return retval;
}

octave_value_list _wrap_plsdiplz(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 4)
  {
    print_usage("plsdiplz");
    return retval;
  }

  float _arg_xmin;
  float _arg_ymin;
  float _arg_xmax;
  float _arg_ymax;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument xmin"); return retval; }
  _arg_xmin = (float)args(1).double_value();

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument ymin"); return retval; }
  _arg_ymin = (float)args(2).double_value();

  if (_n_dims(args(3)) > 0)
    { error("dimension mismatch in argument xmax"); return retval; }
  _arg_xmax = (float)args(3).double_value();

  if (_n_dims(args(4)) > 0)
    { error("dimension mismatch in argument ymax"); return retval; }
  _arg_ymax = (float)args(4).double_value();

    c_plsdiplz(_arg_xmin, _arg_ymin, _arg_xmax, _arg_ymax);
  return retval;
}

octave_value_list _wrap_plsesc(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 1)
  {
    print_usage("plsesc");
    return retval;
  }

  char _arg_esc;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument esc"); return retval; }
  _arg_esc = *args(1).char_matrix_value().data();

    c_plsesc(_arg_esc);
  return retval;
}

octave_value_list _wrap_plsfam(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 3)
  {
    print_usage("plsfam");
    return retval;
  }

  int _arg_fam;
  int _arg_num;
  int _arg_bmax;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument fam"); return retval; }
  _arg_fam = (int)args(1).double_value();

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument num"); return retval; }
  _arg_num = (int)args(2).double_value();

  if (_n_dims(args(3)) > 0)
    { error("dimension mismatch in argument bmax"); return retval; }
  _arg_bmax = (int)args(3).double_value();

    c_plsfam(_arg_fam, _arg_num, _arg_bmax);
  return retval;
}

octave_value_list _wrap_plsfnam(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 1)
  {
    print_usage("plsfnam");
    return retval;
  }

  char * _arg_fnam;
  if (0 > 0)
    { error("dimension mismatch in argument fnam"); return retval; }
  string _fnam = args(1).string_value();
  _fnam += '\0';
  _arg_fnam = (char *)_fnam.data();

    c_plsfnam(_arg_fnam);
  return retval;
}

octave_value_list _wrap_plsmaj(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 2)
  {
    print_usage("plsmaj");
    return retval;
  }

  float _arg_def;
  float _arg_scale;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument def"); return retval; }
  _arg_def = (float)args(1).double_value();

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument scale"); return retval; }
  _arg_scale = (float)args(2).double_value();

    c_plsmaj(_arg_def, _arg_scale);
  return retval;
}

octave_value_list _wrap_plsmin(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 2)
  {
    print_usage("plsmin");
    return retval;
  }

  float _arg_def;
  float _arg_scale;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument def"); return retval; }
  _arg_def = (float)args(1).double_value();

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument scale"); return retval; }
  _arg_scale = (float)args(2).double_value();

    c_plsmin(_arg_def, _arg_scale);
  return retval;
}

octave_value_list _wrap_plsori(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 1)
  {
    print_usage("plsori");
    return retval;
  }

  int _arg_ori;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument ori"); return retval; }
  _arg_ori = (int)args(1).double_value();

    c_plsori(_arg_ori);
  return retval;
}

octave_value_list _wrap_plspage(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 6)
  {
    print_usage("plspage");
    return retval;
  }

  float _arg_xp;
  float _arg_yp;
  int _arg_xleng;
  int _arg_yleng;
  int _arg_xoff;
  int _arg_yoff;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument xp"); return retval; }
  _arg_xp = (float)args(1).double_value();

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument yp"); return retval; }
  _arg_yp = (float)args(2).double_value();

  if (_n_dims(args(3)) > 0)
    { error("dimension mismatch in argument xleng"); return retval; }
  _arg_xleng = (int)args(3).double_value();

  if (_n_dims(args(4)) > 0)
    { error("dimension mismatch in argument yleng"); return retval; }
  _arg_yleng = (int)args(4).double_value();

  if (_n_dims(args(5)) > 0)
    { error("dimension mismatch in argument xoff"); return retval; }
  _arg_xoff = (int)args(5).double_value();

  if (_n_dims(args(6)) > 0)
    { error("dimension mismatch in argument yoff"); return retval; }
  _arg_yoff = (int)args(6).double_value();

    c_plspage(_arg_xp, _arg_yp, _arg_xleng, _arg_yleng, _arg_xoff, _arg_yoff);
  return retval;
}

octave_value_list _wrap_plspause(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 1)
  {
    print_usage("plspause");
    return retval;
  }

  int _arg_pause;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument pause"); return retval; }
  _arg_pause = (int)args(1).double_value();

    c_plspause(_arg_pause);
  return retval;
}

octave_value_list _wrap_plsstrm(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 1)
  {
    print_usage("plsstrm");
    return retval;
  }

  int _arg_strm;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument strm"); return retval; }
  _arg_strm = (int)args(1).double_value();

    c_plsstrm(_arg_strm);
  return retval;
}

octave_value_list _wrap_plssub(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 2)
  {
    print_usage("plssub");
    return retval;
  }

  int _arg_nx;
  int _arg_ny;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument nx"); return retval; }
  _arg_nx = (int)args(1).double_value();

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument ny"); return retval; }
  _arg_ny = (int)args(2).double_value();

    c_plssub(_arg_nx, _arg_ny);
  return retval;
}

octave_value_list _wrap_plssym(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 2)
  {
    print_usage("plssym");
    return retval;
  }

  float _arg_def;
  float _arg_scale;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument def"); return retval; }
  _arg_def = (float)args(1).double_value();

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument scale"); return retval; }
  _arg_scale = (float)args(2).double_value();

    c_plssym(_arg_def, _arg_scale);
  return retval;
}

octave_value_list _wrap_plstar(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 2)
  {
    print_usage("plstar");
    return retval;
  }

  int _arg_nx;
  int _arg_ny;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument nx"); return retval; }
  _arg_nx = (int)args(1).double_value();

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument ny"); return retval; }
  _arg_ny = (int)args(2).double_value();

    c_plstar(_arg_nx, _arg_ny);
  return retval;
}

octave_value_list _wrap_plstart(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 3)
  {
    print_usage("plstart");
    return retval;
  }

  char * _arg_devname;
  int _arg_nx;
  int _arg_ny;
  if (0 > 0)
    { error("dimension mismatch in argument devname"); return retval; }
  string _devname = args(1).string_value();
  _devname += '\0';
  _arg_devname = (char *)_devname.data();

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument nx"); return retval; }
  _arg_nx = (int)args(2).double_value();

  if (_n_dims(args(3)) > 0)
    { error("dimension mismatch in argument ny"); return retval; }
  _arg_ny = (int)args(3).double_value();

    c_plstart(_arg_devname, _arg_nx, _arg_ny);
  return retval;
}

octave_value_list _wrap_plstripa(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 4)
  {
    print_usage("plstripa");
    return retval;
  }

  int _arg_id;
  int _arg_pen;
  float _arg_x;
  float _arg_y;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument id"); return retval; }
  _arg_id = (int)args(1).double_value();

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument pen"); return retval; }
  _arg_pen = (int)args(2).double_value();

  if (_n_dims(args(3)) > 0)
    { error("dimension mismatch in argument x"); return retval; }
  _arg_x = (float)args(3).double_value();

  if (_n_dims(args(4)) > 0)
    { error("dimension mismatch in argument y"); return retval; }
  _arg_y = (float)args(4).double_value();

    c_plstripa(_arg_id, _arg_pen, _arg_x, _arg_y);
  return retval;
}

octave_value_list _wrap_plstripd(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 1)
  {
    print_usage("plstripd");
    return retval;
  }

  int _arg_id;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument id"); return retval; }
  _arg_id = (int)args(1).double_value();

    c_plstripd(_arg_id);
  return retval;
}

octave_value_list _wrap_plstyl(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 3)
  {
    print_usage("plstyl");
    return retval;
  }

  int _arg_nms;
  int _arg_mark;
  int _arg_space;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument nms"); return retval; }
  _arg_nms = (int)args(1).double_value();

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument mark"); return retval; }
  _arg_mark = (int)args(2).double_value();

  if (_n_dims(args(3)) > 0)
    { error("dimension mismatch in argument space"); return retval; }
  _arg_space = (int)args(3).double_value();

    c_plstyl(_arg_nms, &_arg_mark, &_arg_space);
  return retval;
}

octave_value_list _wrap_plsvpa(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 4)
  {
    print_usage("plsvpa");
    return retval;
  }

  float _arg_xmin;
  float _arg_xmax;
  float _arg_ymin;
  float _arg_ymax;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument xmin"); return retval; }
  _arg_xmin = (float)args(1).double_value();

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument xmax"); return retval; }
  _arg_xmax = (float)args(2).double_value();

  if (_n_dims(args(3)) > 0)
    { error("dimension mismatch in argument ymin"); return retval; }
  _arg_ymin = (float)args(3).double_value();

  if (_n_dims(args(4)) > 0)
    { error("dimension mismatch in argument ymax"); return retval; }
  _arg_ymax = (float)args(4).double_value();

    c_plsvpa(_arg_xmin, _arg_xmax, _arg_ymin, _arg_ymax);
  return retval;
}

octave_value_list _wrap_plsxax(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 2)
  {
    print_usage("plsxax");
    return retval;
  }

  int _arg_digmax;
  int _arg_digits;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument digmax"); return retval; }
  _arg_digmax = (int)args(1).double_value();

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument digits"); return retval; }
  _arg_digits = (int)args(2).double_value();

    c_plsxax(_arg_digmax, _arg_digits);
  return retval;
}

octave_value_list _wrap_plsyax(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 2)
  {
    print_usage("plsyax");
    return retval;
  }

  int _arg_digmax;
  int _arg_digits;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument digmax"); return retval; }
  _arg_digmax = (int)args(1).double_value();

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument digits"); return retval; }
  _arg_digits = (int)args(2).double_value();

    c_plsyax(_arg_digmax, _arg_digits);
  return retval;
}

octave_value_list _wrap_plsym(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 3)
  {
    print_usage("plsym");
    return retval;
  }

  int _arg_n;
  float *_arg_x;
  float *_arg_y;
  int _arg_code;
  _arg_n = (_dim(args(2), 0));
  if (  (int)(_dim(args(1), 0)) != (int)(_arg_n) ||
      _n_dims(args(1)) > 1)
    { error("dimension mismatch in argument x"); return retval; }
  Matrix _x = args(1).matrix_value();
  _arg_x = (float *)alloca(_x.dim1() * _x.dim2() * sizeof (float));
  _cvt_double_to(_arg_x, &_x(0,0), _x.dim1()*_x.dim2());

  if (_n_dims(args(2)) > 1)
    { error("dimension mismatch in argument y"); return retval; }
  Matrix _y = args(2).matrix_value();
  _arg_y = (float *)alloca(_y.dim1() * _y.dim2() * sizeof (float));
  _cvt_double_to(_arg_y, &_y(0,0), _y.dim1()*_y.dim2());

  if (_n_dims(args(3)) > 0)
    { error("dimension mismatch in argument code"); return retval; }
  _arg_code = (int)args(3).double_value();

    c_plsym(_arg_n, _arg_x, _arg_y, _arg_code);
  return retval;
}

octave_value_list _wrap_plszax(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 2)
  {
    print_usage("plszax");
    return retval;
  }

  int _arg_digmax;
  int _arg_digits;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument digmax"); return retval; }
  _arg_digmax = (int)args(1).double_value();

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument digits"); return retval; }
  _arg_digits = (int)args(2).double_value();

    c_plszax(_arg_digmax, _arg_digits);
  return retval;
}

octave_value_list _wrap_pltext(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 0)
  {
    print_usage("pltext");
    return retval;
  }

    c_pltext();
  return retval;
}

octave_value_list _wrap_plvasp(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 1)
  {
    print_usage("plvasp");
    return retval;
  }

  float _arg_aspect;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument aspect"); return retval; }
  _arg_aspect = (float)args(1).double_value();

    c_plvasp(_arg_aspect);
  return retval;
}

octave_value_list _wrap_plvpas(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 5)
  {
    print_usage("plvpas");
    return retval;
  }

  float _arg_xmin;
  float _arg_xmax;
  float _arg_ymin;
  float _arg_ymax;
  float _arg_aspect;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument xmin"); return retval; }
  _arg_xmin = (float)args(1).double_value();

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument xmax"); return retval; }
  _arg_xmax = (float)args(2).double_value();

  if (_n_dims(args(3)) > 0)
    { error("dimension mismatch in argument ymin"); return retval; }
  _arg_ymin = (float)args(3).double_value();

  if (_n_dims(args(4)) > 0)
    { error("dimension mismatch in argument ymax"); return retval; }
  _arg_ymax = (float)args(4).double_value();

  if (_n_dims(args(5)) > 0)
    { error("dimension mismatch in argument aspect"); return retval; }
  _arg_aspect = (float)args(5).double_value();

    c_plvpas(_arg_xmin, _arg_xmax, _arg_ymin, _arg_ymax, _arg_aspect);
  return retval;
}

octave_value_list _wrap_plvpor(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 4)
  {
    print_usage("plvpor");
    return retval;
  }

  float _arg_xmin;
  float _arg_xmax;
  float _arg_ymin;
  float _arg_ymax;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument xmin"); return retval; }
  _arg_xmin = (float)args(1).double_value();

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument xmax"); return retval; }
  _arg_xmax = (float)args(2).double_value();

  if (_n_dims(args(3)) > 0)
    { error("dimension mismatch in argument ymin"); return retval; }
  _arg_ymin = (float)args(3).double_value();

  if (_n_dims(args(4)) > 0)
    { error("dimension mismatch in argument ymax"); return retval; }
  _arg_ymax = (float)args(4).double_value();

    c_plvpor(_arg_xmin, _arg_xmax, _arg_ymin, _arg_ymax);
  return retval;
}

octave_value_list _wrap_plvsta(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 0)
  {
    print_usage("plvsta");
    return retval;
  }

    c_plvsta();
  return retval;
}

octave_value_list _wrap_plw3d(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 11)
  {
    print_usage("plw3d");
    return retval;
  }

  float _arg_basex;
  float _arg_basey;
  float _arg_height;
  float _arg_xmin0;
  float _arg_xmax0;
  float _arg_ymin0;
  float _arg_ymax0;
  float _arg_zmin0;
  float _arg_zmax0;
  float _arg_alt;
  float _arg_az;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument basex"); return retval; }
  _arg_basex = (float)args(1).double_value();

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument basey"); return retval; }
  _arg_basey = (float)args(2).double_value();

  if (_n_dims(args(3)) > 0)
    { error("dimension mismatch in argument height"); return retval; }
  _arg_height = (float)args(3).double_value();

  if (_n_dims(args(4)) > 0)
    { error("dimension mismatch in argument xmin0"); return retval; }
  _arg_xmin0 = (float)args(4).double_value();

  if (_n_dims(args(5)) > 0)
    { error("dimension mismatch in argument xmax0"); return retval; }
  _arg_xmax0 = (float)args(5).double_value();

  if (_n_dims(args(6)) > 0)
    { error("dimension mismatch in argument ymin0"); return retval; }
  _arg_ymin0 = (float)args(6).double_value();

  if (_n_dims(args(7)) > 0)
    { error("dimension mismatch in argument ymax0"); return retval; }
  _arg_ymax0 = (float)args(7).double_value();

  if (_n_dims(args(8)) > 0)
    { error("dimension mismatch in argument zmin0"); return retval; }
  _arg_zmin0 = (float)args(8).double_value();

  if (_n_dims(args(9)) > 0)
    { error("dimension mismatch in argument zmax0"); return retval; }
  _arg_zmax0 = (float)args(9).double_value();

  if (_n_dims(args(10)) > 0)
    { error("dimension mismatch in argument alt"); return retval; }
  _arg_alt = (float)args(10).double_value();

  if (_n_dims(args(11)) > 0)
    { error("dimension mismatch in argument az"); return retval; }
  _arg_az = (float)args(11).double_value();

    c_plw3d(_arg_basex, _arg_basey, _arg_height, _arg_xmin0, _arg_xmax0, _arg_ymin0, _arg_ymax0, _arg_zmin0, _arg_zmax0, _arg_alt, _arg_az);
  return retval;
}

octave_value_list _wrap_plwid(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 1)
  {
    print_usage("plwid");
    return retval;
  }

  int _arg_width;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument width"); return retval; }
  _arg_width = (int)args(1).double_value();

    c_plwid(_arg_width);
  return retval;
}

octave_value_list _wrap_plwind(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 4)
  {
    print_usage("plwind");
    return retval;
  }

  float _arg_xmin;
  float _arg_xmax;
  float _arg_ymin;
  float _arg_ymax;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument xmin"); return retval; }
  _arg_xmin = (float)args(1).double_value();

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument xmax"); return retval; }
  _arg_xmax = (float)args(2).double_value();

  if (_n_dims(args(3)) > 0)
    { error("dimension mismatch in argument ymin"); return retval; }
  _arg_ymin = (float)args(3).double_value();

  if (_n_dims(args(4)) > 0)
    { error("dimension mismatch in argument ymax"); return retval; }
  _arg_ymax = (float)args(4).double_value();

    c_plwind(_arg_xmin, _arg_xmax, _arg_ymin, _arg_ymax);
  return retval;
}

octave_value_list _wrap_plxormod(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 1)
  {
    print_usage("plxormod");
    return retval;
  }

  int _arg_icol0;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument icol0"); return retval; }
  _arg_icol0 = (int)args(1).double_value();

    c_plxormod(_arg_icol0);
  return retval;
}

octave_value_list _wrap_plGetCursor(const octave_value_list &args, int nargout)
{
  octave_value_list retval(11, octave_value());
  if (args.length()-1 != 0 ||
      nargout != 11)
  {
    print_usage("plGetCursor");
    return retval;
  }

  int _arg_state;
  int _arg_keysym;
  int _arg_button;
  char *_arg_string;
  int _arg_pX;
  int _arg_pY;
  float _arg_dX;
  float _arg_dY;
  float _arg_wX;
  float _arg_wY;
  int _arg_retval;
  retval(4) = octave_value(charMatrix(1, 1), true);
  _arg_string = (char *)retval(4).char_matrix_value().data();
    _arg_retval = (int)
      my_plGetCursor(&_arg_state, &_arg_keysym, &_arg_button, _arg_string, &_arg_pX, &_arg_pY, &_arg_dX, &_arg_dY, &_arg_wX, &_arg_wY);
  retval(0) = octave_value((double)_arg_retval);
  retval(1) = octave_value((double)_arg_state);
  retval(2) = octave_value((double)_arg_keysym);
  retval(3) = octave_value((double)_arg_button);
  retval(5) = octave_value((double)_arg_pX);
  retval(6) = octave_value((double)_arg_pY);
  retval(7) = octave_value((double)_arg_dX);
  retval(8) = octave_value((double)_arg_dY);
  retval(9) = octave_value((double)_arg_wX);
  retval(10) = octave_value((double)_arg_wY);
  return retval;
}

octave_value_list _wrap_plTranslateCursor(const octave_value_list &args, int nargout)
{
  octave_value_list retval(3, octave_value());
  if (args.length()-1 != 2 ||
      nargout != 3)
  {
    print_usage("plTranslateCursor");
    return retval;
  }

  int _d[2] = { 1,1 };
  int _vec_n = 0;
  int _vidx;
  int _vec_sz;
  float *_arg_x;
  float *_arg_y;
  float *_arg_x_in;
  float *_arg_y_in;
  int *_arg_retval;
  int _vecstride_x_in = 1;
  int _vecstride_y_in = 1;
  if (!_check_input_vectorize(args(1), &_vec_n, _d, 0, &_vecstride_x_in))
    { error("dimension mismatch in argument x_in"); return retval; }
  Matrix _x_in = args(1).matrix_value();
  _arg_x_in = (float *)alloca(_x_in.dim1() * _x_in.dim2() * sizeof (float));
  _cvt_double_to(_arg_x_in, &_x_in(0,0), _x_in.dim1()*_x_in.dim2());

  if (!_check_input_vectorize(args(2), &_vec_n, _d, 0, &_vecstride_y_in))
    { error("dimension mismatch in argument y_in"); return retval; }
  Matrix _y_in = args(2).matrix_value();
  _arg_y_in = (float *)alloca(_y_in.dim1() * _y_in.dim2() * sizeof (float));
  _cvt_double_to(_arg_y_in, &_y_in(0,0), _y_in.dim1()*_y_in.dim2());

  if (_d[0]  != 1 || _d[1]  != 1)
    retval(0) = octave_value(Matrix(_d[0] , _d[1] ));
  _arg_retval = (int *)alloca(_d[0]  * _d[1]  * sizeof (int));
  if (_d[0]  != 1 || _d[1]  != 1)
    retval(1) = octave_value(Matrix(_d[0] , _d[1] ));
  _arg_x = (float *)alloca(_d[0]  * _d[1]  * sizeof (float));
  if (_d[0]  != 1 || _d[1]  != 1)
    retval(2) = octave_value(Matrix(_d[0] , _d[1] ));
  _arg_y = (float *)alloca(_d[0]  * _d[1]  * sizeof (float));
  _vec_sz = _d[0]*_d[1];
  for (_vidx = 0; _vidx < _vec_sz; ++_vidx) {
    _arg_retval[_vidx] = (int)
      my_plTranslateCursor(&_arg_x[_vidx], &_arg_y[_vidx], _arg_x_in[_vecstride_x_in*_vidx], _arg_y_in[_vecstride_y_in*_vidx]);
  }
  if (_arraylen(retval(0)) == 1)
    retval(0) = octave_value(double(*_arg_retval));
  else
    _cvt_to_double(_arg_retval, (double *)retval(0).matrix_value().data(), _arraylen(retval(0)));
  if (_arraylen(retval(1)) == 1)
    retval(1) = octave_value(double(*_arg_x));
  else
    _cvt_to_double(_arg_x, (double *)retval(1).matrix_value().data(), _arraylen(retval(1)));
  if (_arraylen(retval(2)) == 1)
    retval(2) = octave_value(double(*_arg_y));
  else
    _cvt_to_double(_arg_y, (double *)retval(2).matrix_value().data(), _arraylen(retval(2)));
  return retval;
}

octave_value_list _wrap_plcol(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 1)
  {
    print_usage("plcol");
    return retval;
  }

  int _arg_icol0;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument icol0"); return retval; }
  _arg_icol0 = (int)args(1).double_value();

    my_plcol(_arg_icol0);
  return retval;
}

octave_value_list _wrap_plcont(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 7)
  {
    print_usage("plcont");
    return retval;
  }

  float *_arg_f;
  int _arg_nx;
  int _arg_ny;
  int _arg_kx;
  int _arg_lx;
  int _arg_ky;
  int _arg_ly;
  float *_arg_clevel;
  int _arg_nlevel;
  float *_arg_tr;
  _arg_nlevel = (_dim(args(6), 0));
  _arg_nx = (_dim(args(1), 0));
  _arg_ny = (_dim(args(1), 1));
  if (_n_dims(args(1)) > 2)
    { error("dimension mismatch in argument f"); return retval; }
  Matrix _f = args(1).matrix_value();
  _arg_f = (float *)alloca(_f.dim1() * _f.dim2() * sizeof (float));
  _cvt_double_to(_arg_f, &_f(0,0), _f.dim1()*_f.dim2());

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument kx"); return retval; }
  _arg_kx = (int)args(2).double_value();

  if (_n_dims(args(3)) > 0)
    { error("dimension mismatch in argument lx"); return retval; }
  _arg_lx = (int)args(3).double_value();

  if (_n_dims(args(4)) > 0)
    { error("dimension mismatch in argument ky"); return retval; }
  _arg_ky = (int)args(4).double_value();

  if (_n_dims(args(5)) > 0)
    { error("dimension mismatch in argument ly"); return retval; }
  _arg_ly = (int)args(5).double_value();

  if (_n_dims(args(6)) > 1)
    { error("dimension mismatch in argument clevel"); return retval; }
  Matrix _clevel = args(6).matrix_value();
  _arg_clevel = (float *)alloca(_clevel.dim1() * _clevel.dim2() * sizeof (float));
  _cvt_double_to(_arg_clevel, &_clevel(0,0), _clevel.dim1()*_clevel.dim2());

  if (  (int)(_dim(args(7), 0)) != (int)(6) ||
      _n_dims(args(7)) > 1)
    { error("dimension mismatch in argument tr"); return retval; }
  Matrix _tr = args(7).matrix_value();
  _arg_tr = (float *)alloca(_tr.dim1() * _tr.dim2() * sizeof (float));
  _cvt_double_to(_arg_tr, &_tr(0,0), _tr.dim1()*_tr.dim2());

    my_plcont(_arg_f, _arg_nx, _arg_ny, _arg_kx, _arg_lx, _arg_ky, _arg_ly, _arg_clevel, _arg_nlevel, _arg_tr);
  return retval;
}

octave_value_list _wrap_plcont0(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 6)
  {
    print_usage("plcont0");
    return retval;
  }

  float *_arg_f;
  int _arg_nx;
  int _arg_ny;
  int _arg_kx;
  int _arg_lx;
  int _arg_ky;
  int _arg_ly;
  float *_arg_clevel;
  int _arg_nlevel;
  _arg_nlevel = (_dim(args(6), 0));
  _arg_nx = (_dim(args(1), 0));
  _arg_ny = (_dim(args(1), 1));
  if (_n_dims(args(1)) > 2)
    { error("dimension mismatch in argument f"); return retval; }
  Matrix _f = args(1).matrix_value();
  _arg_f = (float *)alloca(_f.dim1() * _f.dim2() * sizeof (float));
  _cvt_double_to(_arg_f, &_f(0,0), _f.dim1()*_f.dim2());

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument kx"); return retval; }
  _arg_kx = (int)args(2).double_value();

  if (_n_dims(args(3)) > 0)
    { error("dimension mismatch in argument lx"); return retval; }
  _arg_lx = (int)args(3).double_value();

  if (_n_dims(args(4)) > 0)
    { error("dimension mismatch in argument ky"); return retval; }
  _arg_ky = (int)args(4).double_value();

  if (_n_dims(args(5)) > 0)
    { error("dimension mismatch in argument ly"); return retval; }
  _arg_ly = (int)args(5).double_value();

  if (_n_dims(args(6)) > 1)
    { error("dimension mismatch in argument clevel"); return retval; }
  Matrix _clevel = args(6).matrix_value();
  _arg_clevel = (float *)alloca(_clevel.dim1() * _clevel.dim2() * sizeof (float));
  _cvt_double_to(_arg_clevel, &_clevel(0,0), _clevel.dim1()*_clevel.dim2());

    my_plcont0(_arg_f, _arg_nx, _arg_ny, _arg_kx, _arg_lx, _arg_ky, _arg_ly, _arg_clevel, _arg_nlevel);
  return retval;
}

octave_value_list _wrap_plcont1(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 8)
  {
    print_usage("plcont1");
    return retval;
  }

  float *_arg_f;
  int _arg_nx;
  int _arg_ny;
  int _arg_kx;
  int _arg_lx;
  int _arg_ky;
  int _arg_ly;
  float *_arg_clevel;
  int _arg_nlevel;
  float *_arg_xg;
  float *_arg_yg;
  _arg_nlevel = (_dim(args(6), 0));
  _arg_nx = (_dim(args(7), 0));
  _arg_ny = (_dim(args(8), 0));
  if (  (int)(_dim(args(1), 0)) != (int)(_arg_nx) ||
        (int)(_dim(args(1), 1)) != (int)(_arg_ny) ||
      _n_dims(args(1)) > 2)
    { error("dimension mismatch in argument f"); return retval; }
  Matrix _f = args(1).matrix_value();
  _arg_f = (float *)alloca(_f.dim1() * _f.dim2() * sizeof (float));
  _cvt_double_to(_arg_f, &_f(0,0), _f.dim1()*_f.dim2());

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument kx"); return retval; }
  _arg_kx = (int)args(2).double_value();

  if (_n_dims(args(3)) > 0)
    { error("dimension mismatch in argument lx"); return retval; }
  _arg_lx = (int)args(3).double_value();

  if (_n_dims(args(4)) > 0)
    { error("dimension mismatch in argument ky"); return retval; }
  _arg_ky = (int)args(4).double_value();

  if (_n_dims(args(5)) > 0)
    { error("dimension mismatch in argument ly"); return retval; }
  _arg_ly = (int)args(5).double_value();

  if (_n_dims(args(6)) > 1)
    { error("dimension mismatch in argument clevel"); return retval; }
  Matrix _clevel = args(6).matrix_value();
  _arg_clevel = (float *)alloca(_clevel.dim1() * _clevel.dim2() * sizeof (float));
  _cvt_double_to(_arg_clevel, &_clevel(0,0), _clevel.dim1()*_clevel.dim2());

  if (_n_dims(args(7)) > 1)
    { error("dimension mismatch in argument xg"); return retval; }
  Matrix _xg = args(7).matrix_value();
  _arg_xg = (float *)alloca(_xg.dim1() * _xg.dim2() * sizeof (float));
  _cvt_double_to(_arg_xg, &_xg(0,0), _xg.dim1()*_xg.dim2());

  if (_n_dims(args(8)) > 1)
    { error("dimension mismatch in argument yg"); return retval; }
  Matrix _yg = args(8).matrix_value();
  _arg_yg = (float *)alloca(_yg.dim1() * _yg.dim2() * sizeof (float));
  _cvt_double_to(_arg_yg, &_yg(0,0), _yg.dim1()*_yg.dim2());

    my_plcont1(_arg_f, _arg_nx, _arg_ny, _arg_kx, _arg_lx, _arg_ky, _arg_ly, _arg_clevel, _arg_nlevel, _arg_xg, _arg_yg);
  return retval;
}

octave_value_list _wrap_plcont2(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 8)
  {
    print_usage("plcont2");
    return retval;
  }

  float *_arg_f;
  int _arg_nx;
  int _arg_ny;
  int _arg_kx;
  int _arg_lx;
  int _arg_ky;
  int _arg_ly;
  float *_arg_clevel;
  int _arg_nlevel;
  float *_arg_xg;
  float *_arg_yg;
  _arg_nlevel = (_dim(args(6), 0));
  _arg_nx = (_dim(args(8), 0));
  _arg_ny = (_dim(args(8), 1));
  if (  (int)(_dim(args(1), 0)) != (int)(_arg_nx) ||
        (int)(_dim(args(1), 1)) != (int)(_arg_ny) ||
      _n_dims(args(1)) > 2)
    { error("dimension mismatch in argument f"); return retval; }
  Matrix _f = args(1).matrix_value();
  _arg_f = (float *)alloca(_f.dim1() * _f.dim2() * sizeof (float));
  _cvt_double_to(_arg_f, &_f(0,0), _f.dim1()*_f.dim2());

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument kx"); return retval; }
  _arg_kx = (int)args(2).double_value();

  if (_n_dims(args(3)) > 0)
    { error("dimension mismatch in argument lx"); return retval; }
  _arg_lx = (int)args(3).double_value();

  if (_n_dims(args(4)) > 0)
    { error("dimension mismatch in argument ky"); return retval; }
  _arg_ky = (int)args(4).double_value();

  if (_n_dims(args(5)) > 0)
    { error("dimension mismatch in argument ly"); return retval; }
  _arg_ly = (int)args(5).double_value();

  if (_n_dims(args(6)) > 1)
    { error("dimension mismatch in argument clevel"); return retval; }
  Matrix _clevel = args(6).matrix_value();
  _arg_clevel = (float *)alloca(_clevel.dim1() * _clevel.dim2() * sizeof (float));
  _cvt_double_to(_arg_clevel, &_clevel(0,0), _clevel.dim1()*_clevel.dim2());

  if (  (int)(_dim(args(7), 0)) != (int)(_arg_nx) ||
        (int)(_dim(args(7), 1)) != (int)(_arg_ny) ||
      _n_dims(args(7)) > 2)
    { error("dimension mismatch in argument xg"); return retval; }
  Matrix _xg = args(7).matrix_value();
  _arg_xg = (float *)alloca(_xg.dim1() * _xg.dim2() * sizeof (float));
  _cvt_double_to(_arg_xg, &_xg(0,0), _xg.dim1()*_xg.dim2());

  if (_n_dims(args(8)) > 2)
    { error("dimension mismatch in argument yg"); return retval; }
  Matrix _yg = args(8).matrix_value();
  _arg_yg = (float *)alloca(_yg.dim1() * _yg.dim2() * sizeof (float));
  _cvt_double_to(_arg_yg, &_yg(0,0), _yg.dim1()*_yg.dim2());

    my_plcont2(_arg_f, _arg_nx, _arg_ny, _arg_kx, _arg_lx, _arg_ky, _arg_ly, _arg_clevel, _arg_nlevel, _arg_xg, _arg_yg);
  return retval;
}

octave_value_list _wrap_plcont2p(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 8)
  {
    print_usage("plcont2p");
    return retval;
  }

  float *_arg_f;
  int _arg_nx;
  int _arg_ny;
  int _arg_kx;
  int _arg_lx;
  int _arg_ky;
  int _arg_ly;
  float *_arg_clevel;
  int _arg_nlevel;
  float *_arg_xg;
  float *_arg_yg;
  _arg_nlevel = (_dim(args(6), 0));
  _arg_nx = (_dim(args(8), 0));
  _arg_ny = (_dim(args(8), 1));
  if (  (int)(_dim(args(1), 0)) != (int)(_arg_nx) ||
        (int)(_dim(args(1), 1)) != (int)(_arg_ny) ||
      _n_dims(args(1)) > 2)
    { error("dimension mismatch in argument f"); return retval; }
  Matrix _f = args(1).matrix_value();
  _arg_f = (float *)alloca(_f.dim1() * _f.dim2() * sizeof (float));
  _cvt_double_to(_arg_f, &_f(0,0), _f.dim1()*_f.dim2());

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument kx"); return retval; }
  _arg_kx = (int)args(2).double_value();

  if (_n_dims(args(3)) > 0)
    { error("dimension mismatch in argument lx"); return retval; }
  _arg_lx = (int)args(3).double_value();

  if (_n_dims(args(4)) > 0)
    { error("dimension mismatch in argument ky"); return retval; }
  _arg_ky = (int)args(4).double_value();

  if (_n_dims(args(5)) > 0)
    { error("dimension mismatch in argument ly"); return retval; }
  _arg_ly = (int)args(5).double_value();

  if (_n_dims(args(6)) > 1)
    { error("dimension mismatch in argument clevel"); return retval; }
  Matrix _clevel = args(6).matrix_value();
  _arg_clevel = (float *)alloca(_clevel.dim1() * _clevel.dim2() * sizeof (float));
  _cvt_double_to(_arg_clevel, &_clevel(0,0), _clevel.dim1()*_clevel.dim2());

  if (  (int)(_dim(args(7), 0)) != (int)(_arg_nx) ||
        (int)(_dim(args(7), 1)) != (int)(_arg_ny) ||
      _n_dims(args(7)) > 2)
    { error("dimension mismatch in argument xg"); return retval; }
  Matrix _xg = args(7).matrix_value();
  _arg_xg = (float *)alloca(_xg.dim1() * _xg.dim2() * sizeof (float));
  _cvt_double_to(_arg_xg, &_xg(0,0), _xg.dim1()*_xg.dim2());

  if (_n_dims(args(8)) > 2)
    { error("dimension mismatch in argument yg"); return retval; }
  Matrix _yg = args(8).matrix_value();
  _arg_yg = (float *)alloca(_yg.dim1() * _yg.dim2() * sizeof (float));
  _cvt_double_to(_arg_yg, &_yg(0,0), _yg.dim1()*_yg.dim2());

    my_plcont2p(_arg_f, _arg_nx, _arg_ny, _arg_kx, _arg_lx, _arg_ky, _arg_ly, _arg_clevel, _arg_nlevel, _arg_xg, _arg_yg);
  return retval;
}

octave_value_list _wrap_plmesh(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 4)
  {
    print_usage("plmesh");
    return retval;
  }

  float *_arg_x;
  float *_arg_y;
  float *_arg_z;
  int _arg_nx;
  int _arg_ny;
  int _arg_opt;
  _arg_nx = (_dim(args(3), 0));
  _arg_ny = (_dim(args(3), 1));
  if (  (int)(_dim(args(1), 0)) != (int)(_arg_nx) ||
      _n_dims(args(1)) > 1)
    { error("dimension mismatch in argument x"); return retval; }
  Matrix _x = args(1).matrix_value();
  _arg_x = (float *)alloca(_x.dim1() * _x.dim2() * sizeof (float));
  _cvt_double_to(_arg_x, &_x(0,0), _x.dim1()*_x.dim2());

  if (  (int)(_dim(args(2), 0)) != (int)(_arg_ny) ||
      _n_dims(args(2)) > 1)
    { error("dimension mismatch in argument y"); return retval; }
  Matrix _y = args(2).matrix_value();
  _arg_y = (float *)alloca(_y.dim1() * _y.dim2() * sizeof (float));
  _cvt_double_to(_arg_y, &_y(0,0), _y.dim1()*_y.dim2());

  if (_n_dims(args(3)) > 2)
    { error("dimension mismatch in argument z"); return retval; }
  Matrix _z = args(3).matrix_value();
  _arg_z = (float *)alloca(_z.dim1() * _z.dim2() * sizeof (float));
  _cvt_double_to(_arg_z, &_z(0,0), _z.dim1()*_z.dim2());

  if (_n_dims(args(4)) > 0)
    { error("dimension mismatch in argument opt"); return retval; }
  _arg_opt = (int)args(4).double_value();

    my_plmesh(_arg_x, _arg_y, _arg_z, _arg_nx, _arg_ny, _arg_opt);
  return retval;
}

octave_value_list _wrap_plot3d(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 5)
  {
    print_usage("plot3d");
    return retval;
  }

  float *_arg_x;
  float *_arg_y;
  float *_arg_z;
  int _arg_nx;
  int _arg_ny;
  int _arg_opt;
  int _arg_side;
  _arg_nx = (_dim(args(3), 0));
  _arg_ny = (_dim(args(3), 1));
  if (  (int)(_dim(args(1), 0)) != (int)(_arg_nx) ||
      _n_dims(args(1)) > 1)
    { error("dimension mismatch in argument x"); return retval; }
  Matrix _x = args(1).matrix_value();
  _arg_x = (float *)alloca(_x.dim1() * _x.dim2() * sizeof (float));
  _cvt_double_to(_arg_x, &_x(0,0), _x.dim1()*_x.dim2());

  if (  (int)(_dim(args(2), 0)) != (int)(_arg_ny) ||
      _n_dims(args(2)) > 1)
    { error("dimension mismatch in argument y"); return retval; }
  Matrix _y = args(2).matrix_value();
  _arg_y = (float *)alloca(_y.dim1() * _y.dim2() * sizeof (float));
  _cvt_double_to(_arg_y, &_y(0,0), _y.dim1()*_y.dim2());

  if (_n_dims(args(3)) > 2)
    { error("dimension mismatch in argument z"); return retval; }
  Matrix _z = args(3).matrix_value();
  _arg_z = (float *)alloca(_z.dim1() * _z.dim2() * sizeof (float));
  _cvt_double_to(_arg_z, &_z(0,0), _z.dim1()*_z.dim2());

  if (_n_dims(args(4)) > 0)
    { error("dimension mismatch in argument opt"); return retval; }
  _arg_opt = (int)args(4).double_value();

  if (_n_dims(args(5)) > 0)
    { error("dimension mismatch in argument side"); return retval; }
  _arg_side = (int)args(5).double_value();

    my_plot3d(_arg_x, _arg_y, _arg_z, _arg_nx, _arg_ny, _arg_opt, _arg_side);
  return retval;
}

octave_value_list _wrap_plotsh3d(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 4)
  {
    print_usage("plotsh3d");
    return retval;
  }

  float *_arg_x;
  float *_arg_y;
  float *_arg_z;
  int _arg_nx;
  int _arg_ny;
  int _arg_side;
  _arg_nx = (_dim(args(3), 0));
  _arg_ny = (_dim(args(3), 1));
  if (  (int)(_dim(args(1), 0)) != (int)(_arg_nx) ||
      _n_dims(args(1)) > 1)
    { error("dimension mismatch in argument x"); return retval; }
  Matrix _x = args(1).matrix_value();
  _arg_x = (float *)alloca(_x.dim1() * _x.dim2() * sizeof (float));
  _cvt_double_to(_arg_x, &_x(0,0), _x.dim1()*_x.dim2());

  if (  (int)(_dim(args(2), 0)) != (int)(_arg_ny) ||
      _n_dims(args(2)) > 1)
    { error("dimension mismatch in argument y"); return retval; }
  Matrix _y = args(2).matrix_value();
  _arg_y = (float *)alloca(_y.dim1() * _y.dim2() * sizeof (float));
  _cvt_double_to(_arg_y, &_y(0,0), _y.dim1()*_y.dim2());

  if (_n_dims(args(3)) > 2)
    { error("dimension mismatch in argument z"); return retval; }
  Matrix _z = args(3).matrix_value();
  _arg_z = (float *)alloca(_z.dim1() * _z.dim2() * sizeof (float));
  _cvt_double_to(_arg_z, &_z(0,0), _z.dim1()*_z.dim2());

  if (_n_dims(args(4)) > 0)
    { error("dimension mismatch in argument side"); return retval; }
  _arg_side = (int)args(4).double_value();

    my_plotsh3d(_arg_x, _arg_y, _arg_z, _arg_nx, _arg_ny, _arg_side);
  return retval;
}

octave_value_list _wrap_plpoly3(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 5)
  {
    print_usage("plpoly3");
    return retval;
  }

  int _arg_n;
  float *_arg_x;
  float *_arg_y;
  float *_arg_z;
  int *_arg_draw;
  int _arg_clockwise;
  _arg_n = (_dim(args(3), 0));
  if (  (int)(_dim(args(1), 0)) != (int)(_arg_n) ||
      _n_dims(args(1)) > 1)
    { error("dimension mismatch in argument x"); return retval; }
  Matrix _x = args(1).matrix_value();
  _arg_x = (float *)alloca(_x.dim1() * _x.dim2() * sizeof (float));
  _cvt_double_to(_arg_x, &_x(0,0), _x.dim1()*_x.dim2());

  if (  (int)(_dim(args(2), 0)) != (int)(_arg_n) ||
      _n_dims(args(2)) > 1)
    { error("dimension mismatch in argument y"); return retval; }
  Matrix _y = args(2).matrix_value();
  _arg_y = (float *)alloca(_y.dim1() * _y.dim2() * sizeof (float));
  _cvt_double_to(_arg_y, &_y(0,0), _y.dim1()*_y.dim2());

  if (_n_dims(args(3)) > 1)
    { error("dimension mismatch in argument z"); return retval; }
  Matrix _z = args(3).matrix_value();
  _arg_z = (float *)alloca(_z.dim1() * _z.dim2() * sizeof (float));
  _cvt_double_to(_arg_z, &_z(0,0), _z.dim1()*_z.dim2());

  if (  (int)(_dim(args(4), 0)) != (int)(4) ||
      _n_dims(args(4)) > 1)
    { error("dimension mismatch in argument draw"); return retval; }
  Matrix _draw = args(4).matrix_value();
  _arg_draw = (int *)alloca(_draw.dim1() * _draw.dim2() * sizeof (int));
  _cvt_double_to(_arg_draw, &_draw(0,0), _draw.dim1()*_draw.dim2());

  if (_n_dims(args(5)) > 0)
    { error("dimension mismatch in argument clockwise"); return retval; }
  _arg_clockwise = (int)args(5).double_value();

    my_plpoly3(_arg_n, _arg_x, _arg_y, _arg_z, _arg_draw, _arg_clockwise);
  return retval;
}

octave_value_list _wrap_plshade(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 17)
  {
    print_usage("plshade");
    return retval;
  }

  float *_arg_a;
  int _arg_nx;
  int _arg_ny;
  char * _arg_defined;
  float _arg_left;
  float _arg_right;
  float _arg_bottom;
  float _arg_top;
  float _arg_shade_min;
  float _arg_shade_max;
  int _arg_sh_cmap;
  float _arg_sh_color;
  int _arg_sh_width;
  int _arg_min_color;
  int _arg_min_width;
  int _arg_max_color;
  int _arg_max_width;
  int _arg_rectangular;
  float *_arg_tr;
  _arg_nx = (_dim(args(1), 0));
  _arg_ny = (_dim(args(1), 1));
  if (_n_dims(args(1)) > 2)
    { error("dimension mismatch in argument a"); return retval; }
  Matrix _a = args(1).matrix_value();
  _arg_a = (float *)alloca(_a.dim1() * _a.dim2() * sizeof (float));
  _cvt_double_to(_arg_a, &_a(0,0), _a.dim1()*_a.dim2());

  if (0 > 0)
    { error("dimension mismatch in argument defined"); return retval; }
  string _defined = args(2).string_value();
  _defined += '\0';
  _arg_defined = (char *)_defined.data();

  if (_n_dims(args(3)) > 0)
    { error("dimension mismatch in argument left"); return retval; }
  _arg_left = (float)args(3).double_value();

  if (_n_dims(args(4)) > 0)
    { error("dimension mismatch in argument right"); return retval; }
  _arg_right = (float)args(4).double_value();

  if (_n_dims(args(5)) > 0)
    { error("dimension mismatch in argument bottom"); return retval; }
  _arg_bottom = (float)args(5).double_value();

  if (_n_dims(args(6)) > 0)
    { error("dimension mismatch in argument top"); return retval; }
  _arg_top = (float)args(6).double_value();

  if (_n_dims(args(7)) > 0)
    { error("dimension mismatch in argument shade_min"); return retval; }
  _arg_shade_min = (float)args(7).double_value();

  if (_n_dims(args(8)) > 0)
    { error("dimension mismatch in argument shade_max"); return retval; }
  _arg_shade_max = (float)args(8).double_value();

  if (_n_dims(args(9)) > 0)
    { error("dimension mismatch in argument sh_cmap"); return retval; }
  _arg_sh_cmap = (int)args(9).double_value();

  if (_n_dims(args(10)) > 0)
    { error("dimension mismatch in argument sh_color"); return retval; }
  _arg_sh_color = (float)args(10).double_value();

  if (_n_dims(args(11)) > 0)
    { error("dimension mismatch in argument sh_width"); return retval; }
  _arg_sh_width = (int)args(11).double_value();

  if (_n_dims(args(12)) > 0)
    { error("dimension mismatch in argument min_color"); return retval; }
  _arg_min_color = (int)args(12).double_value();

  if (_n_dims(args(13)) > 0)
    { error("dimension mismatch in argument min_width"); return retval; }
  _arg_min_width = (int)args(13).double_value();

  if (_n_dims(args(14)) > 0)
    { error("dimension mismatch in argument max_color"); return retval; }
  _arg_max_color = (int)args(14).double_value();

  if (_n_dims(args(15)) > 0)
    { error("dimension mismatch in argument max_width"); return retval; }
  _arg_max_width = (int)args(15).double_value();

  if (_n_dims(args(16)) > 0)
    { error("dimension mismatch in argument rectangular"); return retval; }
  _arg_rectangular = (int)args(16).double_value();

  if (  (int)(_dim(args(17), 0)) != (int)(6) ||
      _n_dims(args(17)) > 1)
    { error("dimension mismatch in argument tr"); return retval; }
  Matrix _tr = args(17).matrix_value();
  _arg_tr = (float *)alloca(_tr.dim1() * _tr.dim2() * sizeof (float));
  _cvt_double_to(_arg_tr, &_tr(0,0), _tr.dim1()*_tr.dim2());

    my_plshade(_arg_a, _arg_nx, _arg_ny, _arg_defined, _arg_left, _arg_right, _arg_bottom, _arg_top, _arg_shade_min, _arg_shade_max, _arg_sh_cmap, _arg_sh_color, _arg_sh_width, _arg_min_color, _arg_min_width, _arg_max_color, _arg_max_width, _arg_rectangular, _arg_tr);
  return retval;
}

octave_value_list _wrap_plshade1(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 18)
  {
    print_usage("plshade1");
    return retval;
  }

  float *_arg_a;
  int _arg_nx;
  int _arg_ny;
  char * _arg_defined;
  float _arg_left;
  float _arg_right;
  float _arg_bottom;
  float _arg_top;
  float _arg_shade_min;
  float _arg_shade_max;
  int _arg_sh_cmap;
  float _arg_sh_color;
  int _arg_sh_width;
  int _arg_min_color;
  int _arg_min_width;
  int _arg_max_color;
  int _arg_max_width;
  int _arg_rectangular;
  float *_arg_xg;
  float *_arg_yg;
  _arg_nx = (_dim(args(17), 0));
  _arg_ny = (_dim(args(18), 0));
  if (  (int)(_dim(args(1), 0)) != (int)(_arg_nx) ||
        (int)(_dim(args(1), 1)) != (int)(_arg_ny) ||
      _n_dims(args(1)) > 2)
    { error("dimension mismatch in argument a"); return retval; }
  Matrix _a = args(1).matrix_value();
  _arg_a = (float *)alloca(_a.dim1() * _a.dim2() * sizeof (float));
  _cvt_double_to(_arg_a, &_a(0,0), _a.dim1()*_a.dim2());

  if (0 > 0)
    { error("dimension mismatch in argument defined"); return retval; }
  string _defined = args(2).string_value();
  _defined += '\0';
  _arg_defined = (char *)_defined.data();

  if (_n_dims(args(3)) > 0)
    { error("dimension mismatch in argument left"); return retval; }
  _arg_left = (float)args(3).double_value();

  if (_n_dims(args(4)) > 0)
    { error("dimension mismatch in argument right"); return retval; }
  _arg_right = (float)args(4).double_value();

  if (_n_dims(args(5)) > 0)
    { error("dimension mismatch in argument bottom"); return retval; }
  _arg_bottom = (float)args(5).double_value();

  if (_n_dims(args(6)) > 0)
    { error("dimension mismatch in argument top"); return retval; }
  _arg_top = (float)args(6).double_value();

  if (_n_dims(args(7)) > 0)
    { error("dimension mismatch in argument shade_min"); return retval; }
  _arg_shade_min = (float)args(7).double_value();

  if (_n_dims(args(8)) > 0)
    { error("dimension mismatch in argument shade_max"); return retval; }
  _arg_shade_max = (float)args(8).double_value();

  if (_n_dims(args(9)) > 0)
    { error("dimension mismatch in argument sh_cmap"); return retval; }
  _arg_sh_cmap = (int)args(9).double_value();

  if (_n_dims(args(10)) > 0)
    { error("dimension mismatch in argument sh_color"); return retval; }
  _arg_sh_color = (float)args(10).double_value();

  if (_n_dims(args(11)) > 0)
    { error("dimension mismatch in argument sh_width"); return retval; }
  _arg_sh_width = (int)args(11).double_value();

  if (_n_dims(args(12)) > 0)
    { error("dimension mismatch in argument min_color"); return retval; }
  _arg_min_color = (int)args(12).double_value();

  if (_n_dims(args(13)) > 0)
    { error("dimension mismatch in argument min_width"); return retval; }
  _arg_min_width = (int)args(13).double_value();

  if (_n_dims(args(14)) > 0)
    { error("dimension mismatch in argument max_color"); return retval; }
  _arg_max_color = (int)args(14).double_value();

  if (_n_dims(args(15)) > 0)
    { error("dimension mismatch in argument max_width"); return retval; }
  _arg_max_width = (int)args(15).double_value();

  if (_n_dims(args(16)) > 0)
    { error("dimension mismatch in argument rectangular"); return retval; }
  _arg_rectangular = (int)args(16).double_value();

  if (_n_dims(args(17)) > 1)
    { error("dimension mismatch in argument xg"); return retval; }
  Matrix _xg = args(17).matrix_value();
  _arg_xg = (float *)alloca(_xg.dim1() * _xg.dim2() * sizeof (float));
  _cvt_double_to(_arg_xg, &_xg(0,0), _xg.dim1()*_xg.dim2());

  if (_n_dims(args(18)) > 1)
    { error("dimension mismatch in argument yg"); return retval; }
  Matrix _yg = args(18).matrix_value();
  _arg_yg = (float *)alloca(_yg.dim1() * _yg.dim2() * sizeof (float));
  _cvt_double_to(_arg_yg, &_yg(0,0), _yg.dim1()*_yg.dim2());

    my_plshade1(_arg_a, _arg_nx, _arg_ny, _arg_defined, _arg_left, _arg_right, _arg_bottom, _arg_top, _arg_shade_min, _arg_shade_max, _arg_sh_cmap, _arg_sh_color, _arg_sh_width, _arg_min_color, _arg_min_width, _arg_max_color, _arg_max_width, _arg_rectangular, _arg_xg, _arg_yg);
  return retval;
}

octave_value_list _wrap_plshade2(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 18)
  {
    print_usage("plshade2");
    return retval;
  }

  float *_arg_a;
  int _arg_nx;
  int _arg_ny;
  char * _arg_defined;
  float _arg_left;
  float _arg_right;
  float _arg_bottom;
  float _arg_top;
  float _arg_shade_min;
  float _arg_shade_max;
  int _arg_sh_cmap;
  float _arg_sh_color;
  int _arg_sh_width;
  int _arg_min_color;
  int _arg_min_width;
  int _arg_max_color;
  int _arg_max_width;
  int _arg_rectangular;
  float *_arg_xg;
  float *_arg_yg;
  _arg_nx = (_dim(args(18), 0));
  _arg_ny = (_dim(args(18), 1));
  if (  (int)(_dim(args(1), 0)) != (int)(_arg_nx) ||
        (int)(_dim(args(1), 1)) != (int)(_arg_ny) ||
      _n_dims(args(1)) > 2)
    { error("dimension mismatch in argument a"); return retval; }
  Matrix _a = args(1).matrix_value();
  _arg_a = (float *)alloca(_a.dim1() * _a.dim2() * sizeof (float));
  _cvt_double_to(_arg_a, &_a(0,0), _a.dim1()*_a.dim2());

  if (0 > 0)
    { error("dimension mismatch in argument defined"); return retval; }
  string _defined = args(2).string_value();
  _defined += '\0';
  _arg_defined = (char *)_defined.data();

  if (_n_dims(args(3)) > 0)
    { error("dimension mismatch in argument left"); return retval; }
  _arg_left = (float)args(3).double_value();

  if (_n_dims(args(4)) > 0)
    { error("dimension mismatch in argument right"); return retval; }
  _arg_right = (float)args(4).double_value();

  if (_n_dims(args(5)) > 0)
    { error("dimension mismatch in argument bottom"); return retval; }
  _arg_bottom = (float)args(5).double_value();

  if (_n_dims(args(6)) > 0)
    { error("dimension mismatch in argument top"); return retval; }
  _arg_top = (float)args(6).double_value();

  if (_n_dims(args(7)) > 0)
    { error("dimension mismatch in argument shade_min"); return retval; }
  _arg_shade_min = (float)args(7).double_value();

  if (_n_dims(args(8)) > 0)
    { error("dimension mismatch in argument shade_max"); return retval; }
  _arg_shade_max = (float)args(8).double_value();

  if (_n_dims(args(9)) > 0)
    { error("dimension mismatch in argument sh_cmap"); return retval; }
  _arg_sh_cmap = (int)args(9).double_value();

  if (_n_dims(args(10)) > 0)
    { error("dimension mismatch in argument sh_color"); return retval; }
  _arg_sh_color = (float)args(10).double_value();

  if (_n_dims(args(11)) > 0)
    { error("dimension mismatch in argument sh_width"); return retval; }
  _arg_sh_width = (int)args(11).double_value();

  if (_n_dims(args(12)) > 0)
    { error("dimension mismatch in argument min_color"); return retval; }
  _arg_min_color = (int)args(12).double_value();

  if (_n_dims(args(13)) > 0)
    { error("dimension mismatch in argument min_width"); return retval; }
  _arg_min_width = (int)args(13).double_value();

  if (_n_dims(args(14)) > 0)
    { error("dimension mismatch in argument max_color"); return retval; }
  _arg_max_color = (int)args(14).double_value();

  if (_n_dims(args(15)) > 0)
    { error("dimension mismatch in argument max_width"); return retval; }
  _arg_max_width = (int)args(15).double_value();

  if (_n_dims(args(16)) > 0)
    { error("dimension mismatch in argument rectangular"); return retval; }
  _arg_rectangular = (int)args(16).double_value();

  if (  (int)(_dim(args(17), 0)) != (int)(_arg_nx) ||
        (int)(_dim(args(17), 1)) != (int)(_arg_ny) ||
      _n_dims(args(17)) > 2)
    { error("dimension mismatch in argument xg"); return retval; }
  Matrix _xg = args(17).matrix_value();
  _arg_xg = (float *)alloca(_xg.dim1() * _xg.dim2() * sizeof (float));
  _cvt_double_to(_arg_xg, &_xg(0,0), _xg.dim1()*_xg.dim2());

  if (_n_dims(args(18)) > 2)
    { error("dimension mismatch in argument yg"); return retval; }
  Matrix _yg = args(18).matrix_value();
  _arg_yg = (float *)alloca(_yg.dim1() * _yg.dim2() * sizeof (float));
  _cvt_double_to(_arg_yg, &_yg(0,0), _yg.dim1()*_yg.dim2());

    my_plshade2(_arg_a, _arg_nx, _arg_ny, _arg_defined, _arg_left, _arg_right, _arg_bottom, _arg_top, _arg_shade_min, _arg_shade_max, _arg_sh_cmap, _arg_sh_color, _arg_sh_width, _arg_min_color, _arg_min_width, _arg_max_color, _arg_max_width, _arg_rectangular, _arg_xg, _arg_yg);
  return retval;
}

octave_value_list _wrap_plstripc(const octave_value_list &args, int )
{
  octave_value_list retval(1, octave_value());
  if (args.length()-1 != 22)
  {
    print_usage("plstripc");
    return retval;
  }

  int _arg_id;
  char * _arg_xspec;
  char * _arg_yspec;
  float _arg_xmin;
  float _arg_xmax;
  float _arg_xjump;
  float _arg_ymin;
  float _arg_ymax;
  float _arg_xlpos;
  float _arg_ylpos;
  int _arg_y_ascl;
  int _arg_acc;
  int _arg_colbox;
  int _arg_collab;
  int *_arg_colline;
  int *_arg_styline;
  char * _arg_legline1;
  char * _arg_legline2;
  char * _arg_legline3;
  char * _arg_legline4;
  char * _arg_labx;
  char * _arg_laby;
  char * _arg_labtop;
  if (0 > 0)
    { error("dimension mismatch in argument xspec"); return retval; }
  string _xspec = args(1).string_value();
  _xspec += '\0';
  _arg_xspec = (char *)_xspec.data();

  if (0 > 0)
    { error("dimension mismatch in argument yspec"); return retval; }
  string _yspec = args(2).string_value();
  _yspec += '\0';
  _arg_yspec = (char *)_yspec.data();

  if (_n_dims(args(3)) > 0)
    { error("dimension mismatch in argument xmin"); return retval; }
  _arg_xmin = (float)args(3).double_value();

  if (_n_dims(args(4)) > 0)
    { error("dimension mismatch in argument xmax"); return retval; }
  _arg_xmax = (float)args(4).double_value();

  if (_n_dims(args(5)) > 0)
    { error("dimension mismatch in argument xjump"); return retval; }
  _arg_xjump = (float)args(5).double_value();

  if (_n_dims(args(6)) > 0)
    { error("dimension mismatch in argument ymin"); return retval; }
  _arg_ymin = (float)args(6).double_value();

  if (_n_dims(args(7)) > 0)
    { error("dimension mismatch in argument ymax"); return retval; }
  _arg_ymax = (float)args(7).double_value();

  if (_n_dims(args(8)) > 0)
    { error("dimension mismatch in argument xlpos"); return retval; }
  _arg_xlpos = (float)args(8).double_value();

  if (_n_dims(args(9)) > 0)
    { error("dimension mismatch in argument ylpos"); return retval; }
  _arg_ylpos = (float)args(9).double_value();

  if (_n_dims(args(10)) > 0)
    { error("dimension mismatch in argument y_ascl"); return retval; }
  _arg_y_ascl = (int)args(10).double_value();

  if (_n_dims(args(11)) > 0)
    { error("dimension mismatch in argument acc"); return retval; }
  _arg_acc = (int)args(11).double_value();

  if (_n_dims(args(12)) > 0)
    { error("dimension mismatch in argument colbox"); return retval; }
  _arg_colbox = (int)args(12).double_value();

  if (_n_dims(args(13)) > 0)
    { error("dimension mismatch in argument collab"); return retval; }
  _arg_collab = (int)args(13).double_value();

  if (  (int)(_dim(args(14), 0)) != (int)(4) ||
      _n_dims(args(14)) > 1)
    { error("dimension mismatch in argument colline"); return retval; }
  Matrix _colline = args(14).matrix_value();
  _arg_colline = (int *)alloca(_colline.dim1() * _colline.dim2() * sizeof (int));
  _cvt_double_to(_arg_colline, &_colline(0,0), _colline.dim1()*_colline.dim2());

  if (  (int)(_dim(args(15), 0)) != (int)(4) ||
      _n_dims(args(15)) > 1)
    { error("dimension mismatch in argument styline"); return retval; }
  Matrix _styline = args(15).matrix_value();
  _arg_styline = (int *)alloca(_styline.dim1() * _styline.dim2() * sizeof (int));
  _cvt_double_to(_arg_styline, &_styline(0,0), _styline.dim1()*_styline.dim2());

  if (0 > 0)
    { error("dimension mismatch in argument legline1"); return retval; }
  string _legline1 = args(16).string_value();
  _legline1 += '\0';
  _arg_legline1 = (char *)_legline1.data();

  if (0 > 0)
    { error("dimension mismatch in argument legline2"); return retval; }
  string _legline2 = args(17).string_value();
  _legline2 += '\0';
  _arg_legline2 = (char *)_legline2.data();

  if (0 > 0)
    { error("dimension mismatch in argument legline3"); return retval; }
  string _legline3 = args(18).string_value();
  _legline3 += '\0';
  _arg_legline3 = (char *)_legline3.data();

  if (0 > 0)
    { error("dimension mismatch in argument legline4"); return retval; }
  string _legline4 = args(19).string_value();
  _legline4 += '\0';
  _arg_legline4 = (char *)_legline4.data();

  if (0 > 0)
    { error("dimension mismatch in argument labx"); return retval; }
  string _labx = args(20).string_value();
  _labx += '\0';
  _arg_labx = (char *)_labx.data();

  if (0 > 0)
    { error("dimension mismatch in argument laby"); return retval; }
  string _laby = args(21).string_value();
  _laby += '\0';
  _arg_laby = (char *)_laby.data();

  if (0 > 0)
    { error("dimension mismatch in argument labtop"); return retval; }
  string _labtop = args(22).string_value();
  _labtop += '\0';
  _arg_labtop = (char *)_labtop.data();

    my_plstripc(&_arg_id, _arg_xspec, _arg_yspec, _arg_xmin, _arg_xmax, _arg_xjump, _arg_ymin, _arg_ymax, _arg_xlpos, _arg_ylpos, _arg_y_ascl, _arg_acc, _arg_colbox, _arg_collab, _arg_colline, _arg_styline, _arg_legline1, _arg_legline2, _arg_legline3, _arg_legline4, _arg_labx, _arg_laby, _arg_labtop);
  retval(0) = octave_value((double)_arg_id);
  return retval;
}

octave_value_list _wrap_plClearOpts(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 0)
  {
    print_usage("plClearOpts");
    return retval;
  }

    plClearOpts();
  return retval;
}

octave_value_list _wrap_plGetFlt(const octave_value_list &args, int )
{
  octave_value_list retval(1, octave_value());
  if (args.length()-1 != 1)
  {
    print_usage("plGetFlt");
    return retval;
  }

  char * _arg_s;
  float _arg_retval;
  if (0 > 0)
    { error("dimension mismatch in argument s"); return retval; }
  string _s = args(1).string_value();
  _s += '\0';
  _arg_s = (char *)_s.data();

    _arg_retval = (float)
      plGetFlt(_arg_s);
  retval(0) = octave_value((double)_arg_retval);
  return retval;
}

octave_value_list _wrap_plGetInt(const octave_value_list &args, int )
{
  octave_value_list retval(1, octave_value());
  if (args.length()-1 != 1)
  {
    print_usage("plGetInt");
    return retval;
  }

  char * _arg_s;
  int _arg_retval;
  if (0 > 0)
    { error("dimension mismatch in argument s"); return retval; }
  string _s = args(1).string_value();
  _s += '\0';
  _arg_s = (char *)_s.data();

    _arg_retval = (int)
      plGetInt(_arg_s);
  retval(0) = octave_value((double)_arg_retval);
  return retval;
}

octave_value_list _wrap_plHLS_RGB(const octave_value_list &args, int nargout)
{
  octave_value_list retval(3, octave_value());
  if (args.length()-1 != 3 ||
      nargout != 3)
  {
    print_usage("plHLS_RGB");
    return retval;
  }

  int _d[2] = { 1,1 };
  int _vec_n = 0;
  int _vidx;
  int _vec_sz;
  float *_arg_h;
  float *_arg_l;
  float *_arg_s;
  float *_arg_p_r;
  float *_arg_p_g;
  float *_arg_p_b;
  int _vecstride_h = 1;
  int _vecstride_l = 1;
  int _vecstride_s = 1;
  if (!_check_input_vectorize(args(1), &_vec_n, _d, 0, &_vecstride_h))
    { error("dimension mismatch in argument h"); return retval; }
  Matrix _h = args(1).matrix_value();
  _arg_h = (float *)alloca(_h.dim1() * _h.dim2() * sizeof (float));
  _cvt_double_to(_arg_h, &_h(0,0), _h.dim1()*_h.dim2());

  if (!_check_input_vectorize(args(2), &_vec_n, _d, 0, &_vecstride_l))
    { error("dimension mismatch in argument l"); return retval; }
  Matrix _l = args(2).matrix_value();
  _arg_l = (float *)alloca(_l.dim1() * _l.dim2() * sizeof (float));
  _cvt_double_to(_arg_l, &_l(0,0), _l.dim1()*_l.dim2());

  if (!_check_input_vectorize(args(3), &_vec_n, _d, 0, &_vecstride_s))
    { error("dimension mismatch in argument s"); return retval; }
  Matrix _s = args(3).matrix_value();
  _arg_s = (float *)alloca(_s.dim1() * _s.dim2() * sizeof (float));
  _cvt_double_to(_arg_s, &_s(0,0), _s.dim1()*_s.dim2());

  if (_d[0]  != 1 || _d[1]  != 1)
    retval(0) = octave_value(Matrix(_d[0] , _d[1] ));
  _arg_p_r = (float *)alloca(_d[0]  * _d[1]  * sizeof (float));
  if (_d[0]  != 1 || _d[1]  != 1)
    retval(1) = octave_value(Matrix(_d[0] , _d[1] ));
  _arg_p_g = (float *)alloca(_d[0]  * _d[1]  * sizeof (float));
  if (_d[0]  != 1 || _d[1]  != 1)
    retval(2) = octave_value(Matrix(_d[0] , _d[1] ));
  _arg_p_b = (float *)alloca(_d[0]  * _d[1]  * sizeof (float));
  _vec_sz = _d[0]*_d[1];
  for (_vidx = 0; _vidx < _vec_sz; ++_vidx) {
    plHLS_RGB(_arg_h[_vecstride_h*_vidx], _arg_l[_vecstride_l*_vidx], _arg_s[_vecstride_s*_vidx], &_arg_p_r[_vidx], &_arg_p_g[_vidx], &_arg_p_b[_vidx]);
  }
  if (_arraylen(retval(0)) == 1)
    retval(0) = octave_value(double(*_arg_p_r));
  else
    _cvt_to_double(_arg_p_r, (double *)retval(0).matrix_value().data(), _arraylen(retval(0)));
  if (_arraylen(retval(1)) == 1)
    retval(1) = octave_value(double(*_arg_p_g));
  else
    _cvt_to_double(_arg_p_g, (double *)retval(1).matrix_value().data(), _arraylen(retval(1)));
  if (_arraylen(retval(2)) == 1)
    retval(2) = octave_value(double(*_arg_p_b));
  else
    _cvt_to_double(_arg_p_b, (double *)retval(2).matrix_value().data(), _arraylen(retval(2)));
  return retval;
}

octave_value_list _wrap_plOptUsage(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 0)
  {
    print_usage("plOptUsage");
    return retval;
  }

    plOptUsage();
  return retval;
}

octave_value_list _wrap_plParseOpts(const octave_value_list &args, int )
{
  octave_value_list retval(1, octave_value());
  if (args.length()-1 != 3)
  {
    print_usage("plParseOpts");
    return retval;
  }

  int _arg_p_argc;
  char * _arg_argv;
  int _arg_mode;
  int _arg_retval;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument p_argc"); return retval; }
  _arg_p_argc = (int)args(1).double_value();

  if (0 > 0)
    { error("dimension mismatch in argument argv"); return retval; }
  string _argv = args(2).string_value();
  _argv += '\0';
  _arg_argv = (char *)_argv.data();

  if (_n_dims(args(3)) > 0)
    { error("dimension mismatch in argument mode"); return retval; }
  _arg_mode = (int)args(3).double_value();

    _arg_retval = (int)
      plParseOpts(&_arg_p_argc, &_arg_argv, _arg_mode);
  retval(0) = octave_value((double)_arg_retval);
  return retval;
}

octave_value_list _wrap_plRGB_HLS(const octave_value_list &args, int nargout)
{
  octave_value_list retval(3, octave_value());
  if (args.length()-1 != 3 ||
      nargout != 3)
  {
    print_usage("plRGB_HLS");
    return retval;
  }

  int _d[2] = { 1,1 };
  int _vec_n = 0;
  int _vidx;
  int _vec_sz;
  float *_arg_r;
  float *_arg_g;
  float *_arg_b;
  float *_arg_p_h;
  float *_arg_p_l;
  float *_arg_p_s;
  int _vecstride_r = 1;
  int _vecstride_g = 1;
  int _vecstride_b = 1;
  if (!_check_input_vectorize(args(1), &_vec_n, _d, 0, &_vecstride_r))
    { error("dimension mismatch in argument r"); return retval; }
  Matrix _r = args(1).matrix_value();
  _arg_r = (float *)alloca(_r.dim1() * _r.dim2() * sizeof (float));
  _cvt_double_to(_arg_r, &_r(0,0), _r.dim1()*_r.dim2());

  if (!_check_input_vectorize(args(2), &_vec_n, _d, 0, &_vecstride_g))
    { error("dimension mismatch in argument g"); return retval; }
  Matrix _g = args(2).matrix_value();
  _arg_g = (float *)alloca(_g.dim1() * _g.dim2() * sizeof (float));
  _cvt_double_to(_arg_g, &_g(0,0), _g.dim1()*_g.dim2());

  if (!_check_input_vectorize(args(3), &_vec_n, _d, 0, &_vecstride_b))
    { error("dimension mismatch in argument b"); return retval; }
  Matrix _b = args(3).matrix_value();
  _arg_b = (float *)alloca(_b.dim1() * _b.dim2() * sizeof (float));
  _cvt_double_to(_arg_b, &_b(0,0), _b.dim1()*_b.dim2());

  if (_d[0]  != 1 || _d[1]  != 1)
    retval(0) = octave_value(Matrix(_d[0] , _d[1] ));
  _arg_p_h = (float *)alloca(_d[0]  * _d[1]  * sizeof (float));
  if (_d[0]  != 1 || _d[1]  != 1)
    retval(1) = octave_value(Matrix(_d[0] , _d[1] ));
  _arg_p_l = (float *)alloca(_d[0]  * _d[1]  * sizeof (float));
  if (_d[0]  != 1 || _d[1]  != 1)
    retval(2) = octave_value(Matrix(_d[0] , _d[1] ));
  _arg_p_s = (float *)alloca(_d[0]  * _d[1]  * sizeof (float));
  _vec_sz = _d[0]*_d[1];
  for (_vidx = 0; _vidx < _vec_sz; ++_vidx) {
    plRGB_HLS(_arg_r[_vecstride_r*_vidx], _arg_g[_vecstride_g*_vidx], _arg_b[_vecstride_b*_vidx], &_arg_p_h[_vidx], &_arg_p_l[_vidx], &_arg_p_s[_vidx]);
  }
  if (_arraylen(retval(0)) == 1)
    retval(0) = octave_value(double(*_arg_p_h));
  else
    _cvt_to_double(_arg_p_h, (double *)retval(0).matrix_value().data(), _arraylen(retval(0)));
  if (_arraylen(retval(1)) == 1)
    retval(1) = octave_value(double(*_arg_p_l));
  else
    _cvt_to_double(_arg_p_l, (double *)retval(1).matrix_value().data(), _arraylen(retval(1)));
  if (_arraylen(retval(2)) == 1)
    retval(2) = octave_value(double(*_arg_p_s));
  else
    _cvt_to_double(_arg_p_s, (double *)retval(2).matrix_value().data(), _arraylen(retval(2)));
  return retval;
}

octave_value_list _wrap_plResetOpts(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 0)
  {
    print_usage("plResetOpts");
    return retval;
  }

    plResetOpts();
  return retval;
}

octave_value_list _wrap_plSetOpt(const octave_value_list &args, int )
{
  octave_value_list retval(1, octave_value());
  if (args.length()-1 != 2)
  {
    print_usage("plSetOpt");
    return retval;
  }

  char * _arg_opt;
  char * _arg_optarg;
  int _arg_retval;
  if (0 > 0)
    { error("dimension mismatch in argument opt"); return retval; }
  string _opt = args(1).string_value();
  _opt += '\0';
  _arg_opt = (char *)_opt.data();

  if (0 > 0)
    { error("dimension mismatch in argument optarg"); return retval; }
  string _optarg = args(2).string_value();
  _optarg += '\0';
  _arg_optarg = (char *)_optarg.data();

    _arg_retval = (int)
      plSetOpt(_arg_opt, _arg_optarg);
  retval(0) = octave_value((double)_arg_retval);
  return retval;
}

octave_value_list _wrap_plSetUsage(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 2)
  {
    print_usage("plSetUsage");
    return retval;
  }

  char * _arg_program_string;
  char * _arg_usage_string;
  if (0 > 0)
    { error("dimension mismatch in argument program_string"); return retval; }
  string _program_string = args(1).string_value();
  _program_string += '\0';
  _arg_program_string = (char *)_program_string.data();

  if (0 > 0)
    { error("dimension mismatch in argument usage_string"); return retval; }
  string _usage_string = args(2).string_value();
  _usage_string += '\0';
  _arg_usage_string = (char *)_usage_string.data();

    plSetUsage(_arg_program_string, _arg_usage_string);
  return retval;
}

octave_value_list _wrap_plarrows(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 8)
  {
    print_usage("plarrows");
    return retval;
  }

  float _arg_u;
  float _arg_v;
  float _arg_x;
  float _arg_y;
  int _arg_n;
  float _arg_scale;
  float _arg_dx;
  float _arg_dy;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument u"); return retval; }
  _arg_u = (float)args(1).double_value();

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument v"); return retval; }
  _arg_v = (float)args(2).double_value();

  if (_n_dims(args(3)) > 0)
    { error("dimension mismatch in argument x"); return retval; }
  _arg_x = (float)args(3).double_value();

  if (_n_dims(args(4)) > 0)
    { error("dimension mismatch in argument y"); return retval; }
  _arg_y = (float)args(4).double_value();

  if (_n_dims(args(5)) > 0)
    { error("dimension mismatch in argument n"); return retval; }
  _arg_n = (int)args(5).double_value();

  if (_n_dims(args(6)) > 0)
    { error("dimension mismatch in argument scale"); return retval; }
  _arg_scale = (float)args(6).double_value();

  if (_n_dims(args(7)) > 0)
    { error("dimension mismatch in argument dx"); return retval; }
  _arg_dx = (float)args(7).double_value();

  if (_n_dims(args(8)) > 0)
    { error("dimension mismatch in argument dy"); return retval; }
  _arg_dy = (float)args(8).double_value();

    plarrows(&_arg_u, &_arg_v, &_arg_x, &_arg_y, _arg_n, _arg_scale, _arg_dx, _arg_dy);
  return retval;
}

octave_value_list _wrap_pldid2pc(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 4)
  {
    print_usage("pldid2pc");
    return retval;
  }

  float _arg_xmin;
  float _arg_ymin;
  float _arg_xmax;
  float _arg_ymax;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument xmin"); return retval; }
  _arg_xmin = (float)args(1).double_value();

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument ymin"); return retval; }
  _arg_ymin = (float)args(2).double_value();

  if (_n_dims(args(3)) > 0)
    { error("dimension mismatch in argument xmax"); return retval; }
  _arg_xmax = (float)args(3).double_value();

  if (_n_dims(args(4)) > 0)
    { error("dimension mismatch in argument ymax"); return retval; }
  _arg_ymax = (float)args(4).double_value();

    pldid2pc(&_arg_xmin, &_arg_ymin, &_arg_xmax, &_arg_ymax);
  return retval;
}

octave_value_list _wrap_pldip2dc(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 4)
  {
    print_usage("pldip2dc");
    return retval;
  }

  float _arg_xmin;
  float _arg_ymin;
  float _arg_xmax;
  float _arg_ymax;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument xmin"); return retval; }
  _arg_xmin = (float)args(1).double_value();

  if (_n_dims(args(2)) > 0)
    { error("dimension mismatch in argument ymin"); return retval; }
  _arg_ymin = (float)args(2).double_value();

  if (_n_dims(args(3)) > 0)
    { error("dimension mismatch in argument xmax"); return retval; }
  _arg_xmax = (float)args(3).double_value();

  if (_n_dims(args(4)) > 0)
    { error("dimension mismatch in argument ymax"); return retval; }
  _arg_ymax = (float)args(4).double_value();

    pldip2dc(&_arg_xmin, &_arg_ymin, &_arg_xmax, &_arg_ymax);
  return retval;
}

octave_value_list _wrap_plsError(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 2)
  {
    print_usage("plsError");
    return retval;
  }

  int _arg_errcode;
  char * _arg_errmsg;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument errcode"); return retval; }
  _arg_errcode = (int)args(1).double_value();

  if (0 > 0)
    { error("dimension mismatch in argument errmsg"); return retval; }
  string _errmsg = args(2).string_value();
  _errmsg += '\0';
  _arg_errmsg = (char *)_errmsg.data();

    plsError(&_arg_errcode, _arg_errmsg);
  return retval;
}

octave_value_list _wrap_plsxwin(const octave_value_list &args, int )
{
  octave_value_list retval(0, octave_value());
  if (args.length()-1 != 1)
  {
    print_usage("plsxwin");
    return retval;
  }

  int _arg_window_id;
  if (_n_dims(args(1)) > 0)
    { error("dimension mismatch in argument window_id"); return retval; }
  _arg_window_id = (int)args(1).double_value();

    plsxwin(_arg_window_id);
  return retval;
}



//
// The main dispatch function.  This function calls the appropriate wrapper
// based on the value of the first argument.
//
DEFUN_DLD(plplot_octave, args, nargout, "See tmp_stub for documentation")
{
  int function_id = int(args(0).double_value());
				// Get the function ID.
  switch (function_id)
  {
  case 0: return _wrap_pl_setcontlabelformat(args, nargout);
  case 1: return _wrap_pl_setcontlabelparam(args, nargout);
  case 2: return _wrap_pladv(args, nargout);
  case 3: return _wrap_plaxes(args, nargout);
  case 4: return _wrap_plbin(args, nargout);
  case 5: return _wrap_plbop(args, nargout);
  case 6: return _wrap_plbox(args, nargout);
  case 7: return _wrap_plbox3(args, nargout);
  case 8: return _wrap_c_plcol0(args, nargout);
  case 9: return _wrap_plcol1(args, nargout);
  case 10: return _wrap_plcpstrm(args, nargout);
  case 11: return _wrap_plend(args, nargout);
  case 12: return _wrap_plend1(args, nargout);
  case 13: return _wrap_plenv(args, nargout);
  case 14: return _wrap_pleop(args, nargout);
  case 15: return _wrap_plerrx(args, nargout);
  case 16: return _wrap_plerry(args, nargout);
  case 17: return _wrap_plfamadv(args, nargout);
  case 18: return _wrap_plfill(args, nargout);
  case 19: return _wrap_plfill3(args, nargout);
  case 20: return _wrap_plflush(args, nargout);
  case 21: return _wrap_plfont(args, nargout);
  case 22: return _wrap_plfontld(args, nargout);
  case 23: return _wrap_plgchr(args, nargout);
  case 24: return _wrap_plgcol0(args, nargout);
  case 25: return _wrap_plgcolbg(args, nargout);
  case 26: return _wrap_c_plgcompression(args, nargout);
  case 27: return _wrap_plgdev(args, nargout);
  case 28: return _wrap_plgdidev(args, nargout);
  case 29: return _wrap_plgdiori(args, nargout);
  case 30: return _wrap_plgdiplt(args, nargout);
  case 31: return _wrap_plgfam(args, nargout);
  case 32: return _wrap_plgfnam(args, nargout);
  case 33: return _wrap_plglevel(args, nargout);
  case 34: return _wrap_plgpage(args, nargout);
  case 35: return _wrap_plgra(args, nargout);
  case 36: return _wrap_plgspa(args, nargout);
  case 37: return _wrap_plgstrm(args, nargout);
  case 38: return _wrap_plgver(args, nargout);
  case 39: return _wrap_plgxax(args, nargout);
  case 40: return _wrap_plgyax(args, nargout);
  case 41: return _wrap_plgzax(args, nargout);
  case 42: return _wrap_plhist(args, nargout);
  case 43: return _wrap_plhls(args, nargout);
  case 44: return _wrap_plinit(args, nargout);
  case 45: return _wrap_pljoin(args, nargout);
  case 46: return _wrap_pllab(args, nargout);
  case 47: return _wrap_pllightsource(args, nargout);
  case 48: return _wrap_plline(args, nargout);
  case 49: return _wrap_plline3(args, nargout);
  case 50: return _wrap_pllsty(args, nargout);
  case 51: return _wrap_plmkstrm(args, nargout);
  case 52: return _wrap_plmtex(args, nargout);
  case 53: return _wrap_plpat(args, nargout);
  case 54: return _wrap_plpoin(args, nargout);
  case 55: return _wrap_plpoin3(args, nargout);
  case 56: return _wrap_plprec(args, nargout);
  case 57: return _wrap_plpsty(args, nargout);
  case 58: return _wrap_plptex(args, nargout);
  case 59: return _wrap_plreplot(args, nargout);
  case 60: return _wrap_plrgb(args, nargout);
  case 61: return _wrap_plrgb1(args, nargout);
  case 62: return _wrap_plschr(args, nargout);
  case 63: return _wrap_plscmap0(args, nargout);
  case 64: return _wrap_plscmap0n(args, nargout);
  case 65: return _wrap_plscmap1(args, nargout);
  case 66: return _wrap_plscmap1l(args, nargout);
  case 67: return _wrap_plscmap1n(args, nargout);
  case 68: return _wrap_plscol0(args, nargout);
  case 69: return _wrap_plscolbg(args, nargout);
  case 70: return _wrap_plscolor(args, nargout);
  case 71: return _wrap_c_plscompression(args, nargout);
  case 72: return _wrap_plsdev(args, nargout);
  case 73: return _wrap_plsdidev(args, nargout);
  case 74: return _wrap_plsdimap(args, nargout);
  case 75: return _wrap_plsdiori(args, nargout);
  case 76: return _wrap_plsdiplt(args, nargout);
  case 77: return _wrap_plsdiplz(args, nargout);
  case 78: return _wrap_plsesc(args, nargout);
  case 79: return _wrap_plsfam(args, nargout);
  case 80: return _wrap_plsfnam(args, nargout);
  case 81: return _wrap_plsmaj(args, nargout);
  case 82: return _wrap_plsmin(args, nargout);
  case 83: return _wrap_plsori(args, nargout);
  case 84: return _wrap_plspage(args, nargout);
  case 85: return _wrap_plspause(args, nargout);
  case 86: return _wrap_plsstrm(args, nargout);
  case 87: return _wrap_plssub(args, nargout);
  case 88: return _wrap_plssym(args, nargout);
  case 89: return _wrap_plstar(args, nargout);
  case 90: return _wrap_plstart(args, nargout);
  case 91: return _wrap_plstripa(args, nargout);
  case 92: return _wrap_plstripd(args, nargout);
  case 93: return _wrap_plstyl(args, nargout);
  case 94: return _wrap_plsvpa(args, nargout);
  case 95: return _wrap_plsxax(args, nargout);
  case 96: return _wrap_plsyax(args, nargout);
  case 97: return _wrap_plsym(args, nargout);
  case 98: return _wrap_plszax(args, nargout);
  case 99: return _wrap_pltext(args, nargout);
  case 100: return _wrap_plvasp(args, nargout);
  case 101: return _wrap_plvpas(args, nargout);
  case 102: return _wrap_plvpor(args, nargout);
  case 103: return _wrap_plvsta(args, nargout);
  case 104: return _wrap_plw3d(args, nargout);
  case 105: return _wrap_plwid(args, nargout);
  case 106: return _wrap_plwind(args, nargout);
  case 107: return _wrap_plxormod(args, nargout);
  case 108: return _wrap_plGetCursor(args, nargout);
  case 109: return _wrap_plTranslateCursor(args, nargout);
  case 110: return _wrap_plcol(args, nargout);
  case 111: return _wrap_plcont(args, nargout);
  case 112: return _wrap_plcont0(args, nargout);
  case 113: return _wrap_plcont1(args, nargout);
  case 114: return _wrap_plcont2(args, nargout);
  case 115: return _wrap_plcont2p(args, nargout);
  case 116: return _wrap_plmesh(args, nargout);
  case 117: return _wrap_plot3d(args, nargout);
  case 118: return _wrap_plotsh3d(args, nargout);
  case 119: return _wrap_plpoly3(args, nargout);
  case 120: return _wrap_plshade(args, nargout);
  case 121: return _wrap_plshade1(args, nargout);
  case 122: return _wrap_plshade2(args, nargout);
  case 123: return _wrap_plstripc(args, nargout);
  case 124: return _wrap_plClearOpts(args, nargout);
  case 125: return _wrap_plGetFlt(args, nargout);
  case 126: return _wrap_plGetInt(args, nargout);
  case 127: return _wrap_plHLS_RGB(args, nargout);
  case 128: return _wrap_plOptUsage(args, nargout);
  case 129: return _wrap_plParseOpts(args, nargout);
  case 130: return _wrap_plRGB_HLS(args, nargout);
  case 131: return _wrap_plResetOpts(args, nargout);
  case 132: return _wrap_plSetOpt(args, nargout);
  case 133: return _wrap_plSetUsage(args, nargout);
  case 134: return _wrap_plarrows(args, nargout);
  case 135: return _wrap_pldid2pc(args, nargout);
  case 136: return _wrap_pldip2dc(args, nargout);
  case 137: return _wrap_plsError(args, nargout);
  case 138: return _wrap_plsxwin(args, nargout);

  default: error("Illegal function ID parameter"); return octave_value();
  }
}

