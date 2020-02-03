/***********************************************************************
 @C-file{
    author              = "Nelson H. F. Beebe",
    version             = "2.00",
    date                = "10 December 2000",
    time                = "08:44:08 MST",
    filename            = "ndiff.c",
    copyright           = "Copyright (c) 2000 Nelson H. F. Beebe. This
			   code is licensed under the GNU General Public
			   License, version 2 or later.",
    address             = "Center for Scientific Computing
			   University of Utah
			   Department of Mathematics, 322 INSCC
			   155 S 1400 E RM 233
			   Salt Lake City, UT 84112-0090
			   USA",
    telephone           = "+1 801 581 5254",
    FAX                 = "+1 801 585 1640, +1 801 581 4148",
    URL                 = "http://www.math.utah.edu/~beebe",
    checksum            = "31885 1715 5905 53017",
    email               = "beebe@math.utah.edu, beebe@acm.org,
			   beebe@computer.org, beebe@ieee.org
			   (Internet)",
    codetable           = "ISO/ASCII",
    keywords            = "numerical file differencing",
    supported           = "yes",
    docstring           = "This program compares two putatively similar
			   files, ignoring small numeric differences.
			   Complete documentation can be found in the
			   accompanying UNIX manual page file,
			   ndiff.man.

			   Usage:
				ndiff [ -? ] [ -abserr abserr ]
				      [ -author ] [ -copyright ]
				      [ -fields n1a-n1b,n2,n3a-n3b,... ]
				      [ -help ] [ -logfile filename ]
				      [ -minwidth nnn ]
				      [ -outfile filename ]
				      [ -precision number-of-bits ]
				      [ -quick ] [ -quiet ]
				      [ -relerr relerr ]
				      [ -separators regexp ] [ -silent ]
				      [ -version ] [ -www ] infile1 infile2

			   The checksum field above contains a CRC-16
			   checksum as the first value, followed by the
			   equivalent of the standard UNIX wc (word
			   count) utility output of lines, words, and
			   characters.  This is produced by Robert
			   Solovay's checksum utility.",
 }
***********************************************************************/

/***********************************************************************
 Coding conventions:

    (1) This program is written in strictly-conforming 1989 ANSI/ISO
	Standard C (ANSI X3.159-1989, ISO/IEC 9899:1990).  No
	concession is made to the now-obsolete Kernighan & Ritchie style
	of C programming, since at the time of writing, the 1989
	language standard is over a decade old.  Standard-conforming
	C compilers have been available since at least the mid-1990s on
	every commercially-significant desktop, server, mini, and
	mainframe computer system.

	The new ANSI/ISO C language standard, ISO/IEC 9899:1999,
	informally known as C99, has been published, but no
	C99-conformant compilers are yet available (in late 2000) to
	test this program.

    (2) Functions are defined in strict lexicographic order and written
	in the 7-bit ASCII character set, which occupies the first 128
	positions in the ISO8859-1 and Unicode character sets.

    (3) Macros are spelled entirely in uppercase.  Global variables are
	spelled with an initial capital letter.  Functions are spelled
	entirely in lowercase.  Embedded underscores are used to make
	names more readable.  Even if lettercase is ignored, there are
	no clashes between local and global names, and all global names
	are unique in the first six characters, ignoring lettercase, as
	required by Section 3.1.2 of ANSI X3.159-1989.

    (4) No preprocessor conditional may depend on any system-specific
	symbols, except in the associated configure script and the
	single confix.h file.

    (5) No system header files may be included, except those
	defined in ANSI/ISO Standard C.  All header file #include
	statements must be bracketed by conditionals #if HAVE_xxx
	... #endif.  The HAVE_xxx symbols are defined in config.h,
	and determined automatically by the autoconfigure process.

    (6) The code has been prettyprinted by the Free Software
	Foundation's GNU Project C prettyprinter, indent, using these
	options: -bap -bcd -bl -bli0 -di4 -i4 -ncdb -nce -ncs -nfc1
	-nip -npcs -nsc (normally stored in .indent.pro in the user's
	home directory).

    (7) In order to be usable for handling files of arbitrary precision,
	this program uses the GNU Multiple Precision library, GMP, if it
	is found to be available at installation time.

	Otherwise, it will fall back to using extended (quadruple)
	precision, using the data type "long double", if that is
	supported.

	Failing that, it will revert to standard double precision (C
	data type "double").

	The only operational difference between these three choices is
	the ability to handle a wider exponent range than normal, and
	detect numeric differences of small magnitude.  The --version
	option will display the version number, along with the
	floating-point precision that is supported, and the machine
	epsilon. The precise numeric value of the latter may depend on
	an earlier --precision nnn option.

    (8) In the event that the program is built using native precision,
	rather than GMP, the code should be relatively insensitive to
	the precise behavior of native arithmetic, including the choice
	of base, the number of bits or digits, the exponent range, and
	the availability of representations of infinity and NaN
	(not-a-number).

	The author has only systems implementing IEEE 754 floating-point
	arithmetic available for testing, but there should be no
	problems with arithmetic in IBM S/390 mainframe format,
	Compaq/DEC VAX format, or SGI/Cray format.

	Nor should the code be sensitive to the presence of longer
	intermediate-precision hardware registers, such as on the
	Intel x86 and Motorola 68K architectures, where registers hold
	80-bit values in temporary real format (1-bit sign, 15-bit
	exponent, 64-bit significand with no hidden bit), while memory
	words hold 64-bit values in double format (1-bit sign, 11-bit
	exponent, 53-bit significand including a leading hidden 1-bit,
	except in the case of denormalized values near the underflow
	limit, where there is no leading 1-bit, and normalization
	requirements are relaxed).

	Special programming care is taken when necessary to ensure that
	longer-precision registers do not produce anomalous results,
	such as in the computation of the machine epsilon.

***********************************************************************/

#include "awklib.h"

#include "ndiff.h"

#if !defined(DATE)
#define DATE "unknown"
#endif

#if !defined(VERSION)
#define VERSION "unknown"
#endif

#if defined(DEBUG)
#define DEBUGPRINT(plist) (void)fprintf plist
#else
#define DEBUGPRINT(plist)
#endif

typedef struct
{
    const char *name;
    size_t min_length;
    size_t(*optfun) (const char *next_arg);
}
option_table;

static awk_table_t Fields;

static const char *CNULL = (const char *)NULL;
static const char *InFile1 = (const char *)NULL;
static const char *InFile2 = (const char *)NULL;

/***********************************************************************
  The precise value of the regular expression, NUMBER_PATTERN, to match
  both an integer and a floating-point number is critical, and
  documented in the accompanying manual page: it must match not only the
  awk- and C-style -nnn, -n.nnn, and -n.nnne+nn, but also the Fortran
  styles -nnn, -n.nnn, -n.D+nn, -.nnD+nn, -nD+nn, -n.nnnQ+nn,
  -n.nnnd+nn, and -n.nnn+nnn.  The Fortran forms will be converted by
  awkfloat() to awk-form.  Ada permits an nonsignificant underscore
  between digits, so we support that too.
***********************************************************************/

