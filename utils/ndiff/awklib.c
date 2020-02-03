/***********************************************************************
 @C-file{
    author              = "Nelson H. F. Beebe",
    version             = "2.00",
    date                = "10 December 2000",
    time                = "07:53:44 MST",
    filename            = "awklib.c",
    address             = "Center for Scientific Computing
			   University of Utah
			   Department of Mathematics, 322 INSCC
			   155 S 1400 E RM 233
			   Salt Lake City, UT 84112-0090
			   USA",
    telephone           = "+1 801 581 5254",
    FAX                 = "+1 801 585 1640, +1 801 581 4148",
    URL                 = "http://www.math.utah.edu/~beebe",
    checksum            = "42645 1073 3121 29602",
    email               = "beebe@math.utah.edu, beebe@acm.org,
			   beebe@computer.org, beebe@ieee.org
			   (Internet)",
    codetable           = "ISO/ASCII",
    keywords            = "awk compatibility library",
    supported           = "yes",
    docstring           = "This file defines a subset of the awk library
			   functions for use by C programs, to assist in
			   the manual translation of awk code to C.

			   Provided that the caller is careful to
			   eventually invoke free() on every
			   newly-allocated string returned by any of
			   these primitives, awk_free_table() on
			   tables returned by awk_split(), and
			   awk_close_infile() on every open input
			   file, this library is designed to be
			   strictly free of memory leaks.

			   The checksum field above contains a CRC-16
			   checksum as the first value, followed by the
			   equivalent of the standard UNIX wc (word
			   count) utility output of lines, words, and
			   characters.  This is produced by Robert
			   Solovay's checksum utility.",
 }
***********************************************************************/

#include "awklib.h"
#include "regexp/regexp.h"

static int _c;
#define awk_is_white(c) (_c = (c), ((_c == ' ') || (_c == '\t') || (_c == '\n')))

#define FREE(p) (void)free((void*)(p))

const char *FILENAME = (const char *)NULL;
const char *FS = (const char *)NULL;
const char *RS = (const char *)NULL;

size_t FNR = 0;
awk_int_t RLENGTH = -1L;
awk_int_t RSTART = 0L;

static int _awk_lib_initialized = 0;

#define ENSURE_INITIALIZATION()	do { if (!_awk_lib_initialized) awk_initialize(); } while (0)

#define OPTIONAL_SIGN_PATTERN		"[-+]?"

#define OPTIONAL_WHITESPACE_PATTERN	"[ \t\n\r\f\v]*"

#define EXPONENT_PATTERN		"[DdEeQq]" OPTIONAL_SIGN_PATTERN "[0-9]+"

static const char *_awk_NaN_regexp =
	"^" OPTIONAL_WHITESPACE_PATTERN OPTIONAL_SIGN_PATTERN
	"("
		"[QqSs]?[Nn][Aa][Nn][QqSs]?"
		"|"
		"[?]+[.][?0]+" EXPONENT_PATTERN
		"|"
		"[?]+[.][?0]+"
	")" OPTIONAL_WHITESPACE_PATTERN "$" ;

static const char *_awk_negative_infinity_regexp =
	"^" OPTIONAL_WHITESPACE_PATTERN
	"("
		"-[Ii][Nn][Ff]"
		"|"
		"-[Ii][Nn][Ff][Ii][Nn][Ii][Tt][Yy]"
		"|"
		"-+[.]-0+" EXPONENT_PATTERN
		"|"
		"-+[.]-0+"
	")"
	OPTIONAL_WHITESPACE_PATTERN "$" ;

static const char *_awk_positive_infinity_regexp =
	"^" OPTIONAL_WHITESPACE_PATTERN
	"("
		"[+]?[Ii][Nn][Ff]"
		"|"
		"[+]?[Ii][Nn][Ff][Ii][Nn][Ii][Tt][Yy]"
		"|"
		"[+]+[.][+]0+" EXPONENT_PATTERN
		"|"
		"[+]+[.][+]0+"
	")"
	OPTIONAL_WHITESPACE_PATTERN "$" ;

