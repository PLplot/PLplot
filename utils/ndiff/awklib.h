#if !defined(AWKLIB_H_)
#define AWKLIB_H_

#include <confix.h>

#if HAVE_CTYPE_H
#include <ctype.h>
#endif

#if HAVE_LIMITS_H
#include <limits.h>
#endif

#if HAVE_STDDEF_H
#include <stddef.h>
#endif

#if HAVE_STDIO_H
#include <stdio.h>
#endif

#if HAVE_STDLIB_H
#include <stdlib.h>
#endif

#if HAVE_STRING_H
#include <string.h>
#endif

typedef struct
{
    const char **table;
    size_t size;
}
awk_table_t;

typedef long awk_int_t;

/* Declarations for awk-like support functions */
extern const char *FILENAME;
extern const char *FS;
extern const char *RS;
extern size_t FNR;
extern awk_int_t RLENGTH;
extern awk_int_t RSTART;

/***********************************************************************
The awklib programming conventions guarantee that global identifiers
are unique in their first six characters, ignoring letter case.
However, short names are a severe barrier to readability, so use the
preprocessor to create shorter names that only the compiler and linker
need to see.
***********************************************************************/

#define awk_free_string			a_f_string
#define awk_free_table			a_f_table
#define awk_string_to_double		a_s_double
#define awk_string_to_long		a_s_long
#define awk_string_to_unsigned_long	a_s_ulong
#define awk_sub				a_sub
#define awk_substr			a_substr
#define awk_tolower			a_tolower
#define awk_toupper			a_toupper

extern void awk_add_element(awk_table_t * the_table, size_t the_index,
			    const char *the_value);
extern void awk_close_infile(const char *filename);
extern const char *awk_dupstr(const char *s);
extern void awk_free_string(const char *s);
extern void awk_free_table(const char **table, size_t n);
extern int awk_getline(const char *infile, const char **line);
extern int awk_gsub(const char *regexp, const char *replacement,
		    char **target);
extern void awk_initialize(void);
extern int awk_is_NaN(const char *s);
extern int awk_is_negative_infinity(const char *s);
extern int awk_is_positive_infinity(const char *s);
extern const char *awk_long_to_string(long n);
extern int awk_match(const char *source, const char *regexp);
extern void awk_new_table(awk_table_t * the_table);
extern char *awk_padstr(const char *s, size_t extra);
extern int awk_sub(const char *regexp, const char *replacement,
		   char **target);
extern const char *awk_substr(const char *source, awk_int_t start,
			      awk_int_t length);
extern size_t awk_split(const char *s, const char ***parts,
			const char *regexp);
extern double awk_string_to_double(const char *s);
extern long awk_string_to_long(const char *s);
extern unsigned long awk_string_to_unsigned_long(const char *s);
extern void awk_terminate(void);
extern const char *awk_tolower(const char *s);
extern const char *awk_toupper(const char *s);
extern const char *awk_unsigned_long_to_string(unsigned long n);
#endif /* !defined(AWKLIB_H_) */