static const char *NUMBER_PATTERN =
    "^[-+]?([0-9](_?[0-9])*([.]?([0-9](_?[0-9])*)*)?|[.][0-9](_?[0-9])*)([DdEeQq]?[-+]?[0-9](_?[0-9])*)?$";

static char *Program_Name = "ndiff";

static int Done = 0;
static int Error_Count = 0;
static int QUIET = 0;
static int SILENT = 0;

static fp_t ABSERR;
static fp_t Macheps;
static fp_t Max_Abserr;
static fp_t Max_Relerr;
static fp_t RELERR;
static fp_t This_Abserr;
static fp_t This_Relerr;
static fp_t Zero;

static size_t Max_Abserr_NF = 0;
static size_t Max_Abserr_NR = 0;
static size_t Max_Relerr_NF = 0;
static size_t Max_Relerr_NR = 0;
static size_t MINWIDTH = 0;
static size_t N_Fields = 0;
static size_t NRLINE = 0;

#if HAVE_GMP
static unsigned long int Precision = 256;
#endif

int main(int argc, char *argv[]);
static void cleanup(void);
static void compare_all(const char *f1line, const char *f2line,
			const char *f1parts[], const char *f2parts[],
			size_t n);
static void compare_files(const char *infile1, const char *infile2);
static void compare_some(const char *f1line, const char *f2line,
			 const char *f1parts[], const char *f2parts[],
			 size_t n);
static void compare_some(const char *f1line, const char *f2line,
			 const char *f1parts[], const char *f2parts[],
			 size_t n);
static int diff_field(const char *field1, const char *field2, size_t nfield);
static void do_args(int argc, char *argv[]);
static size_t do_one_arg(const char *this_arg, const char *next_arg);
static void error(const char *s1, const char *s2, const char *s3);
static void fp_abs(fp_t * result, fp_t a);
static void fp_assign(fp_t * result, fp_t a);
static void fp_clear(fp_t a);
static int fp_cmp(fp_t a, fp_t b);
static void fp_div(fp_t * result, fp_t a, fp_t b);
static void fp_init(fp_t * a);
static void fp_max(fp_t * result, fp_t a, fp_t b);
static void fp_min(fp_t * result, fp_t a, fp_t b);
static void fp_mul(fp_t * result, fp_t a, fp_t b);
static void fp_sub(fp_t * result, fp_t a, fp_t b);
static const char *fp_to_string(fp_t a, int ndigits);
static int ignore(const char *field);
static void initialize(int argc, char *argv[]);
static void initialize_fields(const char *fields);
static void machine_epsilon(fp_t * result);
static void maxrelerr(fp_t * result, fp_t x, fp_t y);
static size_t opt_abserr(const char *next_arg);
static size_t opt_author(const char *next_arg);
static size_t opt_copyright(const char *next_arg);
static size_t opt_fields(const char *next_arg);
static size_t opt_help(const char *next_arg);
static size_t opt_logfile(const char *next_arg);
static size_t opt_minwidth(const char *next_arg);
static size_t opt_outfile(const char *next_arg);
static size_t opt_precision(const char *next_arg);
static size_t opt_quick(const char *next_arg);
static size_t opt_quiet(const char *next_arg);
static size_t opt_relerr(const char *next_arg);
static size_t opt_separators(const char *next_arg);
static size_t opt_silent(const char *next_arg);
static size_t opt_version(const char *next_arg);
static size_t opt_www(const char *next_arg);
static void print_lines(FILE * file, const char *lines[]);
static void report_difference(const char *f1line, const char *f2line,
			      size_t nfield);
static void string_to_fp(fp_t * result, const char *s);
static void usage(void);
static void warning(const char *s1, const char *s2, const char *s3);

int
main(int argc, char *argv[])
{
    initialize(argc, argv);
    if ( (InFile1 == CNULL) || (InFile2 == CNULL) )
	usage();
    else
	compare_files(InFile1, InFile2);
    cleanup();

    return ((Error_Count == 0) ? EXIT_SUCCESS : EXIT_FAILURE);
}


static void
cleanup(void)
{
    /*******************************************************************
      Free dynamically-allocated memory prior to exiting.
    *******************************************************************/

    fp_clear(ABSERR);
    fp_clear(Macheps);
    fp_clear(Max_Abserr);
    fp_clear(Max_Relerr);
    fp_clear(RELERR);
    fp_clear(This_Abserr);
    fp_clear(This_Relerr);
    fp_clear(Zero);

    awk_free_table(Fields.table, N_Fields);
    awk_terminate();
}


static void
compare_all(const char *f1line, const char *f2line, const char *f1parts[],
	    const char *f2parts[], size_t n)
{
    /*******************************************************************
      Compare all fields in f1line and f2line, assuming that they have
      already been split into n parts in f1parts[] and f2parts[].

      If any fields differ, print a diff-style report, and increment
      global variable Error_Count,
    *******************************************************************/

    size_t k;

    for (k = 1; k <= n; ++k)
    {
	if (diff_field(f1parts[k], f2parts[k], k) != 0)
	{
	    report_difference(f1line, f2line, k);
	    return;
	}
    }
}


static void
compare_files(const char *infile1, const char *infile2)
{
    /*******************************************************************
      Compare all lines in two files, printing a diff-style report of
      differences.  If any numeric differences have been found, print a
      one-line report of which matching line had the largest numeric
      difference.  Finally, print a diagnostic if the files differ in
      length.
    *******************************************************************/

    const char *f1line;
    const char *f2line;
    const char **f1parts;
    const char **f2parts;
    size_t n1;
    size_t n2;

    f1line = CNULL;		/* keep optimizers happy */
    f2line = CNULL;
    f1parts = &CNULL;
    f2parts = &CNULL;

    NRLINE = 0;
    while ((awk_getline(infile1, &f1line) > 0)
	   && (awk_getline(infile2, &f2line) > 0))
    {
	NRLINE++;
	n1 = awk_split(f1line, &f1parts, CNULL);
	n2 = awk_split(f2line, &f2parts, CNULL);
	if (n1 == n2)
	{
	    if (N_Fields == 0)
		compare_all(f1line, f2line, f1parts, f2parts, n1);
	    else
		compare_some(f1line, f2line, f1parts, f2parts, n1);
	}
	else
	    report_difference(f1line, f2line, (n1 > n2) ? n1 : n2);
	awk_free_table(f1parts, n1);
	awk_free_table(f2parts, n2);
	awk_free_string(f1line);
	awk_free_string(f2line);
    }
    if (QUIET == 0)
    {
	const char *t;

	if ((fp_cmp(ABSERR, Zero) != 0) && (fp_cmp(Max_Abserr, Zero) > 0))
	{
	    t = fp_to_string(Max_Abserr, 3);
	    (void)printf("### Maximum absolute error in matching lines = %s at line %d field %d\n",
		 t, (int)Max_Abserr_NR, (int)Max_Abserr_NF);
	    awk_free_string(t);

	}
	if ((fp_cmp(RELERR, Zero) != 0) && (fp_cmp(Max_Relerr, Zero) > 0))
	{
	    t = fp_to_string(Max_Relerr, 3);
	    (void)printf("### Maximum relative error in matching lines = %s at line %d field %d\n",
		 t, (int)Max_Relerr_NR, (int)Max_Relerr_NF);
	    awk_free_string(t);
	}
    }
    if (awk_getline(infile1, &f1line) > 0)
    {
	awk_free_string(f1line);
	warning("second file is short", "", "");
    }
    if (awk_getline(infile2, &f2line) > 0)
    {
	awk_free_string(f2line);
	warning("first file is short", "", "");
    }
    awk_close_infile(infile1);
    awk_close_infile(infile2);
}