static const char *CNULL = (const char *)NULL;
static FILE *FNULL = (FILE *) NULL;

static const char *_awk_concat(const char *s1, const char *s2,
			       const char *s3);
static void _awk_error(const char *s);
static FILE *_awk_file_to_fp(const char *filename);
static const char *_awk_find_char(const char *s, int c);
static int _awk_gsub(regexp * compiled_regexp, const char *replacement,
		     char **target);
static int _awk_match(const char *source, regexp * compiled_regexp);
static FILE *_awk_open_infile(const char *filename);
static int _awk_regmatch(const char *s, const char *regexp_pattern);
static const char *_awk_skip_nonwhite(const char *s);
static const char *_awk_skip_white(const char *s);
static size_t _awk_split_complex(const char *s, const char ***parts,
			      const char *field_separator);
static size_t _awk_split_regexp(const char *s, const char ***parts,
			     const char *field_separator);
static size_t _awk_split_simple(const char *s, const char ***parts);
static void regfree(regexp *s);

typedef struct
{
    const char *name;
    FILE *fp;
}
filetable_t;

#if !defined(MAXOPENFILES)
#define MAXOPENFILES 2
#endif

#define CHUNKSIZE 256

static filetable_t FileTable[MAXOPENFILES];


static const char *
_awk_concat(const char *s1, const char *s2, const char *s3)
{
    /*******************************************************************
      Return a newly-allocated string containing the concatenation of
      strings S1, S2, and S3.

      Terminate with a fatal error if memory cannot be allocated.
    *******************************************************************/

    char *t;
    size_t len1;
    size_t len2;
    size_t len3;

    len1 = strlen(s1);
    len2 = strlen(s2);
    len3 = strlen(s3);
    t = awk_padstr(s1, len2 + len3);
    (void)strcpy(&t[len1], s2);
    (void)strcpy(&t[len1 + len2], s3);

    return ((const char *)t);
}


static void
_awk_error(const char *s)
{
    /*******************************************************************
      Report a fatal error and terminate.
    *******************************************************************/

    (void)fprintf(stderr, "FATAL ERROR: %s\n", s);
    exit(EXIT_FAILURE);
}


static FILE *
_awk_file_to_fp(const char *filename)
{
    /*******************************************************************
      Return the file pointer associated with FILENAME, or NULL if
      none.
    *******************************************************************/

    size_t k;

    for (k = 0; k < MAXOPENFILES; ++k)
    {
	if ((FileTable[k].name != CNULL)
	    && (strcmp(FileTable[k].name, filename) == 0))
	    return (FileTable[k].fp);
    }

    return (FNULL);
}


static const char *
_awk_find_char(const char *s, int c)
{
    /*******************************************************************
      Return a pointer to the next occurrence of C or NUL in S,
      whichever comes first.
    *******************************************************************/

    while (*s && ((int)(*s) != c))
	++s;

    return (s);
}


static int
_awk_gsub(regexp * compiled_regexp, const char *replacement, char **target)
{
    /*******************************************************************
      Substitute all leftmost longest substrings in target matching
      COMPILED_REGEXP with REPLACEMENT, updating TARGET to point to
      the new string.  Any storage previously pointed to by TARGET is
      freed, and thus, must have originally been allocated by malloc().

      Return the number of substitions made.
    *******************************************************************/

    int nsub;

    nsub = 0;

    if (_awk_match(*target, compiled_regexp))
    {
	const char *first;
	char *rest;

	first = awk_substr(*target, 1, RSTART - 1);
	rest = (char *)awk_substr(*target, RSTART + RLENGTH, LONG_MAX);
	nsub += _awk_gsub(compiled_regexp, replacement, &rest);
	awk_free_string(*target);
	*target = (char *)_awk_concat(first, replacement, rest);
	awk_free_string(rest);
	awk_free_string(first);
    }

    return (nsub);
}


