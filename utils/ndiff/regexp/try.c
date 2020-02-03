/*
 * Simple test program for regexp(3) stuff.  Knows about debugging hooks.
 *
 *	Copyright (c) 1986 by University of Toronto.
 *	Written by Henry Spencer.  Not derived from licensed software.
 *
 *	Permission is granted to anyone to use this software for any
 *	purpose on any computer system, and to redistribute it freely,
 *	subject to the following restrictions:
 *
 *	1. The author is not responsible for the consequences of use of
 *		this software, no matter how awful, even if they arise
 *		from defects in it.
 *
 *	2. The origin of this software must not be misrepresented, either
 *		by explicit claim or by omission.
 *
 *	3. Altered versions must be plainly marked as such, and must not
 *		be misrepresented as being the original software.
 *
 * Usage: try re [string [output [-]]]
 * The re is compiled and dumped, regexeced against the string, the result
 * is applied to output using regsub().  The - triggers a running narrative
 * from regexec().  Dumping and narrative don't happen unless DEBUG.
 *
 * If there are no arguments, stdin is assumed to be a stream of lines with
 * five fields:  a r.e., a string to match it against, a result code, a
 * source string for regsub, and the proper result.  Result codes are 'c'
 * for compile failure, 'y' for match success, 'n' for match failure.
 * Field separator is tab.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <regexp.h>

#ifdef ERRAVAIL
char *progname;
extern char *mkprogname(const char *);
#endif

#ifdef DEBUG
extern int regnarrate;
#endif

char buf[BUFSIZ];

int errreport = 0;		/* Report errors via errseen? */
const char *errseen = NULL;	/* Error message. */
int status = 0;			/* Exit status. */

#if __STDC__ || __cplusplus
#define ARGS(plist)	plist
#else
#define ARGS(plist)	()
#endif

int		main ARGS((int argc_, char *argv_[]));
void		complain ARGS((const char *s1,const char *s2));
void		multiple ARGS((void));
void		regerror ARGS((const char *s));
void		try_re ARGS((char **fields));

#ifndef ERRAVAIL
void		error ARGS((const char *s1, const char *s2));
#endif

/* ARGSUSED */
int
main(int argc, char *argv[])
{
	regexp *r;
	int i;

#ifdef ERRAVAIL
	progname = mkprogname(argv[0]);
#endif

	if (argc == 1) {
		multiple();
		exit(status);
	}

	r = regcomp(argv[1]);
	if (r == NULL)
		error("regcomp failure", "");
#ifdef DEBUG
	regdump(r);
	if (argc > 4)
		regnarrate++;
#endif
	if (argc > 2) {
		i = regexec(r, argv[2]);
		printf("%d", i);
		for (i = 1; i < NSUBEXP; i++)
			if (r->startp[i] != NULL && r->endp[i] != NULL)
				printf(" \\%d", i);
		printf("\n");
	}
	if (argc > 3) {
		regsub(r, argv[3], buf);
		printf("%s\n", buf);
	}
	return(status);
}

void
regerror(const char *s)
{
	if (errreport)
		errseen = s;
	else
		error(s, "");
}

#ifndef ERRAVAIL
void
error(const char *s1, const char *s2)
{
	fprintf(stderr, "regexp: ");
	fprintf(stderr, s1, s2);
	fprintf(stderr, "\n");
	exit(EXIT_FAILURE);
}
#endif

int lineno;

regexp badregexp;		/* Implicit init to 0. */

void
multiple(void)
{
	char rbuf[BUFSIZ];
	char *field[5];
	char *scan;
	int i;
	regexp *r;

	errreport = 1;
	lineno = 0;
	while (fgets(rbuf, sizeof(rbuf), stdin) != NULL) {
		rbuf[strlen(rbuf)-1] = '\0';	/* Dispense with \n. */
		lineno++;
		scan = rbuf;
		for (i = 0; i < 5; i++) {
			field[i] = scan;
			if (field[i] == NULL) {
				complain("bad testfile format", "");
				exit(EXIT_FAILURE);
			}
			scan = strchr(scan, '\t');
			if (scan != NULL)
				*scan++ = '\0';
		}
		try_re(field);
	}

	/* And finish up with some internal testing... */
	lineno = 9990;
	errseen = NULL;
	if (regcomp((char *)NULL) != NULL || errseen == NULL)
		complain("regcomp(NULL) doesn't complain", "");
	lineno = 9991;
	errseen = NULL;
	if (regexec((regexp *)NULL, "foo") || errseen == NULL)
		complain("regexec(NULL, ...) doesn't complain", "");
	lineno = 9992;
	r = regcomp("foo");
	if (r == NULL) {
		complain("regcomp(\"foo\") fails", "");
		return;
	}
	lineno = 9993;
	errseen = NULL;
	if (regexec(r, (char *)NULL) || errseen == NULL)
		complain("regexec(..., NULL) doesn't complain", "");
	lineno = 9994;
	errseen = NULL;
	regsub((regexp *)NULL, "foo", rbuf);
	if (errseen == NULL)
		complain("regsub(NULL, ..., ...) doesn't complain", "");
	lineno = 9995;
	errseen = NULL;
	regsub(r, (char *)NULL, rbuf);
	if (errseen == NULL)
		complain("regsub(..., NULL, ...) doesn't complain", "");
	lineno = 9996;
	errseen = NULL;
	regsub(r, "foo", (char *)NULL);
	if (errseen == NULL)
		complain("regsub(..., ..., NULL) doesn't complain", "");
	lineno = 9997;
	errseen = NULL;
	if (regexec(&badregexp, "foo") || errseen == NULL)
		complain("regexec(nonsense, ...) doesn't complain", "");
	lineno = 9998;
	errseen = NULL;
	regsub(&badregexp, "foo", rbuf);
	if (errseen == NULL)
		complain("regsub(nonsense, ..., ...) doesn't complain", "");
}

void
try_re(char **fields)
{
	regexp *r;
	char dbuf[BUFSIZ];

	errseen = NULL;
	r = regcomp(fields[0]);
	if (r == NULL) {
		if (*fields[2] != 'c')
			complain("regcomp failure in `%s'", fields[0]);
		return;
	}
	if (*fields[2] == 'c') {
		complain("unexpected regcomp success in `%s'", fields[0]);
		free((char *)r);
		return;
	}
	if (!regexec(r, fields[1])) {
		if (*fields[2] != 'n')
			complain("regexec failure in `%s'", "");
		free((char *)r);
		return;
	}
	if (*fields[2] == 'n') {
		complain("unexpected regexec success", "");
		free((char *)r);
		return;
	}
	errseen = NULL;
	regsub(r, fields[3], dbuf);
	if (errseen != NULL) {
		complain("regsub complaint", "");
		free((char *)r);
		return;
	}
	if (strcmp(dbuf, fields[4]) != 0)
		complain("regsub result `%s' wrong", dbuf);
	free((char *)r);
}

void
complain(const char *s1, const char *s2)
{
	fprintf(stderr, "try: %d: ", lineno);
	fprintf(stderr, s1, s2);
	fprintf(stderr, " (%s)\n", (errseen != NULL) ? errseen : "");
	status = 1;
}