static void
compare_some(const char *f1line, const char *f2line, const char *f1parts[],
	     const char *f2parts[], size_t n)
{
    /*******************************************************************
      Compare selected fields in f1line and f2line, assuming that they
      have already been split into n parts in f1parts[] and f2parts[].
      The globals (N_Fields, Fields[]) define which fields are to be
      compared.

      If any fields differ, print a diff-style report, and increment
      global variable Error_Count.
    *******************************************************************/

    size_t k;
    size_t m;

    for (k = 1; (k <= N_Fields) && (k <= n); ++k)
    {
	m = (size_t) awk_string_to_unsigned_long(Fields.table[k]);
	if ((m <= n) && diff_field(f1parts[m], f2parts[m], m) != 0)
	{
	    report_difference(f1line, f2line, m);
	    return;
	}
    }
}


static int
diff_field(const char *field1, const char *field2, size_t nfield)
{
    /*******************************************************************
      If both fields are identical as strings, return 0.

      Otherwise, if both fields are numeric, return 0 if they are close
      enough (as determined by the globals ABSERR and RELERR), or are both
      ignorable (as determined by MINWIDTH), and otherwise return 1.

      Otherwise, return 1.

      The computed absolute and relative errors are saved in global
      variables (This_Abserr and This_Relerr) for later use in diagnostic
      reports.  These values are always zero for nonnumeric fields.
    *******************************************************************/

    fp_t f1;
    fp_t f2;
    int retval;

    fp_assign(&This_Abserr, Zero);
    fp_assign(&This_Relerr, Zero);
    if (strcmp(field1, field2) == 0)	/* handle the commonest, and easiest, case first */
	return (0);
    else if (awk_match(field1, NUMBER_PATTERN) &&
	     awk_match(field2, NUMBER_PATTERN))
    {
	/* Handle MINWIDTH test while the fields are still strings */
	if (ignore(field1) && ignore(field2))
	    return (0);

	/* Now coerce both fields to floating-point numbers,
	   converting Fortran-style exponents, if necessary. */
	fp_init(&f1);
	fp_init(&f2);

	string_to_fp(&f1, field1);
	string_to_fp(&f2, field2);

	fp_sub(&This_Abserr, f1, f2);
	fp_abs(&This_Abserr, This_Abserr);
	maxrelerr(&This_Relerr, f1, f2);
	fp_abs(&This_Relerr, This_Relerr);

	retval = 0;	/* assume fields match, then try to prove they do not */

	if ((fp_cmp(ABSERR, Zero) != 0) && (fp_cmp(This_Abserr, ABSERR) > 0))
	    retval = 1;			/* mismatch: absolute error too large */
	if ((fp_cmp(RELERR, Zero) != 0) && (fp_cmp(This_Relerr, RELERR) > 0))
	    retval = 1;			/* mismatch: relative error too large */

	if (retval == 0)		/* fields match, so update the */
	{				/* maximum errors in matching lines */
	    if (fp_cmp(This_Abserr, Max_Abserr) > 0)
	    {
		Max_Abserr_NF = nfield;
		Max_Abserr_NR = NRLINE;
		fp_assign(&Max_Abserr, This_Abserr);
	    }
	    if (fp_cmp(This_Relerr, Max_Relerr) > 0)
	    {
		Max_Relerr_NF = nfield;
		Max_Relerr_NR = NRLINE;
		fp_assign(&Max_Relerr, This_Relerr);
	    }
	}

	DEBUGPRINT((stderr,"DEBUG 0.01: f1          = %s\n", fp_to_string(f1,10)));
	DEBUGPRINT((stderr,"DEBUG 0.02: f2          = %s\n", fp_to_string(f2,10)));
	DEBUGPRINT((stderr,"DEBUG 0.03: This_Abserr = %s\n", fp_to_string(This_Abserr,10)));
	DEBUGPRINT((stderr,"DEBUG 0.04: This_Relerr = %s\n", fp_to_string(This_Relerr,10)));
	DEBUGPRINT((stderr,"DEBUG 0.05: Max_Abserr  = %s\n", fp_to_string(Max_Abserr,10)));
	DEBUGPRINT((stderr,"DEBUG 0.06: Max_Relerr  = %s\n", fp_to_string(Max_Relerr,10)));

	fp_clear(f1);
	fp_clear(f2);
	return (retval);
    }
    else if (awk_is_NaN(field1) &&
	     awk_is_NaN(field2))
	return (0);
    else if (awk_is_negative_infinity(field1) &&
	     awk_is_negative_infinity(field2))
	return (0);
    else if (awk_is_positive_infinity(field1) &&
	     awk_is_positive_infinity(field2))
	return (0);
    else
	return (1);
}


static void
do_args(int argc, char *argv[])
{
    /*******************************************************************
      Scan the command-line arguments and hand them each off to
      do_one_arg() for processing.  Terminate if the Done flag is
      set, or if errors occurred.
    *******************************************************************/


    size_t k;

    for (k = 1; k < (size_t)argc; ++k)
	k += do_one_arg(argv[k], argv[k + 1]);

    if (Done || (Error_Count > 0))
    {
	cleanup();
	exit((Error_Count == 0) ? EXIT_SUCCESS : EXIT_FAILURE);
    }
}