static int
_awk_match(const char *source, regexp * compiled_regexp)
{
    /*******************************************************************
      Search SOURCE for the longest leftmost substring matched by
      COMPILED_REGEXP.

      If a match is found, set the global variable RSTART to the index,
      and RLENGTH to the length of the matched substring.

      If there is no match, then set RSTART to 0, and RLENGTH to -1.

      Return RSTART (one, if it starts at the beginning of source).
    *******************************************************************/

    if (regexec(compiled_regexp, source))
    {
	RSTART = 1 + (size_t) (compiled_regexp->startp[0] - source);
	RLENGTH =
	    (awk_int_t) (compiled_regexp->endp[0] -
			 compiled_regexp->startp[0]);
    }
    else
    {
	RSTART = 0;
	RLENGTH = -1;
    }
    return (RSTART);
}


static FILE *
_awk_open_infile(const char *filename)
{
    /*******************************************************************
      Open the input file FILENAME, creating an entry for it in
      FileTable[], and return a FILE* pointer for it.  That pointer
      may be NULL, if the file could not be opened, or too many
      files are already open.
    *******************************************************************/

    size_t k;
    FILE *fp;

    if (strcmp(filename,"-") == 0)
	fp = stdin;
    else if (strcmp(filename,"/dev/stdin") == 0)
	fp = stdin;
    else if (strcmp(filename,"/dev/fd/0") == 0)
	fp = stdin;
    else
	fp = fopen(filename, "r");
    if (fp != FNULL)
    {
	for (k = 0; k < MAXOPENFILES; ++k)
	{
	    if (FileTable[k].name == CNULL)
	    {
		FileTable[k].name = awk_dupstr(filename);
		FileTable[k].fp = fp;
		break;
	    }
	}
	if (k == MAXOPENFILES)
	{			/* too many open input files */
	    (void)fclose(fp);
	    fp = (FILE *) NULL;
	}
    }
    return (fp);
}


static int
_awk_regmatch(const char *s, const char *regexp_pattern)
{
    /*******************************************************************
      Return 1 if S matches regular expression REGEXP, and 0 otherwise.
    *******************************************************************/

    int result;
    regexp *compiled_regexp;

    compiled_regexp = regcomp(regexp_pattern);
    result = regexec(compiled_regexp, s);
    regfree(compiled_regexp);

    return (result ? 1 : 0);
}


static const char *
_awk_skip_nonwhite(const char *s)
{
    /*******************************************************************
      Return a pointer to the next whitespace character in S, or its
      trailing NUL, whichever comes first.
    *******************************************************************/

    while (*s && !awk_is_white(*s))
	++s;
    return (s);
}


static const char *
_awk_skip_white(const char *s)
{
    /*******************************************************************
      Return a pointer to the next non-whitespace character in S, which
      might be its trailing NUL.
    *******************************************************************/

    while (awk_is_white(*s))
	++s;
    return (s);
}


static size_t
_awk_split_complex(const char *s, const char ***parts,
		   const char *field_separator)
{
    /*******************************************************************
      Split the string S into tokens delimited by the single character
      in FIELD_SEPARATOR.  Return them in the newly-allocated array
      PARTS[], and return the number of tokens stored in PARTS[].

      PARTS[] should ultimately freed by a call to awk_free_table().
    *******************************************************************/

    size_t n_parts;
    const char *t;
    awk_table_t the_table;

    awk_new_table(&the_table);
    n_parts = 0;
    t = _awk_find_char(s, field_separator[0]);

    while (t >= s)
    {
	awk_add_element(&the_table, ++n_parts,
			awk_substr(s, 1, (awk_int_t) (t - s)));
	if (*t == '\0')
	    break;
	s = t + 1;
	t = _awk_find_char(s, field_separator[0]);
    }
    *parts = the_table.table;

    return (n_parts);
}


static size_t
_awk_split_regexp(const char *s, const char ***parts,
		  const char *field_separator)
{
    /*******************************************************************
      Split the string S into tokens delimited by the regular expression
      in FIELD_SEPARATOR.  Return them in the newly-allocated array
      PARTS[], and return the number of tokens stored in PARTS[].

      PARTS[] should ultimately freed by a call to awk_free_table().
    *******************************************************************/

    size_t n_parts;
    awk_table_t the_table;
    regexp *compiled_regexp;

    awk_new_table(&the_table);
    n_parts = 0;

    compiled_regexp = regcomp(field_separator);
    while (_awk_match(s, compiled_regexp))
    {
	awk_add_element(&the_table, ++n_parts, awk_substr(s, 1, RSTART - 1));
	s = &s[-1 + RSTART + RLENGTH];
    }
    regfree(compiled_regexp);

    awk_add_element(&the_table, ++n_parts, awk_dupstr(s));
    *parts = the_table.table;

    return (n_parts);
}


static size_t
_awk_split_simple(const char *s, const char ***parts)
{
    /*******************************************************************
      Split the string S into tokens delimited by whitespace, ignoring
      leading and trailing space. Return them in the newly-allocated
      array PARTS[], and return the number of tokens stored in PARTS[].

      PARTS[] should ultimately freed by a call to awk_free_table().
    *******************************************************************/

    size_t n_parts;
    const char *t;
    awk_table_t the_table;

    awk_new_table(&the_table);
    n_parts = 0;
    s = _awk_skip_white(s);
    t = _awk_skip_nonwhite(s);
    while (t > s)
    {
	awk_add_element(&the_table, ++n_parts,
			awk_substr(s, 1, (awk_int_t) (t - s)));
	s = _awk_skip_white(t);
	t = _awk_skip_nonwhite(s);
    }
    *parts = the_table.table;

    return (n_parts);
}


void
awk_add_element(awk_table_t * the_table, size_t the_index,
		const char *the_value)
{
    /*******************************************************************
      Set THE_TABLE[THE_INDEX] = THE_VALUE, growing the table as needed.
      THE_INDEX counts from 1, and THE_TABLE[0] is allocated, but
      unused.  THE_VALUE is not duplicated to ensure a unique string:
      that is the caller's responsibility.
    *******************************************************************/

    ENSURE_INITIALIZATION();

    if (the_table->table == (const char **)NULL)
    {
	the_table->size = the_index + 1 + CHUNKSIZE;
	the_table->table =
	    (const char **)malloc(sizeof(const char *) * the_table->size);
    }
    else if (the_table->size <= the_index)
    {
	the_table->size = the_index + 1 + CHUNKSIZE;
	the_table->table =
	    (const char **)realloc(the_table->table,
				   sizeof(const char *) * the_table->size);
    }
    if (the_table->table == (const char **)NULL)
	_awk_error("awk_add_element(): out of memory");
    the_table->table[the_index] = the_value;
}


void
awk_close_infile(const char *filename)
{
    /*******************************************************************
      Close the file pointer associated with FILENAME, and clear its
      FileTable[] entry.
    *******************************************************************/

    size_t k;

    ENSURE_INITIALIZATION();

    for (k = 0; k < MAXOPENFILES; ++k)
    {
	if ((FileTable[k].name != CNULL)
	    && (strcmp(FileTable[k].name, filename) == 0))
	{
	    (void)fclose(FileTable[k].fp);
	    awk_free_string(FileTable[k].name);
	    FileTable[k].name = CNULL;
	    FileTable[k].fp = FNULL;
	    return;
	}
    }
}


const char *
awk_dupstr(const char *s)
{
    /*******************************************************************
      Return a newly-allocated string containing a copy of S.

      Terminate with a fatal error if memory cannot be allocated.
    *******************************************************************/

    ENSURE_INITIALIZATION();

    return ((const char *)awk_padstr(s, 0));
}


void
awk_free_string(const char *s)
{
    /*******************************************************************
      Free the string s, which should have been dynamically allocated
      by one of the public awk_xxx() functions.
    *******************************************************************/

    ENSURE_INITIALIZATION();

    if (s != CNULL)
	FREE(s);
}


void
awk_free_table(const char **table, size_t n)
{
    /*******************************************************************
      Free N elements of table[], and then the table itself.

      Behavior is unpredictable if table[] and its elements were not
      allocated by malloc().
    *******************************************************************/

    ENSURE_INITIALIZATION();

    if (table != (const char**)NULL)
    {
	while (n > 0)			/* free elements 1..n (NOT 0!) */
	    awk_free_string(table[n--]);

	awk_free_string((const char*)table);
    }
}