static size_t
do_one_arg(const char *this_arg, const char *next_arg)
{
    /*******************************************************************
      Match a command-line argument against the option table, and invoke
      the required processing function on it.  If it is not an argument,
      then it must be one of the command-line input files.
    *******************************************************************/

    size_t extra;
    size_t n;
    size_t m;
    static option_table options[] = {
	{"?", 1, opt_help},
	{"abserr", 1, opt_abserr},
	{"absolute-error", 4, opt_abserr},
	{"author", 2, opt_author},
	{"copyleft", 5, opt_copyright},
	{"copyright", 1, opt_copyright},
	{"fields", 1, opt_fields},
	{"help", 1, opt_help},
	{"logfile", 1, opt_logfile},
	{"minwidth", 1, opt_minwidth},
	{"minimum-width", 4, opt_minwidth},
	{"outfile", 1, opt_outfile},
	{"precision", 1, opt_precision},
	{"quick", 4, opt_quick},
	{"quiet", 1, opt_quiet},
	{"relerr", 1, opt_relerr},
	{"relative-error", 4, opt_relerr},
	{"separators", 2, opt_separators},
	{"silent", 1, opt_silent},
	{"version", 1, opt_version},
	{"www", 1, opt_www},
	{(const char *)NULL, 0, (size_t (*)(const char *))NULL}
    };

    const char *org_arg;
    const char *arg;

    arg = org_arg = awk_tolower(this_arg);
    if ((arg[0] == '-') && (arg[1] == '-'))	/* GNU/POSIX-style --option */
	arg += 2;
    else if ((arg[0] == '-') && (arg[1] != '\0')) /* UNIX-style -option */
	arg++;
    else			/* not option: expect input filename */
    {
	if (InFile1 == CNULL)
	{
	    InFile1 = this_arg;
	    awk_free_string(org_arg);
	    return (0);
	}
	else if (InFile2 == CNULL)
	{
	    InFile2 = this_arg;
	    awk_free_string(org_arg);
	    return (0);
	}
    }

    extra = 0;
    n = strlen(arg);
    for (m = 0; options[m].name != CNULL; ++m)
    {
	if ((n >= options[m].min_length)
	    && (strncmp(options[m].name, arg, n) == 0))
	{
	    extra = options[m].optfun(next_arg);
	    awk_free_string(org_arg);
	    return (extra);
	}
    }
    warning("unrecognized option ", this_arg, "");
    Error_Count++;

    awk_free_string(org_arg);
    return (0);
}


static void
error(const char *s1, const char *s2, const char *s3)
{
    /*******************************************************************
      Report a fatal error and terminate.
    *******************************************************************/

    (void)fprintf(stderr, "FATAL ERROR: %s%s%s\n", s1, s2, s3);
    cleanup();
    exit(EXIT_FAILURE);
}


static void
fp_abs(fp_t * result, fp_t a)
{
    /*******************************************************************
      Store the absolute value of the argument in *result, which is
      assumed to already be properly initialized.
    *******************************************************************/

#if HAVE_GMP
    DEBUGPRINT((stderr,"DEBUG 0.1: a = %s\n", fp_to_string(a,10)));
    mpf_abs(*result, a);
    DEBUGPRINT((stderr,"DEBUG 0.2: abs(a) = %s\n", fp_to_string(*result,10)));
#else
    *result = ((a < 0) ? -a : a);
#endif
}


static void
fp_assign(fp_t * result, fp_t a)
{
    /*******************************************************************
      Assign a to *result, where both are assumed to already be properly
      initialized.
    *******************************************************************/

#if HAVE_GMP
    mpf_set(*result, a);
#else
    *result = a;
#endif
}


static void
fp_clear(fp_t a)
{
    /*******************************************************************
      Clear a floating-point variable.
    *******************************************************************/

#if HAVE_GMP
    mpf_clear(a);
#endif
}


static int
fp_cmp(fp_t a, fp_t b)
{
    /*******************************************************************
      Compare two floating-point numbers, a and b, and return -2 if
      either is a NaN, -1 if a < b, 0 if a == b, and otherwise +1 if
      a > b.
    *******************************************************************/

#if HAVE_GMP
    int n;

    n = mpf_cmp(a, b);

    if (n < 0)
	return (-1);
    else if (n == 0)
	return (0);
    else
	return (+1);
#else
    if (a != a)			/* then a is a NaN */
	return (-2);
    else if (b != b)		/* then b is a NaN */
	return (-2);
    else if (a < b)
	return (-1);
    else if (a == b)
	return (0);
    else
	return (+1);
#endif

}


static void
fp_div(fp_t * result, fp_t a, fp_t b)
{
    /*******************************************************************
      Return a/b in *result, which is assumed to have been properly
      initialized.
    *******************************************************************/

#if HAVE_GMP
    mpf_div(*result, a, b);
#else
    *result = a / b;
#endif

}


static void
fp_init(fp_t *a)
{
    /*******************************************************************
      Initialize a floating-point variable.
    *******************************************************************/

#if HAVE_GMP
    mpf_init(*a);
#elif HAVE_LONG_DOUBLE
    *a = 0.0L;
#else
    *a = 0.0;
#endif
}


static void
fp_max(fp_t * result, fp_t a, fp_t b)
{
    /*******************************************************************
      Store the larger of the two arguments a and b in *result, which
      is assumed to have been properly initialized.  *result may be
      either a or b.
    *******************************************************************/

#if HAVE_GMP
    if (fp_cmp(a, b) > 0)
	mpf_set(*result, a);
    else
	mpf_set(*result, b);
#else
    *result = (a > b) ? a : b;
#endif
}


static void
fp_min(fp_t * result, fp_t a, fp_t b)
{
    /*******************************************************************
      Store the smaller of the two arguments a and b in *result, which
      is assumed to have been properly initialized.  *result may be
      either a or b.
    *******************************************************************/

#if HAVE_GMP
    DEBUGPRINT((stderr,"DEBUG 0.3: a = %s, b = %s\n", fp_to_string(a,10), fp_to_string(b,10)));
    if (fp_cmp(a, b) < 0)
	mpf_set(*result, a);
    else
	mpf_set(*result, b);
    DEBUGPRINT((stderr,"DEBUG 0.4: min(a,b) = %s\n", fp_to_string(*result,10)));
#else
    *result = (a < b) ? a : b;
#endif
}


static void
fp_mul(fp_t * result, fp_t a, fp_t b)
{
    /*******************************************************************
      Return a*b in *result, which is assumed to have been properly
      initialized.
    *******************************************************************/

#if HAVE_GMP
    mpf_mul(*result, a, b);
#else
    *result = a * b;
#endif

}


static void
fp_sub(fp_t * result, fp_t a, fp_t b)
{
    /*******************************************************************
      Store the difference a - b in *result, which is assumed to already
      be properly initialized.
    *******************************************************************/

#if HAVE_GMP
    DEBUGPRINT((stderr,"DEBUG 0.5: a = %s, b = %s\n", fp_to_string(a,10), fp_to_string(b,10)));
    mpf_sub(*result, a, b);
    DEBUGPRINT((stderr,"DEBUG 0.6: fp_sub(*result,a,b) = %s\n", fp_to_string(*result,10)));
#else
    *result = a - b;
#endif
}


static double
fp_to_double(fp_t a)
{
    /*******************************************************************
      Convert a to a double precision value and return it.
    *******************************************************************/

    const char *s;
    double d;

    s = fp_to_string(a, 35);
    d = strtod(s, (char **)NULL);
    awk_free_string(s);

    return (d);
}