int
awk_getline(const char *infile, const char **line)
{
    /*******************************************************************
      Get the next input line from INFILE, and return a
      freshly-allocated string, LINE, containing that line (excluding
      any final newline).  The caller should call free() when the line
      is no longer needed.  The return value is -1 (error), 0
      (end-of-file), or +1 (line found).
    *******************************************************************/

    static char *buffer = (char *)NULL;
    int c;
    FILE *fp;
    size_t len_buffer = 0;
    size_t max_buffer = 0;

    ENSURE_INITIALIZATION();

    fp = _awk_file_to_fp(infile);
    if (fp == (FILE *) NULL)
	fp = _awk_open_infile(infile);
    if (fp == (FILE *) NULL)
	return (-1);
    else if (feof(fp))
	return (0);
    else
    {
	while (((c = fgetc(fp)) != EOF) && (c != '\n'))
	{
	    if (len_buffer >= max_buffer)
	    {
		max_buffer += CHUNKSIZE;
		buffer = (buffer == (char*)NULL) ? (char*)malloc(max_buffer) :
		    (char *)realloc(buffer, max_buffer);
		if (buffer == (char *)NULL)
		    _awk_error("getline(): out of memory");
	    }
	    buffer[len_buffer++] = c;
	}
	if (buffer != (char*)NULL)
	    buffer[len_buffer] = '\0';
	if ((c == EOF) && (len_buffer == 0))
	{
	    if (buffer != (char *)NULL)
		FREE(buffer);
	    buffer = (char *)NULL;
	    len_buffer = 0;
	    max_buffer = 0;
	    return (0);		/* no more data in file */
	}
	else
	{
	    *line = awk_dupstr(buffer);
	    len_buffer = 0;
	    return (1);
	}
    }
}


int
awk_gsub(const char *regular_expression, const char *replacement,
	 char **target)
{
    /*******************************************************************
      Substitute all leftmost longest substrings in target matching
      REGULAR_EXPRESSION with REPLACEMENT, updating TARGET to point to
      the new string.  Any storage previously pointed to by TARGET is
      freed, and thus, must have originally been allocated by malloc().

      Return the number of substitions made.
    *******************************************************************/

    int nsub;
    regexp *compiled_regexp;

    ENSURE_INITIALIZATION();

    compiled_regexp = regcomp(regular_expression);
    nsub = _awk_gsub(compiled_regexp, replacement, target);
    regfree(compiled_regexp);

    return (nsub);
}


void
awk_initialize(void)
{
    /*******************************************************************
      Initialize the awk compatibility library.  This function MUST be
      called before any of the other awk_xxx() functions are called;
      failure to do so will produce unpredictable results.

      On completion of use of the library, call awk_terminate() to
      clean up.

      Calls to the pair awk_initialize() and awk_terminate() must
      bracket all calls to other members of this library.
    *******************************************************************/

    FS = " ";
    RS = "\n";
    RLENGTH = -1;
    RSTART = 0;
    _awk_lib_initialized = 1;
}


int
awk_is_NaN(const char *s)
{
    /*******************************************************************
      Return 1 if S matches a pattern for a NaN (Not-a-Number), and 0
      otherwise.
    *******************************************************************/

    ENSURE_INITIALIZATION();

    return (_awk_regmatch(s,_awk_NaN_regexp));
}


int
awk_is_negative_infinity(const char *s)
{
    /*******************************************************************
      Return 1 if S matches a pattern for a negative infinity, and 0
      otherwise.
    *******************************************************************/

    ENSURE_INITIALIZATION();

    return (_awk_regmatch(s,_awk_negative_infinity_regexp));
}


int
awk_is_positive_infinity(const char *s)
{
    /*******************************************************************
      Return 1 if S matches a pattern for a positive infinity, and 0
      otherwise.
    *******************************************************************/

    ENSURE_INITIALIZATION();

    return (_awk_regmatch(s,_awk_positive_infinity_regexp));
}