static const char *
fp_to_string(fp_t a, int n_digits)
{
    /*******************************************************************
      Convert a to a string in a standard format, and return a pointer
      to a dynamically-allocated string containing the result.  If
      n_digits is negative or zero, then as many digits as required will
      be used.  Otherwise, the number will have 1 non-zero digit before
      the decimal point, and n_digits - 1 digits after.  The exponent is
      always signed, and contains at least two digits, with no embedded
      spaces.
    *******************************************************************/

#if HAVE_GMP
    char *s;
    char *t;
    mp_exp_t e;

    if (mpf_sgn(a) == 0)		/* mpf_get_str() returns "" for zero */
	return (awk_dupstr("0.0e+00"));
    else
    {
	t = mpf_get_str((char *)NULL, (mp_exp_t *) & e, 10, 0, a);
	if ((n_digits > 0) && (n_digits < strlen(t)))
	    t[n_digits] = '\0';
	s = awk_padstr(t, 1 + 40);	/* extra space for e plus 128-bit integer (39 digits + sign) */
	if (t[0] == '-')
	    (void)sprintf(s, "%c%c.%se%+.2ld",
			  (int)t[0], (int)t[1], (t[2] ? &t[2] : "0"), (long int)(e - 1));
	else
	    (void)sprintf(s, "%c.%se%+.2ld",
			  (int)t[0], (t[1] ? &t[1] : "0"), (long int)(e - 1));
	awk_free_string(t);
	return ((const char *)s);
    }
#else
    char s[128];		/* big enough for double and quadruple precision */

#if HAVE_LONG_DOUBLE
    (void)sprintf(s, "%.*Le", n_digits - 1, a);
#else
    (void)sprintf(s, "%.*e", n_digits - 1, a);
#endif

    return (awk_dupstr(s));
#endif

}


static int
ignore(const char *field)
{
    /*******************************************************************
      Return 1 if field is ignorable, because it is shorter than
      MINWIDTH and appears to be a real number.  Otherwise, return 0.
    *******************************************************************/

    return ((MINWIDTH > 0) &&
	    (strlen(field) < MINWIDTH) &&
	    awk_match(field, "[.DdEeQq]"));
}


static void
initialize(int argc, char *argv[])
{
    /*******************************************************************
      Process command-line options, and initialize global variables.
    *******************************************************************/

    Program_Name = (argv[0] == (char*)NULL) ? "ndiff" : argv[0];
    awk_initialize();

#if HAVE_GMP
    mpf_set_default_prec(Precision);
#endif

    fp_init(&ABSERR);
    fp_init(&Macheps);
    fp_init(&Max_Abserr);
    fp_init(&Max_Relerr);
    fp_init(&RELERR);
    fp_init(&This_Abserr);
    fp_init(&This_Relerr);
    fp_init(&Zero);

    fp_assign(&ABSERR, Zero);
    fp_assign(&Max_Abserr, Zero);
    fp_assign(&Max_Relerr, Zero);
    fp_assign(&RELERR, Zero);
    fp_assign(&This_Abserr, Zero);
    fp_assign(&This_Relerr, Zero);
    Max_Abserr_NF = 0;
    Max_Abserr_NR = 0;
    Max_Relerr_NF = 0;
    Max_Relerr_NR = 0;

    awk_new_table(&Fields);
    N_Fields = 0;
    Error_Count = 0;

    machine_epsilon(&Macheps);

    do_args(argc, argv);

    if ((fp_cmp(ABSERR, Zero) == 0) && (fp_cmp(RELERR, Zero) == 0))	/* supply default (see man pages) */
    {
#if HAVE_GMP
	fp_t temp;

	mpf_init_set_d(temp, 8.0);	/* temp = 8.0 */
	mpf_mul(temp, temp, Macheps);	/* temp = 8.0 * Macheps */
	mpf_set_d(RELERR, 1.0e-15);	/* RELERR = 1.0e-15 */
	fp_max(&RELERR, RELERR, temp);
	mpf_clear(temp);
#else
	fp_max(&RELERR, (fp_t) 1.0e-15, (fp_t) 8.0 * Macheps);
#endif
    }
}


static void
initialize_fields(const char *fields)
{
    /*******************************************************************
      Convert a FIELDS=n1a-n1b,n2,n3a-n3b,... specification to a list
      of N_Fields numbers in Fields[].
    *******************************************************************/

    size_t j;
    size_t k;
    size_t m;
    size_t n;
    const char **numbers;
    const char **parts;

    numbers = &CNULL;		/* keep optimizers happy */
    parts = &CNULL;

    N_Fields = 0;
    n = awk_split(fields, &parts, ",");
    for (k = 1; k <= n; ++k)
    {
	m = awk_split(parts[k], &numbers, "-+");
	if (m == 1)
	{
	    if (!awk_match(parts[k], "^[0-9]+$"))
		error("non-numeric fields value [", parts[k], "]");
	    else if (parts[k] == 0)
		error("zero fields value [", parts[k],
		      "]: fields are numbered from 1");
	    else
		awk_add_element(&Fields, ++N_Fields, awk_dupstr(parts[k]));
	}
	else if (m == 2)
	{
	    if ((!awk_match(numbers[1], "^[0-9]+$")) ||
		(!awk_match(numbers[2], "^[0-9]+$")))
		error("non-numeric fields range [", parts[k], "]");
	    else if ((awk_string_to_long(numbers[1]) == 0L) ||
		     (awk_string_to_long(numbers[2]) == 0L))
		error("zero value in fields range [", parts[k],
		      "]: fields are numbered from 1");
	    else if (awk_string_to_long(numbers[1]) >
		     awk_string_to_long(numbers[2]))
		error("bad fields range [", parts[k], "]");
	    else if ((awk_string_to_long(numbers[2]) -
		      awk_string_to_long(numbers[1]) + 1) > 100)
		error("fields range [", parts[k], "] exceeds 100");
	    else
	    {
		for (j = (size_t) awk_string_to_unsigned_long(numbers[1]);
		     j <= (size_t) awk_string_to_unsigned_long(numbers[2]);
		     ++j)
		    awk_add_element(&Fields, ++N_Fields,
				    awk_unsigned_long_to_string(
								(unsigned
								 long)j));
	    }
	}
	else
	    error("bad fields range [", parts[k], "]");
	awk_free_table(numbers, m);
    }
    awk_free_table(parts, n);
}