const char *
awk_long_to_string(long n)
{
    /*******************************************************************
      Return a newly-allocated string containing the decimal
      representation of N.
    *******************************************************************/

    char s[40 + 1];		/* long enough for 128-bit integer */

    ENSURE_INITIALIZATION();

    (void)sprintf(s, "%ld", n);
    return (awk_dupstr(s));
}


int
awk_match(const char *source, const char *regular_expression)
{
    /*******************************************************************
      Search SOURCE for the longest leftmost substring matched by
      REGULAR_EXPRESSION.

      If a match is found, set the global variable RSTART to the index,
      and RLENGTH to the length of the matched substring.

      If there is no match, then set RSTART to 0, and RLENGTH to -1.

      Return RSTART (one, if it starts at the beginning of source).
    *******************************************************************/

    regexp *compiled_regexp;

    ENSURE_INITIALIZATION();

    compiled_regexp = regcomp(regular_expression);

    (void)_awk_match(source, compiled_regexp);

    regfree(compiled_regexp);

    return (RSTART);
}


void
awk_new_table(awk_table_t * the_table)
{
    /*******************************************************************
      Initialize an awk table to empty.
    *******************************************************************/

    ENSURE_INITIALIZATION();

    the_table->table = (const char **)NULL;
    the_table->size = 0;
}


char *
awk_padstr(const char *s, size_t extra)
{
    /*******************************************************************
      Return a newly-allocated string containing a copy of S, with
      EXTRA additional slots at the end.

      Terminate with a fatal error if memory cannot be allocated.
    *******************************************************************/

    size_t n;
    char *t;

    ENSURE_INITIALIZATION();

    n = (s == (char*)NULL) ? 0 : strlen(s);
    t = (char *)malloc(n + 1 + extra);
    if (t == (char *)NULL)
	_awk_error("awk_padstr(): out of memory");
    if (s == (char*)NULL)
	*t = '\0';
    else
	strcpy(t, s);

    return (t);
}


size_t
awk_split(const char *s, const char ***parts, const char *field_separator)
{
    /*******************************************************************
      Split the string S into tokens delimited by separators defined by
      FIELD_SEPARATOR, returning them in the newly-allocated array
      PARTS[], and return the number of tokens stored in PARTS[].

      If FIELD_SEPARATOR is NULL, use the current value of the global
      variable FS in its place.

      When FIELD_SEPARATOR is " " (a single blank), then leading and
      trailing whitespace is ignored, and the tokens are separated by
      runs of whitespace (blanks, tabs, or newlines).

      This quote from Arnold Robbins ``Effective AWK Programming''
      (also available as the online gawk manual) is significant:

	``Normally, fields are separated by whitespace sequences (spaces,
	tabs and newlines), not by single spaces: two spaces in a row do
	not delimit an empty field.  The default value of the field
	separator `FS' is a string containing a single space, `" "'.  If
	this value were interpreted in the usual way, each space
	character would separate fields, so two spaces in a row would
	make an empty field between them.  The reason this does not
	happen is that a single space as the value of `FS' is a special
	case: it is taken to specify the default manner of delimiting
	fields.

	If `FS' is any other single character, such as `","', then each
	occurrence of that character separates two fields.  Two
	consecutive occurrences delimit an empty field.  If the
	character occurs at the beginning or the end of the line, that
	too delimits an empty field.  The space character is the only
	single character which does not follow these rules.''

      When FIELD_SEPARATOR is longer than a single character, it is
      taken to be a regular expression.

      PARTS[] should ultimately freed by a call to awk_free_table().
    *******************************************************************/

    ENSURE_INITIALIZATION();

    if (field_separator == CNULL)
	field_separator = FS;

    if (strcmp(field_separator, " ") == 0)
	return (_awk_split_simple(s, parts));
    else if (strlen(field_separator) == 1)
	return (_awk_split_complex(s, parts, field_separator));
    else
	return (_awk_split_regexp(s, parts, field_separator));
}


int
awk_sub(const char *regular_expression, const char *replacement,
	char **target)
{
    /*******************************************************************
      Substitute the leftmost longest substring in TARGET matching
      REGULAR_EXPRESSION with replacement, updating TARGET to point to
      the new string.  Any storage previously pointed to by TARGET is
      freed, and thus, must have originally been allocated by malloc().

      Return the number of substitions made.
    *******************************************************************/

    int nsub;

    ENSURE_INITIALIZATION();

    nsub = 0;
    if (awk_match(*target, regular_expression))
    {
	const char *first;
	const char *rest;

	first = awk_substr(*target, 1, RSTART - 1);
	rest = awk_substr(*target, RSTART + RLENGTH, LONG_MAX);
	awk_free_string(*target);
	*target = (char *)_awk_concat(first, replacement, rest);
	awk_free_string(first);
	awk_free_string(rest);
	nsub++;
    }

    return (nsub);
}


const char *
awk_substr(const char *source, awk_int_t start, awk_int_t length)
{
    /*******************************************************************
      Return a newly-allocated string containing a substring of SOURCE,
      beginning at START (indexes begin at 1), of LENGTH characters, or
      fewer if the string ends early.

      Design note: because of the common awk idiom,
      substr(s,1,RSTART-1), for the string prefixing the current match,
      it is necessary that the LENGTH argument be a signed integer,
      rather than the usual unsigned integer type size_t used in the
      strxxx() family.
    *******************************************************************/

    size_t n;
    char *t;

    ENSURE_INITIALIZATION();

    n = strlen(&source[start - 1]);

    if (length <= 0)
	n = 0;
    else if ((size_t) length < n)
	n = (size_t) length;

    t = awk_padstr("", n);
    if (n > 0)
	(void)strncpy(t, &source[start - 1], n);
    t[n] = '\0';

    return ((const char *)t);
}


double
awk_string_to_double(const char *s)
{
    ENSURE_INITIALIZATION();

    return (strtod(s,(char**)NULL));
}


long
awk_string_to_long(const char *s)
{
    ENSURE_INITIALIZATION();

    return (strtol(s,(char**)NULL,10));
}


unsigned long
awk_string_to_unsigned_long(const char *s)
{
    ENSURE_INITIALIZATION();

    return (strtoul(s,(char**)NULL,10));
}


void
awk_terminate(void)
{
    /*******************************************************************
      Terminate use of the awk compatibility library.  This function
      MUST be called if memory leaks are to be avoided, and it MUST not
      be called if awk_initialize() has not be called.

      Calls to the pair awk_initialize() and awk_terminate() must
      bracket all calls to other members of this library.
    *******************************************************************/

    ENSURE_INITIALIZATION();

    _awk_lib_initialized = 0;
}


const char *
awk_tolower(const char *s)
{
    /*******************************************************************
      Return a copy of S converted to lowercase.
    *******************************************************************/

    const char *t;
    char *u;

    ENSURE_INITIALIZATION();

    t = awk_dupstr(s);
    for (u = (char *)t; *u; ++u)
    {
	if (isupper(*u))
	    *u = tolower(*u);
    }

    return (t);
}


const char *
awk_toupper(const char *s)
{
    /*******************************************************************
      Return a copy of S converted to uppercase.
    *******************************************************************/

    const char *t;
    char *u;

    ENSURE_INITIALIZATION();

    t = awk_dupstr(s);
    for (u = (char *)t; *u; ++u)
    {
	if (islower(*u))
	    *u = toupper(*u);
    }

    return (t);
}


const char *
awk_unsigned_long_to_string(unsigned long n)
{
    /*******************************************************************
      Return a newly-allocated string containing the decimal
      representation of N.
    *******************************************************************/

    char s[40 + 1];		/* long enough for 128-bit integer */

    ENSURE_INITIALIZATION();

    (void)sprintf(s, "%lu", n);
    return (awk_dupstr(s));
}


static void
regfree(regexp *s)
{
    /*******************************************************************
      Free the compiled regular expression, S, which should have been
      returned from an earlier call to regcomp().

      This function should logically be part of the regexp() package,
      but is sadly missing from it.
    *******************************************************************/

    FREE(s);
}