static void
machine_epsilon(fp_t * result)
{
    /*******************************************************************
      Store the machine epsilon (the smallest number x such that (1 + x)
      differs from 1) in *result, which is assumed to have been properly
      initialized.
    *******************************************************************/

    static fp_t x;
    static fp_t y;

#if HAVE_GMP
    fp_t half;

    mpf_init_set_d(half, 0.5);	/* half = 0.5 */
    mpf_init_set_d(x, 1.0);	/* x = 1.0 */
    mpf_init(y);
    mpf_mul(y, half, x);	/* y = 0.5*x */
    mpf_add_ui(y, y, 1UL);	/* y = 1 + 0.5*x */
    DEBUGPRINT((stderr, "DEBUG: machine_epsilon(): x = %s\ty = %s\n",
		  fp_to_string(x, 3), fp_to_string(y, 0)));
    while (mpf_cmp_ui(y, 1UL) != 0)
    {
	mpf_mul(x, half, x);	/* x = 0.5*x */
	mpf_mul(y, half, x);	/* y = 0.5*x */
	mpf_add_ui(y, y, 1UL);	/* y = 1 + 0.5*x */
	DEBUGPRINT((stderr, "DEBUG: machine_epsilon(): x = %s\ty = %s\n",
		      fp_to_string(x, 3), fp_to_string(y, 0)));
    }

    mpf_clear(y);
    mpf_clear(half);
    mpf_set(*result, x);
    mpf_clear(x);
#elif HAVE_LONG_DOUBLE
    x = 1.0L;
    y = 1.0L + x * 0.5L;
    store(&y);
    while (y != 1.0L)
    {
	store(&x);
	x *= 0.5L;
	y = 1.0L + x * 0.5L;
	store(&y);
	DEBUGPRINT((stderr, "DEBUG: machine_epsilon(): x = %.35Lg\ty = %.35Lg\n",
		      (long double)x, (long double)y));
    }
    *result = x;
#else
    x = 1.0;
    y = 1.0 + x * 0.5;
    store(&y);
    while (y != 1.0)
    {
	store(&x);
	x *= 0.5;
	y = 1.0 + x * 0.5;
	store(&y);
	DEBUGPRINT((stderr, "DEBUG: machine_epsilon(): x = %.35g\ty = %.35g\n",
		      (double)x, (double)y));
    }
    *result = x;
#endif

    DEBUGPRINT((stderr, "DEBUG: Macheps = %s\n", fp_to_string(*result, 0)));
}


static void
maxrelerr(fp_t * result, fp_t x, fp_t y)
{
    /*******************************************************************
      Compute the maximum relative error of two values, x and y, and
      store it in *result.
    *******************************************************************/

    fp_t t1;
    fp_t t2;
    fp_t t3;
    fp_t xabs;
    fp_t yabs;

    fp_init(&t1);
    fp_init(&t2);
    fp_init(&t3);
    fp_init(&xabs);
    fp_init(&yabs);

    DEBUGPRINT((stderr,"DEBUG 0.70: x = %s\n", fp_to_string(x,10)));
    DEBUGPRINT((stderr,"DEBUG 0.71: y = %s\n", fp_to_string(y,10)));

    fp_abs(&xabs, x);
    fp_abs(&yabs, y);

    DEBUGPRINT((stderr,"DEBUG 0.72: x = %s\n", fp_to_string(xabs,10)));
    DEBUGPRINT((stderr,"DEBUG 0.73: y = %s\n", fp_to_string(yabs,10)));

    /* See the documentation of the -relerr option in ndiff.man for the
       explanation of this complex definition: */

    if (fp_cmp(x, y) == 0)
	fp_assign(result, Zero);	/* *result = 0 */
    else if ((fp_cmp(x, Zero) != 0) && (fp_cmp(y, Zero) != 0))
    {
	fp_sub(&t1, x, y);		/* t1 = x - y */
	DEBUGPRINT((stderr,"DEBUG 1: t1 = %s = %s - %s\n", fp_to_string(t1,10), fp_to_string(x,10), fp_to_string(y,10)));
	fp_abs(&t1, t1);		/* t1 = abs(x - y) */
	DEBUGPRINT((stderr,"DEBUG 2: t1 = %s\n", fp_to_string(t1,10)));
	fp_min(&t2, xabs, yabs);	/* t2 = min(abs(x),abs(y)) */
	DEBUGPRINT((stderr,"DEBUG 3: t2 = %s = min(%s,%s)\n", fp_to_string(t1,10), fp_to_string(xabs,10), fp_to_string(yabs,10)));
	fp_div(result, t1, t2);		/* *result = abs(x-y)/min(x,y) */
	DEBUGPRINT((stderr,"DEBUG 4: *result = %s\n", fp_to_string(*result,10)));
    }
    else if ((fp_cmp(x, Zero) == 0) && (fp_cmp(y, Zero) != 0))
	string_to_fp(result,"1");	/* *result = 1 */
    else if ((fp_cmp(y, Zero) == 0) && (fp_cmp(x, Zero) != 0))
	string_to_fp(result,"1");	/* *result = 1 */
    else
	fp_assign(result, Zero);	/* *result = 0 */

    fp_clear(t1);
    fp_clear(t2);
    fp_clear(t3);
    fp_clear(xabs);
    fp_clear(yabs);
}


static size_t
opt_abserr(const char *next_arg)
{
    /*******************************************************************
      Process the -abserr option, and return the number of extra
      arguments consumed.
    *******************************************************************/

    if (next_arg == CNULL)
	error("expected -abserr absolute-error", "", "");
    string_to_fp(&ABSERR, next_arg);
    return (1);
}


static size_t
opt_author(const char *next_arg)
{
    /*******************************************************************
      Process the -author option, and return the number of extra
      arguments consumed.
    *******************************************************************/

    static const char *lines[] = {
	"Author:",
	"\tNelson H. F. Beebe",
	"\tCenter for Scientific Computing",
	"\tUniversity of Utah",
	"\tDepartment of Mathematics, 322 INSCC",
	"\t155 S 1400 E RM 233",
	"\tSalt Lake City, UT 84112-0090",
	"\tUSA",
	"\tEmail: beebe@math.utah.edu, beebe@acm.org,",
	"\t       beebe@computer.org, beebe@ieee.org (Internet)",
	"\tWWW URL: http://www.math.utah.edu/~beebe",
	"\tTelephone: +1 801 581 5254",
	"\tFAX: +1 801 585 1640, +1 801 581 4148",
	(const char *)NULL,
    };

    print_lines(stderr, lines);
    Done = 1;
    return (0);
}


static size_t
opt_copyright(const char *next_arg)
{
    /*******************************************************************
      Process the -copyright option, and return the number of extra
      arguments consumed.
    *******************************************************************/

    static const char *lines[] = {
	"########################################################################",
	"########################################################################",
	"########################################################################",
	"###                                                                  ###",
	"### ndiff: compare putatively similar files, ignoring small numeric  ###",
	"###        differences                                               ###",
	"###                                                                  ###",
	"###              Copyright (C) 2000 Nelson H. F. Beebe               ###",
	"###                                                                  ###",
	"### This program is covered by the GNU General Public License (GPL), ###",
	"### version 2 or later, available as the file COPYING in the program ###",
	"### source distribution, and on the Internet at                      ###",
	"###                                                                  ###",
	"###               ftp://ftp.gnu.org/gnu/GPL                          ###",
	"###                                                                  ###",
	"###               http://www.gnu.org/copyleft/gpl.html               ###",
	"###                                                                  ###",
	"### This program is free software; you can redistribute it and/or    ###",
	"### modify it under the terms of the GNU General Public License as   ###",
	"### published by the Free Software Foundation; either version 2 of   ###",
	"### the License, or (at your option) any later version.              ###",
	"###                                                                  ###",
	"### This program is distributed in the hope that it will be useful,  ###",
	"### but WITHOUT ANY WARRANTY; without even the implied warranty of   ###",
	"### MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the    ###",
	"### GNU General Public License for more details.                     ###",
	"###                                                                  ###",
	"### You should have received a copy of the GNU General Public        ###",
	"### License along with this program; if not, write to the Free       ###",
	"### Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,   ###",
	"### MA 02111-1307 USA.                                               ###",
	"########################################################################",
	"########################################################################",
	"########################################################################",
	(const char *)NULL
    };

    print_lines(stderr, lines);
    Done = 1;
    return (0);
}


static size_t
opt_fields(const char *next_arg)
{
    /*******************************************************************
      Process the -fields option, and return the number of extra
      arguments consumed.
    *******************************************************************/

    if (next_arg == CNULL)
	error("expected -fields fields", "", "");
    initialize_fields(next_arg);
    return (1);
}


static size_t
opt_help(const char *next_arg)
{
    /*******************************************************************
      Process the -help option, and return the number of extra
      arguments consumed.
    *******************************************************************/

    static const char *lines[] = {
	"Usage:",
	"\tndiff [ -? ] [ -abserr abserr ] [ -author ] [ -copyright ]",
	"\t      [ -fields n1a-n1b,n2,n3a-n3b,...  ] [ -help ]",
	"\t      [ -logfile filename ] [ -minwidth nnn ] [ -outfile filename ]",
	"\t      [ -precision number-of-bits ] [ -quick ] [ -quiet ]",
	"\t      [ -relerr relerr ] [ -separators regexp ] [ -silent ]",
	"\t      [ -version ] [ -www ]",
	"\t      infile1 infile2",
	(const char *)NULL
    };

    print_lines(stderr, lines);
    Done = 1;
    return (0);
}


static size_t
opt_logfile(const char *next_arg)
{
    /*******************************************************************
      Process the -logfile option, and return the number of extra
      arguments consumed.
    *******************************************************************/

    if (next_arg == CNULL)
	error("expected -logfile filename", "", "");
    if (freopen(next_arg, "w", stderr) == (FILE *) NULL)
    {
	cleanup();
	exit(EXIT_FAILURE);	/* no stderr error left to report an error on */
    }
    return (1);
}


static size_t
opt_minwidth(const char *next_arg)
{
    /*******************************************************************
      Process the -minwidth option, and return the number of extra
      arguments consumed.
    *******************************************************************/

    if (next_arg == CNULL)
	error("expected -minwidth width", "", "");
    MINWIDTH = (size_t) awk_string_to_unsigned_long(next_arg);
    return (1);
}


static size_t
opt_outfile(const char *next_arg)
{
    /*******************************************************************
      Process the -outfile option, and return the number of extra
      arguments consumed.
    *******************************************************************/

    if (next_arg == CNULL)
	error("expected -outfile filename", "", "");
    if (freopen(next_arg, "w", stdout) == (FILE *) NULL)
	error("could not reopen ", next_arg, " on stdout");
    return (1);
}


static size_t
opt_precision(const char *next_arg)
{
    /*******************************************************************
      Process the -precision option, and return the number of extra
      arguments consumed.
    *******************************************************************/

    size_t n;

    if (next_arg == CNULL)
	error("expected -precision number-of-bits", "", "");
    n = (size_t) awk_string_to_unsigned_long(next_arg);
    if (n > 0)
    {
#if HAVE_GMP
	Precision = n;
	mpf_set_default_prec(Precision);
	machine_epsilon(&Macheps);	/* must be updated when precision changes */
#else
	warning
	    ("no support for multiple-precision arithmetic has been compiled into this version",
	     "", "");
#endif
    }
    return (1);
}


static size_t
opt_quick(const char *next_arg)
{
    /*******************************************************************
      Process the -quick option, and return the number of extra
      arguments consumed.
    *******************************************************************/

    return (0);			/* TO DO: complete this complex function */
}


static size_t
opt_quiet(const char *next_arg)
{
    /*******************************************************************
      Process the -quiet option, and return the number of extra
      arguments consumed.
    *******************************************************************/

    QUIET = 1;
    return (0);
}


static size_t
opt_relerr(const char *next_arg)
{
    /*******************************************************************
      Process the -relerr option, and return the number of extra
      arguments consumed.
    *******************************************************************/
    fp_t one;

    if (next_arg == CNULL)
	error("expected -relerr relative-error", "", "");
    string_to_fp(&RELERR, next_arg);

    fp_init(&one);
    string_to_fp(&one,"1");

    if (fp_cmp(RELERR, Macheps) < 1)
    {
	char s[100];
	const char *t1;
	const char *t2;

	t1 = fp_to_string(RELERR, 3);
	t2 = fp_to_string(Macheps, 6);
	(void)sprintf(s,"RELERR = %s is below machine epsilon %s", t1, t2);
	warning(s,"","");
	awk_free_string(t1);
	awk_free_string(t2);
    }
    else if (fp_cmp(RELERR, one) > 0)
	fp_mul(&RELERR, RELERR, Macheps);

    fp_clear(one);

    return (1);
}


static size_t
opt_separators(const char *next_arg)
{
    /*******************************************************************
      Process the -separators option, and return the number of extra
      arguments consumed.
    *******************************************************************/

    if (next_arg == CNULL)
	error("expected -separators separators", "", "");
    FS = next_arg;
    return (1);
}


static size_t
opt_silent(const char *next_arg)
{
    /*******************************************************************
      Process the -silent option, and return the number of extra
      arguments consumed.
    *******************************************************************/

    SILENT = 1;
    return (0);
}


static size_t
opt_version(const char *next_arg)
{
    /*******************************************************************
      Process the -version option, and return the number of extra
      arguments consumed.
    *******************************************************************/
    const char *t;

    machine_epsilon(&Macheps);

    t = fp_to_string(Macheps, 3);
    (void)fprintf(stderr,
		  "Version %s of [%s] [%s: epsilon = %s]\n",
		  VERSION, DATE, ARITHMETIC, t);
    awk_free_string(t);
    Done = 1;
    return (0);
}


static size_t
opt_www(const char *next_arg)
{
    /*******************************************************************
      Process the -www option, and return the number of extra arguments
      consumed.
    *******************************************************************/

    static const char *lines[] = {
	"Master World-Wide Web source distribution:",
	"\tftp://ftp.math.utah.edu/pub/misc/ndiff-" VERSION ".tar.gz",
       	"\thttp://www.math.utah.edu/pub/misc/ndiff-" VERSION ".tar.gz",
	(const char *)NULL,
    };

    print_lines(stderr, lines);
    Done = 1;
    return (0);
}


static void
print_lines(FILE * outfile, const char *lines[])
{
    /*******************************************************************
      Print the array of lines[] on outfile, supplying a newline after
      each.
    *******************************************************************/

    size_t k;

    for (k = 0; lines[k] != CNULL; ++k)
	(void)fprintf(outfile, "%s\n", lines[k]);
}


static void
report_difference(const char *f1line, const char *f2line, size_t nfield)
{
    /*******************************************************************
      Print a diff-style difference of two lines, but also show in the
      separator line the field number at which they differ, and the global
      absolute and relative errors, if they are nonzero.
    *******************************************************************/

    if (SILENT == 0)
    {
	(void)printf("%luc%lu\n", (unsigned long)NRLINE, (unsigned long)NRLINE);
	(void)printf("< %s\n", f1line);
	if ((fp_cmp(This_Abserr, Zero) != 0)
	    || (fp_cmp(This_Relerr, Zero) != 0))
	{
	    const char *abserr;
	    const char *relerr;
	    fp_t emult;
	    fp_t tenk;

	    fp_init(&emult);
	    fp_init(&tenk);

	    fp_assign(&emult, This_Relerr);
	    fp_div(&emult, emult, Macheps);	/* emult = This_Relerr / Macheps */
	    fp_assign(&tenk, Zero);
	    string_to_fp(&tenk, "10000");
	    abserr = fp_to_string(This_Abserr, 3);
	    relerr = fp_to_string(This_Relerr, 3);

	    if (fp_cmp(ABSERR, Zero) == 0) /* suppress absolute error report */
	    {
		if (fp_cmp(emult, tenk) >= 0)
		    (void)printf("--- field %d\trelative error %s\n",
			   (int)nfield, relerr);
		else
		    (void)printf("--- field %d\trelative error %s [%d*(machine epsilon)]\n",
			 (int)nfield, relerr, (int)(fp_to_double(emult) + 0.5));
	    }
	    else if (fp_cmp(RELERR, Zero) == 0)	/* suppress relative error report */
		(void)printf("--- field %d\tabsolute error %s\n", (int)nfield, abserr);
	    else			/* report both absolute and relative errors */
	    {
		if (fp_cmp(emult, tenk) >= 0)
		    (void)printf("--- field %d\tabsolute error %s\trelative error %s\n",
			   (int)nfield, abserr, relerr);
		else
		    (void)printf("--- field %d\tabsolute error %s\trelative error %s [%d*(machine epsilon)]\n",
			 (int)nfield, abserr, relerr, (int)(fp_to_double(emult) + 0.5));
	    }

	    awk_free_string(abserr);
	    awk_free_string(relerr);

	    fp_clear(emult);
	    fp_clear(tenk);
	}
	else
	    (void)printf("--- field %d\n", (int)nfield);
	(void)printf("> %s\n", f2line);
    }
    Error_Count++;
}


static void
string_to_fp(fp_t * result, const char *s)
{
    /*******************************************************************
      Convert a numeric string to an fp_t floating-point number, and
      return the result as a pointer to a floating-point number in an
      internal buffer which will be overwritten on the next call.

      Fortran use has any of E, e, D, d, Q, or q, or even nothing at
      all, for the exponent letter, but awk and C only allow E and e.

      Ada usefully permits nonsignificant underscores for
      readability: 3.14159265358979323846 and
      3.14159_26535_89793_23846 are equivalent.

      We can safely assume that there are no leading or trailing
      whitespace characters, because all strings passed to this
      function are the result of splitting lines into
      whitespace-delimited fields.
    *******************************************************************/

    char *t;
    const char *t1;
    const char *t2;

    t = (char*)awk_dupstr(s);
    (void)awk_gsub("_", "", &t);	/* remove Ada-style separators */
    (void)awk_gsub("[DdQq]", "e", &t);	/* convert Fortran exponent letters to awk-style */
    if (awk_match(t, "[0-9.][-+][0-9]+$"))	/* then letter-less exponent */
    {
	t1 = awk_substr(t, 1, RSTART);
	t2 = awk_substr(t, RSTART + 1, RLENGTH - 1);
	awk_free_string(t);
	t = awk_padstr("",(size_t)(RSTART + RLENGTH));
	(void)sprintf(t, "%se%s", t1, t2);	/* insert exponent letter e */
	awk_free_string(t1);
	awk_free_string(t2);
    }
    if (awk_match(t, "^[ \t]*[-+]?[.]")) /* missing digit before decimal point? */
    {					/* gmp does not recognize such numbers */
	char *t3;

	t3 = awk_padstr(t,1);
	t3[RSTART + RLENGTH - 2] = '0';		/* supply leading zero digit */
	(void)strcpy(&t3[RSTART+RLENGTH-1],&t[RSTART+RLENGTH-2]);
	awk_free_string(t);
	t = t3;
    }

#if HAVE_GMP
    mpf_set_str(*result, t, 10);
#elif HAVE_LONG_DOUBLE
    (void)sscanf(t, "%Lg", result);
#else
    *result = (fp_t) awk_string_to_double(t);
#endif

    awk_free_string(t);
}


static void
usage(void)
{
    /*******************************************************************
      Print a usage message on stderr, and exit with a failure code.
    *******************************************************************/

    static const char *lines[] = {
	"\t[ -? ] [ -abserr abserr ] [ -author ] [ -copyright ]",
	"\t\t[ -fields n1a-n1b,n2,n3a-n3b,...  ] [ -help ]",
	"\t\t[ -logfile filename ] [ -minwidth nnn ]",
	"\t\t[ -outfile filename ] [ -precision number-of-bits ]",
	"\t\t[ -quick ] [ -quiet ] [ -relerr relerr ]",
	"\t\t[ -separators regexp ] [ -silent ] [ -version ]",
	"\t\t[ -www ] infile1 infile2",
	(const char *)NULL,
    };

    (void)fprintf(stderr, "Usage:\n\t%s", Program_Name);
    print_lines(stderr, lines);
    cleanup();
    exit(EXIT_FAILURE);
}


static void
warning(const char *s1, const char *s2, const char *s3)
{
    /*******************************************************************
      Print a warning message on stderr, using emacs compile-command-style
      message format.
    *******************************************************************/
    if (FNR > 0)
	(void)fprintf(stderr, "%s:%ld:%%%%%s%s%s\n", FILENAME, (long)FNR, s1, s2, s3);
    else			/* special case for diagnostics during initialization */
	(void)fprintf(stderr, "%s%s%s\n", s1, s2, s3);
}
