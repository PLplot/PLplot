#line 1 "./../tk/plframe.c"




























































































































































































#line 1 "./../tk/plserver.h"









































#line 1 "./../../tmp\\plDevs.h"

































































#line 67 "./../../tmp\\plDevs.h"


#line 70 "./../../tmp\\plDevs.h"
#line 43 "./../tk/plserver.h"
#line 1 "./../tk/pltkd.h"


















































#line 1 "./../../include\\plplotP.h"






































































































#line 1 "./../../tmp\\plConfig.h"





































































































#line 103 "./../../tmp\\plConfig.h"
#line 104 "./../../include\\plplotP.h"


typedef char * caddr_t;
#line 108 "./../../include\\plplotP.h"



#line 1 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\ctype.h"














#pragma once
#line 17 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\ctype.h"






#line 24 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\ctype.h"














#line 39 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\ctype.h"
#line 40 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\ctype.h"









#line 50 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\ctype.h"
#line 51 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\ctype.h"














#line 66 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\ctype.h"
#line 67 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\ctype.h"
#line 68 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\ctype.h"






#line 75 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\ctype.h"


typedef unsigned short wchar_t;

#line 80 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\ctype.h"



typedef wchar_t wint_t;
typedef wchar_t wctype_t;

#line 87 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\ctype.h"



#line 91 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\ctype.h"
#line 92 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\ctype.h"































 extern unsigned short _ctype[];









#line 134 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\ctype.h"

 extern unsigned short *_pctype;

 extern wctype_t *_pwctype;
#line 139 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\ctype.h"

#line 141 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\ctype.h"

#line 143 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\ctype.h"

#line 145 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\ctype.h"







				












 int __cdecl _isctype(int, int);
 int __cdecl isalpha(int);
 int __cdecl isupper(int);
 int __cdecl islower(int);
 int __cdecl isdigit(int);
 int __cdecl isxdigit(int);
 int __cdecl isspace(int);
 int __cdecl ispunct(int);
 int __cdecl isalnum(int);
 int __cdecl isprint(int);
 int __cdecl isgraph(int);
 int __cdecl iscntrl(int);
 int __cdecl toupper(int);
 int __cdecl tolower(int);
 int __cdecl _tolower(int);
 int __cdecl _toupper(int);
 int __cdecl __isascii(int);
 int __cdecl __toascii(int);
 int __cdecl __iscsymf(int);
 int __cdecl __iscsym(int);

#line 187 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\ctype.h"








 int __cdecl iswalpha(wint_t);
 int __cdecl iswupper(wint_t);
 int __cdecl iswlower(wint_t);
 int __cdecl iswdigit(wint_t);
 int __cdecl iswxdigit(wint_t);
 int __cdecl iswspace(wint_t);
 int __cdecl iswpunct(wint_t);
 int __cdecl iswalnum(wint_t);
 int __cdecl iswprint(wint_t);
 int __cdecl iswgraph(wint_t);
 int __cdecl iswcntrl(wint_t);
 int __cdecl iswascii(wint_t);
 int __cdecl isleadbyte(int);

 wchar_t __cdecl towupper(wchar_t);
 wchar_t __cdecl towlower(wchar_t);

 int __cdecl iswctype(wint_t, wctype_t);


 int __cdecl is_wctype(wint_t, wctype_t);



#line 220 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\ctype.h"
#line 221 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\ctype.h"





























#line 251 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\ctype.h"

 extern int __mb_cur_max;
#line 254 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\ctype.h"

#line 256 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\ctype.h"

#line 258 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\ctype.h"






























#line 289 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\ctype.h"







































#line 329 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\ctype.h"
#line 330 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\ctype.h"









































#line 372 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\ctype.h"

#line 374 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\ctype.h"






#line 381 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\ctype.h"

























#line 407 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\ctype.h"

#line 409 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\ctype.h"

#line 411 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\ctype.h"






#line 418 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\ctype.h"
#line 112 "./../../include\\plplotP.h"
#line 1 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\math.h"















#pragma once
#line 18 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\math.h"






#line 25 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\math.h"







#pragma pack(push,8)
#line 34 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\math.h"


















































#line 85 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\math.h"







struct _exception {
        int type;       
        char *name;     
        double arg1;    
        double arg2;    
        double retval;  
        } ;


#line 102 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\math.h"







struct _complex {
        double x,y; 
        } ;




#line 117 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\math.h"


#line 120 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\math.h"
#line 121 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\math.h"































 extern double _HUGE;
#line 154 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\math.h"
#line 155 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\math.h"



























#line 183 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\math.h"
        int     __cdecl abs(int);
        double  __cdecl acos(double);
        double  __cdecl asin(double);
        double  __cdecl atan(double);
        double  __cdecl atan2(double, double);
        double  __cdecl cos(double);
        double  __cdecl cosh(double);
        double  __cdecl exp(double);
        double  __cdecl fabs(double);
        double  __cdecl fmod(double, double);
        long    __cdecl labs(long);
        double  __cdecl log(double);
        double  __cdecl log10(double);
        double  __cdecl pow(double, double);
        double  __cdecl sin(double);
        double  __cdecl sinh(double);
        double  __cdecl tan(double);
        double  __cdecl tanh(double);
        double  __cdecl sqrt(double);
#line 203 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\math.h"
 double  __cdecl atof(const char *);
 double  __cdecl _cabs(struct _complex);
 double  __cdecl ceil(double);
 double  __cdecl floor(double);
 double  __cdecl frexp(double, int *);
 double  __cdecl _hypot(double, double);
 double  __cdecl _j0(double);
 double  __cdecl _j1(double);
 double  __cdecl _jn(int, double);
 double  __cdecl ldexp(double, int);
        int     __cdecl _matherr(struct _exception *);
 double  __cdecl modf(double, double *);

 double  __cdecl _y0(double);
 double  __cdecl _y1(double);
 double  __cdecl _yn(int, double);








































#line 260 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\math.h"

































































































































#line 390 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\math.h"
#line 391 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\math.h"
#line 392 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\math.h"














#line 407 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\math.h"



























 extern double HUGE;

 double  __cdecl cabs(struct _complex);
 double  __cdecl hypot(double, double);
 double  __cdecl j0(double);
 double  __cdecl j1(double);
 double  __cdecl jn(int, double);
        int     __cdecl _matherr(struct _exception *);
 double  __cdecl y0(double);
 double  __cdecl y1(double);
 double  __cdecl yn(int, double);

#line 447 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\math.h"
#line 448 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\math.h"

#line 450 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\math.h"




































































































































































































#pragma pack(pop)
#line 648 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\math.h"

#line 650 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\math.h"

#line 113 "./../../include\\plplotP.h"
#line 1 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\string.h"















#pragma once
#line 18 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\string.h"






#line 25 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\string.h"


















































#line 76 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\string.h"



typedef unsigned int size_t;

#line 82 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\string.h"







#line 90 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\string.h"




#line 95 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\string.h"








#line 104 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\string.h"
#line 105 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\string.h"














        void *  __cdecl memcpy(void *, const void *, size_t);
        int     __cdecl memcmp(const void *, const void *, size_t);
        void *  __cdecl memset(void *, int, size_t);
        char *  __cdecl _strset(char *, int);
        char *  __cdecl strcpy(char *, const char *);
        char *  __cdecl strcat(char *, const char *);
        int     __cdecl strcmp(const char *, const char *);
        size_t  __cdecl strlen(const char *);
#line 128 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\string.h"
 void *  __cdecl _memccpy(void *, const void *, int, unsigned int);
 void *  __cdecl memchr(const void *, int, size_t);
 int     __cdecl _memicmp(const void *, const void *, unsigned int);





 void *  __cdecl memmove(void *, const void *, size_t);
#line 138 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\string.h"


 char *  __cdecl strchr(const char *, int);
 int     __cdecl _strcmpi(const char *, const char *);
 int     __cdecl _stricmp(const char *, const char *);
 int     __cdecl strcoll(const char *, const char *);
 int     __cdecl _stricoll(const char *, const char *);
 int     __cdecl _strncoll(const char *, const char *, size_t);
 int     __cdecl _strnicoll(const char *, const char *, size_t);
 size_t  __cdecl strcspn(const char *, const char *);
 char *  __cdecl _strdup(const char *);
 char *  __cdecl _strerror(const char *);
 char *  __cdecl strerror(int);
 char *  __cdecl _strlwr(char *);
 char *  __cdecl strncat(char *, const char *, size_t);
 int     __cdecl strncmp(const char *, const char *, size_t);
 int     __cdecl _strnicmp(const char *, const char *, size_t);
 char *  __cdecl strncpy(char *, const char *, size_t);
 char *  __cdecl _strnset(char *, int, size_t);
 char *  __cdecl strpbrk(const char *, const char *);
 char *  __cdecl strrchr(const char *, int);
 char *  __cdecl _strrev(char *);
 size_t  __cdecl strspn(const char *, const char *);
 char *  __cdecl strstr(const char *, const char *);
 char *  __cdecl strtok(char *, const char *);
 char *  __cdecl _strupr(char *);
 size_t  __cdecl strxfrm (char *, const char *, size_t);









#line 175 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\string.h"





















 void * __cdecl memccpy(void *, const void *, int, unsigned int);
 int __cdecl memicmp(const void *, const void *, unsigned int);
 int __cdecl strcmpi(const char *, const char *);
 int __cdecl stricmp(const char *, const char *);
 char * __cdecl strdup(const char *);
 char * __cdecl strlwr(char *);
 int __cdecl strnicmp(const char *, const char *, size_t);
 char * __cdecl strnset(char *, int, size_t);
 char * __cdecl strrev(char *);
        char * __cdecl strset(char *, int);
 char * __cdecl strupr(char *);

#line 209 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\string.h"

#line 211 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\string.h"







 wchar_t * __cdecl wcscat(wchar_t *, const wchar_t *);
 wchar_t * __cdecl wcschr(const wchar_t *, wchar_t);
 int __cdecl wcscmp(const wchar_t *, const wchar_t *);
 wchar_t * __cdecl wcscpy(wchar_t *, const wchar_t *);
 size_t __cdecl wcscspn(const wchar_t *, const wchar_t *);
 size_t __cdecl wcslen(const wchar_t *);
 wchar_t * __cdecl wcsncat(wchar_t *, const wchar_t *, size_t);
 int __cdecl wcsncmp(const wchar_t *, const wchar_t *, size_t);
 wchar_t * __cdecl wcsncpy(wchar_t *, const wchar_t *, size_t);
 wchar_t * __cdecl wcspbrk(const wchar_t *, const wchar_t *);
 wchar_t * __cdecl wcsrchr(const wchar_t *, wchar_t);
 size_t __cdecl wcsspn(const wchar_t *, const wchar_t *);
 wchar_t * __cdecl wcsstr(const wchar_t *, const wchar_t *);
 wchar_t * __cdecl wcstok(wchar_t *, const wchar_t *);

 wchar_t * __cdecl _wcsdup(const wchar_t *);
 int __cdecl _wcsicmp(const wchar_t *, const wchar_t *);
 int __cdecl _wcsnicmp(const wchar_t *, const wchar_t *, size_t);
 wchar_t * __cdecl _wcsnset(wchar_t *, wchar_t, size_t);
 wchar_t * __cdecl _wcsrev(wchar_t *);
 wchar_t * __cdecl _wcsset(wchar_t *, wchar_t);

 wchar_t * __cdecl _wcslwr(wchar_t *);
 wchar_t * __cdecl _wcsupr(wchar_t *);
 size_t __cdecl wcsxfrm(wchar_t *, const wchar_t *, size_t);
 int __cdecl wcscoll(const wchar_t *, const wchar_t *);
 int __cdecl _wcsicoll(const wchar_t *, const wchar_t *);
 int __cdecl _wcsncoll(const wchar_t *, const wchar_t *, size_t);
 int __cdecl _wcsnicoll(const wchar_t *, const wchar_t *, size_t);






















 wchar_t * __cdecl wcsdup(const wchar_t *);
 int __cdecl wcsicmp(const wchar_t *, const wchar_t *);
 int __cdecl wcsnicmp(const wchar_t *, const wchar_t *, size_t);
 wchar_t * __cdecl wcsnset(wchar_t *, wchar_t, size_t);
 wchar_t * __cdecl wcsrev(wchar_t *);
 wchar_t * __cdecl wcsset(wchar_t *, wchar_t);
 wchar_t * __cdecl wcslwr(wchar_t *);
 wchar_t * __cdecl wcsupr(wchar_t *);
 int __cdecl wcsicoll(const wchar_t *, const wchar_t *);

#line 280 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\string.h"

#line 282 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\string.h"


#line 285 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\string.h"

#line 287 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\string.h"





#line 293 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\string.h"
#line 114 "./../../include\\plplotP.h"









#line 124 "./../../include\\plplotP.h"










#line 135 "./../../include\\plplotP.h"




#line 1 "./../../include\\plplot.h"


























































































































#line 1 "./../../tmp\\plConfig.h"






































































































#line 124 "./../../include\\plplot.h"
































#line 1 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdio.h"















#pragma once
#line 18 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdio.h"






#line 25 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdio.h"







#pragma pack(push,8)
#line 34 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdio.h"

















































#line 84 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdio.h"




















#line 105 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdio.h"









typedef char *  va_list;
#line 116 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdio.h"

#line 118 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdio.h"






#line 125 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdio.h"

#line 127 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdio.h"






























#line 158 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdio.h"





struct _iobuf {
        char *_ptr;
        int   _cnt;
        char *_base;
        int   _flag;
        int   _file;
        int   _charbuf;
        int   _bufsiz;
        char *_tmpfname;
        };
typedef struct _iobuf FILE;

#line 176 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdio.h"











#line 188 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdio.h"










#line 199 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdio.h"



















































#line 251 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdio.h"

 extern FILE _iob[];

#line 255 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdio.h"

#line 257 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdio.h"

#line 259 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdio.h"








typedef __int64 fpos_t;







#line 276 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdio.h"


#line 279 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdio.h"




























 int __cdecl _filbuf(FILE *);
 int __cdecl _flsbuf(int, FILE *);




 FILE * __cdecl _fsopen(const char *, const char *, int);
#line 315 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdio.h"

 void __cdecl clearerr(FILE *);
 int __cdecl fclose(FILE *);
 int __cdecl _fcloseall(void);




 FILE * __cdecl _fdopen(int, const char *);
#line 325 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdio.h"

 int __cdecl feof(FILE *);
 int __cdecl ferror(FILE *);
 int __cdecl fflush(FILE *);
 int __cdecl fgetc(FILE *);
 int __cdecl _fgetchar(void);
 int __cdecl fgetpos(FILE *, fpos_t *);
 char * __cdecl fgets(char *, int, FILE *);




 int __cdecl _fileno(FILE *);
#line 339 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdio.h"

 int __cdecl _flushall(void);
 FILE * __cdecl fopen(const char *, const char *);
 int __cdecl fprintf(FILE *, const char *, ...);
 int __cdecl fputc(int, FILE *);
 int __cdecl _fputchar(int);
 int __cdecl fputs(const char *, FILE *);
 size_t __cdecl fread(void *, size_t, size_t, FILE *);
 FILE * __cdecl freopen(const char *, const char *, FILE *);
 int __cdecl fscanf(FILE *, const char *, ...);
 int __cdecl fsetpos(FILE *, const fpos_t *);
 int __cdecl fseek(FILE *, long, int);
 long __cdecl ftell(FILE *);
 size_t __cdecl fwrite(const void *, size_t, size_t, FILE *);
 int __cdecl getc(FILE *);
 int __cdecl getchar(void);
 int __cdecl _getmaxstdio(void);
 char * __cdecl gets(char *);
 int __cdecl _getw(FILE *);
 void __cdecl perror(const char *);
 int __cdecl _pclose(FILE *);
 FILE * __cdecl _popen(const char *, const char *);
 int __cdecl printf(const char *, ...);
 int __cdecl putc(int, FILE *);
 int __cdecl putchar(int);
 int __cdecl puts(const char *);
 int __cdecl _putw(int, FILE *);
 int __cdecl remove(const char *);
 int __cdecl rename(const char *, const char *);
 void __cdecl rewind(FILE *);
 int __cdecl _rmtmp(void);
 int __cdecl scanf(const char *, ...);
 void __cdecl setbuf(FILE *, char *);
 int __cdecl _setmaxstdio(int);
 int __cdecl setvbuf(FILE *, char *, int, size_t);
 int __cdecl _snprintf(char *, size_t, const char *, ...);
 int __cdecl sprintf(char *, const char *, ...);
 int __cdecl sscanf(const char *, const char *, ...);
 char * __cdecl _tempnam(const char *, const char *);
 FILE * __cdecl tmpfile(void);
 char * __cdecl tmpnam(char *);
 int __cdecl ungetc(int, FILE *);
 int __cdecl _unlink(const char *);
 int __cdecl vfprintf(FILE *, const char *, va_list);
 int __cdecl vprintf(const char *, va_list);
 int __cdecl _vsnprintf(char *, size_t, const char *, va_list);
 int __cdecl vsprintf(char *, const char *, va_list);













 FILE * __cdecl _wfsopen(const wchar_t *, const wchar_t *, int);
#line 401 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdio.h"

 wint_t __cdecl fgetwc(FILE *);
 wint_t __cdecl _fgetwchar(void);
 wint_t __cdecl fputwc(wint_t, FILE *);
 wint_t __cdecl _fputwchar(wint_t);
 wint_t __cdecl getwc(FILE *);
 wint_t __cdecl getwchar(void);
 wint_t __cdecl putwc(wint_t, FILE *);
 wint_t __cdecl putwchar(wint_t);
 wint_t __cdecl ungetwc(wint_t, FILE *);

 wchar_t * __cdecl fgetws(wchar_t *, int, FILE *);
 int __cdecl fputws(const wchar_t *, FILE *);
 wchar_t * __cdecl _getws(wchar_t *);
 int __cdecl _putws(const wchar_t *);

 int __cdecl fwprintf(FILE *, const wchar_t *, ...);
 int __cdecl wprintf(const wchar_t *, ...);
 int __cdecl _snwprintf(wchar_t *, size_t, const wchar_t *, ...);
 int __cdecl swprintf(wchar_t *, const wchar_t *, ...);
 int __cdecl vfwprintf(FILE *, const wchar_t *, va_list);
 int __cdecl vwprintf(const wchar_t *, va_list);
 int __cdecl _vsnwprintf(wchar_t *, size_t, const wchar_t *, va_list);
 int __cdecl vswprintf(wchar_t *, const wchar_t *, va_list);
 int __cdecl fwscanf(FILE *, const wchar_t *, ...);
 int __cdecl swscanf(const wchar_t *, const wchar_t *, ...);
 int __cdecl wscanf(const wchar_t *, ...);






 FILE * __cdecl _wfdopen(int, const wchar_t *);
 FILE * __cdecl _wfopen(const wchar_t *, const wchar_t *);
 FILE * __cdecl _wfreopen(const wchar_t *, const wchar_t *, FILE *);
 void __cdecl _wperror(const wchar_t *);
 FILE * __cdecl _wpopen(const wchar_t *, const wchar_t *);
 int __cdecl _wremove(const wchar_t *);
 wchar_t * __cdecl _wtempnam(const wchar_t *, const wchar_t *);
 wchar_t * __cdecl _wtmpnam(wchar_t *);



#line 446 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdio.h"
#line 447 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdio.h"


#line 450 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdio.h"
















































 int __cdecl fcloseall(void);
 FILE * __cdecl fdopen(int, const char *);
 int __cdecl fgetchar(void);
 int __cdecl fileno(FILE *);
 int __cdecl flushall(void);
 int __cdecl fputchar(int);
 int __cdecl getw(FILE *);
 int __cdecl putw(int, FILE *);
 int __cdecl rmtmp(void);
 char * __cdecl tempnam(const char *, const char *);
 int __cdecl unlink(const char *);

#line 511 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdio.h"

#line 513 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdio.h"







#pragma pack(pop)
#line 522 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdio.h"

#line 524 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdio.h"
#line 157 "./../../include\\plplot.h"
#line 1 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdlib.h"
















#pragma once
#line 19 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdlib.h"






#line 26 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdlib.h"







#pragma pack(push,8)
#line 35 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdlib.h"

















































#line 85 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdlib.h"
































typedef int (__cdecl * _onexit_t)(void);



#line 122 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdlib.h"

#line 124 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdlib.h"






typedef struct _div_t {
        int quot;
        int rem;
} div_t;

typedef struct _ldiv_t {
        long quot;
        long rem;
} ldiv_t;


#line 142 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdlib.h"


























































#line 201 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdlib.h"
















#line 218 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdlib.h"
 extern int errno;               
 extern unsigned long _doserrno; 
#line 221 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdlib.h"



#line 225 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdlib.h"

































































































 extern char * _sys_errlist[];   
 extern int _sys_nerr;   













































#line 370 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdlib.h"
 extern int __argc;          
 extern char ** __argv;      

 extern wchar_t ** __wargv;  
#line 375 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdlib.h"




 extern char **    _environ; 

 extern wchar_t ** _wenviron;    
#line 383 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdlib.h"
#line 384 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdlib.h"

 extern int _fmode;          
 extern int _fileinfo;       

 extern char * _pgmptr;      

 extern wchar_t * _wpgmptr;  
#line 392 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdlib.h"



 extern unsigned int _osver;
 extern unsigned int _winver;
 extern unsigned int _winmajor;
 extern unsigned int _winminor;
#line 400 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdlib.h"

#line 402 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdlib.h"



 void   __cdecl abort(void);



#line 410 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdlib.h"
        int    __cdecl abs(int);
#line 412 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdlib.h"
        int    __cdecl atexit(void (__cdecl *)(void));
 double __cdecl atof(const char *);
 int    __cdecl atoi(const char *);
 long   __cdecl atol(const char *);



 void * __cdecl bsearch(const void *, const void *, size_t, size_t,
        int (__cdecl *)(const void *, const void *));
 void * __cdecl calloc(size_t, size_t);
 div_t  __cdecl div(int, int);
 void   __cdecl exit(int);
 void   __cdecl free(void *);
 char * __cdecl getenv(const char *);
 char * __cdecl _itoa(int, char *, int);

 char * __cdecl _i64toa(__int64, char *, int);
 char * __cdecl _ui64toa(unsigned __int64, char *, int);
 __int64 __cdecl _atoi64(const char *);
#line 432 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdlib.h"


#line 435 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdlib.h"
        long __cdecl labs(long);
#line 437 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdlib.h"
 ldiv_t __cdecl ldiv(long, long);
 char * __cdecl _ltoa(long, char *, int);
 void * __cdecl malloc(size_t);
 int    __cdecl mblen(const char *, size_t);
 size_t __cdecl _mbstrlen(const char *s);
 int    __cdecl mbtowc(wchar_t *, const char *, size_t);
 size_t __cdecl mbstowcs(wchar_t *, const char *, size_t);
 void   __cdecl qsort(void *, size_t, size_t, int (__cdecl *)
        (const void *, const void *));
 int    __cdecl rand(void);
 void * __cdecl realloc(void *, size_t);
 int    __cdecl _set_error_mode(int);
 void   __cdecl srand(unsigned int);
 double __cdecl strtod(const char *, char **);
 long   __cdecl strtol(const char *, char **, int);



 unsigned long __cdecl strtoul(const char *, char **, int);

 int    __cdecl system(const char *);
#line 459 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdlib.h"
 char * __cdecl _ultoa(unsigned long, char *, int);
 int    __cdecl wctomb(char *, wchar_t);
 size_t __cdecl wcstombs(char *, const wchar_t *, size_t);






 wchar_t * __cdecl _itow (int, wchar_t *, int);
 wchar_t * __cdecl _ltow (long, wchar_t *, int);
 wchar_t * __cdecl _ultow (unsigned long, wchar_t *, int);
 double __cdecl wcstod(const wchar_t *, wchar_t **);
 long   __cdecl wcstol(const wchar_t *, wchar_t **, int);
 unsigned long __cdecl wcstoul(const wchar_t *, wchar_t **, int);
 wchar_t * __cdecl _wgetenv(const wchar_t *);
 int    __cdecl _wsystem(const wchar_t *);
 int __cdecl _wtoi(const wchar_t *);
 long __cdecl _wtol(const wchar_t *);

 wchar_t * __cdecl _i64tow(__int64, wchar_t *, int);
 wchar_t * __cdecl _ui64tow(unsigned __int64, wchar_t *, int);
 __int64   __cdecl _wtoi64(const wchar_t *);
#line 483 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdlib.h"


#line 486 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdlib.h"
#line 487 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdlib.h"



 char * __cdecl _ecvt(double, int, int *, int *);
 void   __cdecl _exit(int);
 char * __cdecl _fcvt(double, int, int *, int *);
 char * __cdecl _fullpath(char *, const char *, size_t);
 char * __cdecl _gcvt(double, int, char *);
        unsigned long __cdecl _lrotl(unsigned long, int);
        unsigned long __cdecl _lrotr(unsigned long, int);

 void   __cdecl _makepath(char *, const char *, const char *, const char *,
        const char *);
#line 501 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdlib.h"
        _onexit_t __cdecl _onexit(_onexit_t);
 void   __cdecl perror(const char *);
 int    __cdecl _putenv(const char *);
        unsigned int __cdecl _rotl(unsigned int, int);
        unsigned int __cdecl _rotr(unsigned int, int);
 void   __cdecl _searchenv(const char *, const char *, char *);

 void   __cdecl _splitpath(const char *, char *, char *, char *, char *);
#line 510 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdlib.h"
 void   __cdecl _swab(char *, char *, int);






 wchar_t * __cdecl _wfullpath(wchar_t *, const wchar_t *, size_t);
 void   __cdecl _wmakepath(wchar_t *, const wchar_t *, const wchar_t *, const wchar_t *,
        const wchar_t *);
 void   __cdecl _wperror(const wchar_t *);
 int    __cdecl _wputenv(const wchar_t *);
 void   __cdecl _wsearchenv(const wchar_t *, const wchar_t *, wchar_t *);
 void   __cdecl _wsplitpath(const wchar_t *, wchar_t *, wchar_t *, wchar_t *, wchar_t *);


#line 527 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdlib.h"
#line 528 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdlib.h"




 void __cdecl _seterrormode(int);
 void __cdecl _beep(unsigned, unsigned);
 void __cdecl _sleep(unsigned long);
#line 536 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdlib.h"


#line 539 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdlib.h"






 int __cdecl tolower(int);
#line 547 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdlib.h"

 int __cdecl toupper(int);
#line 550 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdlib.h"

#line 552 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdlib.h"





































#line 590 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdlib.h"





 char * __cdecl ecvt(double, int, int *, int *);
 char * __cdecl fcvt(double, int, int *, int *);
 char * __cdecl gcvt(double, int, char *);
 char * __cdecl itoa(int, char *, int);
 char * __cdecl ltoa(long, char *, int);
        _onexit_t __cdecl onexit(_onexit_t);
 int    __cdecl putenv(const char *);
 void   __cdecl swab(char *, char *, int);
 char * __cdecl ultoa(unsigned long, char *, int);

#line 606 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdlib.h"

#line 608 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdlib.h"

#line 610 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdlib.h"








#pragma pack(pop)
#line 620 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdlib.h"

#line 622 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdlib.h"
#line 158 "./../../include\\plplot.h"































#line 196 "./../../include\\plplot.h"


    
    
    
#line 202 "./../../include\\plplot.h"
    
























#line 228 "./../../include\\plplot.h"
typedef float PLFLT;
#line 230 "./../../include\\plplot.h"



#line 234 "./../../include\\plplot.h"
typedef int PLINT;
#line 236 "./../../include\\plplot.h"



typedef void* PLPointer;























































					













typedef struct {
    char *opt;
    int  (*handler)	(char *, char *, void *);
    void *client_data;
    void *var;
    long mode;
    char *syntax;
    char *desc;
} PLOptionTable;





typedef struct {
    int type;			
    unsigned int state;		
    unsigned int keysym;	
    unsigned int button;	
    char string[16];	
    int pX, pY;			
    float dX, dY;		
    PLFLT wX, wY;		
} PLGraphicsIn;





typedef struct {
    float dxmi, dxma, dymi, dyma;	
    PLFLT wxmi, wxma, wymi, wyma;	
} PLWindow;




typedef struct {
    unsigned int x, y;			
    unsigned int width, height;		
} PLDisplay;














typedef struct {
    PLFLT *f;
    PLINT nx, ny, nz;
} PLfGrid;






typedef struct {
    PLFLT **f;
    PLINT nx, ny;
} PLfGrid2;












typedef struct {
    PLFLT *xg, *yg, *zg;
    PLINT nx, ny, nz;
} PLcGrid;







typedef struct {
    PLFLT **xg, **yg, **zg;
    PLINT nx, ny;
} PLcGrid2;








typedef struct {
    unsigned char r;		
    unsigned char g;		
    unsigned char b;		
} PLColor;



typedef struct {
    PLFLT h;			
    PLFLT l;			
    PLFLT s;			
    PLFLT p;			
    int rev;			
} PLControlPt;




typedef struct {
    PLINT cmd;
    PLINT result;
} PLBufferingCB;

























































#line 494 "./../../include\\plplot.h"
























































































































#line 615 "./../../include\\plplot.h"


















































































































#line 730 "./../../include\\plplot.h"












#line 743 "./../../include\\plplot.h"









	

	



void
c_pladv(PLINT page);



void
plarrows(PLFLT *u, PLFLT *v, PLFLT *x, PLFLT *y, PLINT n,
         PLFLT scale, PLFLT dx, PLFLT dy) ;




void
c_plaxes(PLFLT x0, PLFLT y0, const char *xopt, PLFLT xtick, PLINT nxsub,
	 const char *yopt, PLFLT ytick, PLINT nysub);



void
c_plbin(PLINT nbin, PLFLT *x, PLFLT *y, PLINT center);



void
c_plbop(void);



void
c_plbox(const char *xopt, PLFLT xtick, PLINT nxsub,
	const char *yopt, PLFLT ytick, PLINT nysub);



void
c_plbox3(const char *xopt, const char *xlabel, PLFLT xtick, PLINT nsubx,
	 const char *yopt, const char *ylabel, PLFLT ytick, PLINT nsuby,
	 const char *zopt, const char *zlabel, PLFLT ztick, PLINT nsubz);



void
c_plcol0(PLINT icol0);



void
c_plcol1(PLFLT col1);





void
c_plcont(PLFLT **f, PLINT nx, PLINT ny, PLINT kx, PLINT lx,
	 PLINT ky, PLINT ly, PLFLT *clevel, PLINT nlevel,
	 void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer),
	 PLPointer pltr_data);






void
plfcont(PLFLT (*f2eval) (PLINT, PLINT, PLPointer),
	PLPointer f2eval_data,
	PLINT nx, PLINT ny, PLINT kx, PLINT lx,
	PLINT ky, PLINT ly, PLFLT *clevel, PLINT nlevel,
	void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer),
	PLPointer pltr_data);



void
c_plcpstrm(PLINT iplsr, PLINT flags);




void
pldid2pc(PLFLT *xmin, PLFLT *ymin, PLFLT *xmax, PLFLT *ymax);




void
pldip2dc(PLFLT *xmin, PLFLT *ymin, PLFLT *xmax, PLFLT *ymax);



void
c_plend(void);



void
c_plend1(void);



void
c_plenv(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax,
	PLINT just, PLINT axis);



void
c_pleop(void);



void
c_plerrx(PLINT n, PLFLT *xmin, PLFLT *xmax, PLFLT *y);



void
c_plerry(PLINT n, PLFLT *x, PLFLT *ymin, PLFLT *ymax);



void
c_plfamadv(void);



void
c_plfill(PLINT n, PLFLT *x, PLFLT *y);



void
c_plflush(void);



void
c_plfont(PLINT ifont);



void
c_plfontld(PLINT fnt);



void
c_plgchr(PLFLT *p_def, PLFLT *p_ht);



void
c_plgcol0(PLINT icol0, PLINT *r, PLINT *g, PLINT *b);



void
c_plgcolbg(PLINT *r, PLINT *g, PLINT *b);



void
c_plgdev(char *p_dev);



void
c_plgdidev(PLFLT *p_mar, PLFLT *p_aspect, PLFLT *p_jx, PLFLT *p_jy);



void
c_plgdiori(PLFLT *p_rot);



void
c_plgdiplt(PLFLT *p_xmin, PLFLT *p_ymin, PLFLT *p_xmax, PLFLT *p_ymax);



void
c_plgfam(PLINT *p_fam, PLINT *p_num, PLINT *p_bmax);



void
c_plgfnam(char *fnam);



void
c_plglevel(PLINT *p_level);



void
c_plgpage(PLFLT *p_xp, PLFLT *p_yp,
	  PLINT *p_xleng, PLINT *p_yleng, PLINT *p_xoff, PLINT *p_yoff);



void
c_plgra(void);



void
c_plgspa(PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax);



void
c_plgstrm(PLINT *p_strm);



void
c_plgver(char *p_ver);



void
c_plgxax(PLINT *p_digmax, PLINT *p_digits);



void
c_plgyax(PLINT *p_digmax, PLINT *p_digits);



void
c_plgzax(PLINT *p_digmax, PLINT *p_digits);



void
c_plhist(PLINT n, PLFLT *data, PLFLT datmin, PLFLT datmax,
	 PLINT nbin, PLINT oldwin);



void
c_plhls(PLFLT h, PLFLT l, PLFLT s);



void
c_plinit(void);



void
c_pljoin(PLFLT x1, PLFLT y1, PLFLT x2, PLFLT y2);



void
c_pllab(const char *xlabel, const char *ylabel, const char *tlabel);



void
c_plline(PLINT n, PLFLT *x, PLFLT *y);



void
c_plline3(PLINT n, PLFLT *x, PLFLT *y, PLFLT *z);



void
c_pllsty(PLINT lin);



void
c_plmap( void (*mapform)(PLINT, PLFLT *, PLFLT *), char *type,
         PLFLT minlong, PLFLT maxlong, PLFLT minlat, PLFLT maxlat );



void 
c_plmeridians( void (*mapform)(PLINT, PLFLT *, PLFLT *), 
               PLFLT dlong, PLFLT dlat,
               PLFLT minlong, PLFLT maxlong, PLFLT minlat, PLFLT maxlat );



void
c_plmesh(PLFLT *x, PLFLT *y, PLFLT **z, PLINT nx, PLINT ny, PLINT opt);



void
c_plmkstrm(PLINT *p_strm);



void
c_plmtex(const char *side, PLFLT disp, PLFLT pos, PLFLT just,
	 const char *text);



void
c_plot3d(PLFLT *x, PLFLT *y, PLFLT **z,
	 PLINT nx, PLINT ny, PLINT opt, PLINT side);



void
c_plpat(PLINT nlin, PLINT *inc, PLINT *del);



void
c_plpoin(PLINT n, PLFLT *x, PLFLT *y, PLINT code);



void
c_plpoin3(PLINT n, PLFLT *x, PLFLT *y, PLFLT *z, PLINT code);



void
c_plpoly3(PLINT n, PLFLT *x, PLFLT *y, PLFLT *z, PLINT *draw);



void
c_plprec(PLINT setp, PLINT prec);



void
c_plpsty(PLINT patt);



void
c_plptex(PLFLT x, PLFLT y, PLFLT dx, PLFLT dy, PLFLT just, const char *text);



void
c_plreplot(void);



void
c_plrgb(PLFLT r, PLFLT g, PLFLT b);



void
c_plrgb1(PLINT r, PLINT g, PLINT b);



void
c_plschr(PLFLT def, PLFLT scale);



void
c_plscmap0n(PLINT ncol0);



void
c_plscmap1n(PLINT ncol1);



void
c_plscmap0(PLINT *r, PLINT *g, PLINT *b, PLINT ncol0);



void
c_plscmap1(PLINT *r, PLINT *g, PLINT *b, PLINT ncol1);




void
c_plscmap1l(PLINT itype, PLINT npts, PLFLT *intensity,
	    PLFLT *coord1, PLFLT *coord2, PLFLT *coord3, PLINT *rev);



void
c_plscol0(PLINT icol0, PLINT r, PLINT g, PLINT b);



void
c_plscolbg(PLINT r, PLINT g, PLINT b);



void
c_plscolor(PLINT color);



void
c_plsdev(const char *devname);




void
c_plsdidev(PLFLT mar, PLFLT aspect, PLFLT jx, PLFLT jy);



void
c_plsdimap(PLINT dimxmin, PLINT dimxmax, PLINT dimymin, PLINT dimymax,
	   PLFLT dimxpmm, PLFLT dimypmm);



void
c_plsdiori(PLFLT rot);



void
c_plsdiplt(PLFLT xmin, PLFLT ymin, PLFLT xmax, PLFLT ymax);



void
c_plsdiplz(PLFLT xmin, PLFLT ymin, PLFLT xmax, PLFLT ymax);



void
c_plsesc(char esc);



void
c_plsfam(PLINT fam, PLINT num, PLINT bmax);



void
c_plsfnam(const char *fnam);



void 
c_plshade(PLFLT **a, PLINT nx, PLINT ny, const char **defined,
	  PLFLT left, PLFLT right, PLFLT bottom, PLFLT top,
	  PLFLT shade_min, PLFLT shade_max,
	  PLINT sh_cmap, PLFLT sh_color, PLINT sh_width,
	  PLINT min_color, PLINT min_width,
	  PLINT max_color, PLINT max_width,
	  void (*fill) (PLINT, PLFLT *, PLFLT *), PLINT rectangular,
	  void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer),
	  PLPointer pltr_data);

void 
c_plshade1(PLFLT *a, PLINT nx, PLINT ny, const char *defined,
	 PLFLT left, PLFLT right, PLFLT bottom, PLFLT top,
	 PLFLT shade_min, PLFLT shade_max,
	 PLINT sh_cmap, PLFLT sh_color, PLINT sh_width,
	 PLINT min_color, PLINT min_width,
	 PLINT max_color, PLINT max_width,
	 void (*fill) (PLINT, PLFLT *, PLFLT *), PLINT rectangular,
	 void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer),
	 PLPointer pltr_data);

void 
plfshade(PLFLT (*f2eval) (PLINT, PLINT, PLPointer),
	 PLPointer f2eval_data,
	 PLFLT (*c2eval) (PLINT, PLINT, PLPointer),
	 PLPointer c2eval_data,
	 PLINT nx, PLINT ny, 
	 PLFLT left, PLFLT right, PLFLT bottom, PLFLT top,
	 PLFLT shade_min, PLFLT shade_max,
	 PLINT sh_cmap, PLFLT sh_color, PLINT sh_width,
	 PLINT min_color, PLINT min_width,
	 PLINT max_color, PLINT max_width,
	 void (*fill) (PLINT, PLFLT *, PLFLT *), PLINT rectangular,
	 void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer),
	 PLPointer pltr_data);



void
c_plsmaj(PLFLT def, PLFLT scale);



void
c_plsmin(PLFLT def, PLFLT scale);



void
c_plsori(PLINT ori);



void
c_plspage(PLFLT xp, PLFLT yp, PLINT xleng, PLINT yleng,
	  PLINT xoff, PLINT yoff);



void
c_plspause(PLINT pause);



void
c_plsstrm(PLINT strm);



void
c_plssub(PLINT nx, PLINT ny);



void
c_plssym(PLFLT def, PLFLT scale);



void
c_plstar(PLINT nx, PLINT ny);



void
c_plstart(const char *devname, PLINT nx, PLINT ny);



void
c_plstripc(PLINT *id, char *xspec, char *yspec,
	PLFLT xmin, PLFLT xmax, PLFLT xjump, PLFLT ymin, PLFLT ymax,
	PLFLT xlpos, PLFLT ylpos,
	PLINT y_ascl, PLINT acc,
	PLINT colbox, PLINT collab,
	PLINT colline[], PLINT styline[], char *legline[],
	char *labx, char *laby, char *labtop);



void
c_plstripa(PLINT id, PLINT pen, PLFLT x, PLFLT y);



void
c_plstripd(PLINT id);



void
c_plstyl(PLINT nms, PLINT *mark, PLINT *space);



void
c_plsvpa(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax);



void
c_plsxax(PLINT digmax, PLINT digits);



void
plsxwin(PLINT window_id);



void
c_plsyax(PLINT digmax, PLINT digits);



void
c_plsym(PLINT n, PLFLT *x, PLFLT *y, PLINT code);



void
c_plszax(PLINT digmax, PLINT digits);



void
c_pltext(void);




void
c_plvasp(PLFLT aspect);




void
c_plvpas(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax, PLFLT aspect);



void
c_plvpor(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax);




void
c_plvsta(void);



void
c_plw3d(PLFLT basex, PLFLT basey, PLFLT height, PLFLT xmin0,
	PLFLT xmax0, PLFLT ymin0, PLFLT ymax0, PLFLT zmin0,
	PLFLT zmax0, PLFLT alt, PLFLT az);



void
c_plwid(PLINT width);



void
c_plwind(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax);







void
plgFileDevs(char ***p_menustr, char ***p_devname, int *p_ndev);



void
plgDevs(char ***p_menustr, char ***p_devname, int *p_ndev);



void
plsKeyEH(void (*KeyEH) (PLGraphicsIn *, void *, int *), void *KeyEH_data);



void
plsButtonEH(void (*ButtonEH) (PLGraphicsIn *, void *, int *),
	    void *ButtonEH_data);



void
plsError(PLINT *errcode, char *errmsg);



void
plsexit(int (*handler) (char *));

	



void
pltr0(PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, PLPointer pltr_data);



void
pltr1(PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, PLPointer pltr_data);




void
pltr2(PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, PLPointer pltr_data);




void
pltr2p(PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, PLPointer pltr_data);



void
pltr0f(PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, void *pltr_data);




void
pltr2f(PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, void *pltr_data);



void 
xform(PLFLT x, PLFLT y, PLFLT * tx, PLFLT * ty);

	




PLFLT
plf2eval2(PLINT ix, PLINT iy, PLPointer plf2eval_data);




PLFLT
plf2eval(PLINT ix, PLINT iy, PLPointer plf2eval_data);




PLFLT
plf2evalr(PLINT ix, PLINT iy, PLPointer plf2eval_data);

	



void
plClearOpts(void);



void
plResetOpts(void);



int
plMergeOpts(PLOptionTable *options, char *name, char **notes);



void
plSetUsage(char *program_string, char *usage_string);



int
plSetOpt(char *opt, char *optarg);



int
plParseOpts(int *p_argc, char **argv, PLINT mode);



void
plOptUsage(void);

	



void
plgfile(FILE **p_file);



void
plsfile(FILE *file);



void
plgesc(char *p_esc);



void
pl_cmd(PLINT op, void *ptr);



int 
plFindName(char *p);



char *
plFindCommand(char *fn);




void
plGetName(char *dir, char *subdir, char *filename, char **filespec);



PLINT
plGetInt(char *s);



PLFLT
plGetFlt(char *s);

	



void
plAlloc2dGrid(PLFLT ***f, PLINT nx, PLINT ny);



void
plFree2dGrid(PLFLT **f, PLINT nx, PLINT ny);



void
plHLS_RGB(PLFLT h, PLFLT l, PLFLT s, PLFLT *p_r, PLFLT *p_g, PLFLT *p_b);

void
plRGB_HLS(PLFLT r, PLFLT g, PLFLT b, PLFLT *p_h, PLFLT *p_l, PLFLT *p_s);



int
plGetCursor(PLGraphicsIn *gin);



int
plTranslateCursor(PLGraphicsIn *gin);





#line 1620 "./../../include\\plplot.h"
#line 140 "./../../include\\plplotP.h"



#line 1 "./../../include\\plstrm.h"











































































































#line 1 "./../../include\\pdf.h"




















































#line 54 "./../../include\\pdf.h"



#line 58 "./../../include\\pdf.h"



#line 62 "./../../include\\pdf.h"



#line 66 "./../../include\\pdf.h"





typedef struct {
    FILE *file;				
    unsigned char *buffer;		
    long bp, bufmax;			
} PDFstrm;



typedef struct {
    int   fd;				
    FILE  *file;			
    char  *fileName;			
    char  *fileHandle;			
    int   type;				
    char  *typeName;			
} PLiodev;















void pdf_set		(char *option, int value);
PDFstrm *pdf_fopen	(char *fileName, char *mode);
PDFstrm *pdf_bopen	(unsigned char *buffer, long bufmax);
PDFstrm *pdf_finit	(FILE *file);
int  pdf_close		(PDFstrm *pdfs);

int  pdf_putc		(int c, PDFstrm *pdfs);
int  pdf_getc		(PDFstrm *pdfs);
int  pdf_ungetc		(int c, PDFstrm *pdfs);

int  pdf_rd_header	(PDFstrm *pdfs, char *header);
int  pdf_wr_header	(PDFstrm *pdfs, char *header);
int  pdf_wr_1byte	(PDFstrm *pdfs, unsigned char s);
int  pdf_rd_1byte	(PDFstrm *pdfs, unsigned char *ps);
int  pdf_wr_2bytes	(PDFstrm *pdfs, unsigned short s);
int  pdf_rd_2bytes 	(PDFstrm *pdfs, unsigned short *ps);
int  pdf_wr_2nbytes	(PDFstrm *pdfs, unsigned short *s, PLINT n);
int  pdf_rd_2nbytes 	(PDFstrm *pdfs, unsigned short *s, PLINT n);
int  pdf_wr_4bytes	(PDFstrm *pdfs, unsigned long s);
int  pdf_rd_4bytes 	(PDFstrm *pdfs, unsigned long *ps);
int  pdf_wr_ieeef	(PDFstrm *pdfs, float f);
int  pdf_rd_ieeef	(PDFstrm *pdfs, float *pf);

#line 126 "./../../include\\pdf.h"
#line 109 "./../../include\\plstrm.h"











typedef struct {
    PLFLT pxlx, pxly;
    PLINT xold, yold;

    PLINT xmin, xmax, xlen;
    PLINT ymin, ymax, ylen;

    PLINT xmin_dev, xmax_dev, xlen_dev;
    PLINT ymin_dev, ymax_dev, ylen_dev;

    PLFLT xscale_dev, yscale_dev;
} PLDev;





































































































































































































































































































































































typedef struct {



    PLINT ipls, level, verbose, debug, initialized;
    char *program;



    PLINT icol0, ncol0, icol1, ncol1, ncp1, curcmap;

    PLColor curcolor;
    PLColor *cmap0;
    PLColor *cmap1;

    PLControlPt cmap1cp[32];



    PLINT width;
    PLINT widthset, widthlock;



    PLINT plbuf_read, plbuf_write;
    PLINT device, dev_minor, termin, graphx, nopause;
    PLINT color, colorset;
    PLINT family, member, finc, fflen, bytemax, famadv;
    PLINT dev_fill0, dev_fill1, dev_di, dev_flush, dev_swin;

    char DevName[80];
    FILE *OutFile;
    char *BaseName, *FileName;
    int  output_type;
    PLINT bytecnt, page, linepos;
    PDFstrm *pdfs;

    PLINT dev_npts;
    short *dev_x, *dev_y;

    void *dev;

    void (*KeyEH)	(PLGraphicsIn *gin, void *KeyEH_data,
			 int *exit_eventloop);
    void *KeyEH_data;

    void (*ButtonEH)	(PLGraphicsIn *gin, void *ButtonEH_data,
			 int *exit_eventloop);
    void *ButtonEH_data;

    void (*LocateEH)	(PLGraphicsIn *gin, void *LocateEH_data,
			 int *locate_mode);
    void *LocateEH_data;

    PLFLT xdpi, ydpi;
    PLINT xlength, ylength;
    PLINT xoffset, yoffset;
    PLINT pageset, hack;



    void (*tidy)    (void *);
    void *tidy_data;



    PLINT *errcode;
    char *errmsg;



    char *geometry;
    long window_id;
    int  nopixmap, db;


    struct PlFrame *plFramePtr;



    char *server_name, *server_host, *server_port, *user;
    char *plserver, *plwindow;
    char *tcl_cmd, *auto_path;
    char *tk_file;  
    int  bufmax, dp, server_nokill;



    FILE *plbufFile;
    int  plbufOwner;



    PLINT difilt, diclpxmi, diclpxma, diclpymi, diclpyma;
    PLFLT dipxmin, dipymin, dipxmax, dipymax;
    PLFLT dipxax, dipxb, dipyay, dipyb;
    PLFLT aspdev, aspect, aspori, mar, jx, jy;
    PLFLT didxax, didxb, didyay, didyb;
    PLFLT diorot;
    PLFLT dioxax, dioxay, dioxb, dioyax, dioyay, dioyb;
    PLFLT dimxax, dimxb, dimyay, dimyb;
    PLFLT dimxmin, dimymin, dimxmax, dimymax, dimxpmm, dimypmm;
    PLINT page_status, freeaspect;



    PLINT patt, inclin[2], delta[2], nps;



    PLINT currx, curry;
    PLINT mark[10], space[10], nms;
    PLINT timecnt, alarm, pendn, curel;



    char  esc;



    PLFLT scale;
    PLFLT chrdef, chrht;
    PLFLT symdef, symht;
    PLFLT majdef, majht;
    PLFLT mindef, minht;



    PLINT setpre, precis;
    PLINT xdigmax, ydigmax, zdigmax;
    PLINT xdigits, ydigits, zdigits;



    PLINT vppxmi, vppxma, vppymi, vppyma;
    PLINT sppxmi, sppxma, sppymi, sppyma;
    PLINT clpxmi, clpxma, clpymi, clpyma;
    PLINT phyxmi, phyxma, phyxlen, phyymi, phyyma, phyylen;
    PLINT umx, umy;
    PLFLT xpmm, ypmm;



    PLFLT base3x, base3y, basecx, basecy;
    PLFLT domxmi, domxma, domymi, domyma;
    PLFLT zzscl, ranmi, ranma;
    PLFLT cxx, cxy, cyx, cyy, cyz;



    int nplwin;
    PLWindow plwin[64];



    PLINT nsubx, nsuby, cursub;
    PLFLT spdxmi, spdxma, spdymi, spdyma;
    PLFLT vpdxmi, vpdxma, vpdymi, vpdyma;
    PLFLT vpwxmi, vpwxma, vpwymi, vpwyma;



    PLFLT wpxscl, wpxoff, wpyscl, wpyoff;
    PLFLT wmxscl, wmxoff, wmyscl, wmyoff;

} PLStream;











void
plgpls(PLStream **p_pls);



void
plcol_interp(PLStream *pls, PLColor *newcolor, int i, int ncol);



void
plOpenFile(PLStream *pls);



void
plP_getmember(PLStream *pls);



void
plP_sfnam(PLStream *pls, const char *fnam);



void
plFamInit(PLStream *pls);



void
plGetFam(PLStream *pls);



void
plRotPhy(PLINT orient, PLINT xmin, PLINT ymin, PLINT xmax, PLINT ymax,
	 int *px, int *py);



PLDev *
plAllocDev(PLStream *pls);



void
plGinInit(PLGraphicsIn *gin);





#line 720 "./../../include\\plstrm.h"
#line 144 "./../../include\\plplotP.h"




extern PLStream	*plsc;
#line 1 "./../../include\\pldebug.h"





































#line 1 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdarg.h"















#pragma once
#line 18 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdarg.h"






#line 25 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdarg.h"







#pragma pack(push,8)
#line 34 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdarg.h"






















































































































#line 153 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdarg.h"







#pragma pack(pop)
#line 162 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdarg.h"

#line 164 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stdarg.h"
#line 39 "./../../include\\pldebug.h"










#line 50 "./../../include\\pldebug.h"




















static void
pldebug( const char *fname, ... )
{


















}

#line 94 "./../../include\\pldebug.h"
#line 150 "./../../include\\plplotP.h"
#line 151 "./../../include\\plplotP.h"








#line 160 "./../../include\\plplotP.h"















#line 176 "./../../include\\plplotP.h"


#line 179 "./../../include\\plplotP.h"


#line 182 "./../../include\\plplotP.h"


#line 185 "./../../include\\plplotP.h"


#line 188 "./../../include\\plplotP.h"


#line 191 "./../../include\\plplotP.h"


#line 194 "./../../include\\plplotP.h"



















#line 214 "./../../include\\plplotP.h"




























#line 243 "./../../include\\plplotP.h"











#line 255 "./../../include\\plplotP.h"

























































void
pldtik(PLFLT vmin, PLFLT vmax, PLFLT *tick, PLINT *nsubt);



void
pldprec(PLFLT vmin, PLFLT vmax, PLFLT tick, PLINT lf, 
	PLINT *mode, PLINT *prec, PLINT digmax, PLINT *scale);



void
plP_pllclp(PLINT *x, PLINT *y, PLINT npts,
	   PLINT xmin, PLINT xmax, PLINT ymin, PLINT ymax, 
	   void (*draw) (short *, short *, PLINT));



void
plP_plfclp(PLINT *x, PLINT *y, PLINT npts,
	   PLINT xmin, PLINT xmax, PLINT ymin, PLINT ymax, 
	   void (*draw) (short *, short *, PLINT));



void
plfill_soft(short *x, short *y, PLINT npts);




void
plexit(char *errormsg);



void
pl_exit(void);



void
plwarn(char *errormsg);



void
plabort(char *errormsg);



void
plfntld(PLINT fnt);



void
plfontrel(void);



void
plcmap1_calc(void);




void 
plstik(PLFLT mx, PLFLT my, PLFLT dx, PLFLT dy);




void
plstr(PLINT base, PLFLT *xform, PLINT refx, PLINT refy, const char *string);



void
plxtik(PLINT x, PLINT y, PLINT below, PLINT above);



void
plytik(PLINT x, PLINT y, PLINT left, PLINT right);



void
plP_gdom(PLFLT *p_xmin, PLFLT *p_xmax, PLFLT *p_ymin, PLFLT *p_ymax);



void
plP_grange(PLFLT *p_zscl, PLFLT *p_zmin, PLFLT *p_zmax);



void
plP_gw3wc(PLFLT *p_dxx, PLFLT *p_dxy, PLFLT *p_dyx, PLFLT *p_dyy,
	  PLFLT *p_dyz);



void
plP_gclp(PLINT *p_ixmin, PLINT *p_ixmax, PLINT *p_iymin, PLINT *p_iymax);



void
plP_sclp(PLINT ixmin, PLINT ixmax, PLINT iymin, PLINT iymax);



void
plP_gphy(PLINT *p_ixmin, PLINT *p_ixmax, PLINT *p_iymin, PLINT *p_iymax);



void
plP_gsub(PLINT *p_nx, PLINT *p_ny, PLINT *p_cs);



void
plP_ssub(PLINT nx, PLINT ny, PLINT cs);



void
plP_subpInit(void);



void
plP_gvpd(PLFLT *p_xmin, PLFLT *p_xmax, PLFLT *p_ymin, PLFLT *p_ymax);



void
plP_gvpw(PLFLT *p_xmin, PLFLT *p_xmax, PLFLT *p_ymin, PLFLT *p_ymax);



void
plP_gpixmm(PLFLT *p_x, PLFLT *p_y);



void
plP_setpxl(PLFLT xpmm0, PLFLT ypmm0);



void
plP_gzback(PLINT **zbf, PLINT **zbc, PLFLT **zbt);



void
plP_movphy(PLINT x, PLINT y);



void
plP_draphy(PLINT x, PLINT y);



void
plP_movwor(PLFLT x, PLFLT y);



void
plP_drawor(PLFLT x, PLFLT y);



void
plP_draphy_poly(PLINT *x, PLINT *y, PLINT n);



void
plP_drawor_poly(PLFLT *x, PLFLT *y, PLINT n);



void
plP_setphy(PLINT xmin, PLINT xmax, PLINT ymin, PLINT ymax);



void
plP_setsub(void);



void
plP_gprec(PLINT *p_setp, PLINT *p_prec);



PLFLT
plstrl(const char *string);



PLINT
plP_stindex(const char *str1, const char *str2);



PLINT
plP_strpos(char *str, int chr);



PLINT
plP_stsearch(const char *str, int chr);

	



PLINT
plP_dcpcx(PLFLT x);



PLINT
plP_dcpcy(PLFLT y);



PLINT
plP_mmpcx(PLFLT x);



PLINT
plP_mmpcy(PLFLT y);



PLINT
plP_wcpcx(PLFLT x);



PLINT
plP_wcpcy(PLFLT y);



PLFLT
plP_pcdcx(PLINT x);



PLFLT
plP_pcdcy(PLINT y);



PLFLT
plP_mmdcx(PLFLT x);



PLFLT
plP_mmdcy(PLFLT y);



PLFLT
plP_scdcx(PLFLT x);



PLFLT
plP_scdcy(PLFLT y);



PLFLT
plP_dcmmx(PLFLT x);



PLFLT
plP_dcmmy(PLFLT y);



PLFLT
plP_wcmmx(PLFLT x);



PLFLT
plP_wcmmy(PLFLT y);



PLFLT
plP_dcscx(PLFLT x);



PLFLT
plP_dcscy(PLFLT y);



PLFLT
plP_w3wcx(PLFLT x, PLFLT y, PLFLT z);



PLFLT
plP_w3wcy(PLFLT x, PLFLT y, PLFLT z);

	



void
plP_init(void);



void
plP_line(short *x, short *y);



void
plP_polyline(short *x, short *y, PLINT npts);



void
plP_fill(short *x, short *y, PLINT npts);



void
plP_eop(void);



void
plP_bop(void);



void
plP_tidy(void);



void
plP_state(PLINT op);



void
plP_esc(PLINT op, void *ptr);



void
plP_swin(PLWindow *plwin);



FILE *
plLibOpen(char *fn);



void
plstrm_init(void);





#line 703 "./../../include\\plplotP.h"
#line 52 "./../tk/pltkd.h"
#line 1 "./../tk/pltk.h"
























#line 1 "./../tcl\\pltcl.h"


































#line 1 "./../../include\\plplot.h"



















































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































#line 36 "./../tcl\\pltcl.h"
#line 1 "d:\\tcl-source\\tcl8.3.1\\generic\\tcl.h"



























    














































#line 76 "d:\\tcl-source\\tcl8.3.1\\generic\\tcl.h"
#line 77 "d:\\tcl-source\\tcl8.3.1\\generic\\tcl.h"




#line 82 "d:\\tcl-source\\tcl8.3.1\\generic\\tcl.h"


#line 85 "d:\\tcl-source\\tcl8.3.1\\generic\\tcl.h"


#line 88 "d:\\tcl-source\\tcl8.3.1\\generic\\tcl.h"











#line 100 "d:\\tcl-source\\tcl8.3.1\\generic\\tcl.h"
#line 101 "d:\\tcl-source\\tcl8.3.1\\generic\\tcl.h"













































#line 147 "d:\\tcl-source\\tcl8.3.1\\generic\\tcl.h"










#line 158 "d:\\tcl-source\\tcl8.3.1\\generic\\tcl.h"













#line 172 "d:\\tcl-source\\tcl8.3.1\\generic\\tcl.h"










































#line 215 "d:\\tcl-source\\tcl8.3.1\\generic\\tcl.h"



















#line 235 "d:\\tcl-source\\tcl8.3.1\\generic\\tcl.h"
#line 236 "d:\\tcl-source\\tcl8.3.1\\generic\\tcl.h"

























#line 262 "d:\\tcl-source\\tcl8.3.1\\generic\\tcl.h"
#line 263 "d:\\tcl-source\\tcl8.3.1\\generic\\tcl.h"










#line 274 "d:\\tcl-source\\tcl8.3.1\\generic\\tcl.h"








#line 283 "d:\\tcl-source\\tcl8.3.1\\generic\\tcl.h"












#line 296 "d:\\tcl-source\\tcl8.3.1\\generic\\tcl.h"




















typedef char CHAR;
typedef short SHORT;
typedef long LONG;
#line 320 "d:\\tcl-source\\tcl8.3.1\\generic\\tcl.h"
#line 321 "d:\\tcl-source\\tcl8.3.1\\generic\\tcl.h"












#line 334 "d:\\tcl-source\\tcl8.3.1\\generic\\tcl.h"
    typedef int *ClientData;
#line 336 "d:\\tcl-source\\tcl8.3.1\\generic\\tcl.h"

#line 338 "d:\\tcl-source\\tcl8.3.1\\generic\\tcl.h"




















typedef struct Tcl_Interp {
    char *result;		

    void (*freeProc) (char *blockPtr);
				






    int errorLine;              


} Tcl_Interp;

typedef struct Tcl_AsyncHandler_ *Tcl_AsyncHandler;
typedef struct Tcl_Channel_ *Tcl_Channel;
typedef struct Tcl_Command_ *Tcl_Command;
typedef struct Tcl_Condition_ *Tcl_Condition;
typedef struct Tcl_EncodingState_ *Tcl_EncodingState;
typedef struct Tcl_Encoding_ *Tcl_Encoding;
typedef struct Tcl_Event Tcl_Event;
typedef struct Tcl_Mutex_ *Tcl_Mutex;
typedef struct Tcl_Pid_ *Tcl_Pid;
typedef struct Tcl_RegExp_ *Tcl_RegExp;
typedef struct Tcl_ThreadDataKey_ *Tcl_ThreadDataKey;
typedef struct Tcl_ThreadId_ *Tcl_ThreadId;
typedef struct Tcl_TimerToken_ *Tcl_TimerToken;
typedef struct Tcl_Trace_ *Tcl_Trace;
typedef struct Tcl_Var_ *Tcl_Var;











typedef unsigned (__stdcall Tcl_ThreadCreateProc) (ClientData clientData);


#line 404 "d:\\tcl-source\\tcl8.3.1\\generic\\tcl.h"
















#line 421 "d:\\tcl-source\\tcl8.3.1\\generic\\tcl.h"





















































typedef struct Tcl_RegExpIndices {
    long start;		
    long end;		

} Tcl_RegExpIndices;

typedef struct Tcl_RegExpInfo {
    int nsubs;			

    Tcl_RegExpIndices *matches;	

    long extendStart;		

    long reserved;		
} Tcl_RegExpInfo;






typedef struct stat *Tcl_Stat_;


































typedef enum {TCL_INT, TCL_DOUBLE, TCL_EITHER} Tcl_ValueType;
typedef struct Tcl_Value {
    Tcl_ValueType type;		

    long intValue;		
    double doubleValue;		
} Tcl_Value;







struct Tcl_Obj;





typedef int (Tcl_AppInitProc) (Tcl_Interp *interp);
typedef int (Tcl_AsyncProc) 
(ClientData clientData, Tcl_Interp *interp, int code);
typedef void (Tcl_ChannelProc) (ClientData clientData, int mask);
typedef void (Tcl_CloseProc) (ClientData data);
typedef void (Tcl_CmdDeleteProc) (ClientData clientData);
typedef int (Tcl_CmdProc) 
(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
typedef void (Tcl_CmdTraceProc) 

(ClientData clientData, Tcl_Interp *interp, int level, char *command, Tcl_CmdProc *proc, ClientData cmdClientData, int argc, char *argv[]);
typedef void (Tcl_DupInternalRepProc) 
(struct Tcl_Obj *srcPtr, struct Tcl_Obj *dupPtr);
typedef int (Tcl_EncodingConvertProc)


(ClientData clientData, const char *src, int srcLen, int flags, Tcl_EncodingState *statePtr, char *dst, int dstLen, int *srcReadPtr, int *dstWrotePtr, int *dstCharsPtr);
typedef void (Tcl_EncodingFreeProc)(ClientData clientData);
typedef int (Tcl_EventProc) (Tcl_Event *evPtr, int flags);
typedef void (Tcl_EventCheckProc) 
(ClientData clientData, int flags);
typedef int (Tcl_EventDeleteProc) 
(Tcl_Event *evPtr, ClientData clientData);
typedef void (Tcl_EventSetupProc) 
(ClientData clientData, int flags);
typedef void (Tcl_ExitProc) (ClientData clientData);
typedef void (Tcl_FileProc) (ClientData clientData, int mask);
typedef void (Tcl_FileFreeProc) (ClientData clientData);
typedef void (Tcl_FreeInternalRepProc) (struct Tcl_Obj *objPtr);
typedef void (Tcl_FreeProc) (char *blockPtr);
typedef void (Tcl_IdleProc) (ClientData clientData);
typedef void (Tcl_InterpDeleteProc) 
(ClientData clientData, Tcl_Interp *interp);
typedef int (Tcl_MathProc) 
(ClientData clientData, Tcl_Interp *interp, Tcl_Value *args, Tcl_Value *resultPtr);
typedef void (Tcl_NamespaceDeleteProc) (ClientData clientData);
typedef int (Tcl_ObjCmdProc) 
(ClientData clientData, Tcl_Interp *interp, int objc, struct Tcl_Obj *const objv[]);
typedef int (Tcl_PackageInitProc) (Tcl_Interp *interp);
typedef void (Tcl_PanicProc) (char * format, ...);
typedef void (Tcl_TcpAcceptProc) 
(ClientData callbackData, Tcl_Channel chan, char *address, int port);
typedef void (Tcl_TimerProc) (ClientData clientData);
typedef int (Tcl_SetFromAnyProc) 
(Tcl_Interp *interp, struct Tcl_Obj *objPtr);
typedef void (Tcl_UpdateStringProc) (struct Tcl_Obj *objPtr);
typedef char *(Tcl_VarTraceProc) 
(ClientData clientData, Tcl_Interp *interp, char *part1, char *part2, int flags);
typedef void (Tcl_CreateFileHandlerProc) 
(int fd, int mask, Tcl_FileProc *proc, ClientData clientData);
typedef void (Tcl_DeleteFileHandlerProc) (int fd);







typedef struct Tcl_ObjType {
    char *name;			
    Tcl_FreeInternalRepProc *freeIntRepProc;
				


    Tcl_DupInternalRepProc *dupIntRepProc;
    				

    Tcl_UpdateStringProc *updateStringProc;
    				

    Tcl_SetFromAnyProc *setFromAnyProc;
    				



} Tcl_ObjType;







typedef struct Tcl_Obj {
    int refCount;		
    char *bytes;		










    int length;			

    Tcl_ObjType *typePtr;	



    union {			
	long longValue;		
	double doubleValue;	
	void *otherValuePtr;	
	struct {		
	    void *ptr1;
	    void *ptr2;
	} twoPtrValue;
    } internalRep;
} Tcl_Obj;












void		Tcl_IncrRefCount (Tcl_Obj *objPtr);
void		Tcl_DecrRefCount (Tcl_Obj *objPtr);
int		Tcl_IsShared (Tcl_Obj *objPtr);















#line 693 "d:\\tcl-source\\tcl8.3.1\\generic\\tcl.h"
































typedef struct Tcl_SavedResult {
    char *result;
    Tcl_FreeProc *freeProc;
    Tcl_Obj *objResultPtr;
    char *appendResult;
    int appendAvl;
    int appendUsed;
    char resultSpace[200+1];
} Tcl_SavedResult;








typedef struct Tcl_Namespace {
    char *name;                 



    char *fullName;             

    ClientData clientData;      

    Tcl_NamespaceDeleteProc* deleteProc;
                                

    struct Tcl_Namespace* parentPtr;
                                


} Tcl_Namespace;























typedef struct Tcl_CallFrame {
    Tcl_Namespace *nsPtr;
    int dummy1;
    int dummy2;
    char *dummy3;
    char *dummy4;
    char *dummy5;
    int dummy6;
    char *dummy7;
    char *dummy8;
    int dummy9;
    char* dummy10;
} Tcl_CallFrame;















     
typedef struct Tcl_CmdInfo {
    int isNativeObjectProc;	 



    Tcl_ObjCmdProc *objProc;	 
    ClientData objClientData;	 
    Tcl_CmdProc *proc;		 
    ClientData clientData;	 
    Tcl_CmdDeleteProc *deleteProc;
                                 

    ClientData deleteData;	 

    Tcl_Namespace *namespacePtr; 




} Tcl_CmdInfo;








typedef struct Tcl_DString {
    char *string;		

    int length;			

    int spaceAvl;		

    char staticSpace[200];
				

} Tcl_DString;










 






































































#line 932 "d:\\tcl-source\\tcl8.3.1\\generic\\tcl.h"




























typedef struct Tcl_HashEntry {
    struct Tcl_HashEntry *nextPtr;	


    struct Tcl_HashTable *tablePtr;	
    struct Tcl_HashEntry **bucketPtr;	


    ClientData clientData;		

    union {				
	char *oneWordValue;		
	int words[1];			



	char string[4];			


    } key;				
} Tcl_HashEntry;








typedef struct Tcl_HashTable {
    Tcl_HashEntry **buckets;		


    Tcl_HashEntry *staticBuckets[4];
					

    int numBuckets;			

    int numEntries;			

    int rebuildSize;			

    int downShift;			


    int mask;				

    int keyType;			





    Tcl_HashEntry *(*findProc) 
(struct Tcl_HashTable *tablePtr, const char *key);
    Tcl_HashEntry *(*createProc) 
(struct Tcl_HashTable *tablePtr, const char *key, int *newPtr);
} Tcl_HashTable;






typedef struct Tcl_HashSearch {
    Tcl_HashTable *tablePtr;		
    int nextIndex;			

    Tcl_HashEntry *nextEntryPtr;	

} Tcl_HashSearch;


















































struct Tcl_Event {
    Tcl_EventProc *proc;	
    struct Tcl_Event *nextPtr;	
};





typedef enum {
    TCL_QUEUE_TAIL, TCL_QUEUE_HEAD, TCL_QUEUE_MARK
} Tcl_QueuePosition;
















typedef struct Tcl_Time {
    long sec;			
    long usec;			
} Tcl_Time;

typedef void (Tcl_SetTimerProc) (Tcl_Time *timePtr);
typedef int (Tcl_WaitForEventProc) (Tcl_Time *timePtr);








































typedef int	(Tcl_DriverBlockModeProc) 
( ClientData instanceData, int mode);
typedef int	(Tcl_DriverCloseProc) 
(ClientData instanceData, Tcl_Interp *interp);
typedef int	(Tcl_DriverClose2Proc) 
(ClientData instanceData, Tcl_Interp *interp, int flags);
typedef int	(Tcl_DriverInputProc) 
(ClientData instanceData, char *buf, int toRead, int *errorCodePtr);
typedef int	(Tcl_DriverOutputProc) 
(ClientData instanceData, char *buf, int toWrite, int *errorCodePtr);
typedef int	(Tcl_DriverSeekProc) 
(ClientData instanceData, long offset, int mode, int *errorCodePtr);
typedef int	(Tcl_DriverSetOptionProc) 

( ClientData instanceData, Tcl_Interp *interp, char *optionName, char *value);
typedef int	(Tcl_DriverGetOptionProc) 

( ClientData instanceData, Tcl_Interp *interp, char *optionName, Tcl_DString *dsPtr);
typedef void	(Tcl_DriverWatchProc) 
( ClientData instanceData, int mask);
typedef int	(Tcl_DriverGetHandleProc) 

( ClientData instanceData, int direction, ClientData *handlePtr);





























#line 1209 "d:\\tcl-source\\tcl8.3.1\\generic\\tcl.h"





typedef enum Tcl_EolTranslation {
    TCL_TRANSLATE_AUTO,			
    TCL_TRANSLATE_CR,			
    TCL_TRANSLATE_LF,			
    TCL_TRANSLATE_CRLF			
} Tcl_EolTranslation;









typedef struct Tcl_ChannelType {
    char *typeName;			


    Tcl_DriverBlockModeProc *blockModeProc;
    					

    Tcl_DriverCloseProc *closeProc;	



    Tcl_DriverInputProc *inputProc;	

    Tcl_DriverOutputProc *outputProc;	

    Tcl_DriverSeekProc *seekProc;	

    Tcl_DriverSetOptionProc *setOptionProc;
    					
    Tcl_DriverGetOptionProc *getOptionProc;
    					
    Tcl_DriverWatchProc *watchProc;	

    Tcl_DriverGetHandleProc *getHandleProc;
					

    Tcl_DriverClose2Proc *close2Proc;   



} Tcl_ChannelType;















typedef enum Tcl_PathType {
    TCL_PATH_ABSOLUTE,
    TCL_PATH_RELATIVE,
    TCL_PATH_VOLUME_RELATIVE
} Tcl_PathType;






typedef struct Tcl_NotifierProcs {
    Tcl_SetTimerProc *setTimerProc;
    Tcl_WaitForEventProc *waitForEventProc;
    Tcl_CreateFileHandlerProc *createFileHandlerProc;
    Tcl_DeleteFileHandlerProc *deleteFileHandlerProc;
} Tcl_NotifierProcs;






typedef struct Tcl_EncodingType {
    const char *encodingName;	


    Tcl_EncodingConvertProc *toUtfProc;
				

    Tcl_EncodingConvertProc *fromUtfProc;
				

    Tcl_EncodingFreeProc *freeProc;
				

    ClientData clientData;	

    int nullSize;		




} Tcl_EncodingType;    


















































typedef struct Tcl_Token {
    int type;			

    char *start;		
    int size;			
    int numComponents;		




} Tcl_Token;












































































































typedef struct Tcl_Parse {
    char *commentStart;		


    int commentSize;		



    char *commandStart;		
    int commandSize;		



    int numWords;		

    Tcl_Token *tokenPtr;	




    int numTokens;		
    int tokensAvailable;	

    int errorType;		


    





    char *string;		

    char *end;			

    Tcl_Interp *interp;		

    char *term;			





    int incomplete;		



    Tcl_Token staticTokens[20];
				




} Tcl_Parse;

















































typedef unsigned short Tcl_UniChar;









#line 1603 "d:\\tcl-source\\tcl8.3.1\\generic\\tcl.h"
































extern  char *		Tcl_InitStubs 
(Tcl_Interp *interp, char *version, int exact);


















#line 1 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"





























extern  int		Tcl_PkgProvideEx 

(Tcl_Interp * interp, char * name, char * version, ClientData clientData);

extern  char *		Tcl_PkgRequireEx 

(Tcl_Interp * interp, char * name, char * version, int exact, ClientData * clientDataPtr);

extern  void		Tcl_Panic (char * format, ...);

extern  char *		Tcl_Alloc (unsigned int size);

extern  void		Tcl_Free (char * ptr);

extern  char *		Tcl_Realloc 
(char * ptr, unsigned int size);

extern  char *		Tcl_DbCkalloc 
(unsigned int size, char * file, int line);

extern  int		Tcl_DbCkfree 
(char * ptr, char * file, int line);

extern  char *		Tcl_DbCkrealloc 
(char * ptr, unsigned int size, char * file, int line);




#line 60 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 64 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"

extern  void		Tcl_SetTimer (Tcl_Time * timePtr);

extern  void		Tcl_Sleep (int ms);

extern  int		Tcl_WaitForEvent (Tcl_Time * timePtr);

extern  int		Tcl_AppendAllObjTypes 
( Tcl_Interp * interp, Tcl_Obj * objPtr);

extern  void		Tcl_AppendStringsToObj (Tcl_Obj * objPtr, ...);

extern  void		Tcl_AppendToObj 
(Tcl_Obj * objPtr, char * bytes, int length);

extern  Tcl_Obj *	Tcl_ConcatObj 
(int objc, Tcl_Obj *const objv[]);

extern  int		Tcl_ConvertToType 
(Tcl_Interp * interp, Tcl_Obj * objPtr, Tcl_ObjType * typePtr);

extern  void		Tcl_DbDecrRefCount 
(Tcl_Obj * objPtr, char * file, int line);

extern  void		Tcl_DbIncrRefCount 
(Tcl_Obj * objPtr, char * file, int line);

extern  int		Tcl_DbIsShared 
(Tcl_Obj * objPtr, char * file, int line);

extern  Tcl_Obj *	Tcl_DbNewBooleanObj 
(int boolValue, char * file, int line);

extern  Tcl_Obj *	Tcl_DbNewByteArrayObj 

( unsigned char * bytes, int length, char * file, int line);

extern  Tcl_Obj *	Tcl_DbNewDoubleObj 
(double doubleValue, char * file, int line);

extern  Tcl_Obj *	Tcl_DbNewListObj 
(int objc, Tcl_Obj *const objv[], char * file, int line);

extern  Tcl_Obj *	Tcl_DbNewLongObj 
(long longValue, char * file, int line);

extern  Tcl_Obj *	Tcl_DbNewObj (char * file, int line);

extern  Tcl_Obj *	Tcl_DbNewStringObj 
(const char * bytes, int length, char * file, int line);

extern  Tcl_Obj *	Tcl_DuplicateObj (Tcl_Obj * objPtr);

extern  void		TclFreeObj (Tcl_Obj * objPtr);

extern  int		Tcl_GetBoolean 
(Tcl_Interp * interp, char * str, int * boolPtr);

extern  int		Tcl_GetBooleanFromObj 

( Tcl_Interp * interp, Tcl_Obj * objPtr, int * boolPtr);

extern  unsigned char *	Tcl_GetByteArrayFromObj 
( Tcl_Obj * objPtr, int * lengthPtr);

extern  int		Tcl_GetDouble 
(Tcl_Interp * interp, char * str, double * doublePtr);

extern  int		Tcl_GetDoubleFromObj 

( Tcl_Interp * interp, Tcl_Obj * objPtr, double * doublePtr);

extern  int		Tcl_GetIndexFromObj 

(Tcl_Interp * interp, Tcl_Obj * objPtr, char ** tablePtr, char * msg, int flags, int * indexPtr);

extern  int		Tcl_GetInt 
(Tcl_Interp * interp, char * str, int * intPtr);

extern  int		Tcl_GetIntFromObj 
(Tcl_Interp * interp, Tcl_Obj * objPtr, int * intPtr);

extern  int		Tcl_GetLongFromObj 
(Tcl_Interp * interp, Tcl_Obj * objPtr, long * longPtr);

extern  Tcl_ObjType *	Tcl_GetObjType (char * typeName);

extern  char *		Tcl_GetStringFromObj 
(Tcl_Obj * objPtr, int * lengthPtr);

extern  void		Tcl_InvalidateStringRep 
( Tcl_Obj * objPtr);

extern  int		Tcl_ListObjAppendList 

( Tcl_Interp * interp, Tcl_Obj * listPtr, Tcl_Obj * elemListPtr);

extern  int		Tcl_ListObjAppendElement 

( Tcl_Interp * interp, Tcl_Obj * listPtr, Tcl_Obj * objPtr);

extern  int		Tcl_ListObjGetElements 

( Tcl_Interp * interp, Tcl_Obj * listPtr, int * objcPtr, Tcl_Obj *** objvPtr);

extern  int		Tcl_ListObjIndex 

(Tcl_Interp * interp, Tcl_Obj * listPtr, int index, Tcl_Obj ** objPtrPtr);

extern  int		Tcl_ListObjLength 
(Tcl_Interp * interp, Tcl_Obj * listPtr, int * intPtr);

extern  int		Tcl_ListObjReplace 

(Tcl_Interp * interp, Tcl_Obj * listPtr, int first, int count, int objc, Tcl_Obj *const objv[]);

extern  Tcl_Obj *	Tcl_NewBooleanObj (int boolValue);

extern  Tcl_Obj *	Tcl_NewByteArrayObj 
( unsigned char * bytes, int length);

extern  Tcl_Obj *	Tcl_NewDoubleObj (double doubleValue);

extern  Tcl_Obj *	Tcl_NewIntObj (int intValue);

extern  Tcl_Obj *	Tcl_NewListObj 
(int objc, Tcl_Obj *const objv[]);

extern  Tcl_Obj *	Tcl_NewLongObj (long longValue);

extern  Tcl_Obj *	Tcl_NewObj (void);

extern  Tcl_Obj *	Tcl_NewStringObj 
(const char * bytes, int length);

extern  void		Tcl_SetBooleanObj 
(Tcl_Obj * objPtr, int boolValue);

extern  unsigned char *	Tcl_SetByteArrayLength 
(Tcl_Obj * objPtr, int length);

extern  void		Tcl_SetByteArrayObj 
(Tcl_Obj * objPtr, unsigned char * bytes, int length);

extern  void		Tcl_SetDoubleObj 
(Tcl_Obj * objPtr, double doubleValue);

extern  void		Tcl_SetIntObj 
(Tcl_Obj * objPtr, int intValue);

extern  void		Tcl_SetListObj 
(Tcl_Obj * objPtr, int objc, Tcl_Obj *const objv[]);

extern  void		Tcl_SetLongObj 
(Tcl_Obj * objPtr, long longValue);

extern  void		Tcl_SetObjLength 
(Tcl_Obj * objPtr, int length);

extern  void		Tcl_SetStringObj 
(Tcl_Obj * objPtr, char * bytes, int length);

extern  void		Tcl_AddErrorInfo 
(Tcl_Interp * interp, const char * message);

extern  void		Tcl_AddObjErrorInfo 
(Tcl_Interp * interp, const char * message, int length);

extern  void		Tcl_AllowExceptions (Tcl_Interp * interp);

extern  void		Tcl_AppendElement 
(Tcl_Interp * interp, const char * string);

extern  void		Tcl_AppendResult (Tcl_Interp * interp, ...);

extern  Tcl_AsyncHandler	 Tcl_AsyncCreate 
(Tcl_AsyncProc * proc, ClientData clientData);

extern  void		Tcl_AsyncDelete (Tcl_AsyncHandler async);

extern  int		Tcl_AsyncInvoke 
(Tcl_Interp * interp, int code);

extern  void		Tcl_AsyncMark (Tcl_AsyncHandler async);

extern  int		Tcl_AsyncReady (void);

extern  void		Tcl_BackgroundError (Tcl_Interp * interp);

extern  char		Tcl_Backslash 
(const char * src, int * readPtr);

extern  int		Tcl_BadChannelOption 

( Tcl_Interp * interp, char * optionName, char * optionList);

extern  void		Tcl_CallWhenDeleted 

(Tcl_Interp * interp, Tcl_InterpDeleteProc * proc, ClientData clientData);

extern  void		Tcl_CancelIdleCall 

( Tcl_IdleProc * idleProc, ClientData clientData);

extern  int		Tcl_Close 
(Tcl_Interp * interp, Tcl_Channel chan);

extern  int		Tcl_CommandComplete (char * cmd);

extern  char *		Tcl_Concat (int argc, char ** argv);

extern  int		Tcl_ConvertElement 
(const char * src, char * dst, int flags);

extern  int		Tcl_ConvertCountedElement 

( const char * src, int length, char * dst, int flags);

extern  int		Tcl_CreateAlias 

(Tcl_Interp * slave, char * slaveCmd, Tcl_Interp * target, char * targetCmd, int argc, char ** argv);

extern  int		Tcl_CreateAliasObj 


(Tcl_Interp * slave, char * slaveCmd, Tcl_Interp * target, char * targetCmd, int objc, Tcl_Obj *const objv[]);

extern  Tcl_Channel	Tcl_CreateChannel 

( Tcl_ChannelType * typePtr, char * chanName, ClientData instanceData, int mask);

extern  void		Tcl_CreateChannelHandler 


( Tcl_Channel chan, int mask, Tcl_ChannelProc * proc, ClientData clientData);

extern  void		Tcl_CreateCloseHandler 
(Tcl_Channel chan, Tcl_CloseProc * proc, ClientData clientData);

extern  Tcl_Command	Tcl_CreateCommand 


(Tcl_Interp * interp, char * cmdName, Tcl_CmdProc * proc, ClientData clientData, Tcl_CmdDeleteProc * deleteProc);

extern  void		Tcl_CreateEventSource 


( Tcl_EventSetupProc * setupProc, Tcl_EventCheckProc * checkProc, ClientData clientData);

extern  void		Tcl_CreateExitHandler 
( Tcl_ExitProc * proc, ClientData clientData);

extern  Tcl_Interp *	Tcl_CreateInterp (void);

extern  void		Tcl_CreateMathFunc 


(Tcl_Interp * interp, char * name, int numArgs, Tcl_ValueType * argTypes, Tcl_MathProc * proc, ClientData clientData);

extern  Tcl_Command	Tcl_CreateObjCommand 


( Tcl_Interp * interp, char * cmdName, Tcl_ObjCmdProc * proc, ClientData clientData, Tcl_CmdDeleteProc * deleteProc);

extern  Tcl_Interp *	Tcl_CreateSlave 
(Tcl_Interp * interp, char * slaveName, int isSafe);

extern  Tcl_TimerToken	Tcl_CreateTimerHandler 
(int milliseconds, Tcl_TimerProc * proc, ClientData clientData);

extern  Tcl_Trace	Tcl_CreateTrace 

(Tcl_Interp * interp, int level, Tcl_CmdTraceProc * proc, ClientData clientData);

extern  void		Tcl_DeleteAssocData 
(Tcl_Interp * interp, char * name);

extern  void		Tcl_DeleteChannelHandler 

( Tcl_Channel chan, Tcl_ChannelProc * proc, ClientData clientData);

extern  void		Tcl_DeleteCloseHandler 
(Tcl_Channel chan, Tcl_CloseProc * proc, ClientData clientData);

extern  int		Tcl_DeleteCommand 
(Tcl_Interp * interp, char * cmdName);

extern  int		Tcl_DeleteCommandFromToken 
( Tcl_Interp * interp, Tcl_Command command);

extern  void		Tcl_DeleteEvents 

( Tcl_EventDeleteProc * proc, ClientData clientData);

extern  void		Tcl_DeleteEventSource 


( Tcl_EventSetupProc * setupProc, Tcl_EventCheckProc * checkProc, ClientData clientData);

extern  void		Tcl_DeleteExitHandler 
( Tcl_ExitProc * proc, ClientData clientData);

extern  void		Tcl_DeleteHashEntry 
( Tcl_HashEntry * entryPtr);

extern  void		Tcl_DeleteHashTable 
( Tcl_HashTable * tablePtr);

extern  void		Tcl_DeleteInterp (Tcl_Interp * interp);




#line 378 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"


extern  void		Tcl_DetachPids 
(int numPids, Tcl_Pid * pidPtr);
#line 383 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"

extern  void		Tcl_DeleteTimerHandler 
( Tcl_TimerToken token);

extern  void		Tcl_DeleteTrace 
(Tcl_Interp * interp, Tcl_Trace trace);

extern  void		Tcl_DontCallWhenDeleted 


( Tcl_Interp * interp, Tcl_InterpDeleteProc * proc, ClientData clientData);

extern  int		Tcl_DoOneEvent (int flags);

extern  void		Tcl_DoWhenIdle 
(Tcl_IdleProc * proc, ClientData clientData);

extern  char *		Tcl_DStringAppend 
(Tcl_DString * dsPtr, const char * str, int length);

extern  char *		Tcl_DStringAppendElement 
( Tcl_DString * dsPtr, const char * string);

extern  void		Tcl_DStringEndSublist 
( Tcl_DString * dsPtr);

extern  void		Tcl_DStringFree (Tcl_DString * dsPtr);

extern  void		Tcl_DStringGetResult 
( Tcl_Interp * interp, Tcl_DString * dsPtr);

extern  void		Tcl_DStringInit (Tcl_DString * dsPtr);

extern  void		Tcl_DStringResult 
(Tcl_Interp * interp, Tcl_DString * dsPtr);

extern  void		Tcl_DStringSetLength 
( Tcl_DString * dsPtr, int length);

extern  void		Tcl_DStringStartSublist 
( Tcl_DString * dsPtr);

extern  int		Tcl_Eof (Tcl_Channel chan);

extern  char *		Tcl_ErrnoId (void);

extern  char *		Tcl_ErrnoMsg (int err);

extern  int		Tcl_Eval 
(Tcl_Interp * interp, char * string);

extern  int		Tcl_EvalFile 
(Tcl_Interp * interp, char * fileName);

extern  int		Tcl_EvalObj 
(Tcl_Interp * interp, Tcl_Obj * objPtr);

extern  void		Tcl_EventuallyFree 

( ClientData clientData, Tcl_FreeProc * freeProc);

extern  void		Tcl_Exit (int status);

extern  int		Tcl_ExposeCommand 
(Tcl_Interp * interp, char * hiddenCmdToken, char * cmdName);

extern  int		Tcl_ExprBoolean 
(Tcl_Interp * interp, char * str, int * ptr);

extern  int		Tcl_ExprBooleanObj 
(Tcl_Interp * interp, Tcl_Obj * objPtr, int * ptr);

extern  int		Tcl_ExprDouble 
(Tcl_Interp * interp, char * str, double * ptr);

extern  int		Tcl_ExprDoubleObj 
(Tcl_Interp * interp, Tcl_Obj * objPtr, double * ptr);

extern  int		Tcl_ExprLong 
(Tcl_Interp * interp, char * str, long * ptr);

extern  int		Tcl_ExprLongObj 
(Tcl_Interp * interp, Tcl_Obj * objPtr, long * ptr);

extern  int		Tcl_ExprObj 
(Tcl_Interp * interp, Tcl_Obj * objPtr, Tcl_Obj ** resultPtrPtr);

extern  int		Tcl_ExprString 
(Tcl_Interp * interp, char * string);

extern  void		Tcl_Finalize (void);

extern  void		Tcl_FindExecutable (const char * argv0);

extern  Tcl_HashEntry *	Tcl_FirstHashEntry 

( Tcl_HashTable * tablePtr, Tcl_HashSearch * searchPtr);

extern  int		Tcl_Flush (Tcl_Channel chan);

extern  void		Tcl_FreeResult (Tcl_Interp * interp);

extern  int		Tcl_GetAlias 



(Tcl_Interp * interp, char * slaveCmd, Tcl_Interp ** targetInterpPtr, char ** targetCmdPtr, int * argcPtr, char *** argvPtr);

extern  int		Tcl_GetAliasObj 



(Tcl_Interp * interp, char * slaveCmd, Tcl_Interp ** targetInterpPtr, char ** targetCmdPtr, int * objcPtr, Tcl_Obj *** objv);

extern  ClientData	Tcl_GetAssocData 
(Tcl_Interp * interp, char * name, Tcl_InterpDeleteProc ** procPtr);

extern  Tcl_Channel	Tcl_GetChannel 
(Tcl_Interp * interp, char * chanName, int * modePtr);

extern  int		Tcl_GetChannelBufferSize 
( Tcl_Channel chan);

extern  int		Tcl_GetChannelHandle 
(Tcl_Channel chan, int direction, ClientData * handlePtr);

extern  ClientData	Tcl_GetChannelInstanceData 
( Tcl_Channel chan);

extern  int		Tcl_GetChannelMode (Tcl_Channel chan);

extern  char *		Tcl_GetChannelName (Tcl_Channel chan);

extern  int		Tcl_GetChannelOption 

( Tcl_Interp * interp, Tcl_Channel chan, char * optionName, Tcl_DString * dsPtr);

extern  Tcl_ChannelType * Tcl_GetChannelType (Tcl_Channel chan);

extern  int		Tcl_GetCommandInfo 
(Tcl_Interp * interp, char * cmdName, Tcl_CmdInfo * infoPtr);

extern  char *		Tcl_GetCommandName 
(Tcl_Interp * interp, Tcl_Command command);

extern  int		Tcl_GetErrno (void);

extern  char *		Tcl_GetHostName (void);

extern  int		Tcl_GetInterpPath 

( Tcl_Interp * askInterp, Tcl_Interp * slaveInterp);

extern  Tcl_Interp *	Tcl_GetMaster (Tcl_Interp * interp);

extern  const char *	Tcl_GetNameOfExecutable (void);

extern  Tcl_Obj *	Tcl_GetObjResult (Tcl_Interp * interp);





#line 547 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"

extern  Tcl_PathType	Tcl_GetPathType (char * path);

extern  int		Tcl_Gets 
(Tcl_Channel chan, Tcl_DString * dsPtr);

extern  int		Tcl_GetsObj 
(Tcl_Channel chan, Tcl_Obj * objPtr);

extern  int		Tcl_GetServiceMode (void);

extern  Tcl_Interp *	Tcl_GetSlave 
(Tcl_Interp * interp, char * slaveName);

extern  Tcl_Channel	Tcl_GetStdChannel (int type);

extern  char *		Tcl_GetStringResult (Tcl_Interp * interp);

extern  char *		Tcl_GetVar 
(Tcl_Interp * interp, char * varName, int flags);

extern  char *		Tcl_GetVar2 
(Tcl_Interp * interp, char * part1, char * part2, int flags);

extern  int		Tcl_GlobalEval 
(Tcl_Interp * interp, char * command);

extern  int		Tcl_GlobalEvalObj 
(Tcl_Interp * interp, Tcl_Obj * objPtr);

extern  int		Tcl_HideCommand 
(Tcl_Interp * interp, char * cmdName, char * hiddenCmdToken);

extern  int		Tcl_Init (Tcl_Interp * interp);

extern  void		Tcl_InitHashTable 
( Tcl_HashTable * tablePtr, int keyType);

extern  int		Tcl_InputBlocked (Tcl_Channel chan);

extern  int		Tcl_InputBuffered (Tcl_Channel chan);

extern  int		Tcl_InterpDeleted (Tcl_Interp * interp);

extern  int		Tcl_IsSafe (Tcl_Interp * interp);

extern  char *		Tcl_JoinPath 
(int argc, char ** argv, Tcl_DString * resultPtr);

extern  int		Tcl_LinkVar 
(Tcl_Interp * interp, char * varName, char * addr, int type);


extern  Tcl_Channel	Tcl_MakeFileChannel 
(ClientData handle, int mode);

extern  int		Tcl_MakeSafe (Tcl_Interp * interp);

extern  Tcl_Channel	Tcl_MakeTcpClientChannel 
( ClientData tcpSocket);

extern  char *		Tcl_Merge (int argc, char ** argv);

extern  Tcl_HashEntry *	Tcl_NextHashEntry 
( Tcl_HashSearch * searchPtr);

extern  void		Tcl_NotifyChannel 
(Tcl_Channel channel, int mask);

extern  Tcl_Obj *	Tcl_ObjGetVar2 

(Tcl_Interp * interp, Tcl_Obj * part1Ptr, Tcl_Obj * part2Ptr, int flags);

extern  Tcl_Obj *	Tcl_ObjSetVar2 

(Tcl_Interp * interp, Tcl_Obj * part1Ptr, Tcl_Obj * part2Ptr, Tcl_Obj * newValuePtr, int flags);





#line 629 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"


extern  Tcl_Channel	Tcl_OpenCommandChannel 

( Tcl_Interp * interp, int argc, char ** argv, int flags);
#line 635 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"

extern  Tcl_Channel	Tcl_OpenFileChannel 

(Tcl_Interp * interp, char * fileName, char * modeString, int permissions);

extern  Tcl_Channel	Tcl_OpenTcpClient 

(Tcl_Interp * interp, int port, char * address, char * myaddr, int myport, int async);

extern  Tcl_Channel	Tcl_OpenTcpServer 


(Tcl_Interp * interp, int port, char * host, Tcl_TcpAcceptProc * acceptProc, ClientData callbackData);

extern  void		Tcl_Preserve (ClientData data);

extern  void		Tcl_PrintDouble 
(Tcl_Interp * interp, double value, char * dst);

extern  int		Tcl_PutEnv (const char * string);

extern  char *		Tcl_PosixError (Tcl_Interp * interp);

extern  void		Tcl_QueueEvent 
(Tcl_Event * evPtr, Tcl_QueuePosition position);

extern  int		Tcl_Read 
(Tcl_Channel chan, char * bufPtr, int toRead);



#line 667 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"


extern  void		Tcl_ReapDetachedProcs (void);
#line 671 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"

extern  int		Tcl_RecordAndEval 
(Tcl_Interp * interp, char * cmd, int flags);

extern  int		Tcl_RecordAndEvalObj 

( Tcl_Interp * interp, Tcl_Obj * cmdPtr, int flags);

extern  void		Tcl_RegisterChannel 
(Tcl_Interp * interp, Tcl_Channel chan);

extern  void		Tcl_RegisterObjType 
( Tcl_ObjType * typePtr);

extern  Tcl_RegExp	Tcl_RegExpCompile 
(Tcl_Interp * interp, char * string);

extern  int		Tcl_RegExpExec 

(Tcl_Interp * interp, Tcl_RegExp regexp, const char * str, const char * start);

extern  int		Tcl_RegExpMatch 
(Tcl_Interp * interp, char * str, char * pattern);

extern  void		Tcl_RegExpRange 
(Tcl_RegExp regexp, int index, char ** startPtr, char ** endPtr);

extern  void		Tcl_Release (ClientData clientData);

extern  void		Tcl_ResetResult (Tcl_Interp * interp);

extern  int		Tcl_ScanElement 
(const char * str, int * flagPtr);

extern  int		Tcl_ScanCountedElement 
(const char * str, int length, int * flagPtr);

extern  int		Tcl_Seek 
(Tcl_Channel chan, int offset, int mode);

extern  int		Tcl_ServiceAll (void);

extern  int		Tcl_ServiceEvent (int flags);

extern  void		Tcl_SetAssocData 

(Tcl_Interp * interp, char * name, Tcl_InterpDeleteProc * proc, ClientData clientData);

extern  void		Tcl_SetChannelBufferSize 
( Tcl_Channel chan, int sz);

extern  int		Tcl_SetChannelOption 

( Tcl_Interp * interp, Tcl_Channel chan, char * optionName, char * newValue);

extern  int		Tcl_SetCommandInfo 
(Tcl_Interp * interp, char * cmdName, Tcl_CmdInfo * infoPtr);

extern  void		Tcl_SetErrno (int err);

extern  void		Tcl_SetErrorCode (Tcl_Interp * interp, ...);

extern  void		Tcl_SetMaxBlockTime (Tcl_Time * timePtr);

extern  void		Tcl_SetPanicProc 
( Tcl_PanicProc * panicProc);

extern  int		Tcl_SetRecursionLimit 
( Tcl_Interp * interp, int depth);

extern  void		Tcl_SetResult 
(Tcl_Interp * interp, char * str, Tcl_FreeProc * freeProc);

extern  int		Tcl_SetServiceMode (int mode);

extern  void		Tcl_SetObjErrorCode 
(Tcl_Interp * interp, Tcl_Obj * errorObjPtr);

extern  void		Tcl_SetObjResult 
(Tcl_Interp * interp, Tcl_Obj * resultObjPtr);

extern  void		Tcl_SetStdChannel 
(Tcl_Channel channel, int type);

extern  char *		Tcl_SetVar 
(Tcl_Interp * interp, char * varName, char * newValue, int flags);

extern  char *		Tcl_SetVar2 

(Tcl_Interp * interp, char * part1, char * part2, char * newValue, int flags);

extern  char *		Tcl_SignalId (int sig);

extern  char *		Tcl_SignalMsg (int sig);

extern  void		Tcl_SourceRCFile (Tcl_Interp * interp);

extern  int		Tcl_SplitList 

(Tcl_Interp * interp, const char * listStr, int * argcPtr, char *** argvPtr);

extern  void		Tcl_SplitPath 
(const char * path, int * argcPtr, char *** argvPtr);

extern  void		Tcl_StaticPackage 


(Tcl_Interp * interp, char * pkgName, Tcl_PackageInitProc * initProc, Tcl_PackageInitProc * safeInitProc);

extern  int		Tcl_StringMatch 
(const char * str, const char * pattern);

extern  int		Tcl_Tell (Tcl_Channel chan);

extern  int		Tcl_TraceVar 


(Tcl_Interp * interp, char * varName, int flags, Tcl_VarTraceProc * proc, ClientData clientData);

extern  int		Tcl_TraceVar2 


(Tcl_Interp * interp, char * part1, char * part2, int flags, Tcl_VarTraceProc * proc, ClientData clientData);

extern  char *		Tcl_TranslateFileName 

( Tcl_Interp * interp, char * name, Tcl_DString * bufferPtr);

extern  int		Tcl_Ungets 
(Tcl_Channel chan, char * str, int len, int atHead);

extern  void		Tcl_UnlinkVar 
(Tcl_Interp * interp, char * varName);

extern  int		Tcl_UnregisterChannel 
( Tcl_Interp * interp, Tcl_Channel chan);

extern  int		Tcl_UnsetVar 
(Tcl_Interp * interp, char * varName, int flags);

extern  int		Tcl_UnsetVar2 
(Tcl_Interp * interp, char * part1, char * part2, int flags);

extern  void		Tcl_UntraceVar 


(Tcl_Interp * interp, char * varName, int flags, Tcl_VarTraceProc * proc, ClientData clientData);

extern  void		Tcl_UntraceVar2 


(Tcl_Interp * interp, char * part1, char * part2, int flags, Tcl_VarTraceProc * proc, ClientData clientData);

extern  void		Tcl_UpdateLinkedVar 
(Tcl_Interp * interp, char * varName);

extern  int		Tcl_UpVar 

(Tcl_Interp * interp, char * frameName, char * varName, char * localName, int flags);

extern  int		Tcl_UpVar2 

(Tcl_Interp * interp, char * frameName, char * part1, char * part2, char * localName, int flags);

extern  int		Tcl_VarEval (Tcl_Interp * interp, ...);

extern  ClientData	Tcl_VarTraceInfo 


(Tcl_Interp * interp, char * varName, int flags, Tcl_VarTraceProc * procPtr, ClientData prevClientData);

extern  ClientData	Tcl_VarTraceInfo2 


(Tcl_Interp * interp, char * part1, char * part2, int flags, Tcl_VarTraceProc * procPtr, ClientData prevClientData);

extern  int		Tcl_Write 
(Tcl_Channel chan, char * s, int slen);

extern  void		Tcl_WrongNumArgs 

(Tcl_Interp * interp, int objc, Tcl_Obj *const objv[], char * message);

extern  int		Tcl_DumpActiveMemory (char * fileName);

extern  void		Tcl_ValidateAllMemory 
(char * file, int line);

extern  void		Tcl_AppendResultVA 
(Tcl_Interp * interp, va_list argList);

extern  void		Tcl_AppendStringsToObjVA 
( Tcl_Obj * objPtr, va_list argList);

extern  char *		Tcl_HashStats (Tcl_HashTable * tablePtr);

extern  char *		Tcl_ParseVar 
(Tcl_Interp * interp, char * str, char ** termPtr);

extern  char *		Tcl_PkgPresent 
(Tcl_Interp * interp, char * name, char * version, int exact);

extern  char *		Tcl_PkgPresentEx 

(Tcl_Interp * interp, char * name, char * version, int exact, ClientData * clientDataPtr);

extern  int		Tcl_PkgProvide 
(Tcl_Interp * interp, char * name, char * version);

extern  char *		Tcl_PkgRequire 
(Tcl_Interp * interp, char * name, char * version, int exact);

extern  void		Tcl_SetErrorCodeVA 
(Tcl_Interp * interp, va_list argList);

extern  int		Tcl_VarEvalVA 
(Tcl_Interp * interp, va_list argList);

extern  Tcl_Pid		Tcl_WaitPid 
(Tcl_Pid pid, int * statPtr, int options);




#line 896 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"


extern  void		Tcl_PanicVA 
(char * format, va_list argList);
#line 901 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"

extern  void		Tcl_GetVersion 
(int * major, int * minor, int * patchLevel, int * type);

extern  void		Tcl_InitMemory (Tcl_Interp * interp);

extern  Tcl_Channel	Tcl_StackChannel 


(Tcl_Interp * interp, Tcl_ChannelType * typePtr, ClientData instanceData, int mask, Tcl_Channel prevChan);

extern  void		Tcl_UnstackChannel 
(Tcl_Interp * interp, Tcl_Channel chan);

extern  Tcl_Channel	Tcl_GetStackedChannel (Tcl_Channel chan);



extern  void		Tcl_AppendObjToObj 
(Tcl_Obj * objPtr, Tcl_Obj * appendObjPtr);

extern  Tcl_Encoding	Tcl_CreateEncoding 
( Tcl_EncodingType * typePtr);

extern  void		Tcl_CreateThreadExitHandler 
( Tcl_ExitProc * proc, ClientData clientData);

extern  void		Tcl_DeleteThreadExitHandler 
( Tcl_ExitProc * proc, ClientData clientData);

extern  void		Tcl_DiscardResult 
( Tcl_SavedResult * statePtr);

extern  int		Tcl_EvalEx 
(Tcl_Interp * interp, char * script, int numBytes, int flags);

extern  int		Tcl_EvalObjv 
(Tcl_Interp * interp, int objc, Tcl_Obj *const objv[], int flags);

extern  int		Tcl_EvalObjEx 
(Tcl_Interp * interp, Tcl_Obj * objPtr, int flags);

extern  void		Tcl_ExitThread (int status);

extern  int		Tcl_ExternalToUtf 




(Tcl_Interp * interp, Tcl_Encoding encoding, const char * src, int srcLen, int flags, Tcl_EncodingState * statePtr, char * dst, int dstLen, int * srcReadPtr, int * dstWrotePtr, int * dstCharsPtr);

extern  char *		Tcl_ExternalToUtfDString 

( Tcl_Encoding encoding, const char * src, int srcLen, Tcl_DString * dsPtr);

extern  void		Tcl_FinalizeThread (void);

extern  void		Tcl_FinalizeNotifier 
( ClientData clientData);

extern  void		Tcl_FreeEncoding (Tcl_Encoding encoding);

extern  Tcl_ThreadId	Tcl_GetCurrentThread (void);

extern  Tcl_Encoding	Tcl_GetEncoding 
(Tcl_Interp * interp, const char * name);

extern  char *		Tcl_GetEncodingName 
( Tcl_Encoding encoding);

extern  void		Tcl_GetEncodingNames 
( Tcl_Interp * interp);

extern  int		Tcl_GetIndexFromObjStruct 


( Tcl_Interp * interp, Tcl_Obj * objPtr, char ** tablePtr, int offset, char * msg, int flags, int * indexPtr);

extern  void *		Tcl_GetThreadData 
( Tcl_ThreadDataKey * keyPtr, int size);

extern  Tcl_Obj *	Tcl_GetVar2Ex 
(Tcl_Interp * interp, char * part1, char * part2, int flags);

extern  ClientData	Tcl_InitNotifier (void);

extern  void		Tcl_MutexLock (Tcl_Mutex * mutexPtr);

extern  void		Tcl_MutexUnlock (Tcl_Mutex * mutexPtr);

extern  void		Tcl_ConditionNotify 
( Tcl_Condition * condPtr);

extern  void		Tcl_ConditionWait 

( Tcl_Condition * condPtr, Tcl_Mutex * mutexPtr, Tcl_Time * timePtr);

extern  int		Tcl_NumUtfChars 
(const char * src, int len);

extern  int		Tcl_ReadChars 

(Tcl_Channel channel, Tcl_Obj * objPtr, int charsToRead, int appendFlag);

extern  void		Tcl_RestoreResult 
(Tcl_Interp * interp, Tcl_SavedResult * statePtr);

extern  void		Tcl_SaveResult 
(Tcl_Interp * interp, Tcl_SavedResult * statePtr);

extern  int		Tcl_SetSystemEncoding 
( Tcl_Interp * interp, const char * name);

extern  Tcl_Obj *	Tcl_SetVar2Ex 

(Tcl_Interp * interp, char * part1, char * part2, Tcl_Obj * newValuePtr, int flags);

extern  void		Tcl_ThreadAlert (Tcl_ThreadId threadId);

extern  void		Tcl_ThreadQueueEvent 

( Tcl_ThreadId threadId, Tcl_Event* evPtr, Tcl_QueuePosition position);

extern  Tcl_UniChar	Tcl_UniCharAtIndex 
(const char * src, int index);

extern  Tcl_UniChar	Tcl_UniCharToLower (int ch);

extern  Tcl_UniChar	Tcl_UniCharToTitle (int ch);

extern  Tcl_UniChar	Tcl_UniCharToUpper (int ch);

extern  int		Tcl_UniCharToUtf (int ch, char * buf);

extern  char *		Tcl_UtfAtIndex 
(const char * src, int index);

extern  int		Tcl_UtfCharComplete 
(const char * src, int len);

extern  int		Tcl_UtfBackslash 
(const char * src, int * readPtr, char * dst);

extern  char *		Tcl_UtfFindFirst 
(const char * src, int ch);

extern  char *		Tcl_UtfFindLast 
(const char * src, int ch);

extern  char *		Tcl_UtfNext (const char * src);

extern  char *		Tcl_UtfPrev 
(const char * src, const char * start);

extern  int		Tcl_UtfToExternal 




(Tcl_Interp * interp, Tcl_Encoding encoding, const char * src, int srcLen, int flags, Tcl_EncodingState * statePtr, char * dst, int dstLen, int * srcReadPtr, int * dstWrotePtr, int * dstCharsPtr);

extern  char *		Tcl_UtfToExternalDString 

( Tcl_Encoding encoding, const char * src, int srcLen, Tcl_DString * dsPtr);

extern  int		Tcl_UtfToLower (char * src);

extern  int		Tcl_UtfToTitle (char * src);

extern  int		Tcl_UtfToUniChar 
(const char * src, Tcl_UniChar * chPtr);

extern  int		Tcl_UtfToUpper (char * src);

extern  int		Tcl_WriteChars 
(Tcl_Channel chan, const char * src, int srcLen);

extern  int		Tcl_WriteObj 
(Tcl_Channel chan, Tcl_Obj * objPtr);

extern  char *		Tcl_GetString (Tcl_Obj * objPtr);

extern  char *		Tcl_GetDefaultEncodingDir (void);

extern  void		Tcl_SetDefaultEncodingDir (char * path);

extern  void		Tcl_AlertNotifier (ClientData clientData);

extern  void		Tcl_ServiceModeHook (int mode);

extern  int		Tcl_UniCharIsAlnum (int ch);

extern  int		Tcl_UniCharIsAlpha (int ch);

extern  int		Tcl_UniCharIsDigit (int ch);

extern  int		Tcl_UniCharIsLower (int ch);

extern  int		Tcl_UniCharIsSpace (int ch);

extern  int		Tcl_UniCharIsUpper (int ch);

extern  int		Tcl_UniCharIsWordChar (int ch);

extern  int		Tcl_UniCharLen (Tcl_UniChar * str);

extern  int		Tcl_UniCharNcmp 
(const Tcl_UniChar * cs, const Tcl_UniChar * ct, unsigned long n);

extern  char *		Tcl_UniCharToUtfDString 

( const Tcl_UniChar * string, int numChars, Tcl_DString * dsPtr);

extern  Tcl_UniChar *	Tcl_UtfToUniCharDString 

( const char * string, int length, Tcl_DString * dsPtr);

extern  Tcl_RegExp	Tcl_GetRegExpFromObj 

( Tcl_Interp * interp, Tcl_Obj * patObj, int flags);

extern  Tcl_Obj *	Tcl_EvalTokens 
(Tcl_Interp * interp, Tcl_Token * tokenPtr, int count);

extern  void		Tcl_FreeParse (Tcl_Parse * parsePtr);

extern  void		Tcl_LogCommandInfo 
(Tcl_Interp * interp, char * script, char * command, int length);

extern  int		Tcl_ParseBraces 


(Tcl_Interp * interp, char * string, int numBytes, Tcl_Parse * parsePtr, int append, char ** termPtr);

extern  int		Tcl_ParseCommand 

(Tcl_Interp * interp, char * string, int numBytes, int nested, Tcl_Parse * parsePtr);

extern  int		Tcl_ParseExpr 

(Tcl_Interp * interp, char * string, int numBytes, Tcl_Parse * parsePtr);

extern  int		Tcl_ParseQuotedString 


( Tcl_Interp * interp, char * string, int numBytes, Tcl_Parse * parsePtr, int append, char ** termPtr);

extern  int		Tcl_ParseVarName 

(Tcl_Interp * interp, char * string, int numBytes, Tcl_Parse * parsePtr, int append);

extern  char *		Tcl_GetCwd 
(Tcl_Interp * interp, Tcl_DString * cwdPtr);

extern  int		Tcl_Chdir (const char * dirName);

extern  int		Tcl_Access (const char * path, int mode);

extern  int		Tcl_Stat 
(const char * path, struct stat * bufPtr);

extern  int		Tcl_UtfNcmp 
(const char * s1, const char * s2, unsigned long n);

extern  int		Tcl_UtfNcasecmp 
(const char * s1, const char * s2, unsigned long n);

extern  int		Tcl_StringCaseMatch 
(const char * str, const char * pattern, int nocase);

extern  int		Tcl_UniCharIsControl (int ch);

extern  int		Tcl_UniCharIsGraph (int ch);

extern  int		Tcl_UniCharIsPrint (int ch);

extern  int		Tcl_UniCharIsPunct (int ch);

extern  int		Tcl_RegExpExecObj 

(Tcl_Interp * interp, Tcl_RegExp regexp, Tcl_Obj * objPtr, int offset, int nmatches, int flags);

extern  void		Tcl_RegExpGetInfo 
(Tcl_RegExp regexp, Tcl_RegExpInfo * infoPtr);

extern  Tcl_Obj *	Tcl_NewUnicodeObj 
(Tcl_UniChar * unicode, int numChars);

extern  void		Tcl_SetUnicodeObj 
(Tcl_Obj * objPtr, Tcl_UniChar * unicode, int numChars);

extern  int		Tcl_GetCharLength (Tcl_Obj * objPtr);

extern  Tcl_UniChar	Tcl_GetUniChar 
(Tcl_Obj * objPtr, int index);

extern  Tcl_UniChar *	Tcl_GetUnicode (Tcl_Obj * objPtr);

extern  Tcl_Obj *	Tcl_GetRange 
(Tcl_Obj * objPtr, int first, int last);

extern  void		Tcl_AppendUnicodeToObj 
(Tcl_Obj * objPtr, Tcl_UniChar * unicode, int length);

extern  int		Tcl_RegExpMatchObj 
(Tcl_Interp * interp, Tcl_Obj * stringObj, Tcl_Obj * patternObj);

extern  void		Tcl_SetNotifier 
( Tcl_NotifierProcs * notifierProcPtr);

extern  Tcl_Mutex *	Tcl_GetAllocMutex (void);

extern  int		Tcl_GetChannelNames (Tcl_Interp * interp);

extern  int		Tcl_GetChannelNamesEx 
( Tcl_Interp * interp, char * pattern);

extern  int		Tcl_ProcObjCmd 

(ClientData clientData, Tcl_Interp * interp, int objc, Tcl_Obj *const objv[]);

extern  void		Tcl_ConditionFinalize 
( Tcl_Condition * condPtr);

extern  void		Tcl_MutexFinalize (Tcl_Mutex * mutex);

extern  int		Tcl_CreateThread 


(Tcl_ThreadId * idPtr, Tcl_ThreadCreateProc proc, ClientData clientData, int stackSize, int flags);

typedef struct TclStubHooks {
    struct TclPlatStubs *tclPlatStubs;
    struct TclIntStubs *tclIntStubs;
    struct TclIntPlatStubs *tclIntPlatStubs;
} TclStubHooks;

typedef struct TclStubs {
    int magic;
    struct TclStubHooks *hooks;

    int (*tcl_PkgProvideEx) (Tcl_Interp * interp, char * name, char * version, ClientData clientData); 
    char * (*tcl_PkgRequireEx) (Tcl_Interp * interp, char * name, char * version, int exact, ClientData * clientDataPtr); 
    void (*tcl_Panic) (char * format, ...); 
    char * (*tcl_Alloc) (unsigned int size); 
    void (*tcl_Free) (char * ptr); 
    char * (*tcl_Realloc) (char * ptr, unsigned int size); 
    char * (*tcl_DbCkalloc) (unsigned int size, char * file, int line); 
    int (*tcl_DbCkfree) (char * ptr, char * file, int line); 
    char * (*tcl_DbCkrealloc) (char * ptr, unsigned int size, char * file, int line); 


#line 1254 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"

    void *reserved9;
#line 1257 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"





#line 1263 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"

    void *reserved10;
#line 1266 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



    void (*tcl_SetTimer) (Tcl_Time * timePtr); 
    void (*tcl_Sleep) (int ms); 
    int (*tcl_WaitForEvent) (Tcl_Time * timePtr); 
    int (*tcl_AppendAllObjTypes) (Tcl_Interp * interp, Tcl_Obj * objPtr); 
    void (*tcl_AppendStringsToObj) (Tcl_Obj * objPtr, ...); 
    void (*tcl_AppendToObj) (Tcl_Obj * objPtr, char * bytes, int length); 
    Tcl_Obj * (*tcl_ConcatObj) (int objc, Tcl_Obj *const objv[]); 
    int (*tcl_ConvertToType) (Tcl_Interp * interp, Tcl_Obj * objPtr, Tcl_ObjType * typePtr); 
    void (*tcl_DbDecrRefCount) (Tcl_Obj * objPtr, char * file, int line); 
    void (*tcl_DbIncrRefCount) (Tcl_Obj * objPtr, char * file, int line); 
    int (*tcl_DbIsShared) (Tcl_Obj * objPtr, char * file, int line); 
    Tcl_Obj * (*tcl_DbNewBooleanObj) (int boolValue, char * file, int line); 
    Tcl_Obj * (*tcl_DbNewByteArrayObj) (unsigned char * bytes, int length, char * file, int line); 
    Tcl_Obj * (*tcl_DbNewDoubleObj) (double doubleValue, char * file, int line); 
    Tcl_Obj * (*tcl_DbNewListObj) (int objc, Tcl_Obj *const objv[], char * file, int line); 
    Tcl_Obj * (*tcl_DbNewLongObj) (long longValue, char * file, int line); 
    Tcl_Obj * (*tcl_DbNewObj) (char * file, int line); 
    Tcl_Obj * (*tcl_DbNewStringObj) (const char * bytes, int length, char * file, int line); 
    Tcl_Obj * (*tcl_DuplicateObj) (Tcl_Obj * objPtr); 
    void (*tclFreeObj) (Tcl_Obj * objPtr); 
    int (*tcl_GetBoolean) (Tcl_Interp * interp, char * str, int * boolPtr); 
    int (*tcl_GetBooleanFromObj) (Tcl_Interp * interp, Tcl_Obj * objPtr, int * boolPtr); 
    unsigned char * (*tcl_GetByteArrayFromObj) (Tcl_Obj * objPtr, int * lengthPtr); 
    int (*tcl_GetDouble) (Tcl_Interp * interp, char * str, double * doublePtr); 
    int (*tcl_GetDoubleFromObj) (Tcl_Interp * interp, Tcl_Obj * objPtr, double * doublePtr); 
    int (*tcl_GetIndexFromObj) (Tcl_Interp * interp, Tcl_Obj * objPtr, char ** tablePtr, char * msg, int flags, int * indexPtr); 
    int (*tcl_GetInt) (Tcl_Interp * interp, char * str, int * intPtr); 
    int (*tcl_GetIntFromObj) (Tcl_Interp * interp, Tcl_Obj * objPtr, int * intPtr); 
    int (*tcl_GetLongFromObj) (Tcl_Interp * interp, Tcl_Obj * objPtr, long * longPtr); 
    Tcl_ObjType * (*tcl_GetObjType) (char * typeName); 
    char * (*tcl_GetStringFromObj) (Tcl_Obj * objPtr, int * lengthPtr); 
    void (*tcl_InvalidateStringRep) (Tcl_Obj * objPtr); 
    int (*tcl_ListObjAppendList) (Tcl_Interp * interp, Tcl_Obj * listPtr, Tcl_Obj * elemListPtr); 
    int (*tcl_ListObjAppendElement) (Tcl_Interp * interp, Tcl_Obj * listPtr, Tcl_Obj * objPtr); 
    int (*tcl_ListObjGetElements) (Tcl_Interp * interp, Tcl_Obj * listPtr, int * objcPtr, Tcl_Obj *** objvPtr); 
    int (*tcl_ListObjIndex) (Tcl_Interp * interp, Tcl_Obj * listPtr, int index, Tcl_Obj ** objPtrPtr); 
    int (*tcl_ListObjLength) (Tcl_Interp * interp, Tcl_Obj * listPtr, int * intPtr); 
    int (*tcl_ListObjReplace) (Tcl_Interp * interp, Tcl_Obj * listPtr, int first, int count, int objc, Tcl_Obj *const objv[]); 
    Tcl_Obj * (*tcl_NewBooleanObj) (int boolValue); 
    Tcl_Obj * (*tcl_NewByteArrayObj) (unsigned char * bytes, int length); 
    Tcl_Obj * (*tcl_NewDoubleObj) (double doubleValue); 
    Tcl_Obj * (*tcl_NewIntObj) (int intValue); 
    Tcl_Obj * (*tcl_NewListObj) (int objc, Tcl_Obj *const objv[]); 
    Tcl_Obj * (*tcl_NewLongObj) (long longValue); 
    Tcl_Obj * (*tcl_NewObj) (void); 
    Tcl_Obj * (*tcl_NewStringObj) (const char * bytes, int length); 
    void (*tcl_SetBooleanObj) (Tcl_Obj * objPtr, int boolValue); 
    unsigned char * (*tcl_SetByteArrayLength) (Tcl_Obj * objPtr, int length); 
    void (*tcl_SetByteArrayObj) (Tcl_Obj * objPtr, unsigned char * bytes, int length); 
    void (*tcl_SetDoubleObj) (Tcl_Obj * objPtr, double doubleValue); 
    void (*tcl_SetIntObj) (Tcl_Obj * objPtr, int intValue); 
    void (*tcl_SetListObj) (Tcl_Obj * objPtr, int objc, Tcl_Obj *const objv[]); 
    void (*tcl_SetLongObj) (Tcl_Obj * objPtr, long longValue); 
    void (*tcl_SetObjLength) (Tcl_Obj * objPtr, int length); 
    void (*tcl_SetStringObj) (Tcl_Obj * objPtr, char * bytes, int length); 
    void (*tcl_AddErrorInfo) (Tcl_Interp * interp, const char * message); 
    void (*tcl_AddObjErrorInfo) (Tcl_Interp * interp, const char * message, int length); 
    void (*tcl_AllowExceptions) (Tcl_Interp * interp); 
    void (*tcl_AppendElement) (Tcl_Interp * interp, const char * string); 
    void (*tcl_AppendResult) (Tcl_Interp * interp, ...); 
    Tcl_AsyncHandler (*tcl_AsyncCreate) (Tcl_AsyncProc * proc, ClientData clientData); 
    void (*tcl_AsyncDelete) (Tcl_AsyncHandler async); 
    int (*tcl_AsyncInvoke) (Tcl_Interp * interp, int code); 
    void (*tcl_AsyncMark) (Tcl_AsyncHandler async); 
    int (*tcl_AsyncReady) (void); 
    void (*tcl_BackgroundError) (Tcl_Interp * interp); 
    char (*tcl_Backslash) (const char * src, int * readPtr); 
    int (*tcl_BadChannelOption) (Tcl_Interp * interp, char * optionName, char * optionList); 
    void (*tcl_CallWhenDeleted) (Tcl_Interp * interp, Tcl_InterpDeleteProc * proc, ClientData clientData); 
    void (*tcl_CancelIdleCall) (Tcl_IdleProc * idleProc, ClientData clientData); 
    int (*tcl_Close) (Tcl_Interp * interp, Tcl_Channel chan); 
    int (*tcl_CommandComplete) (char * cmd); 
    char * (*tcl_Concat) (int argc, char ** argv); 
    int (*tcl_ConvertElement) (const char * src, char * dst, int flags); 
    int (*tcl_ConvertCountedElement) (const char * src, int length, char * dst, int flags); 
    int (*tcl_CreateAlias) (Tcl_Interp * slave, char * slaveCmd, Tcl_Interp * target, char * targetCmd, int argc, char ** argv); 
    int (*tcl_CreateAliasObj) (Tcl_Interp * slave, char * slaveCmd, Tcl_Interp * target, char * targetCmd, int objc, Tcl_Obj *const objv[]); 
    Tcl_Channel (*tcl_CreateChannel) (Tcl_ChannelType * typePtr, char * chanName, ClientData instanceData, int mask); 
    void (*tcl_CreateChannelHandler) (Tcl_Channel chan, int mask, Tcl_ChannelProc * proc, ClientData clientData); 
    void (*tcl_CreateCloseHandler) (Tcl_Channel chan, Tcl_CloseProc * proc, ClientData clientData); 
    Tcl_Command (*tcl_CreateCommand) (Tcl_Interp * interp, char * cmdName, Tcl_CmdProc * proc, ClientData clientData, Tcl_CmdDeleteProc * deleteProc); 
    void (*tcl_CreateEventSource) (Tcl_EventSetupProc * setupProc, Tcl_EventCheckProc * checkProc, ClientData clientData); 
    void (*tcl_CreateExitHandler) (Tcl_ExitProc * proc, ClientData clientData); 
    Tcl_Interp * (*tcl_CreateInterp) (void); 
    void (*tcl_CreateMathFunc) (Tcl_Interp * interp, char * name, int numArgs, Tcl_ValueType * argTypes, Tcl_MathProc * proc, ClientData clientData); 
    Tcl_Command (*tcl_CreateObjCommand) (Tcl_Interp * interp, char * cmdName, Tcl_ObjCmdProc * proc, ClientData clientData, Tcl_CmdDeleteProc * deleteProc); 
    Tcl_Interp * (*tcl_CreateSlave) (Tcl_Interp * interp, char * slaveName, int isSafe); 
    Tcl_TimerToken (*tcl_CreateTimerHandler) (int milliseconds, Tcl_TimerProc * proc, ClientData clientData); 
    Tcl_Trace (*tcl_CreateTrace) (Tcl_Interp * interp, int level, Tcl_CmdTraceProc * proc, ClientData clientData); 
    void (*tcl_DeleteAssocData) (Tcl_Interp * interp, char * name); 
    void (*tcl_DeleteChannelHandler) (Tcl_Channel chan, Tcl_ChannelProc * proc, ClientData clientData); 
    void (*tcl_DeleteCloseHandler) (Tcl_Channel chan, Tcl_CloseProc * proc, ClientData clientData); 
    int (*tcl_DeleteCommand) (Tcl_Interp * interp, char * cmdName); 
    int (*tcl_DeleteCommandFromToken) (Tcl_Interp * interp, Tcl_Command command); 
    void (*tcl_DeleteEvents) (Tcl_EventDeleteProc * proc, ClientData clientData); 
    void (*tcl_DeleteEventSource) (Tcl_EventSetupProc * setupProc, Tcl_EventCheckProc * checkProc, ClientData clientData); 
    void (*tcl_DeleteExitHandler) (Tcl_ExitProc * proc, ClientData clientData); 
    void (*tcl_DeleteHashEntry) (Tcl_HashEntry * entryPtr); 
    void (*tcl_DeleteHashTable) (Tcl_HashTable * tablePtr); 
    void (*tcl_DeleteInterp) (Tcl_Interp * interp); 


#line 1372 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"

    void (*tcl_DetachPids) (int numPids, Tcl_Pid * pidPtr); 
#line 1375 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



    void (*tcl_DeleteTimerHandler) (Tcl_TimerToken token); 
    void (*tcl_DeleteTrace) (Tcl_Interp * interp, Tcl_Trace trace); 
    void (*tcl_DontCallWhenDeleted) (Tcl_Interp * interp, Tcl_InterpDeleteProc * proc, ClientData clientData); 
    int (*tcl_DoOneEvent) (int flags); 
    void (*tcl_DoWhenIdle) (Tcl_IdleProc * proc, ClientData clientData); 
    char * (*tcl_DStringAppend) (Tcl_DString * dsPtr, const char * str, int length); 
    char * (*tcl_DStringAppendElement) (Tcl_DString * dsPtr, const char * string); 
    void (*tcl_DStringEndSublist) (Tcl_DString * dsPtr); 
    void (*tcl_DStringFree) (Tcl_DString * dsPtr); 
    void (*tcl_DStringGetResult) (Tcl_Interp * interp, Tcl_DString * dsPtr); 
    void (*tcl_DStringInit) (Tcl_DString * dsPtr); 
    void (*tcl_DStringResult) (Tcl_Interp * interp, Tcl_DString * dsPtr); 
    void (*tcl_DStringSetLength) (Tcl_DString * dsPtr, int length); 
    void (*tcl_DStringStartSublist) (Tcl_DString * dsPtr); 
    int (*tcl_Eof) (Tcl_Channel chan); 
    char * (*tcl_ErrnoId) (void); 
    char * (*tcl_ErrnoMsg) (int err); 
    int (*tcl_Eval) (Tcl_Interp * interp, char * string); 
    int (*tcl_EvalFile) (Tcl_Interp * interp, char * fileName); 
    int (*tcl_EvalObj) (Tcl_Interp * interp, Tcl_Obj * objPtr); 
    void (*tcl_EventuallyFree) (ClientData clientData, Tcl_FreeProc * freeProc); 
    void (*tcl_Exit) (int status); 
    int (*tcl_ExposeCommand) (Tcl_Interp * interp, char * hiddenCmdToken, char * cmdName); 
    int (*tcl_ExprBoolean) (Tcl_Interp * interp, char * str, int * ptr); 
    int (*tcl_ExprBooleanObj) (Tcl_Interp * interp, Tcl_Obj * objPtr, int * ptr); 
    int (*tcl_ExprDouble) (Tcl_Interp * interp, char * str, double * ptr); 
    int (*tcl_ExprDoubleObj) (Tcl_Interp * interp, Tcl_Obj * objPtr, double * ptr); 
    int (*tcl_ExprLong) (Tcl_Interp * interp, char * str, long * ptr); 
    int (*tcl_ExprLongObj) (Tcl_Interp * interp, Tcl_Obj * objPtr, long * ptr); 
    int (*tcl_ExprObj) (Tcl_Interp * interp, Tcl_Obj * objPtr, Tcl_Obj ** resultPtrPtr); 
    int (*tcl_ExprString) (Tcl_Interp * interp, char * string); 
    void (*tcl_Finalize) (void); 
    void (*tcl_FindExecutable) (const char * argv0); 
    Tcl_HashEntry * (*tcl_FirstHashEntry) (Tcl_HashTable * tablePtr, Tcl_HashSearch * searchPtr); 
    int (*tcl_Flush) (Tcl_Channel chan); 
    void (*tcl_FreeResult) (Tcl_Interp * interp); 
    int (*tcl_GetAlias) (Tcl_Interp * interp, char * slaveCmd, Tcl_Interp ** targetInterpPtr, char ** targetCmdPtr, int * argcPtr, char *** argvPtr); 
    int (*tcl_GetAliasObj) (Tcl_Interp * interp, char * slaveCmd, Tcl_Interp ** targetInterpPtr, char ** targetCmdPtr, int * objcPtr, Tcl_Obj *** objv); 
    ClientData (*tcl_GetAssocData) (Tcl_Interp * interp, char * name, Tcl_InterpDeleteProc ** procPtr); 
    Tcl_Channel (*tcl_GetChannel) (Tcl_Interp * interp, char * chanName, int * modePtr); 
    int (*tcl_GetChannelBufferSize) (Tcl_Channel chan); 
    int (*tcl_GetChannelHandle) (Tcl_Channel chan, int direction, ClientData * handlePtr); 
    ClientData (*tcl_GetChannelInstanceData) (Tcl_Channel chan); 
    int (*tcl_GetChannelMode) (Tcl_Channel chan); 
    char * (*tcl_GetChannelName) (Tcl_Channel chan); 
    int (*tcl_GetChannelOption) (Tcl_Interp * interp, Tcl_Channel chan, char * optionName, Tcl_DString * dsPtr); 
    Tcl_ChannelType * (*tcl_GetChannelType) (Tcl_Channel chan); 
    int (*tcl_GetCommandInfo) (Tcl_Interp * interp, char * cmdName, Tcl_CmdInfo * infoPtr); 
    char * (*tcl_GetCommandName) (Tcl_Interp * interp, Tcl_Command command); 
    int (*tcl_GetErrno) (void); 
    char * (*tcl_GetHostName) (void); 
    int (*tcl_GetInterpPath) (Tcl_Interp * askInterp, Tcl_Interp * slaveInterp); 
    Tcl_Interp * (*tcl_GetMaster) (Tcl_Interp * interp); 
    const char * (*tcl_GetNameOfExecutable) (void); 
    Tcl_Obj * (*tcl_GetObjResult) (Tcl_Interp * interp); 


#line 1436 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"

    void *reserved167;
#line 1439 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



    Tcl_PathType (*tcl_GetPathType) (char * path); 
    int (*tcl_Gets) (Tcl_Channel chan, Tcl_DString * dsPtr); 
    int (*tcl_GetsObj) (Tcl_Channel chan, Tcl_Obj * objPtr); 
    int (*tcl_GetServiceMode) (void); 
    Tcl_Interp * (*tcl_GetSlave) (Tcl_Interp * interp, char * slaveName); 
    Tcl_Channel (*tcl_GetStdChannel) (int type); 
    char * (*tcl_GetStringResult) (Tcl_Interp * interp); 
    char * (*tcl_GetVar) (Tcl_Interp * interp, char * varName, int flags); 
    char * (*tcl_GetVar2) (Tcl_Interp * interp, char * part1, char * part2, int flags); 
    int (*tcl_GlobalEval) (Tcl_Interp * interp, char * command); 
    int (*tcl_GlobalEvalObj) (Tcl_Interp * interp, Tcl_Obj * objPtr); 
    int (*tcl_HideCommand) (Tcl_Interp * interp, char * cmdName, char * hiddenCmdToken); 
    int (*tcl_Init) (Tcl_Interp * interp); 
    void (*tcl_InitHashTable) (Tcl_HashTable * tablePtr, int keyType); 
    int (*tcl_InputBlocked) (Tcl_Channel chan); 
    int (*tcl_InputBuffered) (Tcl_Channel chan); 
    int (*tcl_InterpDeleted) (Tcl_Interp * interp); 
    int (*tcl_IsSafe) (Tcl_Interp * interp); 
    char * (*tcl_JoinPath) (int argc, char ** argv, Tcl_DString * resultPtr); 
    int (*tcl_LinkVar) (Tcl_Interp * interp, char * varName, char * addr, int type); 
    void *reserved188;
    Tcl_Channel (*tcl_MakeFileChannel) (ClientData handle, int mode); 
    int (*tcl_MakeSafe) (Tcl_Interp * interp); 
    Tcl_Channel (*tcl_MakeTcpClientChannel) (ClientData tcpSocket); 
    char * (*tcl_Merge) (int argc, char ** argv); 
    Tcl_HashEntry * (*tcl_NextHashEntry) (Tcl_HashSearch * searchPtr); 
    void (*tcl_NotifyChannel) (Tcl_Channel channel, int mask); 
    Tcl_Obj * (*tcl_ObjGetVar2) (Tcl_Interp * interp, Tcl_Obj * part1Ptr, Tcl_Obj * part2Ptr, int flags); 
    Tcl_Obj * (*tcl_ObjSetVar2) (Tcl_Interp * interp, Tcl_Obj * part1Ptr, Tcl_Obj * part2Ptr, Tcl_Obj * newValuePtr, int flags); 


#line 1474 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"

    Tcl_Channel (*tcl_OpenCommandChannel) (Tcl_Interp * interp, int argc, char ** argv, int flags); 
#line 1477 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



    Tcl_Channel (*tcl_OpenFileChannel) (Tcl_Interp * interp, char * fileName, char * modeString, int permissions); 
    Tcl_Channel (*tcl_OpenTcpClient) (Tcl_Interp * interp, int port, char * address, char * myaddr, int myport, int async); 
    Tcl_Channel (*tcl_OpenTcpServer) (Tcl_Interp * interp, int port, char * host, Tcl_TcpAcceptProc * acceptProc, ClientData callbackData); 
    void (*tcl_Preserve) (ClientData data); 
    void (*tcl_PrintDouble) (Tcl_Interp * interp, double value, char * dst); 
    int (*tcl_PutEnv) (const char * string); 
    char * (*tcl_PosixError) (Tcl_Interp * interp); 
    void (*tcl_QueueEvent) (Tcl_Event * evPtr, Tcl_QueuePosition position); 
    int (*tcl_Read) (Tcl_Channel chan, char * bufPtr, int toRead); 


#line 1492 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"

    void (*tcl_ReapDetachedProcs) (void); 
#line 1495 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



    int (*tcl_RecordAndEval) (Tcl_Interp * interp, char * cmd, int flags); 
    int (*tcl_RecordAndEvalObj) (Tcl_Interp * interp, Tcl_Obj * cmdPtr, int flags); 
    void (*tcl_RegisterChannel) (Tcl_Interp * interp, Tcl_Channel chan); 
    void (*tcl_RegisterObjType) (Tcl_ObjType * typePtr); 
    Tcl_RegExp (*tcl_RegExpCompile) (Tcl_Interp * interp, char * string); 
    int (*tcl_RegExpExec) (Tcl_Interp * interp, Tcl_RegExp regexp, const char * str, const char * start); 
    int (*tcl_RegExpMatch) (Tcl_Interp * interp, char * str, char * pattern); 
    void (*tcl_RegExpRange) (Tcl_RegExp regexp, int index, char ** startPtr, char ** endPtr); 
    void (*tcl_Release) (ClientData clientData); 
    void (*tcl_ResetResult) (Tcl_Interp * interp); 
    int (*tcl_ScanElement) (const char * str, int * flagPtr); 
    int (*tcl_ScanCountedElement) (const char * str, int length, int * flagPtr); 
    int (*tcl_Seek) (Tcl_Channel chan, int offset, int mode); 
    int (*tcl_ServiceAll) (void); 
    int (*tcl_ServiceEvent) (int flags); 
    void (*tcl_SetAssocData) (Tcl_Interp * interp, char * name, Tcl_InterpDeleteProc * proc, ClientData clientData); 
    void (*tcl_SetChannelBufferSize) (Tcl_Channel chan, int sz); 
    int (*tcl_SetChannelOption) (Tcl_Interp * interp, Tcl_Channel chan, char * optionName, char * newValue); 
    int (*tcl_SetCommandInfo) (Tcl_Interp * interp, char * cmdName, Tcl_CmdInfo * infoPtr); 
    void (*tcl_SetErrno) (int err); 
    void (*tcl_SetErrorCode) (Tcl_Interp * interp, ...); 
    void (*tcl_SetMaxBlockTime) (Tcl_Time * timePtr); 
    void (*tcl_SetPanicProc) (Tcl_PanicProc * panicProc); 
    int (*tcl_SetRecursionLimit) (Tcl_Interp * interp, int depth); 
    void (*tcl_SetResult) (Tcl_Interp * interp, char * str, Tcl_FreeProc * freeProc); 
    int (*tcl_SetServiceMode) (int mode); 
    void (*tcl_SetObjErrorCode) (Tcl_Interp * interp, Tcl_Obj * errorObjPtr); 
    void (*tcl_SetObjResult) (Tcl_Interp * interp, Tcl_Obj * resultObjPtr); 
    void (*tcl_SetStdChannel) (Tcl_Channel channel, int type); 
    char * (*tcl_SetVar) (Tcl_Interp * interp, char * varName, char * newValue, int flags); 
    char * (*tcl_SetVar2) (Tcl_Interp * interp, char * part1, char * part2, char * newValue, int flags); 
    char * (*tcl_SignalId) (int sig); 
    char * (*tcl_SignalMsg) (int sig); 
    void (*tcl_SourceRCFile) (Tcl_Interp * interp); 
    int (*tcl_SplitList) (Tcl_Interp * interp, const char * listStr, int * argcPtr, char *** argvPtr); 
    void (*tcl_SplitPath) (const char * path, int * argcPtr, char *** argvPtr); 
    void (*tcl_StaticPackage) (Tcl_Interp * interp, char * pkgName, Tcl_PackageInitProc * initProc, Tcl_PackageInitProc * safeInitProc); 
    int (*tcl_StringMatch) (const char * str, const char * pattern); 
    int (*tcl_Tell) (Tcl_Channel chan); 
    int (*tcl_TraceVar) (Tcl_Interp * interp, char * varName, int flags, Tcl_VarTraceProc * proc, ClientData clientData); 
    int (*tcl_TraceVar2) (Tcl_Interp * interp, char * part1, char * part2, int flags, Tcl_VarTraceProc * proc, ClientData clientData); 
    char * (*tcl_TranslateFileName) (Tcl_Interp * interp, char * name, Tcl_DString * bufferPtr); 
    int (*tcl_Ungets) (Tcl_Channel chan, char * str, int len, int atHead); 
    void (*tcl_UnlinkVar) (Tcl_Interp * interp, char * varName); 
    int (*tcl_UnregisterChannel) (Tcl_Interp * interp, Tcl_Channel chan); 
    int (*tcl_UnsetVar) (Tcl_Interp * interp, char * varName, int flags); 
    int (*tcl_UnsetVar2) (Tcl_Interp * interp, char * part1, char * part2, int flags); 
    void (*tcl_UntraceVar) (Tcl_Interp * interp, char * varName, int flags, Tcl_VarTraceProc * proc, ClientData clientData); 
    void (*tcl_UntraceVar2) (Tcl_Interp * interp, char * part1, char * part2, int flags, Tcl_VarTraceProc * proc, ClientData clientData); 
    void (*tcl_UpdateLinkedVar) (Tcl_Interp * interp, char * varName); 
    int (*tcl_UpVar) (Tcl_Interp * interp, char * frameName, char * varName, char * localName, int flags); 
    int (*tcl_UpVar2) (Tcl_Interp * interp, char * frameName, char * part1, char * part2, char * localName, int flags); 
    int (*tcl_VarEval) (Tcl_Interp * interp, ...); 
    ClientData (*tcl_VarTraceInfo) (Tcl_Interp * interp, char * varName, int flags, Tcl_VarTraceProc * procPtr, ClientData prevClientData); 
    ClientData (*tcl_VarTraceInfo2) (Tcl_Interp * interp, char * part1, char * part2, int flags, Tcl_VarTraceProc * procPtr, ClientData prevClientData); 
    int (*tcl_Write) (Tcl_Channel chan, char * s, int slen); 
    void (*tcl_WrongNumArgs) (Tcl_Interp * interp, int objc, Tcl_Obj *const objv[], char * message); 
    int (*tcl_DumpActiveMemory) (char * fileName); 
    void (*tcl_ValidateAllMemory) (char * file, int line); 
    void (*tcl_AppendResultVA) (Tcl_Interp * interp, va_list argList); 
    void (*tcl_AppendStringsToObjVA) (Tcl_Obj * objPtr, va_list argList); 
    char * (*tcl_HashStats) (Tcl_HashTable * tablePtr); 
    char * (*tcl_ParseVar) (Tcl_Interp * interp, char * str, char ** termPtr); 
    char * (*tcl_PkgPresent) (Tcl_Interp * interp, char * name, char * version, int exact); 
    char * (*tcl_PkgPresentEx) (Tcl_Interp * interp, char * name, char * version, int exact, ClientData * clientDataPtr); 
    int (*tcl_PkgProvide) (Tcl_Interp * interp, char * name, char * version); 
    char * (*tcl_PkgRequire) (Tcl_Interp * interp, char * name, char * version, int exact); 
    void (*tcl_SetErrorCodeVA) (Tcl_Interp * interp, va_list argList); 
    int (*tcl_VarEvalVA) (Tcl_Interp * interp, va_list argList); 
    Tcl_Pid (*tcl_WaitPid) (Tcl_Pid pid, int * statPtr, int options); 


#line 1571 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"

    void (*tcl_PanicVA) (char * format, va_list argList); 
#line 1574 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



    void (*tcl_GetVersion) (int * major, int * minor, int * patchLevel, int * type); 
    void (*tcl_InitMemory) (Tcl_Interp * interp); 
    Tcl_Channel (*tcl_StackChannel) (Tcl_Interp * interp, Tcl_ChannelType * typePtr, ClientData instanceData, int mask, Tcl_Channel prevChan); 
    void (*tcl_UnstackChannel) (Tcl_Interp * interp, Tcl_Channel chan); 
    Tcl_Channel (*tcl_GetStackedChannel) (Tcl_Channel chan); 
    void *reserved284;
    void *reserved285;
    void (*tcl_AppendObjToObj) (Tcl_Obj * objPtr, Tcl_Obj * appendObjPtr); 
    Tcl_Encoding (*tcl_CreateEncoding) (Tcl_EncodingType * typePtr); 
    void (*tcl_CreateThreadExitHandler) (Tcl_ExitProc * proc, ClientData clientData); 
    void (*tcl_DeleteThreadExitHandler) (Tcl_ExitProc * proc, ClientData clientData); 
    void (*tcl_DiscardResult) (Tcl_SavedResult * statePtr); 
    int (*tcl_EvalEx) (Tcl_Interp * interp, char * script, int numBytes, int flags); 
    int (*tcl_EvalObjv) (Tcl_Interp * interp, int objc, Tcl_Obj *const objv[], int flags); 
    int (*tcl_EvalObjEx) (Tcl_Interp * interp, Tcl_Obj * objPtr, int flags); 
    void (*tcl_ExitThread) (int status); 
    int (*tcl_ExternalToUtf) (Tcl_Interp * interp, Tcl_Encoding encoding, const char * src, int srcLen, int flags, Tcl_EncodingState * statePtr, char * dst, int dstLen, int * srcReadPtr, int * dstWrotePtr, int * dstCharsPtr); 
    char * (*tcl_ExternalToUtfDString) (Tcl_Encoding encoding, const char * src, int srcLen, Tcl_DString * dsPtr); 
    void (*tcl_FinalizeThread) (void); 
    void (*tcl_FinalizeNotifier) (ClientData clientData); 
    void (*tcl_FreeEncoding) (Tcl_Encoding encoding); 
    Tcl_ThreadId (*tcl_GetCurrentThread) (void); 
    Tcl_Encoding (*tcl_GetEncoding) (Tcl_Interp * interp, const char * name); 
    char * (*tcl_GetEncodingName) (Tcl_Encoding encoding); 
    void (*tcl_GetEncodingNames) (Tcl_Interp * interp); 
    int (*tcl_GetIndexFromObjStruct) (Tcl_Interp * interp, Tcl_Obj * objPtr, char ** tablePtr, int offset, char * msg, int flags, int * indexPtr); 
    void * (*tcl_GetThreadData) (Tcl_ThreadDataKey * keyPtr, int size); 
    Tcl_Obj * (*tcl_GetVar2Ex) (Tcl_Interp * interp, char * part1, char * part2, int flags); 
    ClientData (*tcl_InitNotifier) (void); 
    void (*tcl_MutexLock) (Tcl_Mutex * mutexPtr); 
    void (*tcl_MutexUnlock) (Tcl_Mutex * mutexPtr); 
    void (*tcl_ConditionNotify) (Tcl_Condition * condPtr); 
    void (*tcl_ConditionWait) (Tcl_Condition * condPtr, Tcl_Mutex * mutexPtr, Tcl_Time * timePtr); 
    int (*tcl_NumUtfChars) (const char * src, int len); 
    int (*tcl_ReadChars) (Tcl_Channel channel, Tcl_Obj * objPtr, int charsToRead, int appendFlag); 
    void (*tcl_RestoreResult) (Tcl_Interp * interp, Tcl_SavedResult * statePtr); 
    void (*tcl_SaveResult) (Tcl_Interp * interp, Tcl_SavedResult * statePtr); 
    int (*tcl_SetSystemEncoding) (Tcl_Interp * interp, const char * name); 
    Tcl_Obj * (*tcl_SetVar2Ex) (Tcl_Interp * interp, char * part1, char * part2, Tcl_Obj * newValuePtr, int flags); 
    void (*tcl_ThreadAlert) (Tcl_ThreadId threadId); 
    void (*tcl_ThreadQueueEvent) (Tcl_ThreadId threadId, Tcl_Event* evPtr, Tcl_QueuePosition position); 
    Tcl_UniChar (*tcl_UniCharAtIndex) (const char * src, int index); 
    Tcl_UniChar (*tcl_UniCharToLower) (int ch); 
    Tcl_UniChar (*tcl_UniCharToTitle) (int ch); 
    Tcl_UniChar (*tcl_UniCharToUpper) (int ch); 
    int (*tcl_UniCharToUtf) (int ch, char * buf); 
    char * (*tcl_UtfAtIndex) (const char * src, int index); 
    int (*tcl_UtfCharComplete) (const char * src, int len); 
    int (*tcl_UtfBackslash) (const char * src, int * readPtr, char * dst); 
    char * (*tcl_UtfFindFirst) (const char * src, int ch); 
    char * (*tcl_UtfFindLast) (const char * src, int ch); 
    char * (*tcl_UtfNext) (const char * src); 
    char * (*tcl_UtfPrev) (const char * src, const char * start); 
    int (*tcl_UtfToExternal) (Tcl_Interp * interp, Tcl_Encoding encoding, const char * src, int srcLen, int flags, Tcl_EncodingState * statePtr, char * dst, int dstLen, int * srcReadPtr, int * dstWrotePtr, int * dstCharsPtr); 
    char * (*tcl_UtfToExternalDString) (Tcl_Encoding encoding, const char * src, int srcLen, Tcl_DString * dsPtr); 
    int (*tcl_UtfToLower) (char * src); 
    int (*tcl_UtfToTitle) (char * src); 
    int (*tcl_UtfToUniChar) (const char * src, Tcl_UniChar * chPtr); 
    int (*tcl_UtfToUpper) (char * src); 
    int (*tcl_WriteChars) (Tcl_Channel chan, const char * src, int srcLen); 
    int (*tcl_WriteObj) (Tcl_Channel chan, Tcl_Obj * objPtr); 
    char * (*tcl_GetString) (Tcl_Obj * objPtr); 
    char * (*tcl_GetDefaultEncodingDir) (void); 
    void (*tcl_SetDefaultEncodingDir) (char * path); 
    void (*tcl_AlertNotifier) (ClientData clientData); 
    void (*tcl_ServiceModeHook) (int mode); 
    int (*tcl_UniCharIsAlnum) (int ch); 
    int (*tcl_UniCharIsAlpha) (int ch); 
    int (*tcl_UniCharIsDigit) (int ch); 
    int (*tcl_UniCharIsLower) (int ch); 
    int (*tcl_UniCharIsSpace) (int ch); 
    int (*tcl_UniCharIsUpper) (int ch); 
    int (*tcl_UniCharIsWordChar) (int ch); 
    int (*tcl_UniCharLen) (Tcl_UniChar * str); 
    int (*tcl_UniCharNcmp) (const Tcl_UniChar * cs, const Tcl_UniChar * ct, unsigned long n); 
    char * (*tcl_UniCharToUtfDString) (const Tcl_UniChar * string, int numChars, Tcl_DString * dsPtr); 
    Tcl_UniChar * (*tcl_UtfToUniCharDString) (const char * string, int length, Tcl_DString * dsPtr); 
    Tcl_RegExp (*tcl_GetRegExpFromObj) (Tcl_Interp * interp, Tcl_Obj * patObj, int flags); 
    Tcl_Obj * (*tcl_EvalTokens) (Tcl_Interp * interp, Tcl_Token * tokenPtr, int count); 
    void (*tcl_FreeParse) (Tcl_Parse * parsePtr); 
    void (*tcl_LogCommandInfo) (Tcl_Interp * interp, char * script, char * command, int length); 
    int (*tcl_ParseBraces) (Tcl_Interp * interp, char * string, int numBytes, Tcl_Parse * parsePtr, int append, char ** termPtr); 
    int (*tcl_ParseCommand) (Tcl_Interp * interp, char * string, int numBytes, int nested, Tcl_Parse * parsePtr); 
    int (*tcl_ParseExpr) (Tcl_Interp * interp, char * string, int numBytes, Tcl_Parse * parsePtr); 
    int (*tcl_ParseQuotedString) (Tcl_Interp * interp, char * string, int numBytes, Tcl_Parse * parsePtr, int append, char ** termPtr); 
    int (*tcl_ParseVarName) (Tcl_Interp * interp, char * string, int numBytes, Tcl_Parse * parsePtr, int append); 
    char * (*tcl_GetCwd) (Tcl_Interp * interp, Tcl_DString * cwdPtr); 
    int (*tcl_Chdir) (const char * dirName); 
    int (*tcl_Access) (const char * path, int mode); 
    int (*tcl_Stat) (const char * path, struct stat * bufPtr); 
    int (*tcl_UtfNcmp) (const char * s1, const char * s2, unsigned long n); 
    int (*tcl_UtfNcasecmp) (const char * s1, const char * s2, unsigned long n); 
    int (*tcl_StringCaseMatch) (const char * str, const char * pattern, int nocase); 
    int (*tcl_UniCharIsControl) (int ch); 
    int (*tcl_UniCharIsGraph) (int ch); 
    int (*tcl_UniCharIsPrint) (int ch); 
    int (*tcl_UniCharIsPunct) (int ch); 
    int (*tcl_RegExpExecObj) (Tcl_Interp * interp, Tcl_RegExp regexp, Tcl_Obj * objPtr, int offset, int nmatches, int flags); 
    void (*tcl_RegExpGetInfo) (Tcl_RegExp regexp, Tcl_RegExpInfo * infoPtr); 
    Tcl_Obj * (*tcl_NewUnicodeObj) (Tcl_UniChar * unicode, int numChars); 
    void (*tcl_SetUnicodeObj) (Tcl_Obj * objPtr, Tcl_UniChar * unicode, int numChars); 
    int (*tcl_GetCharLength) (Tcl_Obj * objPtr); 
    Tcl_UniChar (*tcl_GetUniChar) (Tcl_Obj * objPtr, int index); 
    Tcl_UniChar * (*tcl_GetUnicode) (Tcl_Obj * objPtr); 
    Tcl_Obj * (*tcl_GetRange) (Tcl_Obj * objPtr, int first, int last); 
    void (*tcl_AppendUnicodeToObj) (Tcl_Obj * objPtr, Tcl_UniChar * unicode, int length); 
    int (*tcl_RegExpMatchObj) (Tcl_Interp * interp, Tcl_Obj * stringObj, Tcl_Obj * patternObj); 
    void (*tcl_SetNotifier) (Tcl_NotifierProcs * notifierProcPtr); 
    Tcl_Mutex * (*tcl_GetAllocMutex) (void); 
    int (*tcl_GetChannelNames) (Tcl_Interp * interp); 
    int (*tcl_GetChannelNamesEx) (Tcl_Interp * interp, char * pattern); 
    int (*tcl_ProcObjCmd) (ClientData clientData, Tcl_Interp * interp, int objc, Tcl_Obj *const objv[]); 
    void (*tcl_ConditionFinalize) (Tcl_Condition * condPtr); 
    void (*tcl_MutexFinalize) (Tcl_Mutex * mutex); 
    int (*tcl_CreateThread) (Tcl_ThreadId * idPtr, Tcl_ThreadCreateProc proc, ClientData clientData, int stackSize, int flags); 
} TclStubs;




extern TclStubs *tclStubsPtr;













#line 1712 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1716 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1720 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1724 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1728 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1732 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1736 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1740 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1744 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"





#line 1750 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"





#line 1756 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1760 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1764 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1768 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1772 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1776 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1780 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1784 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1788 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1792 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1796 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1800 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1804 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1808 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1812 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1816 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1820 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1824 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1828 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1832 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1836 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1840 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1844 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1848 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1852 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1856 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1860 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1864 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1868 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1872 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1876 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1880 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1884 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1888 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1892 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1896 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1900 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1904 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1908 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1912 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1916 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1920 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1924 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1928 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1932 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1936 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1940 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1944 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1948 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1952 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1956 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1960 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1964 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1968 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1972 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1976 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1980 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1984 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1988 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1992 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 1996 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2000 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2004 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2008 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2012 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2016 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2020 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2024 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2028 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2032 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2036 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2040 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2044 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2048 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2052 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2056 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2060 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2064 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2068 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2072 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2076 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2080 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2084 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2088 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2092 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2096 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2100 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2104 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2108 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2112 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2116 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2120 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2124 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2128 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2132 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2136 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2140 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2144 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2148 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2152 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2156 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"





#line 2162 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"




#line 2167 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"
#line 2168 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2172 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2176 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2180 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2184 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2188 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2192 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2196 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2200 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2204 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2208 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2212 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2216 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2220 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2224 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2228 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2232 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2236 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2240 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2244 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"







#line 2252 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2256 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2260 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2264 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2268 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2272 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2276 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2280 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2284 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2288 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2292 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2296 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2300 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2304 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2308 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2312 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2316 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2320 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2324 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2328 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2332 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2336 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2340 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2344 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2348 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2352 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2356 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2360 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2364 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2368 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2372 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2376 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2380 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2384 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2388 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"





#line 2394 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2398 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2402 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2406 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2410 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2414 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2418 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2422 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2426 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2430 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2434 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"







#line 2442 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2446 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2450 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2454 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2458 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2462 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2466 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2470 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2474 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"




#line 2479 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2483 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2487 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2491 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2495 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2499 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2503 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2507 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"





#line 2513 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"




#line 2518 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"
#line 2519 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2523 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2527 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2531 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2535 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2539 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2543 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2547 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2551 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2555 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"





#line 2561 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"




#line 2566 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"
#line 2567 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2571 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2575 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2579 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2583 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2587 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2591 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2595 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2599 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2603 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2607 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2611 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2615 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2619 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2623 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2627 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2631 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2635 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2639 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2643 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2647 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2651 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2655 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2659 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2663 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2667 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2671 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2675 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2679 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2683 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2687 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2691 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2695 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2699 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2703 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2707 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2711 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2715 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2719 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2723 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2727 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2731 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2735 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2739 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2743 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2747 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2751 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2755 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2759 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2763 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2767 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2771 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2775 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2779 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2783 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2787 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2791 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2795 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"











#line 2807 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2811 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2815 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2819 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2823 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2827 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2831 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2835 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2839 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2843 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2847 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"





#line 2853 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"




#line 2858 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"
#line 2859 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2863 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"







#line 2871 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2875 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2879 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"





#line 2885 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2889 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2893 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2897 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2901 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2905 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2909 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2913 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2917 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2921 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2925 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2929 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2933 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2937 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2941 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2945 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2949 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2953 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2957 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2961 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2965 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2969 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



















#line 2989 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2993 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 2997 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3001 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3005 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3009 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3013 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3017 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3021 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3025 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3029 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3033 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3037 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3041 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3045 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3049 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3053 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3057 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3061 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3065 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3069 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3073 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3077 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3081 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3085 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3089 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3093 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3097 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3101 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3105 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3109 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3113 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3117 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3121 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3125 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3129 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3133 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3137 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3141 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3145 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3149 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3153 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3157 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3161 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3165 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3169 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3173 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3177 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3181 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3185 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3189 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3193 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3197 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3201 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3205 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3209 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3213 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3217 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3221 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3225 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3229 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3233 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3237 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3241 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3245 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3249 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3253 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3257 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3261 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3265 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3269 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3273 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3277 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3281 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3285 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3289 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3293 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3297 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3301 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"











#line 3313 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"

#line 3315 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"



#line 3319 "d:\\tcl-source\\tcl8.3.1\\generic\\tclDecls.h"

#line 1656 "d:\\tcl-source\\tcl8.3.1\\generic\\tcl.h"





extern  void Tcl_Main 
(int argc, char **argv, Tcl_AppInitProc *appInitProc);










extern  int		Tcl_AppInit (Tcl_Interp *interp);

#line 1676 "d:\\tcl-source\\tcl8.3.1\\generic\\tcl.h"







    



    
#line 1689 "d:\\tcl-source\\tcl8.3.1\\generic\\tcl.h"
#line 37 "./../tcl\\pltcl.h"

#line 1 "./../tcl\\tclMatrix.h"








































































































#line 1 "./../../include\\plplot.h"



















































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































#line 106 "./../tcl\\tclMatrix.h"

#line 1 "d:\\tcl-source\\tcl8.3.1\\generic\\tcl.h"
























































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































#line 108 "./../tcl\\tclMatrix.h"



#line 112 "./../tcl\\tclMatrix.h"




#line 117 "./../tcl\\tclMatrix.h"



typedef PLFLT Mat_float;



#line 125 "./../tcl\\tclMatrix.h"
typedef int   Mat_int;
#line 127 "./../tcl\\tclMatrix.h"

enum { TYPE_FLOAT, TYPE_INT };















typedef struct {
    int type;			
    int len;			
    int dim;			
    int n[3];	
    int tracing;		

    char *name;			

    Mat_float *fdata;		
    Mat_int   *idata;		

    Tcl_Interp *interp;		



    void (*put) (ClientData clientData, int index, char *string);
    void (*get) (ClientData clientData, int index, char *string);

} tclMatrix;


















































































































































































































































typedef int (*tclMatrixXtnsnProc) ( tclMatrix *pm, Tcl_Interp *interp,
				    int argc, char *argv[] );

typedef struct tclMatrixXtnsnDescr {
    char *cmd;
    tclMatrixXtnsnProc cmdproc;
    struct tclMatrixXtnsnDescr *next;
} tclMatrixXtnsnDescr;



#line 1 "./../tcl\\matrixDecls.h"




























extern __declspec(dllimport) tclMatrix*	Tcl_GetMatrixPtr 
(Tcl_Interp * interp, char* matName);

extern __declspec(dllimport) int		Tcl_MatrixInstallXtnsn 
(char * cmd, tclMatrixXtnsnProc proc);

typedef struct MatrixStubHooks {
    struct MatrixIntStubs *matrixIntStubs;
} MatrixStubHooks;

typedef struct MatrixStubs {
    int magic;
    struct MatrixStubHooks *hooks;

    tclMatrix* (*tcl_GetMatrixPtr) (Tcl_Interp * interp, char* matName); 
    int (*tcl_MatrixInstallXtnsn) (char * cmd, tclMatrixXtnsnProc proc); 
} MatrixStubs;




extern MatrixStubs *matrixStubsPtr;













#line 65 "./../tcl\\matrixDecls.h"



#line 69 "./../tcl\\matrixDecls.h"

#line 71 "./../tcl\\matrixDecls.h"



#line 75 "./../tcl\\matrixDecls.h"

#line 418 "./../tcl\\tclMatrix.h"
#line 419 "./../tcl\\tclMatrix.h"










extern  char *Matrix_InitStubs (Tcl_Interp *interp, const char *version, int exact);















#line 446 "./../tcl\\tclMatrix.h"
#line 39 "./../tcl\\pltcl.h"








int
pltclMain(int argc, char **argv, char *RcFileName,
	  int (*AppInit)(Tcl_Interp *interp));




int
plTclCmd(char *cmdlist, Tcl_Interp *interp,
	 int argc, char **argv);








#line 66 "./../tcl\\pltcl.h"















extern __declspec(dllimport) int
Pltcl_Init( Tcl_Interp *interp );

extern __declspec(dllimport) int 
PlbasicInit( Tcl_Interp *interp );

#line 88 "./../tcl\\pltcl.h"




int
pls_auto_path(Tcl_Interp *interp);



int
plWait_Until(ClientData, Tcl_Interp *, int, char **);



int
plHost_ID(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);





#line 110 "./../tcl\\pltcl.h"
#line 26 "./../tk/pltk.h"
#line 1 "d:\\tcl-source\\tk8.3.1\\generic\\tk.h"


















































































#line 1 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xlib.h"


































#line 1 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/X.h"










































typedef unsigned long XID;

typedef XID Window;
typedef XID Drawable;
typedef XID Font;
typedef XID Pixmap;
typedef XID Cursor;
typedef XID Colormap;
typedef XID GContext;
typedef XID KeySym;

typedef unsigned long Mask;

typedef unsigned long Atom;

typedef unsigned long VisualID;

typedef unsigned long Time;

typedef unsigned long KeyCode;	





































































































































































		       




























































































































































































































































































































































































































































#line 674 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/X.h"
#line 36 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xlib.h"
#line 37 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xlib.h"





#line 1 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xfuncproto.h"























#line 25 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xfuncproto.h"



#line 29 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xfuncproto.h"





#line 35 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xfuncproto.h"



#line 39 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xfuncproto.h"






#line 46 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xfuncproto.h"
#line 47 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xfuncproto.h"

#line 49 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xfuncproto.h"









#line 59 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xfuncproto.h"

#line 61 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xfuncproto.h"
#line 43 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xlib.h"
#line 44 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xlib.h"





#line 50 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xlib.h"


#line 1 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stddef.h"















#pragma once
#line 18 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stddef.h"






#line 25 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stddef.h"


















































#line 76 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stddef.h"





















#line 98 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stddef.h"
 extern int errno;
#line 100 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stddef.h"





typedef int ptrdiff_t;

#line 108 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stddef.h"

























#line 134 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\stddef.h"
#line 53 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xlib.h"



#line 57 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xlib.h"

typedef char *XPointer;





typedef int Status;
#line 66 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xlib.h"
































































typedef struct _XExtData {
	int number;		
	struct _XExtData *next;	
	int (*free_private)();	
	XPointer private_data;	
} XExtData;




typedef struct {		
	int extension;		
	int major_opcode;	
	int first_event;	
	int first_error;	
} XExtCodes;





typedef struct {
    int depth;
    int bits_per_pixel;
    int scanline_pad;
} XPixmapFormatValues;





typedef struct {
	int function;		
	unsigned long plane_mask;
	unsigned long foreground;
	unsigned long background;
	int line_width;		
	int line_style;	 	
	int cap_style;	  	

	int join_style;	 	
	int fill_style;	 	

	int fill_rule;	  	
	int arc_mode;		
	Pixmap tile;		
	Pixmap stipple;		
	int ts_x_origin;	
	int ts_y_origin;
        Font font;	        
	int subwindow_mode;     
	int graphics_exposures;
	int clip_x_origin;	
	int clip_y_origin;
	Pixmap clip_mask;	
	int dash_offset;	
	char dashes;
} XGCValues;






typedef XGCValues *GC;




typedef struct {
	XExtData *ext_data;	
	VisualID visualid;	


#line 205 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xlib.h"
	int class;		
#line 207 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xlib.h"
	unsigned long red_mask, green_mask, blue_mask;	
	int bits_per_rgb;	
	int map_entries;	
} Visual;



	
typedef struct {
	int depth;		
	int nvisuals;		
	Visual *visuals;	
} Depth;






typedef struct {
	XExtData *ext_data;	
	struct _XDisplay *display;
	Window root;		
	int width, height;	
	int mwidth, mheight;	
	int ndepths;		
	Depth *depths;		
	int root_depth;		
	Visual *root_visual;	
	GC default_gc;		
	Colormap cmap;		
	unsigned long white_pixel;
	unsigned long black_pixel;	
	int max_maps, min_maps;	
	int backing_store;	
	int save_unders;	
	long root_input_mask;	
} Screen;




typedef struct {
	XExtData *ext_data;	
	int depth;		
	int bits_per_pixel;	
	int scanline_pad;	
} ScreenFormat;




typedef struct {
    Pixmap background_pixmap;	
    unsigned long background_pixel;	
    Pixmap border_pixmap;	
    unsigned long border_pixel;	
    int bit_gravity;		
    int win_gravity;		
    int backing_store;		
    unsigned long backing_planes;
    unsigned long backing_pixel;
    int save_under;		
    long event_mask;		
    long do_not_propagate_mask;	
    int override_redirect;	
    Colormap colormap;		
    Cursor cursor;		
} XSetWindowAttributes;

typedef struct {
    int x, y;			
    int width, height;		
    int border_width;		
    int depth;          	
    Visual *visual;		
    Window root;        	


#line 287 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xlib.h"
    int class;			
#line 289 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xlib.h"
    int bit_gravity;		
    int win_gravity;		
    int backing_store;		
    unsigned long backing_planes;
    unsigned long backing_pixel;
    int save_under;		
    Colormap colormap;		
    int map_installed;		
    int map_state;		
    long all_event_masks;	
    long your_event_mask;	
    long do_not_propagate_mask; 
    int override_redirect;	
    Screen *screen;		
} XWindowAttributes;






typedef struct {
	int family;		
	int length;		
	char *address;		
} XHostAddress;




typedef struct _XImage {
    int width, height;		
    int xoffset;		
    int format;			
    char *data;			
    int byte_order;		
    int bitmap_unit;		
    int bitmap_bit_order;	
    int bitmap_pad;		
    int depth;			
    int bytes_per_line;		
    int bits_per_pixel;		
    unsigned long red_mask;	
    unsigned long green_mask;
    unsigned long blue_mask;
    XPointer obdata;		
    struct funcs {		
	struct _XImage *(*create_image)();

	int (*destroy_image)        (struct _XImage *);
	unsigned long (*get_pixel)  (struct _XImage *, int, int);
	int (*put_pixel)            (struct _XImage *, int, int, unsigned long);
	struct _XImage *(*sub_image)(struct _XImage *, int, int, unsigned int, unsigned int);
	int (*add_pixel)            (struct _XImage *, long);






#line 350 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xlib.h"
	} f;
} XImage;




typedef struct {
    int x, y;
    int width, height;
    int border_width;
    Window sibling;
    int stack_mode;
} XWindowChanges;




typedef struct {
	unsigned long pixel;
	unsigned short red, green, blue;
	char flags;  
	char pad;
} XColor;






typedef struct {
    short x1, y1, x2, y2;
} XSegment;

typedef struct {
    short x, y;
} XPoint;
    
typedef struct {
    short x, y;
    unsigned short width, height;
} XRectangle;
    
typedef struct {
    short x, y;
    unsigned short width, height;
    short angle1, angle2;
} XArc;




typedef struct {
        int key_click_percent;
        int bell_percent;
        int bell_pitch;
        int bell_duration;
        int led;
        int led_mode;
        int key;
        int auto_repeat_mode;   
} XKeyboardControl;



typedef struct {
        int key_click_percent;
	int bell_percent;
	unsigned int bell_pitch, bell_duration;
	unsigned long led_mask;
	int global_auto_repeat;
	char auto_repeats[32];
} XKeyboardState;



typedef struct {
        Time time;
	short x, y;
} XTimeCoord;



typedef struct {
 	int max_keypermod;	
 	KeyCode *modifiermap;	
} XModifierKeymap;







typedef struct _XDisplay {
	XExtData *ext_data;	
	struct _XFreeFuncs *free_funcs; 
	int fd;			
	int conn_checker;         
	int proto_major_version;
	int proto_minor_version;
	char *vendor;		
        XID resource_base;	
	XID resource_mask;	
	XID resource_id;	
	int resource_shift;	
	XID (*resource_alloc)(); 
	int byte_order;		
	int bitmap_unit;	
	int bitmap_pad;		
	int bitmap_bit_order;	
	int nformats;		
	ScreenFormat *pixmap_format;	
	int vnumber;		
	int release;		
	struct _XSQEvent *head, *tail;	
	int qlen;		
	unsigned long request;	
	char *last_req;		
	char *buffer;		
	char *bufptr;		
	char *bufmax;		
	unsigned max_request_size; 
	struct _XrmHashBucketRec *db;
	int (*synchandler)();	
	char *display_name;	
	int default_screen;	
	int nscreens;		
	Screen *screens;	
	unsigned long motion_buffer;	
	unsigned long flags;	
	int min_keycode;	
	int max_keycode;	
	KeySym *keysyms;	
	XModifierKeymap *modifiermap;	
	int keysyms_per_keycode;
	char *xdefaults;	
	char *scratch_buffer;	
	unsigned long scratch_length;	
	int ext_number;		
	struct _XExten *ext_procs; 
	







	int (*event_vec[128])();  
	Status (*wire_vec[128])(); 
	KeySym lock_meaning;	   
	struct _XLockInfo *lock;   
	struct _XInternalAsync *async_handlers; 
	unsigned long bigreq_size; 
	struct _XLockPtrs *lock_fns; 
	
	struct _XKeytrans *key_bindings; 
	Font cursor_font;	   
	struct _XDisplayAtoms *atoms; 
	unsigned int mode_switch;  
	struct _XContextDB *context_db; 
	int (**error_vec)();      
	


	struct {
	   XPointer defaultCCCs;  
	   XPointer clientCmaps;  
	   XPointer perVisualIntensityMaps;
				  
	} cms;
	struct _XIMFilter *im_filters;
	struct _XSQEvent *qfree; 
	unsigned long next_event_serial_num; 
	int (*savedsynchandler)(); 
} Display;



#line 530 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xlib.h"







typedef struct {
	int type;		
	unsigned long serial;	
	int send_event;	
	Display *display;	
	Window window;	        
	Window root;	        
	Window subwindow;	
	Time time;		
	int x, y;		
	int x_root, y_root;	
	unsigned int state;	
	unsigned int keycode;	
	int same_screen;	
        char trans_chars[4];
				
	int nbytes;
} XKeyEvent;
typedef XKeyEvent XKeyPressedEvent;
typedef XKeyEvent XKeyReleasedEvent;

typedef struct {
	int type;		
	unsigned long serial;	
	int send_event;	
	Display *display;	
	Window window;	        
	Window root;	        
	Window subwindow;	
	Time time;		
	int x, y;		
	int x_root, y_root;	
	unsigned int state;	
	unsigned int button;	
	int same_screen;	
} XButtonEvent;
typedef XButtonEvent XButtonPressedEvent;
typedef XButtonEvent XButtonReleasedEvent;

typedef struct {
	int type;		
	unsigned long serial;	
	int send_event;	
	Display *display;	
	Window window;	        
	Window root;	        
	Window subwindow;	
	Time time;		
	int x, y;		
	int x_root, y_root;	
	unsigned int state;	
	char is_hint;		
	int same_screen;	
} XMotionEvent;
typedef XMotionEvent XPointerMovedEvent;

typedef struct {
	int type;		
	unsigned long serial;	
	int send_event;	
	Display *display;	
	Window window;	        
	Window root;	        
	Window subwindow;	
	Time time;		
	int x, y;		
	int x_root, y_root;	
	int mode;		
	int detail;
	



	int same_screen;	
	int focus;		
	unsigned int state;	
} XCrossingEvent;
typedef XCrossingEvent XEnterWindowEvent;
typedef XCrossingEvent XLeaveWindowEvent;

typedef struct {
	int type;		
	unsigned long serial;	
	int send_event;	
	Display *display;	
	Window window;		
	int mode;		
	int detail;
	




} XFocusChangeEvent;
typedef XFocusChangeEvent XFocusInEvent;
typedef XFocusChangeEvent XFocusOutEvent;


typedef struct {
	int type;
	unsigned long serial;	
	int send_event;	
	Display *display;	
	Window window;
	char key_vector[32];
} XKeymapEvent;	

typedef struct {
	int type;
	unsigned long serial;	
	int send_event;	
	Display *display;	
	Window window;
	int x, y;
	int width, height;
	int count;		
} XExposeEvent;

typedef struct {
	int type;
	unsigned long serial;	
	int send_event;	
	Display *display;	
	Drawable drawable;
	int x, y;
	int width, height;
	int count;		
	int major_code;		
	int minor_code;		
} XGraphicsExposeEvent;

typedef struct {
	int type;
	unsigned long serial;	
	int send_event;	
	Display *display;	
	Drawable drawable;
	int major_code;		
	int minor_code;		
} XNoExposeEvent;

typedef struct {
	int type;
	unsigned long serial;	
	int send_event;	
	Display *display;	
	Window window;
	int state;		
} XVisibilityEvent;

typedef struct {
	int type;
	unsigned long serial;	
	int send_event;	
	Display *display;	
	Window parent;		
	Window window;		
	int x, y;		
	int width, height;	
	int border_width;	
	int override_redirect;	
} XCreateWindowEvent;

typedef struct {
	int type;
	unsigned long serial;	
	int send_event;	
	Display *display;	
	Window event;
	Window window;
} XDestroyWindowEvent;

typedef struct {
	int type;
	unsigned long serial;	
	int send_event;	
	Display *display;	
	Window event;
	Window window;
	int from_configure;
} XUnmapEvent;

typedef struct {
	int type;
	unsigned long serial;	
	int send_event;	
	Display *display;	
	Window event;
	Window window;
	int override_redirect;	
} XMapEvent;

typedef struct {
	int type;
	unsigned long serial;	
	int send_event;	
	Display *display;	
	Window parent;
	Window window;
} XMapRequestEvent;

typedef struct {
	int type;
	unsigned long serial;	
	int send_event;	
	Display *display;	
	Window event;
	Window window;
	Window parent;
	int x, y;
	int override_redirect;
} XReparentEvent;

typedef struct {
	int type;
	unsigned long serial;	
	int send_event;	
	Display *display;	
	Window event;
	Window window;
	int x, y;
	int width, height;
	int border_width;
	Window above;
	int override_redirect;
} XConfigureEvent;

typedef struct {
	int type;
	unsigned long serial;	
	int send_event;	
	Display *display;	
	Window event;
	Window window;
	int x, y;
} XGravityEvent;

typedef struct {
	int type;
	unsigned long serial;	
	int send_event;	
	Display *display;	
	Window window;
	int width, height;
} XResizeRequestEvent;

typedef struct {
	int type;
	unsigned long serial;	
	int send_event;	
	Display *display;	
	Window parent;
	Window window;
	int x, y;
	int width, height;
	int border_width;
	Window above;
	int detail;		
	unsigned long value_mask;
} XConfigureRequestEvent;

typedef struct {
	int type;
	unsigned long serial;	
	int send_event;	
	Display *display;	
	Window event;
	Window window;
	int place;		
} XCirculateEvent;

typedef struct {
	int type;
	unsigned long serial;	
	int send_event;	
	Display *display;	
	Window parent;
	Window window;
	int place;		
} XCirculateRequestEvent;

typedef struct {
	int type;
	unsigned long serial;	
	int send_event;	
	Display *display;	
	Window window;
	Atom atom;
	Time time;
	int state;		
} XPropertyEvent;

typedef struct {
	int type;
	unsigned long serial;	
	int send_event;	
	Display *display;	
	Window window;
	Atom selection;
	Time time;
} XSelectionClearEvent;

typedef struct {
	int type;
	unsigned long serial;	
	int send_event;	
	Display *display;	
	Window owner;
	Window requestor;
	Atom selection;
	Atom target;
	Atom property;
	Time time;
} XSelectionRequestEvent;

typedef struct {
	int type;
	unsigned long serial;	
	int send_event;	
	Display *display;	
	Window requestor;
	Atom selection;
	Atom target;
	Atom property;		
	Time time;
} XSelectionEvent;

typedef struct {
	int type;
	unsigned long serial;	
	int send_event;	
	Display *display;	
	Window window;
	Colormap colormap;	


#line 874 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xlib.h"
	int new;
#line 876 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xlib.h"
	int state;		
} XColormapEvent;

typedef struct {
	int type;
	unsigned long serial;	
	int send_event;	
	Display *display;	
	Window window;
	Atom message_type;
	int format;
	union {
		char b[20];
		short s[10];
		long l[5];
		} data;
} XClientMessageEvent;

typedef struct {
	int type;
	unsigned long serial;	
	int send_event;	
	Display *display;	
	Window window;		
	int request;		

	int first_keycode;	
	int count;		
} XMappingEvent;

typedef struct {
	int type;
	Display *display;	
	XID resourceid;		
	unsigned long serial;	
	unsigned char error_code;	
	unsigned char request_code;	
	unsigned char minor_code;	
} XErrorEvent;

typedef struct {
	int type;
	unsigned long serial;	
	int send_event;	
	Display *display;
	Window window;	
} XAnyEvent;





typedef union _XEvent {
        int type;		
	XAnyEvent xany;
	XKeyEvent xkey;
	XButtonEvent xbutton;
	XMotionEvent xmotion;
	XCrossingEvent xcrossing;
	XFocusChangeEvent xfocus;
	XExposeEvent xexpose;
	XGraphicsExposeEvent xgraphicsexpose;
	XNoExposeEvent xnoexpose;
	XVisibilityEvent xvisibility;
	XCreateWindowEvent xcreatewindow;
	XDestroyWindowEvent xdestroywindow;
	XUnmapEvent xunmap;
	XMapEvent xmap;
	XMapRequestEvent xmaprequest;
	XReparentEvent xreparent;
	XConfigureEvent xconfigure;
	XGravityEvent xgravity;
	XResizeRequestEvent xresizerequest;
	XConfigureRequestEvent xconfigurerequest;
	XCirculateEvent xcirculate;
	XCirculateRequestEvent xcirculaterequest;
	XPropertyEvent xproperty;
	XSelectionClearEvent xselectionclear;
	XSelectionRequestEvent xselectionrequest;
	XSelectionEvent xselection;
	XColormapEvent xcolormap;
	XClientMessageEvent xclient;
	XMappingEvent xmapping;
	XErrorEvent xerror;
	XKeymapEvent xkeymap;
	long pad[24];
} XEvent;
#line 964 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xlib.h"






typedef struct {
    short	lbearing;	
    short	rbearing;	
    short	width;		
    short	ascent;		
    short	descent;	
    unsigned short attributes;	
} XCharStruct;





typedef struct {
    Atom name;
    unsigned long card32;
} XFontProp;

typedef struct {
    XExtData	*ext_data;	
    Font        fid;            
    unsigned	direction;	
    unsigned	min_char_or_byte2;
    unsigned	max_char_or_byte2;
    unsigned	min_byte1;	
    unsigned	max_byte1;	
    int	all_chars_exist;
    unsigned	default_char;	
    int         n_properties;   
    XFontProp	*properties;	
    XCharStruct	min_bounds;	
    XCharStruct	max_bounds;	
    XCharStruct	*per_char;	
    int		ascent;		
    int		descent;	
} XFontStruct;




typedef struct {
    char *chars;		
    int nchars;			
    int delta;			
    Font font;			
} XTextItem;

typedef struct {		
    unsigned char byte1;
    unsigned char byte2;
} XChar2b;

typedef struct {
    XChar2b *chars;		
    int nchars;			
    int delta;			
    Font font;			
} XTextItem16;


typedef union { Display *display;
		GC gc;
		Visual *visual;
		Screen *screen;
		ScreenFormat *pixmap_format;
		XFontStruct *font; } XEDataObject;

typedef struct {
    XRectangle      max_ink_extent;
    XRectangle      max_logical_extent;
} XFontSetExtents;

typedef struct _XFontSet *XFontSet;

typedef struct {
    char           *chars;
    int             nchars;
    int             delta;
    XFontSet        font_set;
} XmbTextItem;

typedef struct {
    wchar_t        *chars;
    int             nchars;
    int             delta;
    XFontSet        font_set;
} XwcTextItem;

typedef void (*XIMProc)();

typedef struct _XIM *XIM;
typedef struct _XIC *XIC;

typedef unsigned long XIMStyle;

typedef struct {
    unsigned short count_styles;
    XIMStyle *supported_styles;
} XIMStyles;















































typedef void *XVaNestedList;


#line 1120 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xlib.h"

typedef struct {
    XPointer client_data;
    XIMProc callback;
} XIMCallback;

typedef unsigned long XIMFeedback;








typedef struct _XIMText {
    unsigned short length;
    XIMFeedback *feedback;
    int encoding_is_wchar; 
    union {
	char *multi_byte;
	wchar_t *wide_char;
    } string; 
} XIMText;

typedef struct _XIMPreeditDrawCallbackStruct {
    int caret;		
    int chg_first;	
    int chg_length;	
    XIMText *text;
} XIMPreeditDrawCallbackStruct;

typedef enum {
    XIMForwardChar, XIMBackwardChar,
    XIMForwardWord, XIMBackwardWord,
    XIMCaretUp, XIMCaretDown,
    XIMNextLine, XIMPreviousLine,
    XIMLineStart, XIMLineEnd, 
    XIMAbsolutePosition,
    XIMDontChange
} XIMCaretDirection;

typedef enum {
    XIMIsInvisible,	 
    XIMIsPrimary,	
    XIMIsSecondary	
} XIMCaretStyle;

typedef struct _XIMPreeditCaretCallbackStruct {
    int position;		 
    XIMCaretDirection direction; 
    XIMCaretStyle style;	 
} XIMPreeditCaretCallbackStruct;

typedef enum {
    XIMTextType,
    XIMBitmapType
} XIMStatusDataType;
	
typedef struct _XIMStatusDrawCallbackStruct {
    XIMStatusDataType type;
    union {
	XIMText *text;
	Pixmap  bitmap;
    } data;
} XIMStatusDrawCallbackStruct;

typedef int (*XErrorHandler) (	    

    Display*		,
    XErrorEvent*	
#line 1192 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xlib.h"
);





#line 1 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntXlibDecls.h"




















#line 1 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"




















































typedef struct {
    	long flags;	
	int x, y;		
	int width, height;	
	int min_width, min_height;
	int max_width, max_height;
    	int width_inc, height_inc;
	struct {
		int x;	
		int y;	
	} min_aspect, max_aspect;
	int base_width, base_height;		
	int win_gravity;			
} XSizeHints;
























typedef struct {
	long flags;	
	int input;	

	int initial_state;	
	Pixmap icon_pixmap;	
	Window icon_window; 	
	int icon_x, icon_y; 	
	Pixmap icon_mask;	
	XID window_group;	
	
} XWMHints;
























			






typedef struct {
    unsigned char *value;		
    Atom encoding;			
    int format;				
    unsigned long nitems;		
} XTextProperty;





typedef enum {
    XStringStyle,		
    XCompoundTextStyle,		
    XTextStyle,			
    XStdICCTextStyle		
} XICCEncodingStyle;

typedef struct {
	int min_width, min_height;
	int max_width, max_height;
	int width_inc, height_inc;
} XIconSize;

typedef struct {
	char *res_name;
	char *res_class;
} XClassHint;



















typedef struct _XComposeStatus {
    XPointer compose_ptr;	
    int chars_matched;		
} XComposeStatus;


























typedef struct _XRegion *Region; 


 



 






typedef struct {
  Visual *visual;
  VisualID visualid;
  int screen;
  int depth;


#line 233 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
  int class;
#line 235 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
  unsigned long red_mask;
  unsigned long green_mask;
  unsigned long blue_mask;
  int colormap_size;
  int bits_per_rgb;
} XVisualInfo;

















typedef struct {
	Colormap colormap;
	unsigned long red_max;
	unsigned long red_mult;
	unsigned long green_max;
	unsigned long green_mult;
	unsigned long blue_max;
	unsigned long blue_mult;
	unsigned long base_pixel;
	VisualID visualid;		
	XID killid;			
} XStandardColormap;

























typedef int XContext;








extern XClassHint *XAllocClassHint (

    void
#line 308 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern XIconSize *XAllocIconSize (

    void
#line 314 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern XSizeHints *XAllocSizeHints (

    void
#line 320 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern XStandardColormap *XAllocStandardColormap (

    void
#line 326 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern XWMHints *XAllocWMHints (

    void
#line 332 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern void XClipBox(

    Region		,
    XRectangle*		
#line 339 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern Region XCreateRegion(

    void
#line 345 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern char *XDefaultString(

    void
#line 351 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern int XDeleteContext(

    Display*		,
    XID			,
    XContext		
#line 359 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern void XDestroyRegion(

    Region		
#line 365 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern void XEmptyRegion(

    Region		
#line 371 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern void XEqualRegion(

    Region		,
    Region		
#line 378 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern int XFindContext(

    Display*		,
    XID			,
    XContext		,
    XPointer*		
#line 387 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern Status XGetClassHint(

    Display*		,
    Window		,
    XClassHint*		
#line 395 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern Status XGetIconSizes(

    Display*		,
    Window		,
    XIconSize**		,
    int*		
#line 404 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern Status XGetNormalHints(

    Display*		,
    Window		,
    XSizeHints*		
#line 412 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern Status XGetRGBColormaps(

    Display*		,
    Window		,
    XStandardColormap** ,
    int*		,
    Atom		
#line 422 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern Status XGetSizeHints(

    Display*		,
    Window		,
    XSizeHints*		,
    Atom		
#line 431 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern Status XGetStandardColormap(

    Display*		,
    Window		,
    XStandardColormap*	,
    Atom					    
#line 440 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern Status XGetTextProperty(

    Display*		,
    Window		,
    XTextProperty*	,
    Atom		
#line 449 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);


extern Status XGetWMClientMachine(

    Display*		,
    Window		,
    XTextProperty*	
#line 458 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern XWMHints *XGetWMHints(

    Display*		,
    Window				      
#line 465 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern Status XGetWMIconName(

    Display*		,
    Window		,
    XTextProperty*	
#line 473 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern Status XGetWMName(

    Display*		,
    Window		,
    XTextProperty*	
#line 481 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern Status XGetWMNormalHints(

    Display*		,
    Window		,
    XSizeHints*		,
    long*		 
#line 490 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern Status XGetWMSizeHints(

    Display*		,
    Window		,
    XSizeHints*		,
    long*		,
    Atom		
#line 500 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern Status XGetZoomHints(

    Display*		,
    Window		,
    XSizeHints*		
#line 508 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern void XIntersectRegion(

    Region		,
    Region		,
    Region		
#line 516 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern int XLookupString(

    XKeyEvent*		,
    char*		,
    int			,
    KeySym*		,
    XComposeStatus*	
#line 526 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern Status XMatchVisualInfo(

    Display*		,
    int			,
    int			,
    int			,
    XVisualInfo*	
#line 536 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern void XOffsetRegion(

    Region		,
    int			,
    int			
#line 544 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern int XPointInRegion(

    Region		,
    int			,
    int			
#line 552 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern Region XPolygonRegion(

    XPoint*		,
    int			,
    int			
#line 560 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern int XRectInRegion(

    Region		,
    int			,
    int			,
    unsigned int	,
    unsigned int	
#line 570 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern int XSaveContext(

    Display*		,
    XID			,
    XContext		,
    const char*	
#line 579 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern void XSetClassHint(

    Display*		,
    Window		,
    XClassHint*		
#line 587 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern void XSetIconSizes(

    Display*		,
    Window		,
    XIconSize*		,
    int			    
#line 596 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern void XSetNormalHints(

    Display*		,
    Window		,
    XSizeHints*		
#line 604 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern void XSetRGBColormaps(

    Display*		,
    Window		,
    XStandardColormap*	,
    int			,
    Atom		
#line 614 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern void XSetSizeHints(

    Display*		,
    Window		,
    XSizeHints*		,
    Atom		
#line 623 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern void XSetStandardProperties(

    Display*		,
    Window		,
    const char*	,
    const char*	,
    Pixmap		,
    char**		,
    int			,
    XSizeHints*		
#line 636 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern void XSetTextProperty(

    Display*		,
    Window		,
    XTextProperty*	,
    Atom		
#line 645 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern void XSetWMHints(

    Display*		,
    Window		,
    XWMHints*		
#line 653 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern void XSetWMIconName(

    Display*		,
    Window		,
    XTextProperty*	
#line 661 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern void XSetWMName(

    Display*		,
    Window		,
    XTextProperty*	
#line 669 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern void XSetWMNormalHints(

    Display*		,
    Window		,
    XSizeHints*		
#line 677 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern void XSetWMProperties(

    Display*		,
    Window		,
    XTextProperty*	,
    XTextProperty*	,
    char**		,
    int			,
    XSizeHints*		,
    XWMHints*		,
    XClassHint*		
#line 691 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern void XmbSetWMProperties(

    Display*		,
    Window		,
    const char*	,
    const char*	,
    char**		,
    int			,
    XSizeHints*		,
    XWMHints*		,
    XClassHint*		
#line 705 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern void XSetWMSizeHints(

    Display*		,
    Window		,
    XSizeHints*		,
    Atom		
#line 714 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern void XSetRegion(

    Display*		,
    GC			,
    Region		
#line 722 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern void XSetStandardColormap(

    Display*		,
    Window		,
    XStandardColormap*	,
    Atom		
#line 731 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern void XSetZoomHints(

    Display*		,
    Window		,
    XSizeHints*		
#line 739 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern void XShrinkRegion(

    Region		,
    int			,
    int			
#line 747 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern void XSubtractRegion(

    Region		,
    Region		,
    Region		
#line 755 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern int XmbTextListToTextProperty(

    Display*		,
    char**		,
    int			,
    XICCEncodingStyle	,
    XTextProperty*	
#line 765 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern int XwcTextListToTextProperty(

    Display*		,
    wchar_t**		,
    int			,
    XICCEncodingStyle	,
    XTextProperty*	
#line 775 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern void XwcFreeStringList(

    wchar_t**		
#line 781 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern Status XTextPropertyToStringList(

    XTextProperty*	,
    char***		,
    int*		
#line 789 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern int XmbTextPropertyToTextList(

    Display*		,
    XTextProperty*	,
    char***		,
    int*		
#line 798 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern int XwcTextPropertyToTextList(

    Display*		,
    XTextProperty*	,
    wchar_t***		,
    int*		
#line 807 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern void XUnionRectWithRegion(

    XRectangle*		,
    Region		,
    Region		
#line 815 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern void XUnionRegion(

    Region		,
    Region		,
    Region		
#line 823 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern int XWMGeometry(

    Display*		,
    int			,
    const char*	,
    const char*	,
    unsigned int	,
    XSizeHints*		,
    int*		,
    int*		,
    int*		,
    int*		,
    int*		
#line 839 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);

extern void XXorRegion(

    Region		,
    Region		,
    Region		
#line 847 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
);







#line 856 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"
#line 22 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntXlibDecls.h"
#line 23 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntXlibDecls.h"




















extern __declspec(dllimport) void		XSetDashes 

(Display* display, GC gc, int dash_offset, const char* dash_list, int n);

extern __declspec(dllimport) XModifierKeymap*	 XGetModifierMapping (Display* d);

extern __declspec(dllimport) XImage *		XCreateImage 


(Display* d, Visual* v, unsigned int ui1, int i1, int i2, char* cp, unsigned int ui2, unsigned int ui3, int i3, int i4);

extern __declspec(dllimport) XImage *		XGetImage 

(Display* d, Drawable dr, int i1, int i2, unsigned int ui1, unsigned int ui2, unsigned long ul, int i3);

extern __declspec(dllimport) char *		XGetAtomName (Display* d, Atom a);

extern __declspec(dllimport) char *		XKeysymToString (KeySym k);

extern __declspec(dllimport) Colormap		XCreateColormap 
(Display* d, Window w, Visual* v, int i);

extern __declspec(dllimport) Cursor		XCreatePixmapCursor 

(Display* d, Pixmap p1, Pixmap p2, XColor* x1, XColor* x2, unsigned int ui1, unsigned int ui2);

extern __declspec(dllimport) Cursor		XCreateGlyphCursor 

(Display* d, Font f1, Font f2, unsigned int ui1, unsigned int ui2, XColor* x1, XColor* x2);

extern __declspec(dllimport) GContext		XGContextFromGC (GC g);

extern __declspec(dllimport) XHostAddress *	XListHosts (Display* d, int* i, int* b);

extern __declspec(dllimport) KeySym		XKeycodeToKeysym 
(Display* d, unsigned int k, int i);

extern __declspec(dllimport) KeySym		XStringToKeysym (const char* c);

extern __declspec(dllimport) Window		XRootWindow (Display* d, int i);

extern __declspec(dllimport) XErrorHandler	XSetErrorHandler (XErrorHandler x);

extern __declspec(dllimport) Status		XIconifyWindow 
(Display* d, Window w, int i);

extern __declspec(dllimport) Status		XWithdrawWindow 
(Display* d, Window w, int i);

extern __declspec(dllimport) Status		XGetWMColormapWindows 
(Display* d, Window w, Window** wpp, int* ip);

extern __declspec(dllimport) Status		XAllocColor 
(Display* d, Colormap c, XColor* xp);

extern __declspec(dllimport) void		XBell (Display* d, int i);

extern __declspec(dllimport) void		XChangeProperty 

(Display* d, Window w, Atom a1, Atom a2, int i1, int i2, const unsigned char* c, int i3);

extern __declspec(dllimport) void		XChangeWindowAttributes 

(Display* d, Window w, unsigned long ul, XSetWindowAttributes* x);

extern __declspec(dllimport) void		XClearWindow (Display* d, Window w);

extern __declspec(dllimport) void		XConfigureWindow 
(Display* d, Window w, unsigned int i, XWindowChanges* x);

extern __declspec(dllimport) void		XCopyArea 


(Display* d, Drawable dr1, Drawable dr2, GC g, int i1, int i2, unsigned int ui1, unsigned int ui2, int i3, int i4);

extern __declspec(dllimport) void		XCopyPlane 


(Display* d, Drawable dr1, Drawable dr2, GC g, int i1, int i2, unsigned int ui1, unsigned int ui2, int i3, int i4, unsigned long ul);

extern __declspec(dllimport) Pixmap		XCreateBitmapFromData 

(Display* display, Drawable d, const char* data, unsigned int width, unsigned int height);

extern __declspec(dllimport) void		XDefineCursor 
(Display* d, Window w, Cursor c);

extern __declspec(dllimport) void		XDeleteProperty 
(Display* d, Window w, Atom a);

extern __declspec(dllimport) void		XDestroyWindow (Display* d, Window w);

extern __declspec(dllimport) void		XDrawArc 

(Display* d, Drawable dr, GC g, int i1, int i2, unsigned int ui1, unsigned int ui2, int i3, int i4);

extern __declspec(dllimport) void		XDrawLines 
(Display* d, Drawable dr, GC g, XPoint* x, int i1, int i2);

extern __declspec(dllimport) void		XDrawRectangle 

(Display* d, Drawable dr, GC g, int i1, int i2, unsigned int ui1, unsigned int ui2);

extern __declspec(dllimport) void		XFillArc 

(Display* d, Drawable dr, GC g, int i1, int i2, unsigned int ui1, unsigned int ui2, int i3, int i4);

extern __declspec(dllimport) void		XFillPolygon 
(Display* d, Drawable dr, GC g, XPoint* x, int i1, int i2, int i3);

extern __declspec(dllimport) void		XFillRectangles 
(Display* d, Drawable dr, GC g, XRectangle* x, int i);

extern __declspec(dllimport) void		XForceScreenSaver (Display* d, int i);

extern __declspec(dllimport) void		XFreeColormap (Display* d, Colormap c);

extern __declspec(dllimport) void		XFreeColors 
(Display* d, Colormap c, unsigned long* ulp, int i, unsigned long ul);

extern __declspec(dllimport) void		XFreeCursor (Display* d, Cursor c);

extern __declspec(dllimport) void		XFreeModifiermap (XModifierKeymap* x);

extern __declspec(dllimport) Status		XGetGeometry 


(Display* d, Drawable dr, Window* w, int* i1, int* i2, unsigned int* ui1, unsigned int* ui2, unsigned int* ui3, unsigned int* ui4);

extern __declspec(dllimport) void		XGetInputFocus 
(Display* d, Window* w, int* i);

extern __declspec(dllimport) int		XGetWindowProperty 


(Display* d, Window w, Atom a1, long l1, long l2, int b, Atom a2, Atom* ap, int* ip, unsigned long* ulp1, unsigned long* ulp2, unsigned char** cpp);

extern __declspec(dllimport) Status		XGetWindowAttributes 
(Display* d, Window w, XWindowAttributes* x);

extern __declspec(dllimport) int		XGrabKeyboard 
(Display* d, Window w, int b, int i1, int i2, Time t);

extern __declspec(dllimport) int		XGrabPointer 

(Display* d, Window w1, int b, unsigned int ui, int i1, int i2, Window w2, Cursor c, Time t);

extern __declspec(dllimport) KeyCode		XKeysymToKeycode (Display* d, KeySym k);

extern __declspec(dllimport) Status		XLookupColor 
(Display* d, Colormap c1, const char* c2, XColor* x1, XColor* x2);

extern __declspec(dllimport) void		XMapWindow (Display* d, Window w);

extern __declspec(dllimport) void		XMoveResizeWindow 

(Display* d, Window w, int i1, int i2, unsigned int ui1, unsigned int ui2);

extern __declspec(dllimport) void		XMoveWindow 
(Display* d, Window w, int i1, int i2);

extern __declspec(dllimport) void		XNextEvent (Display* d, XEvent* x);

extern __declspec(dllimport) void		XPutBackEvent (Display* d, XEvent* x);

extern __declspec(dllimport) void		XQueryColors 
(Display* d, Colormap c, XColor* x, int i);

extern __declspec(dllimport) int		XQueryPointer 

(Display* d, Window w1, Window* w2, Window* w3, int* i1, int* i2, int* i3, int* i4, unsigned int* ui);

extern __declspec(dllimport) Status		XQueryTree 

(Display* d, Window w1, Window* w2, Window* w3, Window** w4, unsigned int* ui);

extern __declspec(dllimport) void		XRaiseWindow (Display* d, Window w);

extern __declspec(dllimport) void		XRefreshKeyboardMapping 
( XMappingEvent* x);

extern __declspec(dllimport) void		XResizeWindow 
(Display* d, Window w, unsigned int ui1, unsigned int ui2);

extern __declspec(dllimport) void		XSelectInput 
(Display* d, Window w, long l);

extern __declspec(dllimport) Status		XSendEvent 
(Display* d, Window w, int b, long l, XEvent* x);

extern __declspec(dllimport) void		XSetCommand 
(Display* d, Window w, char** c, int i);

extern __declspec(dllimport) void		XSetIconName 
(Display* d, Window w, const char* c);

extern __declspec(dllimport) void		XSetInputFocus 
(Display* d, Window w, int i, Time t);

extern __declspec(dllimport) void		XSetSelectionOwner 
(Display* d, Atom a, Window w, Time t);

extern __declspec(dllimport) void		XSetWindowBackground 
(Display* d, Window w, unsigned long ul);

extern __declspec(dllimport) void		XSetWindowBackgroundPixmap 
(Display* d, Window w, Pixmap p);

extern __declspec(dllimport) void		XSetWindowBorder 
(Display* d, Window w, unsigned long ul);

extern __declspec(dllimport) void		XSetWindowBorderPixmap 
(Display* d, Window w, Pixmap p);

extern __declspec(dllimport) void		XSetWindowBorderWidth 
(Display* d, Window w, unsigned int ui);

extern __declspec(dllimport) void		XSetWindowColormap 
(Display* d, Window w, Colormap c);

extern __declspec(dllimport) int		XTranslateCoordinates 

(Display* d, Window w1, Window w2, int i1, int i2, int* i3, int* i4, Window* w3);

extern __declspec(dllimport) void		XUngrabKeyboard (Display* d, Time t);

extern __declspec(dllimport) void		XUngrabPointer (Display* d, Time t);

extern __declspec(dllimport) void		XUnmapWindow (Display* d, Window w);

extern __declspec(dllimport) void		XWindowEvent 
(Display* d, Window w, long l, XEvent* x);

extern __declspec(dllimport) void		XDestroyIC (XIC x);

extern __declspec(dllimport) int		XFilterEvent (XEvent* x, Window w);

extern __declspec(dllimport) int		XmbLookupString 

(XIC xi, XKeyPressedEvent* xk, char* c, int i, KeySym* k, Status* s);

extern __declspec(dllimport) void		TkPutImage 



(unsigned long * colors, int ncolors, Display* display, Drawable d, GC gc, XImage* image, int src_x, int src_y, int dest_x, int dest_y, unsigned int width, unsigned int height);


extern __declspec(dllimport) Status		XParseColor 

(Display * display, Colormap map, const char* spec, XColor * colorPtr);

extern __declspec(dllimport) GC		XCreateGC 
(Display* display, Drawable d, unsigned long valuemask, XGCValues* values);

extern __declspec(dllimport) void		XFreeGC (Display* display, GC gc);

extern __declspec(dllimport) Atom		XInternAtom 
(Display* display, const char* atom_name, int only_if_exists);

extern __declspec(dllimport) void		XSetBackground 
(Display* display, GC gc, unsigned long foreground);

extern __declspec(dllimport) void		XSetForeground 
(Display* display, GC gc, unsigned long foreground);

extern __declspec(dllimport) void		XSetClipMask 
(Display* display, GC gc, Pixmap pixmap);

extern __declspec(dllimport) void		XSetClipOrigin 
(Display* display, GC gc, int clip_x_origin, int clip_y_origin);

extern __declspec(dllimport) void		XSetTSOrigin 
(Display* display, GC gc, int ts_x_origin, int ts_y_origin);

extern __declspec(dllimport) void		XChangeGC 
(Display * d, GC gc, unsigned long mask, XGCValues * values);

extern __declspec(dllimport) void		XSetFont 
(Display * display, GC gc, Font font);

extern __declspec(dllimport) void		XSetArcMode 
(Display * display, GC gc, int arc_mode);

extern __declspec(dllimport) void		XSetStipple 
(Display * display, GC gc, Pixmap stipple);

extern __declspec(dllimport) void		XSetFillRule 
(Display * display, GC gc, int fill_rule);

extern __declspec(dllimport) void		XSetFillStyle 
(Display * display, GC gc, int fill_style);

extern __declspec(dllimport) void		XSetFunction 
(Display * display, GC gc, int function);

extern __declspec(dllimport) void		XSetLineAttributes 


(Display * display, GC gc, unsigned int line_width, int line_style, int cap_style, int join_style);

extern __declspec(dllimport) int		_XInitImageFuncPtrs (XImage * image);

extern __declspec(dllimport) XIC		XCreateIC (void);

extern __declspec(dllimport) XVisualInfo *	XGetVisualInfo 

(Display* display, long vinfo_mask, XVisualInfo* vinfo_template, int* nitems_return);

extern __declspec(dllimport) void		XSetWMClientMachine 
(Display* display, Window w, XTextProperty* text_prop);

extern __declspec(dllimport) Status		XStringListToTextProperty 
(char** list, int count, XTextProperty* text_prop_return);

extern __declspec(dllimport) void		XDrawLine 
(Display* d, Drawable dr, GC g, int x1, int y1, int x2, int y2);

extern __declspec(dllimport) void		XWarpPointer 

(Display* d, Window s, Window dw, int sx, int sy, unsigned int sw, unsigned int sh, int dx, int dy);

extern __declspec(dllimport) void		XFillRectangle 

(Display* display, Drawable d, GC gc, int x, int y, unsigned int width, unsigned int height);
#line 369 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntXlibDecls.h"































































































































































































































































































typedef struct TkIntXlibStubs {
    int magic;
    struct TkIntXlibStubHooks *hooks;


    void (*xSetDashes) (Display* display, GC gc, int dash_offset, const char* dash_list, int n); 
    XModifierKeymap* (*xGetModifierMapping) (Display* d); 
    XImage * (*xCreateImage) (Display* d, Visual* v, unsigned int ui1, int i1, int i2, char* cp, unsigned int ui2, unsigned int ui3, int i3, int i4); 
    XImage * (*xGetImage) (Display* d, Drawable dr, int i1, int i2, unsigned int ui1, unsigned int ui2, unsigned long ul, int i3); 
    char * (*xGetAtomName) (Display* d, Atom a); 
    char * (*xKeysymToString) (KeySym k); 
    Colormap (*xCreateColormap) (Display* d, Window w, Visual* v, int i); 
    Cursor (*xCreatePixmapCursor) (Display* d, Pixmap p1, Pixmap p2, XColor* x1, XColor* x2, unsigned int ui1, unsigned int ui2); 
    Cursor (*xCreateGlyphCursor) (Display* d, Font f1, Font f2, unsigned int ui1, unsigned int ui2, XColor* x1, XColor* x2); 
    GContext (*xGContextFromGC) (GC g); 
    XHostAddress * (*xListHosts) (Display* d, int* i, int* b); 
    KeySym (*xKeycodeToKeysym) (Display* d, unsigned int k, int i); 
    KeySym (*xStringToKeysym) (const char* c); 
    Window (*xRootWindow) (Display* d, int i); 
    XErrorHandler (*xSetErrorHandler) (XErrorHandler x); 
    Status (*xIconifyWindow) (Display* d, Window w, int i); 
    Status (*xWithdrawWindow) (Display* d, Window w, int i); 
    Status (*xGetWMColormapWindows) (Display* d, Window w, Window** wpp, int* ip); 
    Status (*xAllocColor) (Display* d, Colormap c, XColor* xp); 
    void (*xBell) (Display* d, int i); 
    void (*xChangeProperty) (Display* d, Window w, Atom a1, Atom a2, int i1, int i2, const unsigned char* c, int i3); 
    void (*xChangeWindowAttributes) (Display* d, Window w, unsigned long ul, XSetWindowAttributes* x); 
    void (*xClearWindow) (Display* d, Window w); 
    void (*xConfigureWindow) (Display* d, Window w, unsigned int i, XWindowChanges* x); 
    void (*xCopyArea) (Display* d, Drawable dr1, Drawable dr2, GC g, int i1, int i2, unsigned int ui1, unsigned int ui2, int i3, int i4); 
    void (*xCopyPlane) (Display* d, Drawable dr1, Drawable dr2, GC g, int i1, int i2, unsigned int ui1, unsigned int ui2, int i3, int i4, unsigned long ul); 
    Pixmap (*xCreateBitmapFromData) (Display* display, Drawable d, const char* data, unsigned int width, unsigned int height); 
    void (*xDefineCursor) (Display* d, Window w, Cursor c); 
    void (*xDeleteProperty) (Display* d, Window w, Atom a); 
    void (*xDestroyWindow) (Display* d, Window w); 
    void (*xDrawArc) (Display* d, Drawable dr, GC g, int i1, int i2, unsigned int ui1, unsigned int ui2, int i3, int i4); 
    void (*xDrawLines) (Display* d, Drawable dr, GC g, XPoint* x, int i1, int i2); 
    void (*xDrawRectangle) (Display* d, Drawable dr, GC g, int i1, int i2, unsigned int ui1, unsigned int ui2); 
    void (*xFillArc) (Display* d, Drawable dr, GC g, int i1, int i2, unsigned int ui1, unsigned int ui2, int i3, int i4); 
    void (*xFillPolygon) (Display* d, Drawable dr, GC g, XPoint* x, int i1, int i2, int i3); 
    void (*xFillRectangles) (Display* d, Drawable dr, GC g, XRectangle* x, int i); 
    void (*xForceScreenSaver) (Display* d, int i); 
    void (*xFreeColormap) (Display* d, Colormap c); 
    void (*xFreeColors) (Display* d, Colormap c, unsigned long* ulp, int i, unsigned long ul); 
    void (*xFreeCursor) (Display* d, Cursor c); 
    void (*xFreeModifiermap) (XModifierKeymap* x); 
    Status (*xGetGeometry) (Display* d, Drawable dr, Window* w, int* i1, int* i2, unsigned int* ui1, unsigned int* ui2, unsigned int* ui3, unsigned int* ui4); 
    void (*xGetInputFocus) (Display* d, Window* w, int* i); 
    int (*xGetWindowProperty) (Display* d, Window w, Atom a1, long l1, long l2, int b, Atom a2, Atom* ap, int* ip, unsigned long* ulp1, unsigned long* ulp2, unsigned char** cpp); 
    Status (*xGetWindowAttributes) (Display* d, Window w, XWindowAttributes* x); 
    int (*xGrabKeyboard) (Display* d, Window w, int b, int i1, int i2, Time t); 
    int (*xGrabPointer) (Display* d, Window w1, int b, unsigned int ui, int i1, int i2, Window w2, Cursor c, Time t); 
    KeyCode (*xKeysymToKeycode) (Display* d, KeySym k); 
    Status (*xLookupColor) (Display* d, Colormap c1, const char* c2, XColor* x1, XColor* x2); 
    void (*xMapWindow) (Display* d, Window w); 
    void (*xMoveResizeWindow) (Display* d, Window w, int i1, int i2, unsigned int ui1, unsigned int ui2); 
    void (*xMoveWindow) (Display* d, Window w, int i1, int i2); 
    void (*xNextEvent) (Display* d, XEvent* x); 
    void (*xPutBackEvent) (Display* d, XEvent* x); 
    void (*xQueryColors) (Display* d, Colormap c, XColor* x, int i); 
    int (*xQueryPointer) (Display* d, Window w1, Window* w2, Window* w3, int* i1, int* i2, int* i3, int* i4, unsigned int* ui); 
    Status (*xQueryTree) (Display* d, Window w1, Window* w2, Window* w3, Window** w4, unsigned int* ui); 
    void (*xRaiseWindow) (Display* d, Window w); 
    void (*xRefreshKeyboardMapping) (XMappingEvent* x); 
    void (*xResizeWindow) (Display* d, Window w, unsigned int ui1, unsigned int ui2); 
    void (*xSelectInput) (Display* d, Window w, long l); 
    Status (*xSendEvent) (Display* d, Window w, int b, long l, XEvent* x); 
    void (*xSetCommand) (Display* d, Window w, char** c, int i); 
    void (*xSetIconName) (Display* d, Window w, const char* c); 
    void (*xSetInputFocus) (Display* d, Window w, int i, Time t); 
    void (*xSetSelectionOwner) (Display* d, Atom a, Window w, Time t); 
    void (*xSetWindowBackground) (Display* d, Window w, unsigned long ul); 
    void (*xSetWindowBackgroundPixmap) (Display* d, Window w, Pixmap p); 
    void (*xSetWindowBorder) (Display* d, Window w, unsigned long ul); 
    void (*xSetWindowBorderPixmap) (Display* d, Window w, Pixmap p); 
    void (*xSetWindowBorderWidth) (Display* d, Window w, unsigned int ui); 
    void (*xSetWindowColormap) (Display* d, Window w, Colormap c); 
    int (*xTranslateCoordinates) (Display* d, Window w1, Window w2, int i1, int i2, int* i3, int* i4, Window* w3); 
    void (*xUngrabKeyboard) (Display* d, Time t); 
    void (*xUngrabPointer) (Display* d, Time t); 
    void (*xUnmapWindow) (Display* d, Window w); 
    void (*xWindowEvent) (Display* d, Window w, long l, XEvent* x); 
    void (*xDestroyIC) (XIC x); 
    int (*xFilterEvent) (XEvent* x, Window w); 
    int (*xmbLookupString) (XIC xi, XKeyPressedEvent* xk, char* c, int i, KeySym* k, Status* s); 
    void (*tkPutImage) (unsigned long * colors, int ncolors, Display* display, Drawable d, GC gc, XImage* image, int src_x, int src_y, int dest_x, int dest_y, unsigned int width, unsigned int height); 
    void *reserved81;
    Status (*xParseColor) (Display * display, Colormap map, const char* spec, XColor * colorPtr); 
    GC (*xCreateGC) (Display* display, Drawable d, unsigned long valuemask, XGCValues* values); 
    void (*xFreeGC) (Display* display, GC gc); 
    Atom (*xInternAtom) (Display* display, const char* atom_name, int only_if_exists); 
    void (*xSetBackground) (Display* display, GC gc, unsigned long foreground); 
    void (*xSetForeground) (Display* display, GC gc, unsigned long foreground); 
    void (*xSetClipMask) (Display* display, GC gc, Pixmap pixmap); 
    void (*xSetClipOrigin) (Display* display, GC gc, int clip_x_origin, int clip_y_origin); 
    void (*xSetTSOrigin) (Display* display, GC gc, int ts_x_origin, int ts_y_origin); 
    void (*xChangeGC) (Display * d, GC gc, unsigned long mask, XGCValues * values); 
    void (*xSetFont) (Display * display, GC gc, Font font); 
    void (*xSetArcMode) (Display * display, GC gc, int arc_mode); 
    void (*xSetStipple) (Display * display, GC gc, Pixmap stipple); 
    void (*xSetFillRule) (Display * display, GC gc, int fill_rule); 
    void (*xSetFillStyle) (Display * display, GC gc, int fill_style); 
    void (*xSetFunction) (Display * display, GC gc, int function); 
    void (*xSetLineAttributes) (Display * display, GC gc, unsigned int line_width, int line_style, int cap_style, int join_style); 
    int (*_XInitImageFuncPtrs) (XImage * image); 
    XIC (*xCreateIC) (void); 
    XVisualInfo * (*xGetVisualInfo) (Display* display, long vinfo_mask, XVisualInfo* vinfo_template, int* nitems_return); 
    void (*xSetWMClientMachine) (Display* display, Window w, XTextProperty* text_prop); 
    Status (*xStringListToTextProperty) (char** list, int count, XTextProperty* text_prop_return); 
    void (*xDrawLine) (Display* d, Drawable dr, GC g, int x1, int y1, int x2, int y2); 
    void (*xWarpPointer) (Display* d, Window s, Window dw, int sx, int sy, unsigned int sw, unsigned int sh, int dx, int dy); 
    void (*xFillRectangle) (Display* display, Drawable d, GC gc, int x, int y, unsigned int width, unsigned int height); 
#line 769 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntXlibDecls.h"




























































































} TkIntXlibStubs;




extern TkIntXlibStubs *tkIntXlibStubsPtr;
































































































































































































































































































































































































































































































































































































































































































































































































































#line 1668 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntXlibDecls.h"






#line 1675 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntXlibDecls.h"
#line 1199 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xlib.h"








#line 1208 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xlib.h"
#line 84 "d:\\tcl-source\\tk8.3.1\\generic\\tk.h"
#line 85 "d:\\tcl-source\\tk8.3.1\\generic\\tk.h"
#line 86 "d:\\tcl-source\\tk8.3.1\\generic\\tk.h"





















typedef struct Tk_BindingTable_ *Tk_BindingTable;
typedef struct Tk_Canvas_ *Tk_Canvas;
typedef struct Tk_Cursor_ *Tk_Cursor;
typedef struct Tk_ErrorHandler_ *Tk_ErrorHandler;
typedef struct Tk_Font_ *Tk_Font;
typedef struct Tk_Image__ *Tk_Image;
typedef struct Tk_ImageMaster_ *Tk_ImageMaster;
typedef struct Tk_OptionTable_ *Tk_OptionTable;
typedef struct Tk_PostscriptInfo_ *Tk_PostscriptInfo;
typedef struct Tk_TextLayout_ *Tk_TextLayout;
typedef struct Tk_Window_ *Tk_Window;
typedef struct Tk_3DBorder_ *Tk_3DBorder;





typedef char *Tk_Uid;






typedef enum {
    TK_OPTION_BOOLEAN,
    TK_OPTION_INT,
    TK_OPTION_DOUBLE,
    TK_OPTION_STRING,
    TK_OPTION_STRING_TABLE,
    TK_OPTION_COLOR,
    TK_OPTION_FONT,
    TK_OPTION_BITMAP,
    TK_OPTION_BORDER,
    TK_OPTION_RELIEF,
    TK_OPTION_CURSOR,
    TK_OPTION_JUSTIFY,
    TK_OPTION_ANCHOR,
    TK_OPTION_SYNONYM,
    TK_OPTION_PIXELS,
    TK_OPTION_WINDOW,
    TK_OPTION_END
} Tk_OptionType;









typedef struct Tk_OptionSpec {
    Tk_OptionType type;		


    char *optionName;		

    char *dbName;		
    char *dbClass;		
    char *defValue;		


    int objOffset;		





    int internalOffset;		







    int flags;			

    ClientData clientData;	


    int typeMask;		






} Tk_OptionSpec;




















#line 218 "d:\\tcl-source\\tk8.3.1\\generic\\tk.h"











typedef struct Tk_SavedOption {
    struct TkOption *optionPtr;		

    Tcl_Obj *valuePtr;			



    double internalForm;		











} Tk_SavedOption;





#line 255 "d:\\tcl-source\\tk8.3.1\\generic\\tk.h"

typedef struct Tk_SavedOptions {
    char *recordPtr;			

    Tk_Window tkwin;			

    int numItems;			

    Tk_SavedOption items[20];
					
    struct Tk_SavedOptions *nextPtr;	




} Tk_SavedOptions;















typedef int (Tk_OptionParseProc) 

(ClientData clientData, Tcl_Interp *interp, Tk_Window tkwin, char *value, char *widgRec, int offset);
typedef char *(Tk_OptionPrintProc) 

(ClientData clientData, Tk_Window tkwin, char *widgRec, int offset, Tcl_FreeProc **freeProcPtr);

typedef struct Tk_CustomOption {
    Tk_OptionParseProc *parseProc;	


    Tk_OptionPrintProc *printProc;	


    ClientData clientData;		


} Tk_CustomOption;








typedef struct Tk_ConfigSpec {
    int type;			


    char *argvName;		

    char *dbName;		
    char *dbClass;		
    char *defValue;		

    int offset;			


    int specFlags;		


    Tk_CustomOption *customPtr;	



} Tk_ConfigSpec;






typedef enum {
    TK_CONFIG_BOOLEAN, TK_CONFIG_INT, TK_CONFIG_DOUBLE, TK_CONFIG_STRING,
    TK_CONFIG_UID, TK_CONFIG_COLOR, TK_CONFIG_FONT, TK_CONFIG_BITMAP,
    TK_CONFIG_BORDER, TK_CONFIG_RELIEF, TK_CONFIG_CURSOR, 
    TK_CONFIG_ACTIVE_CURSOR, TK_CONFIG_JUSTIFY, TK_CONFIG_ANCHOR, 
    TK_CONFIG_SYNONYM, TK_CONFIG_CAP_STYLE, TK_CONFIG_JOIN_STYLE,
    TK_CONFIG_PIXELS, TK_CONFIG_MM, TK_CONFIG_WINDOW, TK_CONFIG_CUSTOM, 
    TK_CONFIG_END
} Tk_ConfigTypes;





















#line 370 "d:\\tcl-source\\tk8.3.1\\generic\\tk.h"





typedef struct {
    char *key;		

    int type;		
    char *src;		

    char *dst;		

    char *help;		
} Tk_ArgvInfo;


































typedef enum {
    TK_DEFER_EVENT, TK_PROCESS_EVENT, TK_DISCARD_EVENT
} Tk_RestrictAction;











































typedef enum {
    TK_ANCHOR_N, TK_ANCHOR_NE, TK_ANCHOR_E, TK_ANCHOR_SE,
    TK_ANCHOR_S, TK_ANCHOR_SW, TK_ANCHOR_W, TK_ANCHOR_NW,
    TK_ANCHOR_CENTER
} Tk_Anchor;





typedef enum {
    TK_JUSTIFY_LEFT, TK_JUSTIFY_RIGHT, TK_JUSTIFY_CENTER
} Tk_Justify;






typedef struct Tk_FontMetrics {
    int ascent;			



    int descent;		



    int linespace;		




} Tk_FontMetrics;






















typedef void (Tk_GeomRequestProc) 
(ClientData clientData, Tk_Window tkwin);
typedef void (Tk_GeomLostSlaveProc) 
(ClientData clientData, Tk_Window tkwin);

typedef struct Tk_GeomMgr {
    char *name;			


    Tk_GeomRequestProc *requestProc;
				

    Tk_GeomLostSlaveProc *lostSlaveProc;
				



} Tk_GeomMgr;







































typedef struct {
    int type;
    unsigned long serial;   
    int send_event;	    
    Display *display;	    
    Window event;	    
    Window root;	    
    Window subwindow;	    
    Time time;		    
    int x, y;		    
    int x_root, y_root;	    
    unsigned int state;	    
    Tk_Uid name;	    
    int same_screen;	    
} XVirtualEvent;

typedef struct {
    int type;
    unsigned long serial;   
    int send_event;	    
    Display *display;	    
    Window window;	    
} XActivateDeactivateEvent;
typedef XActivateDeactivateEvent XActivateEvent;
typedef XActivateDeactivateEvent XDeactivateEvent;





















































typedef struct Tk_FakeWin {
    Display *display;
    char *dummy1;
    int screenNum;
    Visual *visual;
    int depth;
    Window window;
    char *dummy2;
    char *dummy3;
    Tk_Window parentPtr;
    char *dummy4;
    char *dummy5;
    char *pathName;
    Tk_Uid nameUid;
    Tk_Uid classUid;
    XWindowChanges changes;
    unsigned int dummy6;
    XSetWindowAttributes atts;
    unsigned long dummy7;
    unsigned int flags;
    char *dummy8;



    ClientData *dummy10;
    int dummy11;
    int dummy12;
    char *dummy13;
    char *dummy14;
    ClientData dummy15;
    int reqWidth, reqHeight;
    int internalBorderWidth;
    char *dummy16;
    char *dummy17;
    ClientData dummy18;
    char *dummy19;
} Tk_FakeWin;





















































































typedef enum {
    TK_STATE_NULL = -1, TK_STATE_ACTIVE, TK_STATE_DISABLED,
    TK_STATE_NORMAL, TK_STATE_HIDDEN
} Tk_State;

typedef struct Tk_SmoothMethod {
    char *name;
    int (*coordProc) 

(Tk_Canvas canvas, double *pointPtr, int numPoints, int numSteps, XPoint xPoints[], double dblPoints[]);
    void (*postscriptProc) 

(Tcl_Interp *interp, Tk_Canvas canvas, double *coordPtr, int numPoints, int numSteps);
} Tk_SmoothMethod;










typedef struct Tk_Item  {
    int id;				


    struct Tk_Item *nextPtr;		



    Tk_Uid staticTagSpace[3];

    Tk_Uid *tagPtr;			



    int tagSpace;			

    int numTags;			

    struct Tk_ItemType *typePtr;	

    int x1, y1, x2, y2;			





    struct Tk_Item *prevPtr;		



    Tk_State state;			
    char *reserved1;			
    int redraw_flags;			

    







} Tk_Item;































typedef int	Tk_ItemCreateProc 

(Tcl_Interp *interp, Tk_Canvas canvas, Tk_Item *itemPtr, int argc, Tcl_Obj *const objv[]);
typedef int	Tk_ItemConfigureProc 

(Tcl_Interp *interp, Tk_Canvas canvas, Tk_Item *itemPtr, int argc, Tcl_Obj *const objv[], int flags);
typedef int	Tk_ItemCoordProc 

(Tcl_Interp *interp, Tk_Canvas canvas, Tk_Item *itemPtr, int argc, Tcl_Obj *const argv[]);
#line 886 "d:\\tcl-source\\tk8.3.1\\generic\\tk.h"
typedef void	Tk_ItemDeleteProc 
(Tk_Canvas canvas, Tk_Item *itemPtr, Display *display);
typedef void	Tk_ItemDisplayProc 

(Tk_Canvas canvas, Tk_Item *itemPtr, Display *display, Drawable dst, int x, int y, int width, int height);
typedef double	Tk_ItemPointProc 
(Tk_Canvas canvas, Tk_Item *itemPtr, double *pointPtr);
typedef int	Tk_ItemAreaProc 
(Tk_Canvas canvas, Tk_Item *itemPtr, double *rectPtr);
typedef int	Tk_ItemPostscriptProc 
(Tcl_Interp *interp, Tk_Canvas canvas, Tk_Item *itemPtr, int prepass);
typedef void	Tk_ItemScaleProc 

(Tk_Canvas canvas, Tk_Item *itemPtr, double originX, double originY, double scaleX, double scaleY);
typedef void	Tk_ItemTranslateProc 
(Tk_Canvas canvas, Tk_Item *itemPtr, double deltaX, double deltaY);
typedef int	Tk_ItemIndexProc 

(Tcl_Interp *interp, Tk_Canvas canvas, Tk_Item *itemPtr, char *indexString, int *indexPtr);
typedef void	Tk_ItemCursorProc 
(Tk_Canvas canvas, Tk_Item *itemPtr, int index);
typedef int	Tk_ItemSelectionProc 

(Tk_Canvas canvas, Tk_Item *itemPtr, int offset, char *buffer, int maxBytes);
typedef void	Tk_ItemInsertProc 
(Tk_Canvas canvas, Tk_Item *itemPtr, int beforeThis, char *string);
typedef void	Tk_ItemDCharsProc 
(Tk_Canvas canvas, Tk_Item *itemPtr, int first, int last);



typedef struct Tk_ItemType {
    char *name;				

    int itemSize;			

    Tk_ItemCreateProc *createProc;	

    Tk_ConfigSpec *configSpecs;		


    Tk_ItemConfigureProc *configProc;	

    Tk_ItemCoordProc *coordProc;	

    Tk_ItemDeleteProc *deleteProc;	

    Tk_ItemDisplayProc *displayProc;	

    int alwaysRedraw;			


    Tk_ItemPointProc *pointProc;	

    Tk_ItemAreaProc *areaProc;		

    Tk_ItemPostscriptProc *postscriptProc;
					


    Tk_ItemScaleProc *scaleProc;	

    Tk_ItemTranslateProc *translateProc;

    Tk_ItemIndexProc *indexProc;	


    Tk_ItemCursorProc *icursorProc;	

    Tk_ItemSelectionProc *selectionProc;


    Tk_ItemInsertProc *insertProc;	

    Tk_ItemDCharsProc *dCharsProc;	

    struct Tk_ItemType *nextPtr;	

    char *reserved1;			
    int   reserved2;			
    char *reserved3;			
    char *reserved4;
} Tk_ItemType;

#line 971 "d:\\tcl-source\\tk8.3.1\\generic\\tk.h"









typedef struct Tk_CanvasTextInfo {
    Tk_3DBorder selBorder;	

    int selBorderWidth;		

    XColor *selFgColorPtr;	

    Tk_Item *selItemPtr;	


    int selectFirst;		

    int selectLast;		

    Tk_Item *anchorItemPtr;	


    int selectAnchor;		



    Tk_3DBorder insertBorder;	

    int insertWidth;		

    int insertBorderWidth;	

    Tk_Item *focusItemPtr;	


    int gotFocus;		

    int cursorOn;		


} Tk_CanvasTextInfo;





typedef struct Tk_Dash {
    int number;
    union {
	char *pt;
	char array[sizeof(char *)];
    } pattern;
} Tk_Dash;

typedef struct Tk_TSOffset {
    int flags;			
    int xoffset;		
    int yoffset;		
} Tk_TSOffset;














typedef struct Tk_Outline {
    GC gc;			
    double width;		
    double activeWidth;		
    double disabledWidth;	
    int offset;			
    Tk_Dash dash;		
    Tk_Dash activeDash;		
    Tk_Dash disabledDash;	
    void *reserved1;		
    void *reserved2;
    void *reserved3;
    Tk_TSOffset tsoffset;	
    XColor *color;		
    XColor *activeColor;	
    XColor *disabledColor;	
    Pixmap stipple;		
    Pixmap activeStipple;	
    Pixmap disabledStipple;	
} Tk_Outline;










typedef struct Tk_ImageType Tk_ImageType;





typedef int (Tk_ImageCreateProc) 

(Tcl_Interp *interp, char *name, int objc, Tcl_Obj *const objv[], Tk_ImageType *typePtr, Tk_ImageMaster master, ClientData *masterDataPtr);
#line 1088 "d:\\tcl-source\\tk8.3.1\\generic\\tk.h"
typedef ClientData (Tk_ImageGetProc) 
(Tk_Window tkwin, ClientData masterData);
typedef void (Tk_ImageDisplayProc) 

(ClientData instanceData, Display *display, Drawable drawable, int imageX, int imageY, int width, int height, int drawableX, int drawableY);
typedef void (Tk_ImageFreeProc) 
(ClientData instanceData, Display *display);
typedef void (Tk_ImageDeleteProc) (ClientData masterData);
typedef void (Tk_ImageChangedProc) 

(ClientData clientData, int x, int y, int width, int height, int imageWidth, int imageHeight);
typedef int (Tk_ImagePostscriptProc) 

(ClientData clientData, Tcl_Interp *interp, Tk_Window tkwin, Tk_PostscriptInfo psinfo, int x, int y, int width, int height, int prepass);










struct Tk_ImageType {
    char *name;			
    Tk_ImageCreateProc *createProc;
				

    Tk_ImageGetProc *getProc;	


    Tk_ImageDisplayProc *displayProc;
				

    Tk_ImageFreeProc *freeProc;	


    Tk_ImageDeleteProc *deleteProc;
				



    Tk_ImagePostscriptProc *postscriptProc;
				

    struct Tk_ImageType *nextPtr;
				


    char *reserved;		
};














typedef void *Tk_PhotoHandle;





typedef struct Tk_PhotoImageBlock {
    unsigned char *pixelPtr;	
    int		width;		
    int		height;		
    int		pitch;		

    int		pixelSize;	

    int		offset[4];	


} Tk_PhotoImageBlock;






typedef struct Tk_PhotoImageFormat Tk_PhotoImageFormat;


















typedef int (Tk_ImageFileMatchProc) 

(Tcl_Channel chan, const char *fileName, Tcl_Obj *format, int *widthPtr, int *heightPtr, Tcl_Interp *interp);
typedef int (Tk_ImageStringMatchProc) 

(Tcl_Obj *dataObj, Tcl_Obj *format, int *widthPtr, int *heightPtr, Tcl_Interp *interp);
typedef int (Tk_ImageFileReadProc) 


(Tcl_Interp *interp, Tcl_Channel chan, const char *fileName, Tcl_Obj *format, Tk_PhotoHandle imageHandle, int destX, int destY, int width, int height, int srcX, int srcY);
typedef int (Tk_ImageStringReadProc) 

(Tcl_Interp *interp, Tcl_Obj *dataObj, Tcl_Obj *format, Tk_PhotoHandle imageHandle, int destX, int destY, int width, int height, int srcX, int srcY);
typedef int (Tk_ImageFileWriteProc) 
(Tcl_Interp *interp, const char *fileName, Tcl_Obj *format, Tk_PhotoImageBlock *blockPtr);
typedef int (Tk_ImageStringWriteProc) 
(Tcl_Interp *interp, Tcl_Obj *format, Tk_PhotoImageBlock *blockPtr);
#line 1215 "d:\\tcl-source\\tk8.3.1\\generic\\tk.h"








struct Tk_PhotoImageFormat {
    char *name;			
    Tk_ImageFileMatchProc *fileMatchProc;
				

    Tk_ImageStringMatchProc *stringMatchProc;
				

    Tk_ImageFileReadProc *fileReadProc;
				

    Tk_ImageStringReadProc *stringReadProc;
				

    Tk_ImageFileWriteProc *fileWriteProc;
				

    Tk_ImageStringWriteProc *stringWriteProc;
				


    struct Tk_PhotoImageFormat *nextPtr;
				


};

extern __declspec(dllimport) void		Tk_CreateOldImageType 
( Tk_ImageType *typePtr);
extern __declspec(dllimport) void		Tk_CreateOldPhotoImageFormat 
( Tk_PhotoImageFormat *formatPtr);




#line 1259 "d:\\tcl-source\\tk8.3.1\\generic\\tk.h"



















































char *Tk_InitStubs (Tcl_Interp *interp, char *version, int exact);






#line 1318 "d:\\tcl-source\\tk8.3.1\\generic\\tk.h"

void Tk_InitImageArgs (Tcl_Interp *interp, int argc, char ***argv);





#line 1326 "d:\\tcl-source\\tk8.3.1\\generic\\tk.h"










typedef int (Tk_ErrorProc) 
(ClientData clientData, XErrorEvent *errEventPtr);
typedef void (Tk_EventProc) 
(ClientData clientData, XEvent *eventPtr);
typedef int (Tk_GenericProc) 
(ClientData clientData, XEvent *eventPtr);
typedef int (Tk_GetSelProc) 
(ClientData clientData, Tcl_Interp *interp, char *portion);
typedef void (Tk_LostSelProc) (ClientData clientData);
typedef Tk_RestrictAction (Tk_RestrictProc) 
( ClientData clientData, XEvent *eventPtr);
typedef int (Tk_SelectionProc) 
(ClientData clientData, int offset, char *buffer, int maxBytes);










#line 1 "d:\\tcl-source\\tk8.3.1\\generic\\tkDecls.h"


































extern __declspec(dllimport) void		Tk_MainLoop (void);

extern __declspec(dllimport) XColor *		Tk_3DBorderColor (Tk_3DBorder border);

extern __declspec(dllimport) GC		Tk_3DBorderGC 
(Tk_Window tkwin, Tk_3DBorder border, int which);

extern __declspec(dllimport) void		Tk_3DHorizontalBevel 


(Tk_Window tkwin, Drawable drawable, Tk_3DBorder border, int x, int y, int width, int height, int leftIn, int rightIn, int topBevel, int relief);

extern __declspec(dllimport) void		Tk_3DVerticalBevel 


(Tk_Window tkwin, Drawable drawable, Tk_3DBorder border, int x, int y, int width, int height, int leftBevel, int relief);

extern __declspec(dllimport) void		Tk_AddOption 
(Tk_Window tkwin, char * name, char * value, int priority);

extern __declspec(dllimport) void		Tk_BindEvent 


( Tk_BindingTable bindingTable, XEvent * eventPtr, Tk_Window tkwin, int numObjects, ClientData * objectPtr);

extern __declspec(dllimport) void		Tk_CanvasDrawableCoords 

( Tk_Canvas canvas, double x, double y, short * drawableXPtr, short * drawableYPtr);

extern __declspec(dllimport) void		Tk_CanvasEventuallyRedraw 

( Tk_Canvas canvas, int x1, int y1, int x2, int y2);

extern __declspec(dllimport) int		Tk_CanvasGetCoord 

(Tcl_Interp * interp, Tk_Canvas canvas, char * str, double * doublePtr);

extern __declspec(dllimport) Tk_CanvasTextInfo * Tk_CanvasGetTextInfo 
( Tk_Canvas canvas);

extern __declspec(dllimport) int		Tk_CanvasPsBitmap 

(Tcl_Interp * interp, Tk_Canvas canvas, Pixmap bitmap, int x, int y, int width, int height);

extern __declspec(dllimport) int		Tk_CanvasPsColor 
(Tcl_Interp * interp, Tk_Canvas canvas, XColor * colorPtr);

extern __declspec(dllimport) int		Tk_CanvasPsFont 
(Tcl_Interp * interp, Tk_Canvas canvas, Tk_Font font);

extern __declspec(dllimport) void		Tk_CanvasPsPath 

(Tcl_Interp * interp, Tk_Canvas canvas, double * coordPtr, int numPoints);

extern __declspec(dllimport) int		Tk_CanvasPsStipple 
(Tcl_Interp * interp, Tk_Canvas canvas, Pixmap bitmap);

extern __declspec(dllimport) double		Tk_CanvasPsY (Tk_Canvas canvas, double y);

extern __declspec(dllimport) void		Tk_CanvasSetStippleOrigin 
( Tk_Canvas canvas, GC gc);

extern __declspec(dllimport) int		Tk_CanvasTagsParseProc 


( ClientData clientData, Tcl_Interp * interp, Tk_Window tkwin, char * value, char * widgRec, int offset);

extern __declspec(dllimport) char *		Tk_CanvasTagsPrintProc 


( ClientData clientData, Tk_Window tkwin, char * widgRec, int offset, Tcl_FreeProc ** freeProcPtr);

extern __declspec(dllimport) Tk_Window	Tk_CanvasTkwin (Tk_Canvas canvas);

extern __declspec(dllimport) void		Tk_CanvasWindowCoords 

(Tk_Canvas canvas, double x, double y, short * screenXPtr, short * screenYPtr);

extern __declspec(dllimport) void		Tk_ChangeWindowAttributes 

( Tk_Window tkwin, unsigned long valueMask, XSetWindowAttributes * attsPtr);

extern __declspec(dllimport) int		Tk_CharBbox 

(Tk_TextLayout layout, int index, int * xPtr, int * yPtr, int * widthPtr, int * heightPtr);

extern __declspec(dllimport) void		Tk_ClearSelection 
(Tk_Window tkwin, Atom selection);

extern __declspec(dllimport) int		Tk_ClipboardAppend 

(Tcl_Interp * interp, Tk_Window tkwin, Atom target, Atom format, char* buffer);

extern __declspec(dllimport) int		Tk_ClipboardClear 
(Tcl_Interp * interp, Tk_Window tkwin);

extern __declspec(dllimport) int		Tk_ConfigureInfo 

(Tcl_Interp * interp, Tk_Window tkwin, Tk_ConfigSpec * specs, char * widgRec, char * argvName, int flags);

extern __declspec(dllimport) int		Tk_ConfigureValue 

(Tcl_Interp * interp, Tk_Window tkwin, Tk_ConfigSpec * specs, char * widgRec, char * argvName, int flags);

extern __declspec(dllimport) int		Tk_ConfigureWidget 


(Tcl_Interp * interp, Tk_Window tkwin, Tk_ConfigSpec * specs, int argc, char ** argv, char * widgRec, int flags);

extern __declspec(dllimport) void		Tk_ConfigureWindow 

(Tk_Window tkwin, unsigned int valueMask, XWindowChanges * valuePtr);

extern __declspec(dllimport) Tk_TextLayout	Tk_ComputeTextLayout 


(Tk_Font font, const char * str, int numChars, int wrapLength, Tk_Justify justify, int flags, int * widthPtr, int * heightPtr);

extern __declspec(dllimport) Tk_Window	Tk_CoordsToWindow 
(int rootX, int rootY, Tk_Window tkwin);

extern __declspec(dllimport) unsigned long	Tk_CreateBinding 


(Tcl_Interp * interp, Tk_BindingTable bindingTable, ClientData object, char * eventStr, char * command, int append);

extern __declspec(dllimport) Tk_BindingTable	Tk_CreateBindingTable 
( Tcl_Interp * interp);

extern __declspec(dllimport) Tk_ErrorHandler	Tk_CreateErrorHandler 


(Display * display, int errNum, int request, int minorCode, Tk_ErrorProc * errorProc, ClientData clientData);

extern __declspec(dllimport) void		Tk_CreateEventHandler 

(Tk_Window token, unsigned long mask, Tk_EventProc * proc, ClientData clientData);

extern __declspec(dllimport) void		Tk_CreateGenericHandler 
( Tk_GenericProc * proc, ClientData clientData);

extern __declspec(dllimport) void		Tk_CreateImageType 
( Tk_ImageType * typePtr);

extern __declspec(dllimport) void		Tk_CreateItemType (Tk_ItemType * typePtr);

extern __declspec(dllimport) void		Tk_CreatePhotoImageFormat 
( Tk_PhotoImageFormat * formatPtr);

extern __declspec(dllimport) void		Tk_CreateSelHandler 


(Tk_Window tkwin, Atom selection, Atom target, Tk_SelectionProc * proc, ClientData clientData, Atom format);

extern __declspec(dllimport) Tk_Window	Tk_CreateWindow 

(Tcl_Interp * interp, Tk_Window parent, char * name, char * screenName);

extern __declspec(dllimport) Tk_Window	Tk_CreateWindowFromPath 

( Tcl_Interp * interp, Tk_Window tkwin, char * pathName, char * screenName);

extern __declspec(dllimport) int		Tk_DefineBitmap 

(Tcl_Interp * interp, const char * name, char * source, int width, int height);

extern __declspec(dllimport) void		Tk_DefineCursor 
(Tk_Window window, Tk_Cursor cursor);

extern __declspec(dllimport) void		Tk_DeleteAllBindings 

( Tk_BindingTable bindingTable, ClientData object);

extern __declspec(dllimport) int		Tk_DeleteBinding 

(Tcl_Interp * interp, Tk_BindingTable bindingTable, ClientData object, char * eventStr);

extern __declspec(dllimport) void		Tk_DeleteBindingTable 
( Tk_BindingTable bindingTable);

extern __declspec(dllimport) void		Tk_DeleteErrorHandler 
( Tk_ErrorHandler handler);

extern __declspec(dllimport) void		Tk_DeleteEventHandler 

(Tk_Window token, unsigned long mask, Tk_EventProc * proc, ClientData clientData);

extern __declspec(dllimport) void		Tk_DeleteGenericHandler 
( Tk_GenericProc * proc, ClientData clientData);

extern __declspec(dllimport) void		Tk_DeleteImage 
(Tcl_Interp * interp, char * name);

extern __declspec(dllimport) void		Tk_DeleteSelHandler 
(Tk_Window tkwin, Atom selection, Atom target);

extern __declspec(dllimport) void		Tk_DestroyWindow (Tk_Window tkwin);

extern __declspec(dllimport) char *		Tk_DisplayName (Tk_Window tkwin);

extern __declspec(dllimport) int		Tk_DistanceToTextLayout 
( Tk_TextLayout layout, int x, int y);

extern __declspec(dllimport) void		Tk_Draw3DPolygon 


(Tk_Window tkwin, Drawable drawable, Tk_3DBorder border, XPoint * pointPtr, int numPoints, int borderWidth, int leftRelief);

extern __declspec(dllimport) void		Tk_Draw3DRectangle 


(Tk_Window tkwin, Drawable drawable, Tk_3DBorder border, int x, int y, int width, int height, int borderWidth, int relief);

extern __declspec(dllimport) void		Tk_DrawChars 


(Display * display, Drawable drawable, GC gc, Tk_Font tkfont, const char * source, int numBytes, int x, int y);

extern __declspec(dllimport) void		Tk_DrawFocusHighlight 
(Tk_Window tkwin, GC gc, int width, Drawable drawable);

extern __declspec(dllimport) void		Tk_DrawTextLayout 


(Display * display, Drawable drawable, GC gc, Tk_TextLayout layout, int x, int y, int firstChar, int lastChar);

extern __declspec(dllimport) void		Tk_Fill3DPolygon 


(Tk_Window tkwin, Drawable drawable, Tk_3DBorder border, XPoint * pointPtr, int numPoints, int borderWidth, int leftRelief);

extern __declspec(dllimport) void		Tk_Fill3DRectangle 


(Tk_Window tkwin, Drawable drawable, Tk_3DBorder border, int x, int y, int width, int height, int borderWidth, int relief);

extern __declspec(dllimport) Tk_PhotoHandle	Tk_FindPhoto 
(Tcl_Interp * interp, char * imageName);

extern __declspec(dllimport) Font		Tk_FontId (Tk_Font font);

extern __declspec(dllimport) void		Tk_Free3DBorder (Tk_3DBorder border);

extern __declspec(dllimport) void		Tk_FreeBitmap 
(Display * display, Pixmap bitmap);

extern __declspec(dllimport) void		Tk_FreeColor (XColor * colorPtr);

extern __declspec(dllimport) void		Tk_FreeColormap 
(Display * display, Colormap colormap);

extern __declspec(dllimport) void		Tk_FreeCursor 
(Display * display, Tk_Cursor cursor);

extern __declspec(dllimport) void		Tk_FreeFont (Tk_Font f);

extern __declspec(dllimport) void		Tk_FreeGC (Display * display, GC gc);

extern __declspec(dllimport) void		Tk_FreeImage (Tk_Image image);

extern __declspec(dllimport) void		Tk_FreeOptions 

(Tk_ConfigSpec * specs, char * widgRec, Display * display, int needFlags);

extern __declspec(dllimport) void		Tk_FreePixmap 
(Display * display, Pixmap pixmap);

extern __declspec(dllimport) void		Tk_FreeTextLayout 
( Tk_TextLayout textLayout);

extern __declspec(dllimport) void		Tk_FreeXId (Display * display, XID xid);

extern __declspec(dllimport) GC		Tk_GCForColor 
(XColor * colorPtr, Drawable drawable);

extern __declspec(dllimport) void		Tk_GeometryRequest 
(Tk_Window tkwin, int reqWidth, int reqHeight);

extern __declspec(dllimport) Tk_3DBorder	Tk_Get3DBorder 
(Tcl_Interp * interp, Tk_Window tkwin, Tk_Uid colorName);

extern __declspec(dllimport) void		Tk_GetAllBindings 

(Tcl_Interp * interp, Tk_BindingTable bindingTable, ClientData object);

extern __declspec(dllimport) int		Tk_GetAnchor 
(Tcl_Interp * interp, char * str, Tk_Anchor * anchorPtr);

extern __declspec(dllimport) char *		Tk_GetAtomName 
(Tk_Window tkwin, Atom atom);

extern __declspec(dllimport) char *		Tk_GetBinding 

(Tcl_Interp * interp, Tk_BindingTable bindingTable, ClientData object, char * eventStr);

extern __declspec(dllimport) Pixmap		Tk_GetBitmap 
(Tcl_Interp * interp, Tk_Window tkwin, const char * str);

extern __declspec(dllimport) Pixmap		Tk_GetBitmapFromData 

( Tcl_Interp * interp, Tk_Window tkwin, char * source, int width, int height);

extern __declspec(dllimport) int		Tk_GetCapStyle 
(Tcl_Interp * interp, char * str, int * capPtr);

extern __declspec(dllimport) XColor *		Tk_GetColor 
(Tcl_Interp * interp, Tk_Window tkwin, Tk_Uid name);

extern __declspec(dllimport) XColor *		Tk_GetColorByValue 
(Tk_Window tkwin, XColor * colorPtr);

extern __declspec(dllimport) Colormap		Tk_GetColormap 
(Tcl_Interp * interp, Tk_Window tkwin, char * str);

extern __declspec(dllimport) Tk_Cursor	Tk_GetCursor 
(Tcl_Interp * interp, Tk_Window tkwin, Tk_Uid str);

extern __declspec(dllimport) Tk_Cursor	Tk_GetCursorFromData 



( Tcl_Interp * interp, Tk_Window tkwin, char * source, char * mask, int width, int height, int xHot, int yHot, Tk_Uid fg, Tk_Uid bg);

extern __declspec(dllimport) Tk_Font		Tk_GetFont 
(Tcl_Interp * interp, Tk_Window tkwin, const char * str);

extern __declspec(dllimport) Tk_Font		Tk_GetFontFromObj 
(Tk_Window tkwin, Tcl_Obj * objPtr);

extern __declspec(dllimport) void		Tk_GetFontMetrics 
(Tk_Font font, Tk_FontMetrics * fmPtr);

extern __declspec(dllimport) GC		Tk_GetGC 

(Tk_Window tkwin, unsigned long valueMask, XGCValues * valuePtr);

extern __declspec(dllimport) Tk_Image		Tk_GetImage 


(Tcl_Interp * interp, Tk_Window tkwin, char * name, Tk_ImageChangedProc * changeProc, ClientData clientData);

extern __declspec(dllimport) ClientData	Tk_GetImageMasterData 

( Tcl_Interp * interp, char * name, Tk_ImageType ** typePtrPtr);

extern __declspec(dllimport) Tk_ItemType *	Tk_GetItemTypes (void);

extern __declspec(dllimport) int		Tk_GetJoinStyle 
(Tcl_Interp * interp, char * str, int * joinPtr);

extern __declspec(dllimport) int		Tk_GetJustify 
(Tcl_Interp * interp, char * str, Tk_Justify * justifyPtr);

extern __declspec(dllimport) int		Tk_GetNumMainWindows (void);

extern __declspec(dllimport) Tk_Uid		Tk_GetOption 
(Tk_Window tkwin, char * name, char * className);

extern __declspec(dllimport) int		Tk_GetPixels 
(Tcl_Interp * interp, Tk_Window tkwin, char * str, int * intPtr);

extern __declspec(dllimport) Pixmap		Tk_GetPixmap 
(Display * display, Drawable d, int width, int height, int depth);

extern __declspec(dllimport) int		Tk_GetRelief 
(Tcl_Interp * interp, char * name, int * reliefPtr);

extern __declspec(dllimport) void		Tk_GetRootCoords 
(Tk_Window tkwin, int * xPtr, int * yPtr);

extern __declspec(dllimport) int		Tk_GetScrollInfo 

(Tcl_Interp * interp, int argc, char ** argv, double * dblPtr, int * intPtr);

extern __declspec(dllimport) int		Tk_GetScreenMM 

(Tcl_Interp * interp, Tk_Window tkwin, char * str, double * doublePtr);

extern __declspec(dllimport) int		Tk_GetSelection 

(Tcl_Interp * interp, Tk_Window tkwin, Atom selection, Atom target, Tk_GetSelProc * proc, ClientData clientData);

extern __declspec(dllimport) Tk_Uid		Tk_GetUid (const char * str);

extern __declspec(dllimport) Visual *		Tk_GetVisual 

(Tcl_Interp * interp, Tk_Window tkwin, char * str, int * depthPtr, Colormap * colormapPtr);

extern __declspec(dllimport) void		Tk_GetVRootGeometry 

(Tk_Window tkwin, int * xPtr, int * yPtr, int * widthPtr, int * heightPtr);

extern __declspec(dllimport) int		Tk_Grab 
(Tcl_Interp * interp, Tk_Window tkwin, int grabGlobal);

extern __declspec(dllimport) void		Tk_HandleEvent (XEvent * eventPtr);

extern __declspec(dllimport) Tk_Window	Tk_IdToWindow 
(Display * display, Window window);

extern __declspec(dllimport) void		Tk_ImageChanged 

(Tk_ImageMaster master, int x, int y, int width, int height, int imageWidth, int imageHeight);

extern __declspec(dllimport) int		Tk_Init (Tcl_Interp * interp);

extern __declspec(dllimport) Atom		Tk_InternAtom 
(Tk_Window tkwin, char * name);

extern __declspec(dllimport) int		Tk_IntersectTextLayout 

( Tk_TextLayout layout, int x, int y, int width, int height);

extern __declspec(dllimport) void		Tk_MaintainGeometry 

(Tk_Window slave, Tk_Window master, int x, int y, int width, int height);

extern __declspec(dllimport) Tk_Window	Tk_MainWindow (Tcl_Interp * interp);

extern __declspec(dllimport) void		Tk_MakeWindowExist (Tk_Window tkwin);

extern __declspec(dllimport) void		Tk_ManageGeometry 
(Tk_Window tkwin, Tk_GeomMgr * mgrPtr, ClientData clientData);

extern __declspec(dllimport) void		Tk_MapWindow (Tk_Window tkwin);

extern __declspec(dllimport) int		Tk_MeasureChars 

(Tk_Font tkfont, const char * source, int numBytes, int maxPixels, int flags, int * lengthPtr);

extern __declspec(dllimport) void		Tk_MoveResizeWindow 
(Tk_Window tkwin, int x, int y, int width, int height);

extern __declspec(dllimport) void		Tk_MoveWindow 
(Tk_Window tkwin, int x, int y);

extern __declspec(dllimport) void		Tk_MoveToplevelWindow 
(Tk_Window tkwin, int x, int y);

extern __declspec(dllimport) char *		Tk_NameOf3DBorder (Tk_3DBorder border);

extern __declspec(dllimport) char *		Tk_NameOfAnchor (Tk_Anchor anchor);

extern __declspec(dllimport) char *		Tk_NameOfBitmap 
(Display * display, Pixmap bitmap);

extern __declspec(dllimport) char *		Tk_NameOfCapStyle (int cap);

extern __declspec(dllimport) char *		Tk_NameOfColor (XColor * colorPtr);

extern __declspec(dllimport) char *		Tk_NameOfCursor 
(Display * display, Tk_Cursor cursor);

extern __declspec(dllimport) char *		Tk_NameOfFont (Tk_Font font);

extern __declspec(dllimport) char *		Tk_NameOfImage 
( Tk_ImageMaster imageMaster);

extern __declspec(dllimport) char *		Tk_NameOfJoinStyle (int join);

extern __declspec(dllimport) char *		Tk_NameOfJustify (Tk_Justify justify);

extern __declspec(dllimport) char *		Tk_NameOfRelief (int relief);

extern __declspec(dllimport) Tk_Window	Tk_NameToWindow 
(Tcl_Interp * interp, char * pathName, Tk_Window tkwin);

extern __declspec(dllimport) void		Tk_OwnSelection 

(Tk_Window tkwin, Atom selection, Tk_LostSelProc * proc, ClientData clientData);

extern __declspec(dllimport) int		Tk_ParseArgv 

(Tcl_Interp * interp, Tk_Window tkwin, int * argcPtr, char ** argv, Tk_ArgvInfo * argTable, int flags);

extern __declspec(dllimport) void		Tk_PhotoPutBlock 

(Tk_PhotoHandle handle, Tk_PhotoImageBlock * blockPtr, int x, int y, int width, int height);

extern __declspec(dllimport) void		Tk_PhotoPutZoomedBlock 



( Tk_PhotoHandle handle, Tk_PhotoImageBlock * blockPtr, int x, int y, int width, int height, int zoomX, int zoomY, int subsampleX, int subsampleY);

extern __declspec(dllimport) int		Tk_PhotoGetImage 
(Tk_PhotoHandle handle, Tk_PhotoImageBlock * blockPtr);

extern __declspec(dllimport) void		Tk_PhotoBlank (Tk_PhotoHandle handle);

extern __declspec(dllimport) void		Tk_PhotoExpand 
(Tk_PhotoHandle handle, int width, int height);

extern __declspec(dllimport) void		Tk_PhotoGetSize 
(Tk_PhotoHandle handle, int * widthPtr, int * heightPtr);

extern __declspec(dllimport) void		Tk_PhotoSetSize 
(Tk_PhotoHandle handle, int width, int height);

extern __declspec(dllimport) int		Tk_PointToChar 
(Tk_TextLayout layout, int x, int y);

extern __declspec(dllimport) int		Tk_PostscriptFontName 
(Tk_Font tkfont, Tcl_DString * dsPtr);

extern __declspec(dllimport) void		Tk_PreserveColormap 
(Display * display, Colormap colormap);

extern __declspec(dllimport) void		Tk_QueueWindowEvent 
(XEvent * eventPtr, Tcl_QueuePosition position);

extern __declspec(dllimport) void		Tk_RedrawImage 


(Tk_Image image, int imageX, int imageY, int width, int height, Drawable drawable, int drawableX, int drawableY);

extern __declspec(dllimport) void		Tk_ResizeWindow 
(Tk_Window tkwin, int width, int height);

extern __declspec(dllimport) int		Tk_RestackWindow 
(Tk_Window tkwin, int aboveBelow, Tk_Window other);

extern __declspec(dllimport) Tk_RestrictProc * Tk_RestrictEvents 

( Tk_RestrictProc * proc, ClientData arg, ClientData * prevArgPtr);

extern __declspec(dllimport) int		Tk_SafeInit (Tcl_Interp * interp);

extern __declspec(dllimport) char *		Tk_SetAppName 
(Tk_Window tkwin, char * name);

extern __declspec(dllimport) void		Tk_SetBackgroundFromBorder 
( Tk_Window tkwin, Tk_3DBorder border);

extern __declspec(dllimport) void		Tk_SetClass 
(Tk_Window tkwin, char * className);

extern __declspec(dllimport) void		Tk_SetGrid 

(Tk_Window tkwin, int reqWidth, int reqHeight, int gridWidth, int gridHeight);

extern __declspec(dllimport) void		Tk_SetInternalBorder 
(Tk_Window tkwin, int width);

extern __declspec(dllimport) void		Tk_SetWindowBackground 
(Tk_Window tkwin, unsigned long pixel);

extern __declspec(dllimport) void		Tk_SetWindowBackgroundPixmap 
( Tk_Window tkwin, Pixmap pixmap);

extern __declspec(dllimport) void		Tk_SetWindowBorder 
(Tk_Window tkwin, unsigned long pixel);

extern __declspec(dllimport) void		Tk_SetWindowBorderWidth 
(Tk_Window tkwin, int width);

extern __declspec(dllimport) void		Tk_SetWindowBorderPixmap 
( Tk_Window tkwin, Pixmap pixmap);

extern __declspec(dllimport) void		Tk_SetWindowColormap 
(Tk_Window tkwin, Colormap colormap);

extern __declspec(dllimport) int		Tk_SetWindowVisual 

(Tk_Window tkwin, Visual * visual, int depth, Colormap colormap);

extern __declspec(dllimport) void		Tk_SizeOfBitmap 

(Display * display, Pixmap bitmap, int * widthPtr, int * heightPtr);

extern __declspec(dllimport) void		Tk_SizeOfImage 
(Tk_Image image, int * widthPtr, int * heightPtr);

extern __declspec(dllimport) int		Tk_StrictMotif (Tk_Window tkwin);

extern __declspec(dllimport) void		Tk_TextLayoutToPostscript 
( Tcl_Interp * interp, Tk_TextLayout layout);

extern __declspec(dllimport) int		Tk_TextWidth 
(Tk_Font font, const char * str, int numBytes);

extern __declspec(dllimport) void		Tk_UndefineCursor (Tk_Window window);

extern __declspec(dllimport) void		Tk_UnderlineChars 


(Display * display, Drawable drawable, GC gc, Tk_Font tkfont, const char * source, int x, int y, int firstByte, int lastByte);

extern __declspec(dllimport) void		Tk_UnderlineTextLayout 


( Display * display, Drawable drawable, GC gc, Tk_TextLayout layout, int x, int y, int underline);

extern __declspec(dllimport) void		Tk_Ungrab (Tk_Window tkwin);

extern __declspec(dllimport) void		Tk_UnmaintainGeometry 
(Tk_Window slave, Tk_Window master);

extern __declspec(dllimport) void		Tk_UnmapWindow (Tk_Window tkwin);

extern __declspec(dllimport) void		Tk_UnsetGrid (Tk_Window tkwin);

extern __declspec(dllimport) void		Tk_UpdatePointer 
(Tk_Window tkwin, int x, int y, int state);

extern __declspec(dllimport) Pixmap		Tk_AllocBitmapFromObj 

( Tcl_Interp * interp, Tk_Window tkwin, Tcl_Obj * objPtr);

extern __declspec(dllimport) Tk_3DBorder	Tk_Alloc3DBorderFromObj 

( Tcl_Interp * interp, Tk_Window tkwin, Tcl_Obj * objPtr);

extern __declspec(dllimport) XColor *		Tk_AllocColorFromObj 

( Tcl_Interp * interp, Tk_Window tkwin, Tcl_Obj * objPtr);

extern __declspec(dllimport) Tk_Cursor	Tk_AllocCursorFromObj 

( Tcl_Interp * interp, Tk_Window tkwin, Tcl_Obj * objPtr);

extern __declspec(dllimport) Tk_Font		Tk_AllocFontFromObj 
(Tcl_Interp * interp, Tk_Window tkwin, Tcl_Obj * objPtr);

extern __declspec(dllimport) Tk_OptionTable	Tk_CreateOptionTable 

( Tcl_Interp * interp, const Tk_OptionSpec * templatePtr);

extern __declspec(dllimport) void		Tk_DeleteOptionTable 
( Tk_OptionTable optionTable);

extern __declspec(dllimport) void		Tk_Free3DBorderFromObj 
(Tk_Window tkwin, Tcl_Obj * objPtr);

extern __declspec(dllimport) void		Tk_FreeBitmapFromObj 
(Tk_Window tkwin, Tcl_Obj * objPtr);

extern __declspec(dllimport) void		Tk_FreeColorFromObj 
(Tk_Window tkwin, Tcl_Obj * objPtr);

extern __declspec(dllimport) void		Tk_FreeConfigOptions 
(char * recordPtr, Tk_OptionTable optionToken, Tk_Window tkwin);

extern __declspec(dllimport) void		Tk_FreeSavedOptions 
( Tk_SavedOptions * savePtr);

extern __declspec(dllimport) void		Tk_FreeCursorFromObj 
(Tk_Window tkwin, Tcl_Obj * objPtr);

extern __declspec(dllimport) void		Tk_FreeFontFromObj 
(Tk_Window tkwin, Tcl_Obj * objPtr);

extern __declspec(dllimport) Tk_3DBorder	Tk_Get3DBorderFromObj 
(Tk_Window tkwin, Tcl_Obj * objPtr);

extern __declspec(dllimport) int		Tk_GetAnchorFromObj 
(Tcl_Interp * interp, Tcl_Obj * objPtr, Tk_Anchor * anchorPtr);

extern __declspec(dllimport) Pixmap		Tk_GetBitmapFromObj 
(Tk_Window tkwin, Tcl_Obj * objPtr);

extern __declspec(dllimport) XColor *		Tk_GetColorFromObj 
(Tk_Window tkwin, Tcl_Obj * objPtr);

extern __declspec(dllimport) Tk_Cursor	Tk_GetCursorFromObj 
(Tk_Window tkwin, Tcl_Obj * objPtr);

extern __declspec(dllimport) Tcl_Obj *	Tk_GetOptionInfo 

(Tcl_Interp * interp, char * recordPtr, Tk_OptionTable optionTable, Tcl_Obj * namePtr, Tk_Window tkwin);

extern __declspec(dllimport) Tcl_Obj *	Tk_GetOptionValue 

(Tcl_Interp * interp, char * recordPtr, Tk_OptionTable optionTable, Tcl_Obj * namePtr, Tk_Window tkwin);

extern __declspec(dllimport) int		Tk_GetJustifyFromObj 

( Tcl_Interp * interp, Tcl_Obj * objPtr, Tk_Justify * justifyPtr);

extern __declspec(dllimport) int		Tk_GetMMFromObj 

(Tcl_Interp * interp, Tk_Window tkwin, Tcl_Obj * objPtr, double * doublePtr);

extern __declspec(dllimport) int		Tk_GetPixelsFromObj 

(Tcl_Interp * interp, Tk_Window tkwin, Tcl_Obj * objPtr, int * intPtr);

extern __declspec(dllimport) int		Tk_GetReliefFromObj 
(Tcl_Interp * interp, Tcl_Obj * objPtr, int * resultPtr);

extern __declspec(dllimport) int		Tk_GetScrollInfoObj 

(Tcl_Interp * interp, int objc, Tcl_Obj *const objv[], double * dblPtr, int * intPtr);

extern __declspec(dllimport) int		Tk_InitOptions 

(Tcl_Interp * interp, char * recordPtr, Tk_OptionTable optionToken, Tk_Window tkwin);

extern __declspec(dllimport) void		Tk_MainEx 

(int argc, char ** argv, Tcl_AppInitProc * appInitProc, Tcl_Interp * interp);

extern __declspec(dllimport) void		Tk_RestoreSavedOptions 
( Tk_SavedOptions * savePtr);

extern __declspec(dllimport) int		Tk_SetOptions 



(Tcl_Interp * interp, char * recordPtr, Tk_OptionTable optionTable, int objc, Tcl_Obj *const objv[], Tk_Window tkwin, Tk_SavedOptions * savePtr, int * maskPtr);

extern __declspec(dllimport) void		Tk_InitConsoleChannels 
( Tcl_Interp * interp);

extern __declspec(dllimport) int		Tk_CreateConsoleWindow 
( Tcl_Interp * interp);

extern __declspec(dllimport) void		Tk_CreateSmoothMethod 

( Tcl_Interp * interp, Tk_SmoothMethod * method);



extern __declspec(dllimport) int		Tk_GetDash 
(Tcl_Interp * interp, const char * value, Tk_Dash * dash);

extern __declspec(dllimport) void		Tk_CreateOutline (Tk_Outline * outline);

extern __declspec(dllimport) void		Tk_DeleteOutline 
(Display * display, Tk_Outline * outline);

extern __declspec(dllimport) int		Tk_ConfigOutlineGC 

(XGCValues * gcValues, Tk_Canvas canvas, Tk_Item * item, Tk_Outline * outline);

extern __declspec(dllimport) int		Tk_ChangeOutlineGC 
(Tk_Canvas canvas, Tk_Item * item, Tk_Outline * outline);

extern __declspec(dllimport) int		Tk_ResetOutlineGC 
(Tk_Canvas canvas, Tk_Item * item, Tk_Outline * outline);

extern __declspec(dllimport) int		Tk_CanvasPsOutline 
(Tk_Canvas canvas, Tk_Item * item, Tk_Outline * outline);

extern __declspec(dllimport) void		Tk_SetTSOrigin 
(Tk_Window tkwin, GC gc, int x, int y);

extern __declspec(dllimport) int		Tk_CanvasGetCoordFromObj 

( Tcl_Interp * interp, Tk_Canvas canvas, Tcl_Obj * obj, double * doublePtr);

extern __declspec(dllimport) void		Tk_CanvasSetOffset 
(Tk_Canvas canvas, GC gc, Tk_TSOffset * offset);

extern __declspec(dllimport) void		Tk_DitherPhoto 
(Tk_PhotoHandle handle, int x, int y, int width, int height);

extern __declspec(dllimport) int		Tk_PostscriptBitmap 


(Tcl_Interp * interp, Tk_Window tkwin, Tk_PostscriptInfo psInfo, Pixmap bitmap, int startX, int startY, int width, int height);

extern __declspec(dllimport) int		Tk_PostscriptColor 
(Tcl_Interp * interp, Tk_PostscriptInfo psInfo, XColor * colorPtr);

extern __declspec(dllimport) int		Tk_PostscriptFont 
(Tcl_Interp * interp, Tk_PostscriptInfo psInfo, Tk_Font font);

extern __declspec(dllimport) int		Tk_PostscriptImage 


(Tk_Image image, Tcl_Interp * interp, Tk_Window tkwin, Tk_PostscriptInfo psinfo, int x, int y, int width, int height, int prepass);

extern __declspec(dllimport) void		Tk_PostscriptPath 

(Tcl_Interp * interp, Tk_PostscriptInfo psInfo, double * coordPtr, int numPoints);

extern __declspec(dllimport) int		Tk_PostscriptStipple 

( Tcl_Interp * interp, Tk_Window tkwin, Tk_PostscriptInfo psInfo, Pixmap bitmap);

extern __declspec(dllimport) double		Tk_PostscriptY 
(double y, Tk_PostscriptInfo psInfo);

extern __declspec(dllimport) int		Tk_PostscriptPhoto 


(Tcl_Interp * interp, Tk_PhotoImageBlock * blockPtr, Tk_PostscriptInfo psInfo, int width, int height);

typedef struct TkStubHooks {
    struct TkPlatStubs *tkPlatStubs;
    struct TkIntStubs *tkIntStubs;
    struct TkIntPlatStubs *tkIntPlatStubs;
    struct TkIntXlibStubs *tkIntXlibStubs;
} TkStubHooks;

typedef struct TkStubs {
    int magic;
    struct TkStubHooks *hooks;

    void (*tk_MainLoop) (void); 
    XColor * (*tk_3DBorderColor) (Tk_3DBorder border); 
    GC (*tk_3DBorderGC) (Tk_Window tkwin, Tk_3DBorder border, int which); 
    void (*tk_3DHorizontalBevel) (Tk_Window tkwin, Drawable drawable, Tk_3DBorder border, int x, int y, int width, int height, int leftIn, int rightIn, int topBevel, int relief); 
    void (*tk_3DVerticalBevel) (Tk_Window tkwin, Drawable drawable, Tk_3DBorder border, int x, int y, int width, int height, int leftBevel, int relief); 
    void (*tk_AddOption) (Tk_Window tkwin, char * name, char * value, int priority); 
    void (*tk_BindEvent) (Tk_BindingTable bindingTable, XEvent * eventPtr, Tk_Window tkwin, int numObjects, ClientData * objectPtr); 
    void (*tk_CanvasDrawableCoords) (Tk_Canvas canvas, double x, double y, short * drawableXPtr, short * drawableYPtr); 
    void (*tk_CanvasEventuallyRedraw) (Tk_Canvas canvas, int x1, int y1, int x2, int y2); 
    int (*tk_CanvasGetCoord) (Tcl_Interp * interp, Tk_Canvas canvas, char * str, double * doublePtr); 
    Tk_CanvasTextInfo * (*tk_CanvasGetTextInfo) (Tk_Canvas canvas); 
    int (*tk_CanvasPsBitmap) (Tcl_Interp * interp, Tk_Canvas canvas, Pixmap bitmap, int x, int y, int width, int height); 
    int (*tk_CanvasPsColor) (Tcl_Interp * interp, Tk_Canvas canvas, XColor * colorPtr); 
    int (*tk_CanvasPsFont) (Tcl_Interp * interp, Tk_Canvas canvas, Tk_Font font); 
    void (*tk_CanvasPsPath) (Tcl_Interp * interp, Tk_Canvas canvas, double * coordPtr, int numPoints); 
    int (*tk_CanvasPsStipple) (Tcl_Interp * interp, Tk_Canvas canvas, Pixmap bitmap); 
    double (*tk_CanvasPsY) (Tk_Canvas canvas, double y); 
    void (*tk_CanvasSetStippleOrigin) (Tk_Canvas canvas, GC gc); 
    int (*tk_CanvasTagsParseProc) (ClientData clientData, Tcl_Interp * interp, Tk_Window tkwin, char * value, char * widgRec, int offset); 
    char * (*tk_CanvasTagsPrintProc) (ClientData clientData, Tk_Window tkwin, char * widgRec, int offset, Tcl_FreeProc ** freeProcPtr); 
    Tk_Window (*tk_CanvasTkwin) (Tk_Canvas canvas); 
    void (*tk_CanvasWindowCoords) (Tk_Canvas canvas, double x, double y, short * screenXPtr, short * screenYPtr); 
    void (*tk_ChangeWindowAttributes) (Tk_Window tkwin, unsigned long valueMask, XSetWindowAttributes * attsPtr); 
    int (*tk_CharBbox) (Tk_TextLayout layout, int index, int * xPtr, int * yPtr, int * widthPtr, int * heightPtr); 
    void (*tk_ClearSelection) (Tk_Window tkwin, Atom selection); 
    int (*tk_ClipboardAppend) (Tcl_Interp * interp, Tk_Window tkwin, Atom target, Atom format, char* buffer); 
    int (*tk_ClipboardClear) (Tcl_Interp * interp, Tk_Window tkwin); 
    int (*tk_ConfigureInfo) (Tcl_Interp * interp, Tk_Window tkwin, Tk_ConfigSpec * specs, char * widgRec, char * argvName, int flags); 
    int (*tk_ConfigureValue) (Tcl_Interp * interp, Tk_Window tkwin, Tk_ConfigSpec * specs, char * widgRec, char * argvName, int flags); 
    int (*tk_ConfigureWidget) (Tcl_Interp * interp, Tk_Window tkwin, Tk_ConfigSpec * specs, int argc, char ** argv, char * widgRec, int flags); 
    void (*tk_ConfigureWindow) (Tk_Window tkwin, unsigned int valueMask, XWindowChanges * valuePtr); 
    Tk_TextLayout (*tk_ComputeTextLayout) (Tk_Font font, const char * str, int numChars, int wrapLength, Tk_Justify justify, int flags, int * widthPtr, int * heightPtr); 
    Tk_Window (*tk_CoordsToWindow) (int rootX, int rootY, Tk_Window tkwin); 
    unsigned long (*tk_CreateBinding) (Tcl_Interp * interp, Tk_BindingTable bindingTable, ClientData object, char * eventStr, char * command, int append); 
    Tk_BindingTable (*tk_CreateBindingTable) (Tcl_Interp * interp); 
    Tk_ErrorHandler (*tk_CreateErrorHandler) (Display * display, int errNum, int request, int minorCode, Tk_ErrorProc * errorProc, ClientData clientData); 
    void (*tk_CreateEventHandler) (Tk_Window token, unsigned long mask, Tk_EventProc * proc, ClientData clientData); 
    void (*tk_CreateGenericHandler) (Tk_GenericProc * proc, ClientData clientData); 
    void (*tk_CreateImageType) (Tk_ImageType * typePtr); 
    void (*tk_CreateItemType) (Tk_ItemType * typePtr); 
    void (*tk_CreatePhotoImageFormat) (Tk_PhotoImageFormat * formatPtr); 
    void (*tk_CreateSelHandler) (Tk_Window tkwin, Atom selection, Atom target, Tk_SelectionProc * proc, ClientData clientData, Atom format); 
    Tk_Window (*tk_CreateWindow) (Tcl_Interp * interp, Tk_Window parent, char * name, char * screenName); 
    Tk_Window (*tk_CreateWindowFromPath) (Tcl_Interp * interp, Tk_Window tkwin, char * pathName, char * screenName); 
    int (*tk_DefineBitmap) (Tcl_Interp * interp, const char * name, char * source, int width, int height); 
    void (*tk_DefineCursor) (Tk_Window window, Tk_Cursor cursor); 
    void (*tk_DeleteAllBindings) (Tk_BindingTable bindingTable, ClientData object); 
    int (*tk_DeleteBinding) (Tcl_Interp * interp, Tk_BindingTable bindingTable, ClientData object, char * eventStr); 
    void (*tk_DeleteBindingTable) (Tk_BindingTable bindingTable); 
    void (*tk_DeleteErrorHandler) (Tk_ErrorHandler handler); 
    void (*tk_DeleteEventHandler) (Tk_Window token, unsigned long mask, Tk_EventProc * proc, ClientData clientData); 
    void (*tk_DeleteGenericHandler) (Tk_GenericProc * proc, ClientData clientData); 
    void (*tk_DeleteImage) (Tcl_Interp * interp, char * name); 
    void (*tk_DeleteSelHandler) (Tk_Window tkwin, Atom selection, Atom target); 
    void (*tk_DestroyWindow) (Tk_Window tkwin); 
    char * (*tk_DisplayName) (Tk_Window tkwin); 
    int (*tk_DistanceToTextLayout) (Tk_TextLayout layout, int x, int y); 
    void (*tk_Draw3DPolygon) (Tk_Window tkwin, Drawable drawable, Tk_3DBorder border, XPoint * pointPtr, int numPoints, int borderWidth, int leftRelief); 
    void (*tk_Draw3DRectangle) (Tk_Window tkwin, Drawable drawable, Tk_3DBorder border, int x, int y, int width, int height, int borderWidth, int relief); 
    void (*tk_DrawChars) (Display * display, Drawable drawable, GC gc, Tk_Font tkfont, const char * source, int numBytes, int x, int y); 
    void (*tk_DrawFocusHighlight) (Tk_Window tkwin, GC gc, int width, Drawable drawable); 
    void (*tk_DrawTextLayout) (Display * display, Drawable drawable, GC gc, Tk_TextLayout layout, int x, int y, int firstChar, int lastChar); 
    void (*tk_Fill3DPolygon) (Tk_Window tkwin, Drawable drawable, Tk_3DBorder border, XPoint * pointPtr, int numPoints, int borderWidth, int leftRelief); 
    void (*tk_Fill3DRectangle) (Tk_Window tkwin, Drawable drawable, Tk_3DBorder border, int x, int y, int width, int height, int borderWidth, int relief); 
    Tk_PhotoHandle (*tk_FindPhoto) (Tcl_Interp * interp, char * imageName); 
    Font (*tk_FontId) (Tk_Font font); 
    void (*tk_Free3DBorder) (Tk_3DBorder border); 
    void (*tk_FreeBitmap) (Display * display, Pixmap bitmap); 
    void (*tk_FreeColor) (XColor * colorPtr); 
    void (*tk_FreeColormap) (Display * display, Colormap colormap); 
    void (*tk_FreeCursor) (Display * display, Tk_Cursor cursor); 
    void (*tk_FreeFont) (Tk_Font f); 
    void (*tk_FreeGC) (Display * display, GC gc); 
    void (*tk_FreeImage) (Tk_Image image); 
    void (*tk_FreeOptions) (Tk_ConfigSpec * specs, char * widgRec, Display * display, int needFlags); 
    void (*tk_FreePixmap) (Display * display, Pixmap pixmap); 
    void (*tk_FreeTextLayout) (Tk_TextLayout textLayout); 
    void (*tk_FreeXId) (Display * display, XID xid); 
    GC (*tk_GCForColor) (XColor * colorPtr, Drawable drawable); 
    void (*tk_GeometryRequest) (Tk_Window tkwin, int reqWidth, int reqHeight); 
    Tk_3DBorder (*tk_Get3DBorder) (Tcl_Interp * interp, Tk_Window tkwin, Tk_Uid colorName); 
    void (*tk_GetAllBindings) (Tcl_Interp * interp, Tk_BindingTable bindingTable, ClientData object); 
    int (*tk_GetAnchor) (Tcl_Interp * interp, char * str, Tk_Anchor * anchorPtr); 
    char * (*tk_GetAtomName) (Tk_Window tkwin, Atom atom); 
    char * (*tk_GetBinding) (Tcl_Interp * interp, Tk_BindingTable bindingTable, ClientData object, char * eventStr); 
    Pixmap (*tk_GetBitmap) (Tcl_Interp * interp, Tk_Window tkwin, const char * str); 
    Pixmap (*tk_GetBitmapFromData) (Tcl_Interp * interp, Tk_Window tkwin, char * source, int width, int height); 
    int (*tk_GetCapStyle) (Tcl_Interp * interp, char * str, int * capPtr); 
    XColor * (*tk_GetColor) (Tcl_Interp * interp, Tk_Window tkwin, Tk_Uid name); 
    XColor * (*tk_GetColorByValue) (Tk_Window tkwin, XColor * colorPtr); 
    Colormap (*tk_GetColormap) (Tcl_Interp * interp, Tk_Window tkwin, char * str); 
    Tk_Cursor (*tk_GetCursor) (Tcl_Interp * interp, Tk_Window tkwin, Tk_Uid str); 
    Tk_Cursor (*tk_GetCursorFromData) (Tcl_Interp * interp, Tk_Window tkwin, char * source, char * mask, int width, int height, int xHot, int yHot, Tk_Uid fg, Tk_Uid bg); 
    Tk_Font (*tk_GetFont) (Tcl_Interp * interp, Tk_Window tkwin, const char * str); 
    Tk_Font (*tk_GetFontFromObj) (Tk_Window tkwin, Tcl_Obj * objPtr); 
    void (*tk_GetFontMetrics) (Tk_Font font, Tk_FontMetrics * fmPtr); 
    GC (*tk_GetGC) (Tk_Window tkwin, unsigned long valueMask, XGCValues * valuePtr); 
    Tk_Image (*tk_GetImage) (Tcl_Interp * interp, Tk_Window tkwin, char * name, Tk_ImageChangedProc * changeProc, ClientData clientData); 
    ClientData (*tk_GetImageMasterData) (Tcl_Interp * interp, char * name, Tk_ImageType ** typePtrPtr); 
    Tk_ItemType * (*tk_GetItemTypes) (void); 
    int (*tk_GetJoinStyle) (Tcl_Interp * interp, char * str, int * joinPtr); 
    int (*tk_GetJustify) (Tcl_Interp * interp, char * str, Tk_Justify * justifyPtr); 
    int (*tk_GetNumMainWindows) (void); 
    Tk_Uid (*tk_GetOption) (Tk_Window tkwin, char * name, char * className); 
    int (*tk_GetPixels) (Tcl_Interp * interp, Tk_Window tkwin, char * str, int * intPtr); 
    Pixmap (*tk_GetPixmap) (Display * display, Drawable d, int width, int height, int depth); 
    int (*tk_GetRelief) (Tcl_Interp * interp, char * name, int * reliefPtr); 
    void (*tk_GetRootCoords) (Tk_Window tkwin, int * xPtr, int * yPtr); 
    int (*tk_GetScrollInfo) (Tcl_Interp * interp, int argc, char ** argv, double * dblPtr, int * intPtr); 
    int (*tk_GetScreenMM) (Tcl_Interp * interp, Tk_Window tkwin, char * str, double * doublePtr); 
    int (*tk_GetSelection) (Tcl_Interp * interp, Tk_Window tkwin, Atom selection, Atom target, Tk_GetSelProc * proc, ClientData clientData); 
    Tk_Uid (*tk_GetUid) (const char * str); 
    Visual * (*tk_GetVisual) (Tcl_Interp * interp, Tk_Window tkwin, char * str, int * depthPtr, Colormap * colormapPtr); 
    void (*tk_GetVRootGeometry) (Tk_Window tkwin, int * xPtr, int * yPtr, int * widthPtr, int * heightPtr); 
    int (*tk_Grab) (Tcl_Interp * interp, Tk_Window tkwin, int grabGlobal); 
    void (*tk_HandleEvent) (XEvent * eventPtr); 
    Tk_Window (*tk_IdToWindow) (Display * display, Window window); 
    void (*tk_ImageChanged) (Tk_ImageMaster master, int x, int y, int width, int height, int imageWidth, int imageHeight); 
    int (*tk_Init) (Tcl_Interp * interp); 
    Atom (*tk_InternAtom) (Tk_Window tkwin, char * name); 
    int (*tk_IntersectTextLayout) (Tk_TextLayout layout, int x, int y, int width, int height); 
    void (*tk_MaintainGeometry) (Tk_Window slave, Tk_Window master, int x, int y, int width, int height); 
    Tk_Window (*tk_MainWindow) (Tcl_Interp * interp); 
    void (*tk_MakeWindowExist) (Tk_Window tkwin); 
    void (*tk_ManageGeometry) (Tk_Window tkwin, Tk_GeomMgr * mgrPtr, ClientData clientData); 
    void (*tk_MapWindow) (Tk_Window tkwin); 
    int (*tk_MeasureChars) (Tk_Font tkfont, const char * source, int numBytes, int maxPixels, int flags, int * lengthPtr); 
    void (*tk_MoveResizeWindow) (Tk_Window tkwin, int x, int y, int width, int height); 
    void (*tk_MoveWindow) (Tk_Window tkwin, int x, int y); 
    void (*tk_MoveToplevelWindow) (Tk_Window tkwin, int x, int y); 
    char * (*tk_NameOf3DBorder) (Tk_3DBorder border); 
    char * (*tk_NameOfAnchor) (Tk_Anchor anchor); 
    char * (*tk_NameOfBitmap) (Display * display, Pixmap bitmap); 
    char * (*tk_NameOfCapStyle) (int cap); 
    char * (*tk_NameOfColor) (XColor * colorPtr); 
    char * (*tk_NameOfCursor) (Display * display, Tk_Cursor cursor); 
    char * (*tk_NameOfFont) (Tk_Font font); 
    char * (*tk_NameOfImage) (Tk_ImageMaster imageMaster); 
    char * (*tk_NameOfJoinStyle) (int join); 
    char * (*tk_NameOfJustify) (Tk_Justify justify); 
    char * (*tk_NameOfRelief) (int relief); 
    Tk_Window (*tk_NameToWindow) (Tcl_Interp * interp, char * pathName, Tk_Window tkwin); 
    void (*tk_OwnSelection) (Tk_Window tkwin, Atom selection, Tk_LostSelProc * proc, ClientData clientData); 
    int (*tk_ParseArgv) (Tcl_Interp * interp, Tk_Window tkwin, int * argcPtr, char ** argv, Tk_ArgvInfo * argTable, int flags); 
    void (*tk_PhotoPutBlock) (Tk_PhotoHandle handle, Tk_PhotoImageBlock * blockPtr, int x, int y, int width, int height); 
    void (*tk_PhotoPutZoomedBlock) (Tk_PhotoHandle handle, Tk_PhotoImageBlock * blockPtr, int x, int y, int width, int height, int zoomX, int zoomY, int subsampleX, int subsampleY); 
    int (*tk_PhotoGetImage) (Tk_PhotoHandle handle, Tk_PhotoImageBlock * blockPtr); 
    void (*tk_PhotoBlank) (Tk_PhotoHandle handle); 
    void (*tk_PhotoExpand) (Tk_PhotoHandle handle, int width, int height); 
    void (*tk_PhotoGetSize) (Tk_PhotoHandle handle, int * widthPtr, int * heightPtr); 
    void (*tk_PhotoSetSize) (Tk_PhotoHandle handle, int width, int height); 
    int (*tk_PointToChar) (Tk_TextLayout layout, int x, int y); 
    int (*tk_PostscriptFontName) (Tk_Font tkfont, Tcl_DString * dsPtr); 
    void (*tk_PreserveColormap) (Display * display, Colormap colormap); 
    void (*tk_QueueWindowEvent) (XEvent * eventPtr, Tcl_QueuePosition position); 
    void (*tk_RedrawImage) (Tk_Image image, int imageX, int imageY, int width, int height, Drawable drawable, int drawableX, int drawableY); 
    void (*tk_ResizeWindow) (Tk_Window tkwin, int width, int height); 
    int (*tk_RestackWindow) (Tk_Window tkwin, int aboveBelow, Tk_Window other); 
    Tk_RestrictProc * (*tk_RestrictEvents) (Tk_RestrictProc * proc, ClientData arg, ClientData * prevArgPtr); 
    int (*tk_SafeInit) (Tcl_Interp * interp); 
    char * (*tk_SetAppName) (Tk_Window tkwin, char * name); 
    void (*tk_SetBackgroundFromBorder) (Tk_Window tkwin, Tk_3DBorder border); 
    void (*tk_SetClass) (Tk_Window tkwin, char * className); 
    void (*tk_SetGrid) (Tk_Window tkwin, int reqWidth, int reqHeight, int gridWidth, int gridHeight); 
    void (*tk_SetInternalBorder) (Tk_Window tkwin, int width); 
    void (*tk_SetWindowBackground) (Tk_Window tkwin, unsigned long pixel); 
    void (*tk_SetWindowBackgroundPixmap) (Tk_Window tkwin, Pixmap pixmap); 
    void (*tk_SetWindowBorder) (Tk_Window tkwin, unsigned long pixel); 
    void (*tk_SetWindowBorderWidth) (Tk_Window tkwin, int width); 
    void (*tk_SetWindowBorderPixmap) (Tk_Window tkwin, Pixmap pixmap); 
    void (*tk_SetWindowColormap) (Tk_Window tkwin, Colormap colormap); 
    int (*tk_SetWindowVisual) (Tk_Window tkwin, Visual * visual, int depth, Colormap colormap); 
    void (*tk_SizeOfBitmap) (Display * display, Pixmap bitmap, int * widthPtr, int * heightPtr); 
    void (*tk_SizeOfImage) (Tk_Image image, int * widthPtr, int * heightPtr); 
    int (*tk_StrictMotif) (Tk_Window tkwin); 
    void (*tk_TextLayoutToPostscript) (Tcl_Interp * interp, Tk_TextLayout layout); 
    int (*tk_TextWidth) (Tk_Font font, const char * str, int numBytes); 
    void (*tk_UndefineCursor) (Tk_Window window); 
    void (*tk_UnderlineChars) (Display * display, Drawable drawable, GC gc, Tk_Font tkfont, const char * source, int x, int y, int firstByte, int lastByte); 
    void (*tk_UnderlineTextLayout) (Display * display, Drawable drawable, GC gc, Tk_TextLayout layout, int x, int y, int underline); 
    void (*tk_Ungrab) (Tk_Window tkwin); 
    void (*tk_UnmaintainGeometry) (Tk_Window slave, Tk_Window master); 
    void (*tk_UnmapWindow) (Tk_Window tkwin); 
    void (*tk_UnsetGrid) (Tk_Window tkwin); 
    void (*tk_UpdatePointer) (Tk_Window tkwin, int x, int y, int state); 
    Pixmap (*tk_AllocBitmapFromObj) (Tcl_Interp * interp, Tk_Window tkwin, Tcl_Obj * objPtr); 
    Tk_3DBorder (*tk_Alloc3DBorderFromObj) (Tcl_Interp * interp, Tk_Window tkwin, Tcl_Obj * objPtr); 
    XColor * (*tk_AllocColorFromObj) (Tcl_Interp * interp, Tk_Window tkwin, Tcl_Obj * objPtr); 
    Tk_Cursor (*tk_AllocCursorFromObj) (Tcl_Interp * interp, Tk_Window tkwin, Tcl_Obj * objPtr); 
    Tk_Font (*tk_AllocFontFromObj) (Tcl_Interp * interp, Tk_Window tkwin, Tcl_Obj * objPtr); 
    Tk_OptionTable (*tk_CreateOptionTable) (Tcl_Interp * interp, const Tk_OptionSpec * templatePtr); 
    void (*tk_DeleteOptionTable) (Tk_OptionTable optionTable); 
    void (*tk_Free3DBorderFromObj) (Tk_Window tkwin, Tcl_Obj * objPtr); 
    void (*tk_FreeBitmapFromObj) (Tk_Window tkwin, Tcl_Obj * objPtr); 
    void (*tk_FreeColorFromObj) (Tk_Window tkwin, Tcl_Obj * objPtr); 
    void (*tk_FreeConfigOptions) (char * recordPtr, Tk_OptionTable optionToken, Tk_Window tkwin); 
    void (*tk_FreeSavedOptions) (Tk_SavedOptions * savePtr); 
    void (*tk_FreeCursorFromObj) (Tk_Window tkwin, Tcl_Obj * objPtr); 
    void (*tk_FreeFontFromObj) (Tk_Window tkwin, Tcl_Obj * objPtr); 
    Tk_3DBorder (*tk_Get3DBorderFromObj) (Tk_Window tkwin, Tcl_Obj * objPtr); 
    int (*tk_GetAnchorFromObj) (Tcl_Interp * interp, Tcl_Obj * objPtr, Tk_Anchor * anchorPtr); 
    Pixmap (*tk_GetBitmapFromObj) (Tk_Window tkwin, Tcl_Obj * objPtr); 
    XColor * (*tk_GetColorFromObj) (Tk_Window tkwin, Tcl_Obj * objPtr); 
    Tk_Cursor (*tk_GetCursorFromObj) (Tk_Window tkwin, Tcl_Obj * objPtr); 
    Tcl_Obj * (*tk_GetOptionInfo) (Tcl_Interp * interp, char * recordPtr, Tk_OptionTable optionTable, Tcl_Obj * namePtr, Tk_Window tkwin); 
    Tcl_Obj * (*tk_GetOptionValue) (Tcl_Interp * interp, char * recordPtr, Tk_OptionTable optionTable, Tcl_Obj * namePtr, Tk_Window tkwin); 
    int (*tk_GetJustifyFromObj) (Tcl_Interp * interp, Tcl_Obj * objPtr, Tk_Justify * justifyPtr); 
    int (*tk_GetMMFromObj) (Tcl_Interp * interp, Tk_Window tkwin, Tcl_Obj * objPtr, double * doublePtr); 
    int (*tk_GetPixelsFromObj) (Tcl_Interp * interp, Tk_Window tkwin, Tcl_Obj * objPtr, int * intPtr); 
    int (*tk_GetReliefFromObj) (Tcl_Interp * interp, Tcl_Obj * objPtr, int * resultPtr); 
    int (*tk_GetScrollInfoObj) (Tcl_Interp * interp, int objc, Tcl_Obj *const objv[], double * dblPtr, int * intPtr); 
    int (*tk_InitOptions) (Tcl_Interp * interp, char * recordPtr, Tk_OptionTable optionToken, Tk_Window tkwin); 
    void (*tk_MainEx) (int argc, char ** argv, Tcl_AppInitProc * appInitProc, Tcl_Interp * interp); 
    void (*tk_RestoreSavedOptions) (Tk_SavedOptions * savePtr); 
    int (*tk_SetOptions) (Tcl_Interp * interp, char * recordPtr, Tk_OptionTable optionTable, int objc, Tcl_Obj *const objv[], Tk_Window tkwin, Tk_SavedOptions * savePtr, int * maskPtr); 
    void (*tk_InitConsoleChannels) (Tcl_Interp * interp); 
    int (*tk_CreateConsoleWindow) (Tcl_Interp * interp); 
    void (*tk_CreateSmoothMethod) (Tcl_Interp * interp, Tk_SmoothMethod * method); 
    void *reserved218;
    void *reserved219;
    int (*tk_GetDash) (Tcl_Interp * interp, const char * value, Tk_Dash * dash); 
    void (*tk_CreateOutline) (Tk_Outline * outline); 
    void (*tk_DeleteOutline) (Display * display, Tk_Outline * outline); 
    int (*tk_ConfigOutlineGC) (XGCValues * gcValues, Tk_Canvas canvas, Tk_Item * item, Tk_Outline * outline); 
    int (*tk_ChangeOutlineGC) (Tk_Canvas canvas, Tk_Item * item, Tk_Outline * outline); 
    int (*tk_ResetOutlineGC) (Tk_Canvas canvas, Tk_Item * item, Tk_Outline * outline); 
    int (*tk_CanvasPsOutline) (Tk_Canvas canvas, Tk_Item * item, Tk_Outline * outline); 
    void (*tk_SetTSOrigin) (Tk_Window tkwin, GC gc, int x, int y); 
    int (*tk_CanvasGetCoordFromObj) (Tcl_Interp * interp, Tk_Canvas canvas, Tcl_Obj * obj, double * doublePtr); 
    void (*tk_CanvasSetOffset) (Tk_Canvas canvas, GC gc, Tk_TSOffset * offset); 
    void (*tk_DitherPhoto) (Tk_PhotoHandle handle, int x, int y, int width, int height); 
    int (*tk_PostscriptBitmap) (Tcl_Interp * interp, Tk_Window tkwin, Tk_PostscriptInfo psInfo, Pixmap bitmap, int startX, int startY, int width, int height); 
    int (*tk_PostscriptColor) (Tcl_Interp * interp, Tk_PostscriptInfo psInfo, XColor * colorPtr); 
    int (*tk_PostscriptFont) (Tcl_Interp * interp, Tk_PostscriptInfo psInfo, Tk_Font font); 
    int (*tk_PostscriptImage) (Tk_Image image, Tcl_Interp * interp, Tk_Window tkwin, Tk_PostscriptInfo psinfo, int x, int y, int width, int height, int prepass); 
    void (*tk_PostscriptPath) (Tcl_Interp * interp, Tk_PostscriptInfo psInfo, double * coordPtr, int numPoints); 
    int (*tk_PostscriptStipple) (Tcl_Interp * interp, Tk_Window tkwin, Tk_PostscriptInfo psInfo, Pixmap bitmap); 
    double (*tk_PostscriptY) (double y, Tk_PostscriptInfo psInfo); 
    int (*tk_PostscriptPhoto) (Tcl_Interp * interp, Tk_PhotoImageBlock * blockPtr, Tk_PostscriptInfo psInfo, int width, int height); 
} TkStubs;




extern TkStubs *tkStubsPtr;

































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































#line 2042 "d:\\tcl-source\\tk8.3.1\\generic\\tkDecls.h"






#line 2049 "d:\\tcl-source\\tk8.3.1\\generic\\tkDecls.h"

#line 1360 "d:\\tcl-source\\tk8.3.1\\generic\\tk.h"






#line 1367 "d:\\tcl-source\\tk8.3.1\\generic\\tk.h"







    



    
#line 1380 "d:\\tcl-source\\tk8.3.1\\generic\\tk.h"
#line 27 "./../tk/pltk.h"




















extern __declspec(dllimport) int
Pltk_Init( Tcl_Interp *interp );



int
pltkMain(int argc, char **argv, char *RcFileName,
	 int (*AppInit)(Tcl_Interp *interp));





#line 61 "./../tk/pltk.h"
#line 53 "./../tk/pltkd.h"
#line 1 "./../../include\\pdf.h"





























































































































#line 54 "./../tk/pltkd.h"

#line 1 "d:\\tcl-source\\tk8.3.1\\generic\\tk.h"



































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































#line 56 "./../tk/pltkd.h"






typedef struct {
    Tk_Window	w;			
    Tcl_Interp	*interp;		
    PLINT	xold, yold;		
    unsigned	width, height;		
    int		exit_eventloop;		
    int		pass_thru;		
    char	*cmdbuf;		
    int		cmdbuf_len;		
    PLiodev	*iodev;			
    char	*updatecmd;		
    int	child_pid;		
    int		instr;			
    int		max_instr;		
    int		locate_mode;		
    PLGraphicsIn gin;			
} TkDev;

#line 81 "./../tk/pltkd.h"
#line 44 "./../tk/plserver.h"
#line 1 "./../tcl\\pltcl.h"













































































































#line 45 "./../tk/plserver.h"



typedef struct {
    char  *client;			
    PDFstrm *pdfs;			
    PLiodev *iodev;			
    int   nbytes;			
    int   at_bop, at_eop;		

    short xmin, xmax, ymin, ymax;	
    PLFLT xold, yold;			
} PLRDev;








int
plFrameCmd(ClientData clientData, Tcl_Interp *interp,
	   int argc, char **argv);




void
plr_start(PLRDev *plr);



int
plr_process(PLRDev *plr);

#line 82 "./../tk/plserver.h"
#line 190 "./../tk/plframe.c"
#line 1 "./../../include\\pltkwd.h"






































#line 1 "./../../include\\plplot.h"



















































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































#line 40 "./../../include\\pltkwd.h"
#line 1 "./../../include\\plstrm.h"











































































































#line 1 "./../../include\\pdf.h"





























































































































#line 109 "./../../include\\plstrm.h"



































































































































































































































































































































































































































































































































































































































#line 41 "./../../include\\pltkwd.h"







#line 1 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\sys/types.h"















#pragma once
#line 18 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\sys/types.h"






#line 25 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\sys/types.h"



typedef long time_t;

#line 31 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\sys/types.h"




typedef unsigned short _ino_t;		






typedef unsigned short ino_t;
#line 44 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\sys/types.h"
#line 45 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\sys/types.h"


#line 48 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\sys/types.h"







typedef unsigned int _dev_t;		
#line 57 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\sys/types.h"






typedef unsigned int dev_t;
#line 65 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\sys/types.h"
#line 66 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\sys/types.h"


#line 69 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\sys/types.h"




typedef long _off_t;			






typedef long off_t;
#line 82 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\sys/types.h"
#line 83 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\sys/types.h"


#line 86 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\sys/types.h"

#line 88 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\sys/types.h"
#line 49 "./../../include\\pltkwd.h"
#line 50 "./../../include\\pltkwd.h"
#line 51 "./../../include\\pltkwd.h"

#line 1 "d:\\tcl-source\\tk8.3.1\\generic\\tk.h"



































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































#line 53 "./../../include\\pltkwd.h"



#line 1 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xlib.h"























































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































#line 57 "./../../include\\pltkwd.h"
#line 1 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"























































































































































































































































































































































































































































































































































































































































































































































































































































































#line 58 "./../../include\\pltkwd.h"
#line 1 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/cursorfont.h"















































































#line 59 "./../../include\\pltkwd.h"
#line 1 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/keysym.h"





































#line 1 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/keysymdef.h"













































































































































































































































#line 239 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/keysymdef.h"










































































































































































































#line 442 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/keysymdef.h"
































































#line 507 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/keysymdef.h"





























#line 537 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/keysymdef.h"












































#line 582 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/keysymdef.h"











































































































































































































































































































































#line 914 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/keysymdef.h"
































































































































































































































































#line 39 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/keysym.h"
#line 40 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/keysym.h"
#line 60 "./../../include\\pltkwd.h"
#line 61 "./../../include\\pltkwd.h"







typedef struct {
    int		nstreams;		
    int		ixwd;			
    char	*displayName;		
    int		screen;			
    Display	*display;		
    Visual	*visual;		
    GC		gcXor;			
    Colormap	map;			
    unsigned	depth;			
    int		color;			
    int		ncol0;			
    int		ncol1;			
    XColor	cmap0[16];		
    XColor	cmap1[256];		
    XColor	fgcolor;		
    Tk_Cursor	xhair_cursor;		
} TkwDisplay;



typedef struct {
    TkwDisplay	*tkwd;			

    Window	window;			
    Pixmap	pixmap;			
    GC		gc;			
    XColor	curcolor;		

    long	event_mask;		
    int		exit_eventloop;		
    long	init_width;		
    long	init_height;		

    unsigned	width, height, border;	

    double	xscale_init;		
    double	yscale_init;		
    double	xscale;			
    double	yscale;			

    short	xlen, ylen;		

    int		write_to_window;	
    int		write_to_pixmap;	

    int		instr;			
    int		max_instr;		

    PLGraphicsIn gin;			

    int		locate_mode;		
    int		drawing_xhairs;		
    XPoint	xhair_x[2], xhair_y[2];	

    void (*MasterEH) (PLStream *, XEvent *);	
} TkwDev;











void
plD_open_tkw(PLStream *pls);



void
PLColor_to_XColor(PLColor *plcolor, XColor *xcolor);



void
PLColor_from_XColor(PLColor *plcolor, XColor *xcolor);


int
PLColor_from_XColor_Changed(PLColor *plcolor, XColor *xcolor);



int
pl_AreWeGrayscale(struct PlFrame *plf);



void
plX_setBGFG(PLStream *pls);



void
PLX_save_colormap(Display *display, Colormap map);





#line 173 "./../../include\\pltkwd.h"
#line 191 "./../tk/plframe.c"





#line 197 "./../tk/plframe.c"



















#line 217 "./../tk/plframe.c"


#line 1 "d:\\tcl-source\\tk8.3.1\\generic\\tkInt.h"


























#line 1 "d:\\tcl-source\\tk8.3.1\\generic\\tkPort.h"


























#line 1 "d:\\tcl-source\\tk8.3.1\\win\\tkWinPort.h"


















#line 1 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xlib.h"























































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































#line 20 "d:\\tcl-source\\tk8.3.1\\win\\tkWinPort.h"
#line 1 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/cursorfont.h"















































































#line 21 "d:\\tcl-source\\tk8.3.1\\win\\tkWinPort.h"
#line 1 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/keysym.h"





































#line 1 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/keysymdef.h"













































































































































































































































#line 239 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/keysymdef.h"










































































































































































































#line 442 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/keysymdef.h"
































































#line 507 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/keysymdef.h"





























#line 537 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/keysymdef.h"












































#line 582 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/keysymdef.h"











































































































































































































































































































































#line 914 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/keysymdef.h"
































































































































































































































































#line 39 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/keysym.h"
#line 40 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/keysym.h"
#line 22 "d:\\tcl-source\\tk8.3.1\\win\\tkWinPort.h"
#line 1 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xatom.h"














































































#line 80 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xatom.h"
#line 23 "d:\\tcl-source\\tk8.3.1\\win\\tkWinPort.h"
#line 1 "d:\\tcl-source\\tk8.3.1\\xlib\\X11/Xutil.h"























































































































































































































































































































































































































































































































































































































































































































































































































































































#line 24 "d:\\tcl-source\\tk8.3.1\\win\\tkWinPort.h"

#line 1 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\malloc.h"















#pragma once
#line 18 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\malloc.h"






#line 25 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\malloc.h"







#pragma pack(push,8)
#line 34 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\malloc.h"

















































#line 84 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\malloc.h"
























typedef struct _heapinfo {
        int * _pentry;
        size_t _size;
        int _useflag;
        } _HEAPINFO;

#line 115 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\malloc.h"










#line 126 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\malloc.h"

extern unsigned int _amblksiz;

#line 130 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\malloc.h"

#line 132 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\malloc.h"




 void *  __cdecl ckcalloc(size_t, size_t);
 void    __cdecl (tclStubsPtr->tcl_Free)((char*)(void *));
 void *  __cdecl (tclStubsPtr->tcl_Alloc)(size_t);
 void *  __cdecl realloc(void *, size_t);


#line 143 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\malloc.h"



void *          __cdecl _alloca(size_t);
 void *  __cdecl _expand(void *, size_t);

 size_t  __cdecl _get_sbh_threshold(void);
 int     __cdecl _set_sbh_threshold(size_t);
 int     __cdecl _heapadd(void *, size_t);
 int     __cdecl _heapchk(void);
 int     __cdecl _heapmin(void);
 int     __cdecl _heapset(unsigned int);
 int     __cdecl _heapwalk(_HEAPINFO *);
 size_t  __cdecl _heapused(size_t *, size_t *);
#line 158 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\malloc.h"
 size_t  __cdecl _msize(void *);




#line 164 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\malloc.h"



#line 168 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\malloc.h"

#line 170 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\malloc.h"



























#pragma pack(pop)
#line 199 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\malloc.h"

#line 201 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\malloc.h"
#line 26 "d:\\tcl-source\\tk8.3.1\\win\\tkWinPort.h"
#line 1 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\errno.h"
















#pragma once
#line 19 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\errno.h"






#line 26 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\errno.h"


















































#line 77 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\errno.h"







#line 85 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\errno.h"
 extern int errno;
#line 87 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\errno.h"



















































#line 139 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\errno.h"
#line 27 "d:\\tcl-source\\tk8.3.1\\win\\tkWinPort.h"




#line 1 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\limits.h"















#pragma once
#line 18 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\limits.h"






#line 25 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\limits.h"













#line 39 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\limits.h"
















#line 56 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\limits.h"





#line 62 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\limits.h"





#line 68 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\limits.h"








#line 77 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\limits.h"








#line 86 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\limits.h"
































#line 119 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\limits.h"
#line 32 "d:\\tcl-source\\tk8.3.1\\win\\tkWinPort.h"
#line 1 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\fcntl.h"















#pragma once
#line 18 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\fcntl.h"






#line 25 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\fcntl.h"























































#line 81 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\fcntl.h"

#line 83 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\fcntl.h"
#line 33 "d:\\tcl-source\\tk8.3.1\\win\\tkWinPort.h"
#line 1 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\io.h"














#pragma once
#line 17 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\io.h"






#line 24 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\io.h"







#pragma pack(push,8)
#line 33 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\io.h"



















































#line 85 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\io.h"






#line 92 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\io.h"







typedef unsigned long _fsize_t; 

#line 102 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\io.h"





struct _finddata_t {
    unsigned	attrib;
    time_t	time_create;	
    time_t	time_access;	
    time_t	time_write;
    _fsize_t	size;
    char	name[260];
};


struct _finddatai64_t {
    unsigned	attrib;
    time_t	time_create;	
    time_t	time_access;	
    time_t	time_write;
    __int64	size;
    char	name[260];
};
#line 126 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\io.h"


#line 129 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\io.h"



struct _wfinddata_t {
    unsigned	attrib;
    time_t	time_create;	
    time_t	time_access;	
    time_t	time_write;
    _fsize_t	size;
    wchar_t	name[260];
};


struct _wfinddatai64_t {
    unsigned	attrib;
    time_t	time_create;	
    time_t	time_access;	
    time_t	time_write;
    __int64	size;
    wchar_t	name[260];
};
#line 151 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\io.h"


#line 154 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\io.h"










#line 165 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\io.h"



 int __cdecl _access(const char *, int);
 int __cdecl _chmod(const char *, int);
 int __cdecl _chsize(int, long);
 int __cdecl _close(int);
 int __cdecl _commit(int);
 int __cdecl _creat(const char *, int);
 int __cdecl _dup(int);
 int __cdecl _dup2(int, int);
 int __cdecl _eof(int);
 long __cdecl _filelength(int);

 long __cdecl _findfirst(const char *, struct _finddata_t *);
 int __cdecl _findnext(long, struct _finddata_t *);
 int __cdecl _findclose(long);
#line 183 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\io.h"
 int __cdecl _isatty(int);
 int __cdecl _locking(int, int, long);
 long __cdecl _lseek(int, long, int);
 char * __cdecl _mktemp(char *);
 int __cdecl _open(const char *, int, ...);

 int __cdecl _pipe(int *, unsigned int, int);
#line 191 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\io.h"
 int __cdecl _read(int, void *, unsigned int);
 int __cdecl remove(const char *);
 int __cdecl rename(const char *, const char *);
 int __cdecl _setmode(int, int);
 int __cdecl _sopen(const char *, int, int, ...);
 long __cdecl _tell(int);
 int __cdecl _umask(int);
 int __cdecl _unlink(const char *);
 int __cdecl _write(int, const void *, unsigned int);


 __int64 __cdecl _filelengthi64(int);
 long __cdecl _findfirsti64(const char *, struct _finddatai64_t *);
 int __cdecl _findnexti64(long, struct _finddatai64_t *);
 __int64 __cdecl _lseeki64(int, __int64, int);
 __int64 __cdecl _telli64(int);
#line 208 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\io.h"






 int __cdecl _waccess(const wchar_t *, int);
 int __cdecl _wchmod(const wchar_t *, int);
 int __cdecl _wcreat(const wchar_t *, int);
 long __cdecl _wfindfirst(const wchar_t *, struct _wfinddata_t *);
 int __cdecl _wfindnext(long, struct _wfinddata_t *);
 int __cdecl _wunlink(const wchar_t *);
 int __cdecl _wrename(const wchar_t *, const wchar_t *);
 int __cdecl _wopen(const wchar_t *, int, ...);
 int __cdecl _wsopen(const wchar_t *, int, int, ...);
 wchar_t * __cdecl _wmktemp(wchar_t *);


 long __cdecl _wfindfirsti64(const wchar_t *, struct _wfinddatai64_t *);
 int __cdecl _wfindnexti64(long, struct _wfinddatai64_t *);
#line 229 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\io.h"


#line 232 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\io.h"
#line 233 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\io.h"


 long __cdecl _get_osfhandle(int);
 int __cdecl _open_osfhandle(long, int);

































 int __cdecl access(const char *, int);
 int __cdecl chmod(const char *, int);
 int __cdecl chsize(int, long);
 int __cdecl close(int);
 int __cdecl creat(const char *, int);
 int __cdecl dup(int);
 int __cdecl dup2(int, int);
 int __cdecl eof(int);
 long __cdecl filelength(int);
 int __cdecl isatty(int);
 int __cdecl locking(int, int, long);
 long __cdecl lseek(int, long, int);
 char * __cdecl mktemp(char *);
 int __cdecl open(const char *, int, ...);
 int __cdecl read(int, void *, unsigned int);
 int __cdecl setmode(int, int);
 int __cdecl sopen(const char *, int, int, ...);
 long __cdecl tell(int);
 int __cdecl umask(int);
 int __cdecl unlink(const char *);
 int __cdecl write(int, const void *, unsigned int);

#line 293 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\io.h"

#line 295 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\io.h"





#line 301 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\io.h"


#pragma pack(pop)
#line 305 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\io.h"

#line 307 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\io.h"
#line 34 "d:\\tcl-source\\tk8.3.1\\win\\tkWinPort.h"







#line 1 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\sys/stat.h"















#pragma once
#line 18 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\sys/stat.h"






#line 25 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\sys/stat.h"



#pragma pack(push,8)
#line 30 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\sys/stat.h"

















































#line 80 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\sys/stat.h"
















#line 97 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\sys/stat.h"






struct _stat {
	_dev_t st_dev;
	_ino_t st_ino;
	unsigned short st_mode;
	short st_nlink;
	short st_uid;
	short st_gid;
	_dev_t st_rdev;
	_off_t st_size;
	time_t st_atime;
	time_t st_mtime;
	time_t st_ctime;
	};





struct stat {
	_dev_t st_dev;
	_ino_t st_ino;
	unsigned short st_mode;
	short st_nlink;
	short st_uid;
	short st_gid;
	_dev_t st_rdev;
	_off_t st_size;
	time_t st_atime;
	time_t st_mtime;
	time_t st_ctime;
	};

#line 136 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\sys/stat.h"


struct _stati64 {
	_dev_t st_dev;
	_ino_t st_ino;
	unsigned short st_mode;
	short st_nlink;
	short st_uid;
	short st_gid;
	_dev_t st_rdev;
	__int64 st_size;
	time_t st_atime;
	time_t st_mtime;
	time_t st_ctime;
	};
#line 152 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\sys/stat.h"


#line 155 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\sys/stat.h"














 int __cdecl _fstat(int, struct _stat *);
 int __cdecl _stat(const char *, struct _stat *);


 int __cdecl _fstati64(int, struct _stati64 *);
 int __cdecl _stati64(const char *, struct _stati64 *);
#line 176 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\sys/stat.h"






 int __cdecl _wstat(const wchar_t *, struct _stat *);


 int __cdecl _wstati64(const wchar_t *, struct _stati64 *);
#line 187 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\sys/stat.h"


#line 190 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\sys/stat.h"
#line 191 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\sys/stat.h"




















 int __cdecl fstat(int, struct stat *);
 int __cdecl stat(const char *, struct stat *);
#line 214 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\sys/stat.h"

#line 216 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\sys/stat.h"







#pragma pack(pop)
#line 225 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\sys/stat.h"

#line 227 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\sys/stat.h"
#line 42 "d:\\tcl-source\\tk8.3.1\\win\\tkWinPort.h"
#line 43 "d:\\tcl-source\\tk8.3.1\\win\\tkWinPort.h"

#line 1 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\time.h"
















#pragma once
#line 19 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\time.h"






#line 26 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\time.h"







#pragma pack(push,8)
#line 35 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\time.h"

















































#line 85 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\time.h"






#line 92 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\time.h"









typedef long clock_t;

#line 104 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\time.h"



















struct tm {
        int tm_sec;     
        int tm_min;     
        int tm_hour;    
        int tm_mday;    
        int tm_mon;     
        int tm_year;    
        int tm_wday;    
        int tm_yday;    
        int tm_isdst;   
        };

#line 136 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\time.h"





































































#line 206 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\time.h"


 extern int _daylight;


 extern long _dstbias;


 extern long _timezone;


 extern char * _tzname[2];

#line 220 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\time.h"

#line 222 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\time.h"




 char * __cdecl asctime(const struct tm *);
 char * __cdecl ctime(const time_t *);
 clock_t __cdecl clock(void);
 double __cdecl difftime(time_t, time_t);
 struct tm * __cdecl gmtime(const time_t *);
 struct tm * __cdecl localtime(const time_t *);
 time_t __cdecl mktime(struct tm *);
 size_t __cdecl strftime(char *, size_t, const char *,
        const struct tm *);
 char * __cdecl _strdate(char *);
 char * __cdecl _strtime(char *);
 time_t __cdecl time(time_t *);




 void __cdecl _tzset(void);
#line 244 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\time.h"



unsigned __cdecl _getsystime(struct tm *);
unsigned __cdecl _setsystime(struct tm *, unsigned);












 
 wchar_t * __cdecl _wasctime(const struct tm *);
 wchar_t * __cdecl _wctime(const time_t *);
 size_t __cdecl wcsftime(wchar_t *, size_t, const wchar_t *,
        const struct tm *);
 wchar_t * __cdecl _wstrdate(wchar_t *);
 wchar_t * __cdecl _wstrtime(wchar_t *);


#line 271 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\time.h"
#line 272 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\time.h"






























#line 303 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\time.h"

 extern int daylight;
 extern long timezone;
 extern char * tzname[2];

#line 309 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\time.h"

 void __cdecl tzset(void);

#line 313 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\time.h"

#line 315 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\time.h"








#pragma pack(pop)
#line 325 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\time.h"

#line 327 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\time.h"
#line 45 "d:\\tcl-source\\tk8.3.1\\win\\tkWinPort.h"
#line 1 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\tchar.h"
















#pragma once
#line 19 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\tchar.h"





#pragma warning(disable:4514)       
 
#line 27 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\tchar.h"










#line 38 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\tchar.h"




















































































































































































































































































































































































































#line 443 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\tchar.h"






















































































































#line 562 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\tchar.h"

























#line 588 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\tchar.h"
































































































































































































































































































typedef char            _TCHAR;
typedef signed char     _TSCHAR;
typedef unsigned char   _TUCHAR;
typedef char            _TXCHAR;
typedef int             _TINT;

#line 883 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\tchar.h"



typedef char            TCHAR;
#line 888 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\tchar.h"

#line 890 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\tchar.h"






























































#line 953 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\tchar.h"
__inline size_t __cdecl _tclen(const char *_cpc) { return (_cpc,1); }
__inline void __cdecl _tccpy(char *_pc1, const char *_cpc2) { *_pc1 = *_cpc2; }
__inline int __cdecl _tccmp(const char *_cpc1, const char *_cpc2) { return (int) (((unsigned char)*_cpc1)-((unsigned char)*_cpc2)); }
#line 957 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\tchar.h"








































#line 998 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\tchar.h"
__inline char * __cdecl _strdec(const char * _cpc1, const char * _cpc2) { return (char *)(_cpc1,(_cpc2-1)); }
__inline char * __cdecl _strinc(const char * _pc) { return (char *)(_pc+1); }
__inline unsigned int __cdecl _strnextc(const char * _cpc) { return (unsigned int)*_cpc; }
__inline char * __cdecl _strninc(const char * _pc, size_t _sz) { return (char *)(_pc+_sz); }
__inline size_t __cdecl _strncnt( const char * _cpc, size_t _sz) { size_t len; len = strlen(_cpc); return (len>_sz) ? _sz : len; }
__inline char * __cdecl _strspnp( const char * _cpc1, const char * _cpc2) { return (*(_cpc1 += strspn(_cpc1,_cpc2))!='\0') ? (char*)_cpc1 : ((void *)0); }
#line 1005 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\tchar.h"


#line 1008 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\tchar.h"

#line 1010 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\tchar.h"













#line 1024 "C:\\Program Files\\DevStudio\\VC\\INCLUDE\\tchar.h"
#line 46 "d:\\tcl-source\\tk8.3.1\\win\\tkWinPort.h"



#line 50 "d:\\tcl-source\\tk8.3.1\\win\\tkWinPort.h"




#line 55 "d:\\tcl-source\\tk8.3.1\\win\\tkWinPort.h"




















#line 76 "d:\\tcl-source\\tk8.3.1\\win\\tkWinPort.h"












































struct timezone {
    int tz_minuteswest;
    int tz_dsttime;
};





#line 130 "d:\\tcl-source\\tk8.3.1\\win\\tkWinPort.h"
#line 28 "d:\\tcl-source\\tk8.3.1\\generic\\tkPort.h"






#line 35 "d:\\tcl-source\\tk8.3.1\\generic\\tkPort.h"

#line 37 "d:\\tcl-source\\tk8.3.1\\generic\\tkPort.h"
#line 28 "d:\\tcl-source\\tk8.3.1\\generic\\tkInt.h"
#line 29 "d:\\tcl-source\\tk8.3.1\\generic\\tkInt.h"





typedef struct TkColormap TkColormap;
typedef struct TkGrabEvent TkGrabEvent;
typedef struct TkpCursor_ *TkpCursor;
typedef struct TkRegion_ *TkRegion;
typedef struct TkStressedCmap TkStressedCmap;
typedef struct TkBindInfo_ *TkBindInfo;





typedef int (TkBindEvalProc) 

(ClientData clientData, Tcl_Interp *interp, XEvent *eventPtr, Tk_Window tkwin, KeySym keySym);
typedef void (TkBindFreeProc) (ClientData clientData);
typedef Window (TkClassCreateProc) 
(Tk_Window tkwin, Window parent, ClientData instanceData);
typedef void (TkClassGeometryProc) (ClientData instanceData);
typedef void (TkClassModalProc) 
(Tk_Window tkwin, XEvent *eventPtr);







typedef struct TkClassProcs {
    TkClassCreateProc *createProc;
				


    TkClassGeometryProc *geometryProc;
				


    TkClassModalProc *modalProc;
				


} TkClassProcs;







typedef struct TkCursor {
    Tk_Cursor cursor;		
    Display *display;		

    int resourceRefCount;	









    int objRefCount;		

    Tcl_HashTable *otherTable;	

    Tcl_HashEntry *hashPtr;	

    Tcl_HashEntry *idHashPtr;	

    struct TkCursor *nextPtr;	



} TkCursor;








typedef struct TkDisplay {
    Display *display;		
    struct TkDisplay *nextPtr;	
    char *name;			

    Time lastEventTime;		


    



    int borderInit;             
    Tcl_HashTable borderTable;  


    



    int atomInit;		

    Tcl_HashTable nameTable;	
    Tcl_HashTable atomTable;	

    



    int bindInfoStale;		


    unsigned int modeModMask;	


    unsigned int metaModMask;	


    unsigned int altModMask;	


    enum {LU_IGNORE, LU_CAPS, LU_SHIFT} lockUsage;
				
    int numModKeyCodes;		

    KeyCode *modKeyCodes;	




    


  
    int bitmapInit;             
    int bitmapAutoNumber;       
    Tcl_HashTable bitmapNameTable;    
                                

    Tcl_HashTable bitmapIdTable;

    Tcl_HashTable bitmapDataTable;    
                                




    



    int numIdSearches;          
    int numSlowSearches;

    



    int colorInit;              
    TkStressedCmap *stressPtr;	


    Tcl_HashTable colorNameTable;
                                

    Tcl_HashTable colorValueTable;
                                


    



    int cursorInit;             
    Tcl_HashTable cursorNameTable;
                                

    Tcl_HashTable cursorDataTable;
                                

    Tcl_HashTable cursorIdTable;
                                

    char cursorString[20];      
    Font cursorFont;		


    



    struct TkErrorHandler *errorPtr;
				


    int deleteCount;		




    



    struct TkWindowEvent *delayedMotionPtr;
				







    



    int focusDebug;             

    struct TkWindow *implicitWinPtr;
				




    struct TkWindow *focusPtr;	








    


    
    Tcl_HashTable gcValueTable; 

    Tcl_HashTable gcIdTable;     
    int gcInit;                 


    



    Tcl_HashTable maintainHashTable;
                                


    int geomInit;    

    


  
    Tcl_HashTable uidTable;     
    int uidInit;                

    



    struct TkWindow *grabWinPtr;
				

    struct TkWindow *eventualGrabWinPtr;
				


    struct TkWindow *buttonWinPtr;
				


    struct TkWindow *serverWinPtr;
				






    TkGrabEvent *firstGrabEventPtr;
				




    TkGrabEvent *lastGrabEventPtr;
				

    int grabFlags;		


    



    int gridInit;               
    Tcl_HashTable gridHashTable;


    



    int imageId;                

    



    int postCommandGeneration;  

    





    



    int packInit;              
    Tcl_HashTable packerHashTable;
                               

    

    



    int placeInit;              
    Tcl_HashTable masterTable;  

    Tcl_HashTable slaveTable;   


    



    struct TkSelectionInfo *selectionInfoPtr;
				



    Atom multipleAtom;		

    Atom incrAtom;		
    Atom targetsAtom;		
    Atom timestampAtom;		
    Atom textAtom;		
    Atom compoundTextAtom;	
    Atom applicationAtom;	
    Atom windowAtom;		
    Atom clipboardAtom;		

    Tk_Window clipWindow;	



    int clipboardActive;	

    struct TkMainInfo *clipboardAppPtr;
				
    struct TkClipboardTarget *clipTargetPtr;
				




    



    Tk_Window commTkwin;	



    Atom commProperty;		
    Atom registryProperty;	

    Atom appNameProperty;	


    



    struct TkIdStack *idStackPtr;
				


    XID (*defaultAllocProc) (Display *display);
				
    struct TkIdStack *windowStackPtr;
				


    int idCleanupScheduled;	



    



    int wmTracing;              




    struct TkWmInfo *firstWmPtr;  
    struct TkWmInfo *foregroundWmPtr;    
                                

    




    int destroyCount;		

    unsigned long lastDestroyRequest;
				




    



    TkColormap *cmapPtr;	


    






    Tcl_HashTable winTable;	

    int refCount;		




    


    int mouseButtonState;	

    int warpInProgress;
    Window warpWindow;
    int warpX;
    int warpY;
    int useInputMethods;	
} TkDisplay;







typedef struct TkErrorHandler {
    TkDisplay *dispPtr;		
    unsigned long firstRequest;	

    unsigned long lastRequest;	




    int error;			


    int request;		


    int minorCode;		


    Tk_ErrorProc *errorProc;	


    ClientData clientData;	

    struct TkErrorHandler *nextPtr;
				


} TkErrorHandler;










typedef struct TkEventHandler {
    unsigned long mask;		

    Tk_EventProc *proc;		

    ClientData clientData;	
    struct TkEventHandler *nextPtr;
				


} TkEventHandler;








typedef struct TkMainInfo {
    int refCount;		






    struct TkWindow *winPtr;	
    Tcl_Interp *interp;		
    Tcl_HashTable nameTable;	


    Tk_BindingTable bindingTable;
				

    TkBindInfo bindInfo;	

    struct TkFontInfo *fontInfoPtr;
				


    



    struct TkToplevelFocusInfo *tlFocusPtr;
				


    struct TkDisplayFocusInfo *displayFocusPtr;
				




    struct ElArray *optionRootPtr;
				


    Tcl_HashTable imageTable;	

    int strictMotif;		

    struct TkMainInfo *nextPtr;	

} TkMainInfo;







typedef struct {
    char *source;		
    int width, height;		
    int native;			

} TkPredefBitmap;












typedef struct TkWindow {

    



    Display *display;		
    TkDisplay *dispPtr;		

    int screenNum;		

    Visual *visual;		

    int depth;			
    Window window;		


    struct TkWindow *childList;	


    struct TkWindow *lastChildPtr;
				


    struct TkWindow *parentPtr;	




    struct TkWindow *nextPtr;	


    TkMainInfo *mainPtr;	







    



    char *pathName;		






    Tk_Uid nameUid;		

    Tk_Uid classUid;		


    







    XWindowChanges changes;	

    unsigned int dirtyChanges;	

    XSetWindowAttributes atts;	
    unsigned long dirtyAtts;	


    unsigned int flags;		



    



    TkEventHandler *handlerList;






    




    ClientData *tagPtr;		


    int numTags;		

    




    int optionLevel;		




    



    struct TkSelHandler *selHandlerList;
				



    



    Tk_GeomMgr *geomMgrPtr;	

    ClientData geomData;	
    int reqWidth, reqHeight;	



    int internalBorderWidth;	




    



    struct TkWmInfo *wmInfoPtr;	





    



    TkClassProcs *classProcsPtr;
    ClientData instanceData;

    



    struct TkWindowPrivate *privatePtr;
} TkWindow;







typedef struct TkStateMap {
    int numKey;			
    char *strKey;		
} TkStateMap;






typedef struct TkpClipMask {
    int type;			
    union {
	Pixmap pixmap;
	TkRegion region;
    } value;
} TkpClipMask;








extern TkDisplay *tkDisplayList;








































extern Tk_SmoothMethod		tkBezierSmoothMethod;
extern Tk_ImageType		tkBitmapImageType;
extern Tk_PhotoImageFormat	tkImgFmtGIF;
extern void			(*tkHandleEventProc) 
( XEvent* eventPtr);
extern Tk_PhotoImageFormat	tkImgFmtPPM;
extern TkMainInfo		*tkMainWindowList;
extern Tk_ImageType		tkPhotoImageType;
extern Tcl_HashTable		tkPredefBitmapTable;
extern int			tkSendSerial;

#line 1 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"





































extern __declspec(dllimport) TkWindow *	TkAllocWindow 
(TkDisplay * dispPtr, int screenNum, TkWindow * parentPtr);

extern __declspec(dllimport) void		TkBezierPoints 
(double control[], int numSteps, double * coordPtr);

extern __declspec(dllimport) void		TkBezierScreenPoints 

(Tk_Canvas canvas, double control[], int numSteps, XPoint * xPointPtr);

extern __declspec(dllimport) void		TkBindDeadWindow (TkWindow * winPtr);

extern __declspec(dllimport) void		TkBindEventProc 
(TkWindow * winPtr, XEvent * eventPtr);

extern __declspec(dllimport) void		TkBindFree (TkMainInfo * mainPtr);

extern __declspec(dllimport) void		TkBindInit (TkMainInfo * mainPtr);

extern __declspec(dllimport) void		TkChangeEventWindow 
(XEvent * eventPtr, TkWindow * winPtr);

extern __declspec(dllimport) int		TkClipInit 
(Tcl_Interp * interp, TkDisplay * dispPtr);

extern __declspec(dllimport) void		TkComputeAnchor 


(Tk_Anchor anchor, Tk_Window tkwin, int padX, int padY, int innerWidth, int innerHeight, int * xPtr, int * yPtr);

extern __declspec(dllimport) int		TkCopyAndGlobalEval 
(Tcl_Interp * interp, char * script);

extern __declspec(dllimport) unsigned long	TkCreateBindingProcedure 





( Tcl_Interp * interp, Tk_BindingTable bindingTable, ClientData object, char * eventString, TkBindEvalProc * evalProc, TkBindFreeProc * freeProc, ClientData clientData);

extern __declspec(dllimport) TkCursor *	TkCreateCursorFromData 


(Tk_Window tkwin, char * source, char * mask, int width, int height, int xHot, int yHot, XColor fg, XColor bg);

extern __declspec(dllimport) int		TkCreateFrame 

(ClientData clientData, Tcl_Interp * interp, int argc, char ** argv, int toplevel, char * appName);

extern __declspec(dllimport) Tk_Window	TkCreateMainWindow 
(Tcl_Interp * interp, char * screenName, char * baseName);

extern __declspec(dllimport) Time		TkCurrentTime (TkDisplay * dispPtr);

extern __declspec(dllimport) void		TkDeleteAllImages (TkMainInfo * mainPtr);

extern __declspec(dllimport) void		TkDoConfigureNotify (TkWindow * winPtr);

extern __declspec(dllimport) void		TkDrawInsetFocusHighlight 

( Tk_Window tkwin, GC gc, int width, Drawable drawable, int padding);

extern __declspec(dllimport) void		TkEventDeadWindow (TkWindow * winPtr);

extern __declspec(dllimport) void		TkFillPolygon 


(Tk_Canvas canvas, double * coordPtr, int numPoints, Display * display, Drawable drawable, GC gc, GC outlineGC);

extern __declspec(dllimport) int		TkFindStateNum 


(Tcl_Interp * interp, const char * option, const TkStateMap * mapPtr, const char * strKey);

extern __declspec(dllimport) char *		TkFindStateString 
( const TkStateMap * mapPtr, int numKey);

extern __declspec(dllimport) void		TkFocusDeadWindow (TkWindow * winPtr);

extern __declspec(dllimport) int		TkFocusFilterEvent 
(TkWindow * winPtr, XEvent * eventPtr);

extern __declspec(dllimport) TkWindow *	TkFocusKeyEvent 
(TkWindow * winPtr, XEvent * eventPtr);

extern __declspec(dllimport) void		TkFontPkgInit (TkMainInfo * mainPtr);

extern __declspec(dllimport) void		TkFontPkgFree (TkMainInfo * mainPtr);

extern __declspec(dllimport) void		TkFreeBindingTags (TkWindow * winPtr);

extern __declspec(dllimport) void		TkpFreeCursor (TkCursor * cursorPtr);

extern __declspec(dllimport) char *		TkGetBitmapData 


(Tcl_Interp * interp, char * string, char * fileName, int * widthPtr, int * heightPtr, int * hotXPtr, int * hotYPtr);

extern __declspec(dllimport) void		TkGetButtPoints 

(double p1[], double p2[], double width, int project, double m1[], double m2[]);

extern __declspec(dllimport) TkCursor *	TkGetCursorByName 
(Tcl_Interp * interp, Tk_Window tkwin, Tk_Uid string);

extern __declspec(dllimport) char *		TkGetDefaultScreenName 
( Tcl_Interp * interp, char * screenName);

extern __declspec(dllimport) TkDisplay *	TkGetDisplay (Display * display);

extern __declspec(dllimport) int		TkGetDisplayOf 

(Tcl_Interp * interp, int objc, Tcl_Obj *const objv[], Tk_Window * tkwinPtr);

extern __declspec(dllimport) TkWindow *	TkGetFocusWin (TkWindow * winPtr);

extern __declspec(dllimport) int		TkGetInterpNames 
(Tcl_Interp * interp, Tk_Window tkwin);

extern __declspec(dllimport) int		TkGetMiterPoints 

(double p1[], double p2[], double p3[], double width, double m1[], double m2[]);

extern __declspec(dllimport) void		TkGetPointerCoords 
(Tk_Window tkwin, int * xPtr, int * yPtr);

extern __declspec(dllimport) void		TkGetServerInfo 
(Tcl_Interp * interp, Tk_Window tkwin);

extern __declspec(dllimport) void		TkGrabDeadWindow (TkWindow * winPtr);

extern __declspec(dllimport) int		TkGrabState (TkWindow * winPtr);

extern __declspec(dllimport) void		TkIncludePoint 
(Tk_Item * itemPtr, double * pointPtr);

extern __declspec(dllimport) void		TkInOutEvents 


(XEvent * eventPtr, TkWindow * sourcePtr, TkWindow * destPtr, int leaveType, int enterType, Tcl_QueuePosition position);

extern __declspec(dllimport) void		TkInstallFrameMenu (Tk_Window tkwin);

extern __declspec(dllimport) char *		TkKeysymToString (KeySym keysym);

extern __declspec(dllimport) int		TkLineToArea 
(double end1Ptr[], double end2Ptr[], double rectPtr[]);

extern __declspec(dllimport) double		TkLineToPoint 
(double end1Ptr[], double end2Ptr[], double pointPtr[]);

extern __declspec(dllimport) int		TkMakeBezierCurve 


(Tk_Canvas canvas, double * pointPtr, int numPoints, int numSteps, XPoint xPoints[], double dblPoints[]);

extern __declspec(dllimport) void		TkMakeBezierPostscript 

( Tcl_Interp * interp, Tk_Canvas canvas, double * pointPtr, int numPoints);

extern __declspec(dllimport) void		TkOptionClassChanged (TkWindow * winPtr);

extern __declspec(dllimport) void		TkOptionDeadWindow (TkWindow * winPtr);

extern __declspec(dllimport) int		TkOvalToArea 
(double * ovalPtr, double * rectPtr);

extern __declspec(dllimport) double		TkOvalToPoint 
(double ovalPtr[], double width, int filled, double pointPtr[]);

extern __declspec(dllimport) int		TkpChangeFocus 
(TkWindow * winPtr, int force);

extern __declspec(dllimport) void		TkpCloseDisplay (TkDisplay * dispPtr);

extern __declspec(dllimport) void		TkpClaimFocus 
(TkWindow * topLevelPtr, int force);

extern __declspec(dllimport) void		TkpDisplayWarning 
(char * msg, char * title);

extern __declspec(dllimport) void		TkpGetAppName 
(Tcl_Interp * interp, Tcl_DString * name);

extern __declspec(dllimport) TkWindow *	TkpGetOtherWindow (TkWindow * winPtr);

extern __declspec(dllimport) TkWindow *	TkpGetWrapperWindow (TkWindow * winPtr);

extern __declspec(dllimport) int		TkpInit (Tcl_Interp * interp);

extern __declspec(dllimport) void		TkpInitializeMenuBindings 

( Tcl_Interp * interp, Tk_BindingTable bindingTable);

extern __declspec(dllimport) void		TkpMakeContainer (Tk_Window tkwin);

extern __declspec(dllimport) void		TkpMakeMenuWindow 
(Tk_Window tkwin, int transient);

extern __declspec(dllimport) Window		TkpMakeWindow 
(TkWindow * winPtr, Window parent);

extern __declspec(dllimport) void		TkpMenuNotifyToplevelCreate 
( Tcl_Interp * interp1, char * menuName);

extern __declspec(dllimport) TkDisplay *	TkpOpenDisplay (char * display_name);

extern __declspec(dllimport) int		TkPointerEvent 
(XEvent * eventPtr, TkWindow * winPtr);

extern __declspec(dllimport) int		TkPolygonToArea 
(double * polyPtr, int numPoints, double * rectPtr);

extern __declspec(dllimport) double		TkPolygonToPoint 
(double * polyPtr, int numPoints, double * pointPtr);

extern __declspec(dllimport) int		TkPositionInTree 
(TkWindow * winPtr, TkWindow * treePtr);

extern __declspec(dllimport) void		TkpRedirectKeyEvent 
(TkWindow * winPtr, XEvent * eventPtr);

extern __declspec(dllimport) void		TkpSetMainMenubar 
(Tcl_Interp * interp, Tk_Window tkwin, char * menuName);

extern __declspec(dllimport) int		TkpUseWindow 
(Tcl_Interp * interp, Tk_Window tkwin, char * string);

extern __declspec(dllimport) int		TkpWindowWasRecentlyDeleted 
(Window win, TkDisplay * dispPtr);

extern __declspec(dllimport) void		TkQueueEventForAllChildren 
( TkWindow * winPtr, XEvent * eventPtr);

extern __declspec(dllimport) int		TkReadBitmapFile 




(Display* display, Drawable d, const char* filename, unsigned int* width_return, unsigned int* height_return, Pixmap* bitmap_return, int* x_hot_return, int* y_hot_return);

extern __declspec(dllimport) int		TkScrollWindow 

(Tk_Window tkwin, GC gc, int x, int y, int width, int height, int dx, int dy, TkRegion damageRgn);

extern __declspec(dllimport) void		TkSelDeadWindow (TkWindow * winPtr);

extern __declspec(dllimport) void		TkSelEventProc 
(Tk_Window tkwin, XEvent * eventPtr);

extern __declspec(dllimport) void		TkSelInit (Tk_Window tkwin);

extern __declspec(dllimport) void		TkSelPropProc (XEvent * eventPtr);

extern __declspec(dllimport) void		TkSetClassProcs 

(Tk_Window tkwin, TkClassProcs * procs, ClientData instanceData);

extern __declspec(dllimport) void		TkSetWindowMenuBar 

(Tcl_Interp * interp, Tk_Window tkwin, char * oldMenuName, char * menuName);

extern __declspec(dllimport) KeySym		TkStringToKeysym (char * name);

extern __declspec(dllimport) int		TkThickPolyLineToArea 

(double * coordPtr, int numPoints, double width, int capStyle, int joinStyle, double * rectPtr);

extern __declspec(dllimport) void		TkWmAddToColormapWindows 
( TkWindow * winPtr);

extern __declspec(dllimport) void		TkWmDeadWindow (TkWindow * winPtr);

extern __declspec(dllimport) TkWindow *	TkWmFocusToplevel (TkWindow * winPtr);

extern __declspec(dllimport) void		TkWmMapWindow (TkWindow * winPtr);

extern __declspec(dllimport) void		TkWmNewWindow (TkWindow * winPtr);

extern __declspec(dllimport) void		TkWmProtocolEventProc 
(TkWindow * winPtr, XEvent * evenvPtr);

extern __declspec(dllimport) void		TkWmRemoveFromColormapWindows 
( TkWindow * winPtr);

extern __declspec(dllimport) void		TkWmRestackToplevel 
(TkWindow * winPtr, int aboveBelow, TkWindow * otherPtr);

extern __declspec(dllimport) void		TkWmSetClass (TkWindow * winPtr);

extern __declspec(dllimport) void		TkWmUnmapWindow (TkWindow * winPtr);

extern __declspec(dllimport) Tcl_Obj *	TkDebugBitmap 
(Tk_Window tkwin, char * name);

extern __declspec(dllimport) Tcl_Obj *	TkDebugBorder 
(Tk_Window tkwin, char * name);

extern __declspec(dllimport) Tcl_Obj *	TkDebugCursor 
(Tk_Window tkwin, char * name);

extern __declspec(dllimport) Tcl_Obj *	TkDebugColor 
(Tk_Window tkwin, char * name);

extern __declspec(dllimport) Tcl_Obj *	TkDebugConfig 
(Tcl_Interp * interp, Tk_OptionTable table);

extern __declspec(dllimport) Tcl_Obj *	TkDebugFont 
(Tk_Window tkwin, char * name);

extern __declspec(dllimport) int		TkFindStateNumObj 

(Tcl_Interp * interp, Tcl_Obj * optionPtr, const TkStateMap * mapPtr, Tcl_Obj * keyPtr);

extern __declspec(dllimport) Tcl_HashTable *	TkGetBitmapPredefTable (void);

extern __declspec(dllimport) TkDisplay *	TkGetDisplayList (void);

extern __declspec(dllimport) TkMainInfo *	TkGetMainInfoList (void);

extern __declspec(dllimport) int		TkGetWindowFromObj 

(Tcl_Interp * interp, Tk_Window tkwin, Tcl_Obj * objPtr, Tk_Window * windowPtr);

extern __declspec(dllimport) char *		TkpGetString 
(TkWindow * winPtr, XEvent * eventPtr, Tcl_DString * dsPtr);

extern __declspec(dllimport) void		TkpGetSubFonts 
(Tcl_Interp * interp, Tk_Font tkfont);

extern __declspec(dllimport) Tcl_Obj *	TkpGetSystemDefault 
(Tk_Window tkwin, char * dbName, char * className);

extern __declspec(dllimport) void		TkpMenuThreadInit (void);


extern __declspec(dllimport) void		TkClipBox 
(TkRegion rgn, XRectangle* rect_return);
#line 378 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"







extern __declspec(dllimport) TkRegion		TkCreateRegion (void);
#line 387 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"






extern __declspec(dllimport) void		TkDestroyRegion (TkRegion rgn);
#line 395 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"






extern __declspec(dllimport) void		TkIntersectRegion 
(TkRegion sra, TkRegion srcb, TkRegion dr_return);
#line 404 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"







extern __declspec(dllimport) int		TkRectInRegion 

(TkRegion rgn, int x, int y, unsigned int width, unsigned int height);
#line 415 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"








extern __declspec(dllimport) void		TkSetRegion 
(Display* display, GC gc, TkRegion rgn);
#line 426 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"







extern __declspec(dllimport) void		TkUnionRectWithRegion 
(XRectangle* rect, TkRegion src, TkRegion dr_return);
#line 436 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"



































































extern __declspec(dllimport) void		TkpDrawHighlightBorder 

(Tk_Window tkwin, GC fgGC, GC bgGC, int highlightWidth, Drawable drawable);

extern __declspec(dllimport) void		TkSetFocusWin 
(TkWindow * winPtr, int force);

extern __declspec(dllimport) void		TkpSetKeycodeAndState 
(Tk_Window tkwin, KeySym keySym, XEvent * eventPtr);

extern __declspec(dllimport) KeySym		TkpGetKeySym 
(TkDisplay * dispPtr, XEvent * eventPtr);

extern __declspec(dllimport) void		TkpInitKeymapInfo (TkDisplay * dispPtr);

typedef struct TkIntStubs {
    int magic;
    struct TkIntStubHooks *hooks;

    TkWindow * (*tkAllocWindow) (TkDisplay * dispPtr, int screenNum, TkWindow * parentPtr); 
    void (*tkBezierPoints) (double control[], int numSteps, double * coordPtr); 
    void (*tkBezierScreenPoints) (Tk_Canvas canvas, double control[], int numSteps, XPoint * xPointPtr); 
    void (*tkBindDeadWindow) (TkWindow * winPtr); 
    void (*tkBindEventProc) (TkWindow * winPtr, XEvent * eventPtr); 
    void (*tkBindFree) (TkMainInfo * mainPtr); 
    void (*tkBindInit) (TkMainInfo * mainPtr); 
    void (*tkChangeEventWindow) (XEvent * eventPtr, TkWindow * winPtr); 
    int (*tkClipInit) (Tcl_Interp * interp, TkDisplay * dispPtr); 
    void (*tkComputeAnchor) (Tk_Anchor anchor, Tk_Window tkwin, int padX, int padY, int innerWidth, int innerHeight, int * xPtr, int * yPtr); 
    int (*tkCopyAndGlobalEval) (Tcl_Interp * interp, char * script); 
    unsigned long (*tkCreateBindingProcedure) (Tcl_Interp * interp, Tk_BindingTable bindingTable, ClientData object, char * eventString, TkBindEvalProc * evalProc, TkBindFreeProc * freeProc, ClientData clientData); 
    TkCursor * (*tkCreateCursorFromData) (Tk_Window tkwin, char * source, char * mask, int width, int height, int xHot, int yHot, XColor fg, XColor bg); 
    int (*tkCreateFrame) (ClientData clientData, Tcl_Interp * interp, int argc, char ** argv, int toplevel, char * appName); 
    Tk_Window (*tkCreateMainWindow) (Tcl_Interp * interp, char * screenName, char * baseName); 
    Time (*tkCurrentTime) (TkDisplay * dispPtr); 
    void (*tkDeleteAllImages) (TkMainInfo * mainPtr); 
    void (*tkDoConfigureNotify) (TkWindow * winPtr); 
    void (*tkDrawInsetFocusHighlight) (Tk_Window tkwin, GC gc, int width, Drawable drawable, int padding); 
    void (*tkEventDeadWindow) (TkWindow * winPtr); 
    void (*tkFillPolygon) (Tk_Canvas canvas, double * coordPtr, int numPoints, Display * display, Drawable drawable, GC gc, GC outlineGC); 
    int (*tkFindStateNum) (Tcl_Interp * interp, const char * option, const TkStateMap * mapPtr, const char * strKey); 
    char * (*tkFindStateString) (const TkStateMap * mapPtr, int numKey); 
    void (*tkFocusDeadWindow) (TkWindow * winPtr); 
    int (*tkFocusFilterEvent) (TkWindow * winPtr, XEvent * eventPtr); 
    TkWindow * (*tkFocusKeyEvent) (TkWindow * winPtr, XEvent * eventPtr); 
    void (*tkFontPkgInit) (TkMainInfo * mainPtr); 
    void (*tkFontPkgFree) (TkMainInfo * mainPtr); 
    void (*tkFreeBindingTags) (TkWindow * winPtr); 
    void (*tkpFreeCursor) (TkCursor * cursorPtr); 
    char * (*tkGetBitmapData) (Tcl_Interp * interp, char * string, char * fileName, int * widthPtr, int * heightPtr, int * hotXPtr, int * hotYPtr); 
    void (*tkGetButtPoints) (double p1[], double p2[], double width, int project, double m1[], double m2[]); 
    TkCursor * (*tkGetCursorByName) (Tcl_Interp * interp, Tk_Window tkwin, Tk_Uid string); 
    char * (*tkGetDefaultScreenName) (Tcl_Interp * interp, char * screenName); 
    TkDisplay * (*tkGetDisplay) (Display * display); 
    int (*tkGetDisplayOf) (Tcl_Interp * interp, int objc, Tcl_Obj *const objv[], Tk_Window * tkwinPtr); 
    TkWindow * (*tkGetFocusWin) (TkWindow * winPtr); 
    int (*tkGetInterpNames) (Tcl_Interp * interp, Tk_Window tkwin); 
    int (*tkGetMiterPoints) (double p1[], double p2[], double p3[], double width, double m1[], double m2[]); 
    void (*tkGetPointerCoords) (Tk_Window tkwin, int * xPtr, int * yPtr); 
    void (*tkGetServerInfo) (Tcl_Interp * interp, Tk_Window tkwin); 
    void (*tkGrabDeadWindow) (TkWindow * winPtr); 
    int (*tkGrabState) (TkWindow * winPtr); 
    void (*tkIncludePoint) (Tk_Item * itemPtr, double * pointPtr); 
    void (*tkInOutEvents) (XEvent * eventPtr, TkWindow * sourcePtr, TkWindow * destPtr, int leaveType, int enterType, Tcl_QueuePosition position); 
    void (*tkInstallFrameMenu) (Tk_Window tkwin); 
    char * (*tkKeysymToString) (KeySym keysym); 
    int (*tkLineToArea) (double end1Ptr[], double end2Ptr[], double rectPtr[]); 
    double (*tkLineToPoint) (double end1Ptr[], double end2Ptr[], double pointPtr[]); 
    int (*tkMakeBezierCurve) (Tk_Canvas canvas, double * pointPtr, int numPoints, int numSteps, XPoint xPoints[], double dblPoints[]); 
    void (*tkMakeBezierPostscript) (Tcl_Interp * interp, Tk_Canvas canvas, double * pointPtr, int numPoints); 
    void (*tkOptionClassChanged) (TkWindow * winPtr); 
    void (*tkOptionDeadWindow) (TkWindow * winPtr); 
    int (*tkOvalToArea) (double * ovalPtr, double * rectPtr); 
    double (*tkOvalToPoint) (double ovalPtr[], double width, int filled, double pointPtr[]); 
    int (*tkpChangeFocus) (TkWindow * winPtr, int force); 
    void (*tkpCloseDisplay) (TkDisplay * dispPtr); 
    void (*tkpClaimFocus) (TkWindow * topLevelPtr, int force); 
    void (*tkpDisplayWarning) (char * msg, char * title); 
    void (*tkpGetAppName) (Tcl_Interp * interp, Tcl_DString * name); 
    TkWindow * (*tkpGetOtherWindow) (TkWindow * winPtr); 
    TkWindow * (*tkpGetWrapperWindow) (TkWindow * winPtr); 
    int (*tkpInit) (Tcl_Interp * interp); 
    void (*tkpInitializeMenuBindings) (Tcl_Interp * interp, Tk_BindingTable bindingTable); 
    void (*tkpMakeContainer) (Tk_Window tkwin); 
    void (*tkpMakeMenuWindow) (Tk_Window tkwin, int transient); 
    Window (*tkpMakeWindow) (TkWindow * winPtr, Window parent); 
    void (*tkpMenuNotifyToplevelCreate) (Tcl_Interp * interp1, char * menuName); 
    TkDisplay * (*tkpOpenDisplay) (char * display_name); 
    int (*tkPointerEvent) (XEvent * eventPtr, TkWindow * winPtr); 
    int (*tkPolygonToArea) (double * polyPtr, int numPoints, double * rectPtr); 
    double (*tkPolygonToPoint) (double * polyPtr, int numPoints, double * pointPtr); 
    int (*tkPositionInTree) (TkWindow * winPtr, TkWindow * treePtr); 
    void (*tkpRedirectKeyEvent) (TkWindow * winPtr, XEvent * eventPtr); 
    void (*tkpSetMainMenubar) (Tcl_Interp * interp, Tk_Window tkwin, char * menuName); 
    int (*tkpUseWindow) (Tcl_Interp * interp, Tk_Window tkwin, char * string); 
    int (*tkpWindowWasRecentlyDeleted) (Window win, TkDisplay * dispPtr); 
    void (*tkQueueEventForAllChildren) (TkWindow * winPtr, XEvent * eventPtr); 
    int (*tkReadBitmapFile) (Display* display, Drawable d, const char* filename, unsigned int* width_return, unsigned int* height_return, Pixmap* bitmap_return, int* x_hot_return, int* y_hot_return); 
    int (*tkScrollWindow) (Tk_Window tkwin, GC gc, int x, int y, int width, int height, int dx, int dy, TkRegion damageRgn); 
    void (*tkSelDeadWindow) (TkWindow * winPtr); 
    void (*tkSelEventProc) (Tk_Window tkwin, XEvent * eventPtr); 
    void (*tkSelInit) (Tk_Window tkwin); 
    void (*tkSelPropProc) (XEvent * eventPtr); 
    void (*tkSetClassProcs) (Tk_Window tkwin, TkClassProcs * procs, ClientData instanceData); 
    void (*tkSetWindowMenuBar) (Tcl_Interp * interp, Tk_Window tkwin, char * oldMenuName, char * menuName); 
    KeySym (*tkStringToKeysym) (char * name); 
    int (*tkThickPolyLineToArea) (double * coordPtr, int numPoints, double width, int capStyle, int joinStyle, double * rectPtr); 
    void (*tkWmAddToColormapWindows) (TkWindow * winPtr); 
    void (*tkWmDeadWindow) (TkWindow * winPtr); 
    TkWindow * (*tkWmFocusToplevel) (TkWindow * winPtr); 
    void (*tkWmMapWindow) (TkWindow * winPtr); 
    void (*tkWmNewWindow) (TkWindow * winPtr); 
    void (*tkWmProtocolEventProc) (TkWindow * winPtr, XEvent * evenvPtr); 
    void (*tkWmRemoveFromColormapWindows) (TkWindow * winPtr); 
    void (*tkWmRestackToplevel) (TkWindow * winPtr, int aboveBelow, TkWindow * otherPtr); 
    void (*tkWmSetClass) (TkWindow * winPtr); 
    void (*tkWmUnmapWindow) (TkWindow * winPtr); 
    Tcl_Obj * (*tkDebugBitmap) (Tk_Window tkwin, char * name); 
    Tcl_Obj * (*tkDebugBorder) (Tk_Window tkwin, char * name); 
    Tcl_Obj * (*tkDebugCursor) (Tk_Window tkwin, char * name); 
    Tcl_Obj * (*tkDebugColor) (Tk_Window tkwin, char * name); 
    Tcl_Obj * (*tkDebugConfig) (Tcl_Interp * interp, Tk_OptionTable table); 
    Tcl_Obj * (*tkDebugFont) (Tk_Window tkwin, char * name); 
    int (*tkFindStateNumObj) (Tcl_Interp * interp, Tcl_Obj * optionPtr, const TkStateMap * mapPtr, Tcl_Obj * keyPtr); 
    Tcl_HashTable * (*tkGetBitmapPredefTable) (void); 
    TkDisplay * (*tkGetDisplayList) (void); 
    TkMainInfo * (*tkGetMainInfoList) (void); 
    int (*tkGetWindowFromObj) (Tcl_Interp * interp, Tk_Window tkwin, Tcl_Obj * objPtr, Tk_Window * windowPtr); 
    char * (*tkpGetString) (TkWindow * winPtr, XEvent * eventPtr, Tcl_DString * dsPtr); 
    void (*tkpGetSubFonts) (Tcl_Interp * interp, Tk_Font tkfont); 
    Tcl_Obj * (*tkpGetSystemDefault) (Tk_Window tkwin, char * dbName, char * className); 
    void (*tkpMenuThreadInit) (void); 


#line 638 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"

    void (*tkClipBox) (TkRegion rgn, XRectangle* rect_return); 
#line 641 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"





#line 647 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"

    TkRegion (*tkCreateRegion) (void); 
#line 650 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"





#line 656 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"

    void (*tkDestroyRegion) (TkRegion rgn); 
#line 659 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"





#line 665 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"

    void (*tkIntersectRegion) (TkRegion sra, TkRegion srcb, TkRegion dr_return); 
#line 668 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"





#line 674 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"

    int (*tkRectInRegion) (TkRegion rgn, int x, int y, unsigned int width, unsigned int height); 
#line 677 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"





#line 683 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"

    void (*tkSetRegion) (Display* display, GC gc, TkRegion rgn); 
#line 686 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"





#line 692 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"

    void (*tkUnionRectWithRegion) (XRectangle* rect, TkRegion src, TkRegion dr_return); 
#line 695 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"





#line 701 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"

    void *reserved120;
#line 704 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"





#line 710 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"

    void *reserved121;
#line 713 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"





#line 719 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"

    void *reserved122;
#line 722 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"





#line 728 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"

    void *reserved123;
#line 731 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"





#line 737 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"

    void *reserved124;
#line 740 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"





#line 746 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"

    void *reserved125;
#line 749 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"





#line 755 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"

    void *reserved126;
#line 758 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"





#line 764 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"

    void *reserved127;
#line 767 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"





#line 773 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"

    void *reserved128;
#line 776 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"



    void *reserved129;


#line 783 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"

    void *reserved130;
#line 786 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"





#line 792 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"

    void *reserved131;
#line 795 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"



    void *reserved132;


#line 802 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"

    void *reserved133;
#line 805 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"





#line 811 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"

    void *reserved134;
#line 814 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"



    void (*tkpDrawHighlightBorder) (Tk_Window tkwin, GC fgGC, GC bgGC, int highlightWidth, Drawable drawable); 
    void (*tkSetFocusWin) (TkWindow * winPtr, int force); 
    void (*tkpSetKeycodeAndState) (Tk_Window tkwin, KeySym keySym, XEvent * eventPtr); 
    KeySym (*tkpGetKeySym) (TkDisplay * dispPtr, XEvent * eventPtr); 
    void (*tkpInitKeymapInfo) (TkDisplay * dispPtr); 
} TkIntStubs;




extern TkIntStubs *tkIntStubsPtr;







































































































































































































































































































































































































































































































































































































































































#line 1476 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"






#line 1483 "d:\\tcl-source\\tk8.3.1\\generic\\tkIntDecls.h"

#line 879 "d:\\tcl-source\\tk8.3.1\\generic\\tkInt.h"











extern __declspec(dllimport) int		Tcl_AfterCmd 
(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);
extern __declspec(dllimport) int		Tk_BellObjCmd 

(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
extern __declspec(dllimport) int		Tk_BindCmd 
(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);
extern __declspec(dllimport) int		Tk_BindtagsCmd 
(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);
extern __declspec(dllimport) int		Tk_ButtonObjCmd 

(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
extern __declspec(dllimport) int		Tk_CanvasObjCmd 
(ClientData clientData, Tcl_Interp *interp, int argc, Tcl_Obj *const objv[]);
extern __declspec(dllimport) int		Tk_CheckbuttonObjCmd 

(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
extern __declspec(dllimport) int		Tk_ClipboardCmd 
(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);
extern __declspec(dllimport) int              Tk_ChooseColorObjCmd 

( ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
extern __declspec(dllimport) int              Tk_ChooseDirectoryObjCmd 

( ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
extern __declspec(dllimport) int              Tk_ChooseFontObjCmd 

(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
extern __declspec(dllimport) int		Tk_DestroyObjCmd 

(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
extern __declspec(dllimport) int		Tk_EntryObjCmd 

(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
extern __declspec(dllimport) int		Tk_EventObjCmd 

(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
extern __declspec(dllimport) int		Tk_FileeventCmd 
(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);
extern __declspec(dllimport) int		Tk_FrameObjCmd 

(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
extern __declspec(dllimport) int		Tk_FocusObjCmd 

(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
extern __declspec(dllimport) int		Tk_FontObjCmd 

(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
extern __declspec(dllimport) int              Tk_GetOpenFileObjCmd 

(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
extern __declspec(dllimport) int              Tk_GetSaveFileObjCmd 

(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
extern __declspec(dllimport) int		Tk_GrabCmd 
(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);
extern __declspec(dllimport) int		Tk_GridCmd 
(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);
extern __declspec(dllimport) int		Tk_ImageObjCmd 

(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
extern __declspec(dllimport) int		Tk_LabelObjCmd 

(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
extern __declspec(dllimport) int		Tk_ListboxObjCmd 

(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
extern __declspec(dllimport) int		Tk_LowerObjCmd 

(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
extern __declspec(dllimport) int		Tk_MenubuttonObjCmd 

(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
extern __declspec(dllimport) int              Tk_MessageBoxObjCmd 

(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
extern __declspec(dllimport) int		Tk_MessageCmd 
(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);
extern __declspec(dllimport) int		Tk_OptionObjCmd 

(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
extern __declspec(dllimport) int		Tk_PackCmd 
(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);
extern __declspec(dllimport) int		Tk_PlaceCmd 
(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);
extern __declspec(dllimport) int		Tk_RadiobuttonObjCmd 

(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
extern __declspec(dllimport) int		Tk_RaiseObjCmd 

(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
extern __declspec(dllimport) int		Tk_ScaleObjCmd 

(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
extern __declspec(dllimport) int		Tk_ScrollbarCmd 
(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);
extern __declspec(dllimport) int		Tk_SelectionCmd 
(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);
extern __declspec(dllimport) int		Tk_SendCmd 
(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);
extern __declspec(dllimport) int		Tk_SendObjCmd 

(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
extern __declspec(dllimport) int		Tk_TextCmd 
(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);
extern __declspec(dllimport) int		Tk_TkObjCmd 

(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
extern __declspec(dllimport) int		Tk_TkwaitCmd 
(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);
extern __declspec(dllimport) int		Tk_ToplevelObjCmd 

(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
extern __declspec(dllimport) int		Tk_UpdateObjCmd 

(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
extern __declspec(dllimport) int		Tk_WinfoObjCmd 

(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[]);
extern __declspec(dllimport) int		Tk_WmCmd 
(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);

void	TkConsolePrint 
(Tcl_Interp *interp, int devId, char *buffer, long size);

extern __declspec(dllimport) void		TkEventInit (void);

extern __declspec(dllimport) int		TkCreateMenuCmd (Tcl_Interp *interp);
extern __declspec(dllimport) int		TkDeadAppCmd 
(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);

extern __declspec(dllimport) int		TkpTestembedCmd 
(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);
extern __declspec(dllimport) int		TkCanvasGetCoordObj 

(Tcl_Interp *interp, Tk_Canvas canvas, Tcl_Obj *obj, double *doublePtr);
extern __declspec(dllimport) int		TkCanvasDashParseProc 


( ClientData clientData, Tcl_Interp *interp, Tk_Window tkwin, const char *value, char *widgRec, int offset);
extern __declspec(dllimport) char *		TkCanvasDashPrintProc 


( ClientData clientData, Tk_Window tkwin, char *widgRec, int offset, Tcl_FreeProc **freeProcPtr);
extern __declspec(dllimport) int		TkGetDoublePixels 

(Tcl_Interp *interp, Tk_Window tkwin, const char *string, double *doublePtr);
extern __declspec(dllimport) int		TkOffsetParseProc 


( ClientData clientData, Tcl_Interp *interp, Tk_Window tkwin, const char *value, char *widgRec, int offset);
extern __declspec(dllimport) char *		TkOffsetPrintProc 


( ClientData clientData, Tk_Window tkwin, char *widgRec, int offset, Tcl_FreeProc **freeProcPtr);
extern __declspec(dllimport) int		TkOrientParseProc 


( ClientData clientData, Tcl_Interp *interp, Tk_Window tkwin, const char *value, char *widgRec, int offset);
extern __declspec(dllimport) char *		TkOrientPrintProc 


( ClientData clientData, Tk_Window tkwin, char *widgRec, int offset, Tcl_FreeProc **freeProcPtr);
extern __declspec(dllimport) int		TkPixelParseProc 


( ClientData clientData, Tcl_Interp *interp, Tk_Window tkwin, const char *value, char *widgRec, int offset);
extern __declspec(dllimport) char *		TkPixelPrintProc 


( ClientData clientData, Tk_Window tkwin, char *widgRec, int offset, Tcl_FreeProc **freeProcPtr);
extern __declspec(dllimport) int		TkPostscriptImage 


(Tcl_Interp *interp, Tk_Window tkwin, Tk_PostscriptInfo psInfo, XImage *ximage, int x, int y, int width, int height);
extern __declspec(dllimport) int		TkSmoothParseProc 

(ClientData clientData, Tcl_Interp *interp, Tk_Window tkwin, const char *value, char *recordPtr, int offset);
extern __declspec(dllimport) char *		TkSmoothPrintProc 

(ClientData clientData, Tk_Window tkwin, char *recordPtr, int offset, Tcl_FreeProc **freeProcPtr);
extern __declspec(dllimport) int		TkStateParseProc 


( ClientData clientData, Tcl_Interp *interp, Tk_Window tkwin, const char *value, char *widgRec, int offset);
extern __declspec(dllimport) char *		TkStatePrintProc 


( ClientData clientData, Tk_Window tkwin, char *widgRec, int offset, Tcl_FreeProc **freeProcPtr);
extern __declspec(dllimport) int		TkTileParseProc 


( ClientData clientData, Tcl_Interp *interp, Tk_Window tkwin, const char *value, char *widgRec, int offset);
extern __declspec(dllimport) char *		TkTilePrintProc 


( ClientData clientData, Tk_Window tkwin, char *widgRec, int offset, Tcl_FreeProc **freeProcPtr);




extern __declspec(dllimport) int		TkUnsupported1Cmd 
(ClientData clientData, Tcl_Interp *interp, int argc, char **argv);




#line 1098 "d:\\tcl-source\\tk8.3.1\\generic\\tkInt.h"
#line 220 "./../tk/plframe.c"
#line 1 "./../tk/tcpip.h"



















#line 1 "./../../include\\pdf.h"





























































































































#line 21 "./../tk/tcpip.h"
#line 1 "d:\\tcl-source\\tcl8.3.1\\generic\\tcl.h"
























































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































#line 22 "./../tk/tcpip.h"



int
pl_PacketSend		
(Tcl_Interp *interp, PLiodev *iodev, PDFstrm *pdfs);



int
pl_PacketReceive	
(Tcl_Interp *interp, PLiodev *iodev, PDFstrm *pdfs);

#line 36 "./../tk/tcpip.h"
#line 221 "./../tk/plframe.c"















extern int plplot_ccmap;





















typedef struct PlFrame {



    Tk_Window tkwin;            



    Display *display;           


    Tcl_Interp *interp;         


    Tcl_Command widgetCmd;      
    Tk_3DBorder border;         

    int borderWidth;            
    int relief;                 
    int width;                  

    int height;                 

    Tk_Cursor cursor;           
    int flags;                  






    int tkwin_initted;          
    PLStream *pls;              
    PLINT ipls;                 
    PLINT ipls_save;            

    PLRDev *plr;                
    char *plpr_cmd;             


    int active_plot;            
    int isActive;               
    


    PLDisplay pldis;            
    int prevWidth;              
    int prevHeight;             



    char *SaveFnam;             

    char **devDesc;             

    char **devName;             




    GC xorGC;                   
    XPoint pts[5];              
    int continue_draw;          
    Tk_Cursor xhair_cursor;     
    PLFLT xl, xr, yl, yr;       
    char *xScrollCmd;           


    char *yScrollCmd;           





    char *bopCmd;               
    char *eopCmd;               



    int xhairs;                 
    int drawing_xhairs;         
    XPoint xhair_x[2];          
    XPoint xhair_y[2];          



    int rband;                  
    int drawing_rband;          
    XPoint rband_pt[2];         
    int double_buffer;		
} PlFrame;

































static Tk_ConfigSpec configSpecs[] = {
    {TK_CONFIG_BOOLEAN, "-activeplot", (char *) ((void *)0), (char *) ((void *)0),
        "1", ((int) (size_t)&(((PlFrame *)0)->active_plot)), 8},
    {TK_CONFIG_BORDER, "-background", "background", "Background",
	"Black", ((int) (size_t)&(((PlFrame *)0)->border)),
	2},
    {TK_CONFIG_BORDER, "-background", "background", "Background",
	"White", ((int) (size_t)&(((PlFrame *)0)->border)),
	4},
    {TK_CONFIG_SYNONYM, "-bd", "borderWidth", (char *) ((void *)0),
	(char *) ((void *)0), 0, 0},
    {TK_CONFIG_SYNONYM, "-bg", "background", (char *) ((void *)0),
	(char *) ((void *)0), 0, 0},
    {TK_CONFIG_STRING, "-bopcmd", "bopcmd", "PgCommand",
        (char *) ((void *)0), ((int) (size_t)&(((PlFrame *)0)->bopCmd)), 1},
    {TK_CONFIG_PIXELS, "-borderwidth", "borderWidth", "BorderWidth",
	"0", ((int) (size_t)&(((PlFrame *)0)->borderWidth)), 0},
    {TK_CONFIG_BOOLEAN, "-doublebuffer", (char *) ((void *)0), (char *) ((void *)0),
        "0", ((int) (size_t)&(((PlFrame *)0)->double_buffer)), 8},
    {TK_CONFIG_ACTIVE_CURSOR, "-cursor", "cursor", "Cursor",
        ((char *) ((void *)0)), ((int) (size_t)&(((PlFrame *)0)->cursor)), 1},
    {TK_CONFIG_STRING, "-eopcmd", "eopcmd", "PgCommand",
        (char *) ((void *)0), ((int) (size_t)&(((PlFrame *)0)->eopCmd)), 1},
    {TK_CONFIG_PIXELS, "-height", "height", "Height",
        "250", ((int) (size_t)&(((PlFrame *)0)->height)), 0},
    {TK_CONFIG_RELIEF, "-relief", "relief", "Relief",
        "flat", ((int) (size_t)&(((PlFrame *)0)->relief)), 0},
    {TK_CONFIG_PIXELS, "-width", "width", "Width",
        "250", ((int) (size_t)&(((PlFrame *)0)->width)), 0},
    {TK_CONFIG_BOOLEAN, "-xhairs", (char *) ((void *)0), (char *) ((void *)0),
        "0", ((int) (size_t)&(((PlFrame *)0)->xhairs)), 8},
    {TK_CONFIG_BOOLEAN, "-rubberband", (char *) ((void *)0), (char *) ((void *)0),
        "0", ((int) (size_t)&(((PlFrame *)0)->rband)), 8},
	{TK_CONFIG_STRING, "-xscrollcommand", "xScrollCommand", "ScrollCommand",
        (char *) ((void *)0), ((int) (size_t)&(((PlFrame *)0)->xScrollCmd)), 1},
    {TK_CONFIG_STRING, "-yscrollcommand", "yScrollCommand", "ScrollCommand",
        (char *) ((void *)0), ((int) (size_t)&(((PlFrame *)0)->yScrollCmd)), 1},
    {TK_CONFIG_END, (char *) ((void *)0), (char *) ((void *)0), (char *) ((void *)0),
        (char *) ((void *)0), 0, 0}
};





int   plFrameCmd        (ClientData, Tcl_Interp *, int, char **);
void PlframeAtEop(Tcl_Interp *interp, register PlFrame *plFramePtr);
void PlframeAtBop(Tcl_Interp *interp, register PlFrame *plFramePtr);



static void  DestroyPlFrame     (char *);
static void  DisplayPlFrame     (ClientData);
static void  PlFrameInit        (ClientData);
static void  PlFrameFirstInit   (ClientData clientData);
static void  PlFrameConfigureEH (ClientData, XEvent *);
static void  PlFrameExposeEH    (ClientData, XEvent *);
static void  PlFrameMotionEH    (ClientData, register XEvent *);
static void  PlFrameEnterEH     (ClientData, register XEvent *);
static void  PlFrameLeaveEH     (ClientData, register XEvent *);
static void  PlFrameKeyEH       (ClientData, register XEvent *);
static void  PlFrameButtonPressEH    (ClientData    clientData, register XEvent *);
static int   PlFrameWidgetCmd   (ClientData, Tcl_Interp *, int, char **);
static int   ReadData           (ClientData, int);
static void  Install_cmap       (PlFrame *plFramePtr);



static int   Closelink          (Tcl_Interp *, PlFrame *, int, char **);
static int   Cmd                (Tcl_Interp *, PlFrame *, int, char **);
static int   ConfigurePlFrame   (Tcl_Interp *, PlFrame *, int, char **, int);
static int   Draw               (Tcl_Interp *, PlFrame *, int, char **);
static int   Info               (Tcl_Interp *, PlFrame *, int, char **);
static int   Openlink           (Tcl_Interp *, PlFrame *, int, char **);
static int   Orient             (Tcl_Interp *, PlFrame *, int, char **);
static int   Page               (Tcl_Interp *, PlFrame *, int, char **);
static int   NextPage           (Tcl_Interp *, PlFrame *, int, char **);
static int   Print              (Tcl_Interp *, PlFrame *, int, char **);
static int   Redraw             (Tcl_Interp *, PlFrame *, int, char **);
static int   Save               (Tcl_Interp *, PlFrame *, int, char **);
static int   View               (Tcl_Interp *, PlFrame *, int, char **);
static int   Scroll             (Tcl_Interp *, PlFrame *);
static int   report             (Tcl_Interp *, PlFrame *, int, char **);



static void  ActiveState        (register PlFrame *plFramePtr, int on);
static void  CreateXhairs       (PlFrame *);
static void  DestroyXhairs      (PlFrame *);
static void  DrawXhairs         (PlFrame *, int, int);
static void  UpdateXhairs       (PlFrame *);



static void  CreateRband        (PlFrame *);
static void  DestroyRband       (PlFrame *);
static void  DrawRband          (PlFrame *, int, int);
static void  UpdateRband        (PlFrame *);



static void  gbox               (PLFLT *, PLFLT *, PLFLT *, PLFLT *, char **);
static void  UpdateVScrollbar   (register PlFrame *);
static void  UpdateHScrollbar   (register PlFrame *);



















int
plFrameCmd(ClientData clientData, Tcl_Interp *interp,
           int argc, char **argv)
{
    
    
    Tk_Window tkwin;
    register PlFrame *plFramePtr;
    register PLRDev *plr;
    int i, ndev;

    ;

    if (argc < 2) {
        (tclStubsPtr->tcl_AppendResult)(interp, "wrong # args: should be \"",
                argv[0], " pathName ?options?\"", (char *) ((void *)0));
        return 1;
    }



    tkwin = Tk_CreateWindowFromPath(interp, Tk_MainWindow(interp), argv[1], (char *) ((void *)0));
    if (tkwin == ((void *)0)) {
        return 1;
    }
    Tk_SetClass(tkwin, "plframe");

    plFramePtr = (PlFrame *) (tclStubsPtr->tcl_Alloc)(sizeof(PlFrame));
    plFramePtr->tkwin = tkwin;
    plFramePtr->display = (((Tk_FakeWin *) (tkwin))->display);
    plFramePtr->interp = interp;
    plFramePtr->widgetCmd = 
      (tclStubsPtr->tcl_CreateCommand)(interp, (((Tk_FakeWin *) (plFramePtr->tkwin))->pathName),
			PlFrameWidgetCmd, (ClientData) plFramePtr, 
			(Tcl_CmdDeleteProc*) ((void *)0));
    plFramePtr->xorGC = ((void *)0);
    plFramePtr->border = ((void *)0);
    plFramePtr->cursor = 0L;
    plFramePtr->xhair_cursor = 0L;
    plFramePtr->flags = 0;
    plFramePtr->width  = (((Tk_FakeWin *) (plFramePtr->tkwin))->changes.width);
    plFramePtr->height = (((Tk_FakeWin *) (plFramePtr->tkwin))->changes.height);
    plFramePtr->prevWidth = 0;
    plFramePtr->prevHeight = 0;
    plFramePtr->continue_draw = 0;
    plFramePtr->ipls = 0;
    plFramePtr->ipls_save = 0;
    plFramePtr->tkwin_initted = 0;
    plFramePtr->plpr_cmd = ((void *)0);
    plFramePtr->bopCmd = ((void *)0);
    plFramePtr->eopCmd = ((void *)0);
    plFramePtr->xhairs = 0;
    plFramePtr->active_plot = 1;
    plFramePtr->isActive = 0;
    plFramePtr->drawing_xhairs = 0;
    plFramePtr->rband = 0;
    plFramePtr->drawing_rband = 0;
    plFramePtr->xScrollCmd = ((void *)0);
    plFramePtr->yScrollCmd = ((void *)0);
    plFramePtr->xl = 0.;
    plFramePtr->yl = 0.;
    plFramePtr->xr = 1.;
    plFramePtr->yr = 1.;
    plFramePtr->SaveFnam = ((void *)0);

    plFramePtr->plr = (PLRDev *) (tclStubsPtr->tcl_Alloc)(sizeof(PLRDev));
    plr = plFramePtr->plr;
    plr->pdfs = ((void *)0);
    plr->iodev = (PLiodev *) (tclStubsPtr->tcl_Alloc)(sizeof(PLiodev));
    plr_start(plr);



    c_plmkstrm(&plFramePtr->ipls);
    plgpls(&plFramePtr->pls);
    
    plFramePtr->pls->plFramePtr = plFramePtr;
        


    plFramePtr->xhair_cursor =
        Tk_GetCursor(plFramePtr->interp, plFramePtr->tkwin, "crosshair");



    plD_open_tkw(plFramePtr->pls);



    plFramePtr->devDesc = (char **) (tclStubsPtr->tcl_Alloc)(20 * sizeof(char **));
    plFramePtr->devName = (char **) (tclStubsPtr->tcl_Alloc)(20 * sizeof(char **));
    for (i = 0; i < 20; i++) {
        plFramePtr->devDesc[i] = ((void *)0);
        plFramePtr->devName[i] = ((void *)0);
    }
    ndev = 20;
    plgFileDevs(&plFramePtr->devDesc, &plFramePtr->devName, &ndev);



    Tk_CreateEventHandler(plFramePtr->tkwin, (1L<<17),
                          PlFrameConfigureEH, (ClientData) plFramePtr);

    Tk_CreateEventHandler(plFramePtr->tkwin, (1L<<15),
                          PlFrameExposeEH, (ClientData) plFramePtr);


    ActiveState(plFramePtr, 1);
    
    if (ConfigurePlFrame(interp, plFramePtr, argc-2, argv+2, 0) != 0) {
        Tk_DestroyWindow(plFramePtr->tkwin);
        return 1;
    }
    Tk_MakeWindowExist(plFramePtr->tkwin);
    PlFrameFirstInit((ClientData)plFramePtr);
    Tk_GeometryRequest(plFramePtr->tkwin,200,200);
    
    interp->result = (((Tk_FakeWin *) (plFramePtr->tkwin))->pathName);

    return 0;
}



















static int
PlFrameWidgetCmd(ClientData clientData, Tcl_Interp *interp,
                 int argc, char **argv)
{
    register PlFrame *plFramePtr = (PlFrame *) clientData;
    int result = 0;
    int length;
    char c;

    ;
    
    if (argc < 2) {
        (tclStubsPtr->tcl_AppendResult)(interp, "wrong # args: should be \"",
                argv[0], " option ?arg arg ...?\"", (char *) ((void *)0));
        return 1;
    }
    (tclStubsPtr->tcl_Preserve)((ClientData) plFramePtr);
    c = argv[1][0];
    length = strlen(argv[1]);



    if ((c == 'c') && (strncmp(argv[1], "cmd", length) == 0)) {
        result = Cmd(interp, plFramePtr, argc-2, argv+2);
    }



    else if ((c == 'c') && (strncmp(argv[1], "cget", length) == 0)
	    && (length >= 2)) {
	if (argc != 3) {
	    (tclStubsPtr->tcl_AppendResult)(interp, "wrong # args: should be \"",
		    argv[0], " cget option\"",
		    (char *) ((void *)0));
	    result = 1;
	    goto done;
	}
	result = Tk_ConfigureValue(interp, plFramePtr->tkwin, configSpecs,
		(char *) plFramePtr, argv[2], 0);
    } else if ((c == 'c') && (strncmp(argv[1], "configure", length) == 0)) {
        if (argc == 2) {
            result = Tk_ConfigureInfo(interp, plFramePtr->tkwin, configSpecs,
                    (char *) plFramePtr, (char *) ((void *)0), 0);
        }
        else if (argc == 3) {
            result = Tk_ConfigureInfo(interp, plFramePtr->tkwin, configSpecs,
                    (char *) plFramePtr, argv[2], 0);
        }
        else {
            result = ConfigurePlFrame(interp, plFramePtr, argc-2, argv+2,
                    1);
        }
    }



    else if ((c == 'c') && (strncmp(argv[1], "closelink", length) == 0)) {
        if (argc > 2) {
            (tclStubsPtr->tcl_AppendResult)(interp, "wrong # args: should be \"",
                    argv[0], (char *) ((void *)0));
            result = 1;
            goto done;
        }
        else {
            result = Closelink(interp, plFramePtr, argc-2, argv+2);
        }
    }



    else if ((c == 'd') && (strncmp(argv[1], "draw", length) == 0)) {
        if (argc == 2) {
            (tclStubsPtr->tcl_AppendResult)(interp, "wrong # args: should be \"",
                    argv[0], " draw op ?options?\"", (char *) ((void *)0));
            result = 1;
            goto done;
        }
        else {
            result = Draw(interp, plFramePtr, argc-2, argv+2);
        }
    }



    else if ((c == 'i') && (strncmp(argv[1], "info", length) == 0)) {
        result = Info(interp, plFramePtr, argc-2, argv+2);
    }



    else if ((c == 'n') && (strncmp(argv[1], "nextpage", length) == 0)) {
        result = NextPage(interp, plFramePtr, argc-2, argv+2);
    }



    else if ((c == 'o') && (strncmp(argv[1], "orient", length) == 0)) {
        result = Orient(interp, plFramePtr, argc-2, argv+2);
    }



    else if ((c == 'o') && (strncmp(argv[1], "openlink", length) == 0)) {
        if (argc < 3) {
            (tclStubsPtr->tcl_AppendResult)(interp, "wrong # args: should be \"",
                    argv[0], " option ?arg arg ...?\"", (char *) ((void *)0));
            result = 1;
            goto done;
        }
        else {
            result = Openlink(interp, plFramePtr, argc-2, argv+2);
        }
    }



    else if ((c == 'p') && (strncmp(argv[1], "page", length) == 0)) {
        result = Page(interp, plFramePtr, argc-2, argv+2);
    }



    else if ((c == 'p') && (strncmp(argv[1], "print", length) == 0)) {
        result = Print(interp, plFramePtr, argc-2, argv+2);
    }



    else if ((c == 'r') && (strncmp(argv[1], "redraw", length) == 0)) {
        if (argc > 2) {
            (tclStubsPtr->tcl_AppendResult)(interp, "wrong # args: should be \"",
                    argv[0], " redraw\"", (char *) ((void *)0));
            result = 1;
            goto done;
        }
        else {
            result = Redraw(interp, plFramePtr, argc-2, argv+2);
        }
    }



    else if ((c == 'r') && (strncmp(argv[1], "report", length) == 0)) {
        result = report( interp, plFramePtr, argc-2, argv+2 );
    }



    else if ((c == 's') && (strncmp(argv[1], "save", length) == 0)) {
        result = Save(interp, plFramePtr, argc-2, argv+2);
    }



    else if ((c == 'v') && (strncmp(argv[1], "view", length) == 0)) {
        result = View(interp, plFramePtr, argc-2, argv+2);
    }



    else if ((c == 'x') && (strncmp(argv[1], "xview", length) == 0)) {
	int count, type;
	double width = plFramePtr->xr - plFramePtr->xl;
	
	double fraction;

	type = Tk_GetScrollInfo(interp, argc, argv, &fraction, &count);
	switch (type) {
	  case 4:
	    result = 1;
	    goto done;
	  case 1:
	    plFramePtr->xl = fraction;
	    plFramePtr->xr = fraction + width;
	    break;
	  case 2:
	    plFramePtr->xl += count * width * .9;
	    plFramePtr->xr += count * width * .9;
	    break;
	  case 3:
	    plFramePtr->xl += count*width/50;
	    plFramePtr->xr += count*width/50;
	    break;
	}
	if(plFramePtr->xr > 1.0) {
	    plFramePtr->xr = 1.0;
	    plFramePtr->xl = 1.0 - width;
	} else if(plFramePtr->xl < 0.0) {
	    plFramePtr->xl = 0.0;
	    plFramePtr->xr = width;
	}
	Scroll(interp,plFramePtr);
    }



    else if ((c == 'y') && (strncmp(argv[1], "yview", length) == 0)) {
	int count, type;
	double height = plFramePtr->yr - plFramePtr->yl;
	
	double fraction;

	type = Tk_GetScrollInfo(interp, argc, argv, &fraction, &count);
	switch (type) {
	  case 4:
	    result = 1;
	    goto done;
	  case 1:
	    plFramePtr->yl = 1.0-fraction -height;
	    plFramePtr->yr = 1.0-fraction;
	    break;
	  case 2:
	    plFramePtr->yl -= count * height * .9;
	    plFramePtr->yr -= count * height * .9;
	    break;
	  case 3:
	    plFramePtr->yl -= count*height/50;
	    plFramePtr->yr -= count*height/50;
	    break;
	}
	if(plFramePtr->yr > 1.0) {
	    plFramePtr->yr = 1.0;
	    plFramePtr->yl = 1.0 - height;
	} else if(plFramePtr->yl < 0.0) {
	    plFramePtr->yl = 0.0;
	    plFramePtr->yr = height;
	}
	Scroll(interp,plFramePtr);
    }



    else {
        (tclStubsPtr->tcl_AppendResult)(interp, "bad option \"", argv[1],
         "\":  must be closelink, cmd, configure, draw, info, nextpage ",
         "openlink, orient, page, print, redraw, save, view, ",
         "xview, or yview", (char *) ((void *)0));

        result = 1;
    }

 done:
    (tclStubsPtr->tcl_Release)((ClientData) plFramePtr);
    return result;
}



















static void DestroyPlFrame(char* clientData)
{
    register PlFrame *plFramePtr = (PlFrame *) clientData;
    register PLRDev *plr = plFramePtr->plr;
    TkwDev *dev = (TkwDev *) plFramePtr->pls->dev;

    ;

    dev->exit_eventloop = 1;
    if (plFramePtr->border != ((void *)0)) {
        Tk_Free3DBorder(plFramePtr->border);
    }
    if (plFramePtr->plpr_cmd != ((void *)0)) {
        (tclStubsPtr->tcl_Free)((char *) plFramePtr->plpr_cmd);
    }
    if (plFramePtr->cursor != 0L) {
        Tk_FreeCursor(plFramePtr->display, plFramePtr->cursor);
    }
    if (plFramePtr->xhair_cursor != 0L) {
        Tk_FreeCursor(plFramePtr->display, plFramePtr->xhair_cursor);
    }
    if (plFramePtr->xorGC != ((void *)0)) {
        Tk_FreeGC(plFramePtr->display, plFramePtr->xorGC);
    }
    if (plFramePtr->yScrollCmd != ((void *)0)) {
        (tclStubsPtr->tcl_Free)((char *) plFramePtr->yScrollCmd);
    }
    if (plFramePtr->xScrollCmd != ((void *)0)) {
        (tclStubsPtr->tcl_Free)((char *) plFramePtr->xScrollCmd);
    }
    if (plFramePtr->SaveFnam != ((void *)0)) {
        (tclStubsPtr->tcl_Free)((char *) plFramePtr->SaveFnam);
    }
    if (plFramePtr->devDesc != ((void *)0)) {
        (tclStubsPtr->tcl_Free)((char *) plFramePtr->devDesc);
    }
    if (plFramePtr->devName != ((void *)0)) {
        (tclStubsPtr->tcl_Free)((char *) plFramePtr->devName);
    }



    pdf_close(plr->pdfs);
    (tclStubsPtr->tcl_Free)((char *) plFramePtr->plr->iodev);



    c_plsstrm( plFramePtr->ipls );
    c_plend1();



    (tclStubsPtr->tcl_Free)((char *) plFramePtr->plr);
    (tclStubsPtr->tcl_Free)((char *) plFramePtr);
}


















static void
PlFrameConfigureEH(ClientData clientData, register XEvent *eventPtr)
{
    register PlFrame *plFramePtr = (PlFrame *) clientData;
    register Tk_Window tkwin = plFramePtr->tkwin;

    ;

    switch (eventPtr->type) {

    case 22:
        pldebug("PLFrameConfigureEH", "ConfigureNotify\n");
        plFramePtr->flags |= 2;
        plFramePtr->width  = (((Tk_FakeWin *) (tkwin))->changes.width);
        plFramePtr->height = (((Tk_FakeWin *) (tkwin))->changes.height);
        if ((tkwin != ((void *)0)) && !(plFramePtr->flags & 1)) {
            (tclStubsPtr->tcl_DoWhenIdle)(DisplayPlFrame, (ClientData) plFramePtr);
            plFramePtr->flags |= 1;
            plFramePtr->flags |= 8|16;
        }
        break;

    case 17:
        pldebug("PLFrameConfigureEH", "DestroyNotify\n");
        (tclStubsPtr->tcl_DeleteCommand)(plFramePtr->interp, (((Tk_FakeWin *) (tkwin))->pathName));
        plFramePtr->tkwin = ((void *)0);
        if (plFramePtr->flags & 1) {
            (tclStubsPtr->tcl_CancelIdleCall)(DisplayPlFrame, (ClientData) plFramePtr);
        }
        (tclStubsPtr->tcl_EventuallyFree)((ClientData) plFramePtr, DestroyPlFrame);
        break;

    case 19:
        pldebug("PLFrameConfigureEH", "MapNotify\n");
        if (plFramePtr->flags & 1) {
            (tclStubsPtr->tcl_CancelIdleCall)(DisplayPlFrame, (ClientData) plFramePtr);
        }












#line 1032 "./../tk/plframe.c"
	
	
	(tclStubsPtr->tcl_DoWhenIdle)(PlFrameInit, (ClientData) plFramePtr);
	break;
    }
}






















static void
PlFrameExposeEH(ClientData clientData, register XEvent *eventPtr)
{
    register PlFrame *plFramePtr = (PlFrame *) clientData;
    XExposeEvent *event = (XExposeEvent *) eventPtr;
    register Tk_Window tkwin = plFramePtr->tkwin;

    ;

    pldebug("PLFrameExposeEH", "Expose\n");



    if ( ! (plFramePtr->drawing_xhairs || plFramePtr->drawing_rband) ) {
        int x0_old, x1_old, y0_old, y1_old, x0_new, x1_new, y0_new, y1_new;

        x0_old = plFramePtr->pldis.x;
        y0_old = plFramePtr->pldis.y;
        x1_old = x0_old + plFramePtr->pldis.width;
        y1_old = y0_old + plFramePtr->pldis.height;

        x0_new = event->x;
        y0_new = event->y;
        x1_new = x0_new + event->width;
        y1_new = y0_new + event->height;

        plFramePtr->pldis.x      = (((x0_old) < (x0_new)) ? (x0_old) : (x0_new));
        plFramePtr->pldis.y      = (((y0_old) < (y0_new)) ? (y0_old) : (y0_new));
        plFramePtr->pldis.width  = (((x1_old) > (x1_new)) ? (x1_old) : (x1_new)) - plFramePtr->pldis.x;
        plFramePtr->pldis.height = (((y1_old) > (y1_new)) ? (y1_old) : (y1_new)) - plFramePtr->pldis.y;
    }



    if (event->count == 0) {
        if ((tkwin != ((void *)0)) && !(plFramePtr->flags & 1)) {
            (tclStubsPtr->tcl_DoWhenIdle)(DisplayPlFrame, (ClientData) plFramePtr);
            plFramePtr->width  = (((Tk_FakeWin *) (tkwin))->changes.width);
            plFramePtr->height = (((Tk_FakeWin *) (tkwin))->changes.height);
            plFramePtr->flags |= 1;
        }
    }
}


















static void
PlFrameMotionEH(ClientData clientData, register XEvent *eventPtr)
{
    register PlFrame *plFramePtr = (PlFrame *) clientData;
    XMotionEvent *event = (XMotionEvent *) eventPtr;

    ;

    if (plFramePtr->drawing_xhairs) {
        DrawXhairs(plFramePtr, event->x, event->y);
    }
    if (plFramePtr->drawing_rband) {
        DrawRband(plFramePtr, event->x, event->y);
    }
}

















static void
PlFrameEnterEH(ClientData clientData, register XEvent *eventPtr)
{
    register PlFrame *plFramePtr = (PlFrame *) clientData;
    XCrossingEvent *crossingEvent = (XCrossingEvent *) eventPtr;

    ;

    if (plFramePtr->xhairs) {
        DrawXhairs(plFramePtr, crossingEvent->x, crossingEvent->y);
        plFramePtr->drawing_xhairs = 1;
    }
    if (plFramePtr->rband) {
        plFramePtr->drawing_rband = 1;
        UpdateRband(plFramePtr);
        DrawRband(plFramePtr, crossingEvent->x, crossingEvent->y);
    }
}

















static void
PlFrameLeaveEH(ClientData clientData, register XEvent *eventPtr)
{
    register PlFrame *plFramePtr = (PlFrame *) clientData;

    ;

    if (plFramePtr->drawing_xhairs) {
        UpdateXhairs(plFramePtr);
        plFramePtr->drawing_xhairs = 0;
    }
    if (plFramePtr->drawing_rband) {
        UpdateRband(plFramePtr);
        plFramePtr->drawing_rband = 0;
    }
}

static void
PlFrameButtonPressEH(ClientData clientData, register XEvent *eventPtr)
{
    register PlFrame *plFramePtr = (PlFrame *) clientData;
    XButtonEvent *event = (XButtonEvent *) eventPtr;
    register Tk_Window tkwin = plFramePtr->tkwin;
    
    switch (event->state) {
      case 256: 
        if(plFramePtr->drawing_rband) {
            UpdateRband(plFramePtr);
        }
        if(plFramePtr->rband)
            CreateRband(plFramePtr);
        break;
    }
}




















static void
PlFrameKeyEH(ClientData clientData, register XEvent *eventPtr)
{
    register PlFrame *plFramePtr = (PlFrame *) clientData;
    XKeyEvent *event = (XKeyEvent *) eventPtr;
    Tk_Window tkwin = plFramePtr->tkwin;
    Tcl_DString ds;
    KeySym keysym;
    char* string;
    XComposeStatus cs;

    ;
    string = TkpGetString((TkWindow*)tkwin,eventPtr,&ds);
    pldebug("PlFrameKeyEH", "Translation: %s\n", string);
    keysym = TkStringToKeysym(string);
    (tclStubsPtr->tcl_DStringFree)(&ds);
	
    if (((((unsigned)(keysym) >= 0xFFE1) && ((unsigned)(keysym) <= 0xFFEE)) || ((unsigned)(keysym) == 0xFF7E) || ((unsigned)(keysym) == 0xFF7F))) {
        eventPtr->type = 0;
    } else if ((((unsigned)(keysym) >= 0xFF50) && ((unsigned)(keysym) < 0xFF60))) {
        int x1, y1, dx = 0, dy = 0;
        int x0 = event->x, y0 = event->y;
        int xmin = 0, xmax = (((Tk_FakeWin *) (tkwin))->changes.width) - 1;
        int ymin = 0, ymax = (((Tk_FakeWin *) (tkwin))->changes.height) - 1;

        switch (keysym) {
        case 0xFF51:
            dx = -1;
            break;
        case 0xFF53:
            dx = 1;
            break;
        case 0xFF52:
            dy = -1;
            break;
        case 0xFF54:
            dy = 1;
            break;
        }

    

    

        if (event->state & 0x01) {
            dx *= 5;
            dy *= 5;
        }

    

        if (event->state & 0x02) {
            dx *= 5;
            dy *= 5;
        }

    

        if (event->state & 0x04) {
            dx *= 5;
            dy *= 5;
        }

    

        if (event->state & 0x08) {
            dx *= 5;
            dy *= 5;
        }

    

        x1 = x0 + dx;
        y1 = y0 + dy;

        if (x1 < xmin) dx = xmin - x0;
        if (y1 < ymin) dy = ymin - y0;
        if (x1 > xmax) dx = xmax - x0;
        if (y1 > ymax) dy = ymax - y0;

    

        XWarpPointer(plFramePtr->display, (((Tk_FakeWin *) (tkwin))->window),
                     0L, 0, 0, 0, 0, dx, dy);
        eventPtr->type = 0;
    }
}







static void
CreateXhairs(PlFrame *plFramePtr)
{
    register Tk_Window tkwin = plFramePtr->tkwin;
    Window root, child;
    int root_x, root_y, win_x, win_y;
    unsigned int mask;





    if (XQueryPointer(plFramePtr->display, (((Tk_FakeWin *) (tkwin))->window),
                      &root, &child, &root_x, &root_y, &win_x, &win_y,
                      &mask)) {

        






        if (win_x >= 0 && win_x < (((Tk_FakeWin *) (tkwin))->changes.width) &&
            win_y >= 0 && win_y < (((Tk_FakeWin *) (tkwin))->changes.height)) {
            DrawXhairs(plFramePtr, win_x, win_y);
            plFramePtr->drawing_xhairs = 1;
        }
    }

}







static void
DestroyXhairs(PlFrame *plFramePtr)
{



    UpdateXhairs(plFramePtr);
    plFramePtr->drawing_xhairs = 0;
}







static void
DrawXhairs(PlFrame *plFramePtr, int x0, int y0)
{
    register Tk_Window tkwin = plFramePtr->tkwin;
    int xmin = 0, xmax = (((Tk_FakeWin *) (tkwin))->changes.width) - 1;
    int ymin = 0, ymax = (((Tk_FakeWin *) (tkwin))->changes.height) - 1;

    if (plFramePtr->drawing_xhairs)
        UpdateXhairs(plFramePtr);

    plFramePtr->xhair_x[0].x = xmin; plFramePtr->xhair_x[0].y = y0;
    plFramePtr->xhair_x[1].x = xmax; plFramePtr->xhair_x[1].y = y0;

    plFramePtr->xhair_y[0].x = x0; plFramePtr->xhair_y[0].y = ymin;
    plFramePtr->xhair_y[1].x = x0; plFramePtr->xhair_y[1].y = ymax;

    UpdateXhairs(plFramePtr);
}







static void
UpdateXhairs(PlFrame *plFramePtr)
{
    register Tk_Window tkwin = plFramePtr->tkwin;

    XDrawLines((((Tk_FakeWin *) (tkwin))->display), (((Tk_FakeWin *) (tkwin))->window),
               plFramePtr->xorGC, plFramePtr->xhair_x, 2,
               0);

    XDrawLines((((Tk_FakeWin *) (tkwin))->display), (((Tk_FakeWin *) (tkwin))->window),
               plFramePtr->xorGC, plFramePtr->xhair_y, 2,
               0);
}







static void
CreateRband(PlFrame *plFramePtr)
{
    register Tk_Window tkwin = plFramePtr->tkwin;
    Window root, child;
    int root_x, root_y, win_x, win_y;
    unsigned int mask;



    if (XQueryPointer(plFramePtr->display, (((Tk_FakeWin *) (tkwin))->window),
                      &root, &child, &root_x, &root_y, &win_x, &win_y,
                      &mask)) {
        






        if (win_x >= 0 && win_x < (((Tk_FakeWin *) (tkwin))->changes.width) &&
            win_y >= 0 && win_y < (((Tk_FakeWin *) (tkwin))->changes.height)) {

        
            plFramePtr->rband_pt[0].x = win_x;
            plFramePtr->rband_pt[0].y = win_y;

            DrawRband(plFramePtr, win_x, win_y);
            plFramePtr->drawing_rband = 1;
        } else {
        



            plFramePtr->rband_pt[0].x = 0;
            plFramePtr->rband_pt[0].y = 0;

            DrawRband(plFramePtr, win_x, win_y);
            plFramePtr->drawing_rband = 1;
        }
    }

}







static void
DestroyRband(PlFrame *plFramePtr)
{



    UpdateRband(plFramePtr);
    plFramePtr->drawing_rband = 0;
}







static void
DrawRband(PlFrame *plFramePtr, int x0, int y0)
{
    register Tk_Window tkwin = plFramePtr->tkwin;
    int xmin = 0, xmax = (((Tk_FakeWin *) (tkwin))->changes.width) - 1;
    int ymin = 0, ymax = (((Tk_FakeWin *) (tkwin))->changes.height) - 1;



    if (plFramePtr->drawing_rband)
        UpdateRband(plFramePtr);

    plFramePtr->rband_pt[1].x = x0; plFramePtr->rband_pt[1].y = y0;

    UpdateRband(plFramePtr);
}







static void
UpdateRband(PlFrame *plFramePtr)
{
    register Tk_Window tkwin = plFramePtr->tkwin;

    XDrawLines((((Tk_FakeWin *) (tkwin))->display), (((Tk_FakeWin *) (tkwin))->window),
               plFramePtr->xorGC, plFramePtr->rband_pt, 2,
               0);
}


static void PlFrameFirstInit(ClientData clientData) {
    register PlFrame *plFramePtr = (PlFrame *) clientData;
    register Tk_Window tkwin = plFramePtr->tkwin;

    c_plsstrm(plFramePtr->ipls);
    c_plsdev("tkwin");

    plsxwin((((Tk_FakeWin *) (tkwin))->window));
    c_plspause(0);
    c_plinit();
    if (plplot_ccmap) {
	Install_cmap(plFramePtr);
    }
    c_plbop();
    
    plFramePtr->tkwin_initted = 1;
    plFramePtr->width  = (((Tk_FakeWin *) (tkwin))->changes.width);
    plFramePtr->height = (((Tk_FakeWin *) (tkwin))->changes.height);
    plFramePtr->prevWidth = plFramePtr->width;
    plFramePtr->prevHeight = plFramePtr->height;
}

















static void
PlFrameInit(ClientData clientData)
{
    register PlFrame *plFramePtr = (PlFrame *) clientData;
    register Tk_Window tkwin = plFramePtr->tkwin;



    plFramePtr->flags |= 1;
    plFramePtr->flags |= 8 | 16;



    DisplayPlFrame(clientData);

    if (plFramePtr->xhairs)
        CreateXhairs(plFramePtr);

    if (plFramePtr->rband)
        CreateRband(plFramePtr);
}


















static void
Install_cmap(PlFrame *plFramePtr)
{
    TkwDev *dev;



    dev = (TkwDev *) plFramePtr->pls->dev;
    Tk_SetWindowColormap(Tk_MainWindow(plFramePtr->interp), dev->tkwd->map);





















#line 1643 "./../tk/plframe.c"
}


















static void
DisplayPlFrame(ClientData clientData)
{
    register PlFrame *plFramePtr = (PlFrame *) clientData;
    register Tk_Window tkwin = plFramePtr->tkwin;

    ;



    if (plFramePtr->flags & 8) {
        UpdateVScrollbar(plFramePtr);
    }
    if (plFramePtr->flags & 16) {
        UpdateHScrollbar(plFramePtr);
    }
    plFramePtr->flags &= ~(8|16);



    if ((plFramePtr->tkwin == ((void *)0)) || ! (((Tk_FakeWin *) (tkwin))->flags & 1)) {
        plFramePtr->flags &= ~1;
        return;
    }



    if (plFramePtr->flags & 1) {
        plFramePtr->flags &= ~1;

    
    

        if (plFramePtr->flags & 2) {
            plFramePtr->flags |= 1;
            plFramePtr->flags &= ~2;
            (tclStubsPtr->tcl_DoWhenIdle)(DisplayPlFrame, clientData);
            return;
        }

    

	if ((plFramePtr->border != ((void *)0)) &&
	    (plFramePtr->relief != 0)) {
	    Tk_Draw3DRectangle(plFramePtr->tkwin, (((Tk_FakeWin *) (tkwin))->window),
			       plFramePtr->border, 0, 0, (((Tk_FakeWin *) (tkwin))->changes.width), (((Tk_FakeWin *) (tkwin))->changes.height),
			       plFramePtr->borderWidth, plFramePtr->relief);
	}

    

        if (plFramePtr->flags & 4) {
            plFramePtr->flags &= ~4;
            c_plsstrm(plFramePtr->ipls);
            pl_cmd(6, (void *) ((void *)0));
        }

    

        else if ((plFramePtr->width != plFramePtr->prevWidth) ||
                 (plFramePtr->height != plFramePtr->prevHeight)) {

            plFramePtr->pldis.width = plFramePtr->width;
            plFramePtr->pldis.height = plFramePtr->height;

            c_plsstrm(plFramePtr->ipls);
            pl_cmd(5, (void *) &(plFramePtr->pldis));
            plFramePtr->prevWidth = plFramePtr->width;
            plFramePtr->prevHeight = plFramePtr->height;
        }

    

        else {
            if (plFramePtr->drawing_xhairs) {
                





                XClearWindow(plFramePtr->display, (((Tk_FakeWin *) (tkwin))->window));
                #line 1745 "./../tk/plframe.c"
                ;
		c_plsstrm(plFramePtr->ipls);
                pl_cmd(4, ((void *)0));
            }
            else {
		c_plsstrm(plFramePtr->ipls);
                pl_cmd(4, (void *) &(plFramePtr->pldis));
            }

        

            plFramePtr->pldis.x      = (((Tk_FakeWin *) (tkwin))->changes.x) + (((Tk_FakeWin *) (tkwin))->changes.width);
            plFramePtr->pldis.y      = (((Tk_FakeWin *) (tkwin))->changes.y) + (((Tk_FakeWin *) (tkwin))->changes.height);
            plFramePtr->pldis.width  = -(((Tk_FakeWin *) (tkwin))->changes.width);
            plFramePtr->pldis.height = -(((Tk_FakeWin *) (tkwin))->changes.height);
        }

    

        if (plFramePtr->drawing_xhairs) {
            UpdateXhairs(plFramePtr);
        }

    

        if (plFramePtr->drawing_rband) {
            UpdateRband(plFramePtr);
        }
    }
}











static int
scol0(Tcl_Interp *interp, register PlFrame *plFramePtr,
      int i, char *col, int *p_changed)
{
    PLStream *pls = plFramePtr->pls;
    XColor xcol;
    PLINT r, g, b;

    if ( col == ((void *)0) ) {
        (tclStubsPtr->tcl_AppendResult)(interp, "color value not specified",
                         (char *) ((void *)0));
        return 1;
    }

    if ( ! XParseColor(plFramePtr->display,
                       (((Tk_FakeWin *) (plFramePtr->tkwin))->atts.colormap), col, &xcol)) {
        (tclStubsPtr->tcl_AppendResult)(interp, "Couldn't parse color ", col,
                         (char *) ((void *)0));
        return 1;
    }

    r = (unsigned) (xcol.red   & 0xFF00) >> 8;
    g = (unsigned) (xcol.green & 0xFF00) >> 8;
    b = (unsigned) (xcol.blue  & 0xFF00) >> 8;

    if ( (pls->cmap0[i].r != r) ||
         (pls->cmap0[i].g != g) ||
         (pls->cmap0[i].b != b) ) {

        pls->cmap0[i].r = r;
        pls->cmap0[i].g = g;
        pls->cmap0[i].b = b;
        *p_changed = 1;
    }

    return 0;
}







static int
scol1(Tcl_Interp *interp, register PlFrame *plFramePtr,
      int i, char *col, char *pos, char *rev, int *p_changed)
{
    PLStream *pls = plFramePtr->pls;
    XColor xcol;
    PLFLT h, l, s, r, g, b, p;
    int reverse;

    if ( col == ((void *)0) ) {
        (tclStubsPtr->tcl_AppendResult)(interp, "color value not specified",
                         (char *) ((void *)0));
        return 1;
    }

    if ( pos == ((void *)0) ) {
        (tclStubsPtr->tcl_AppendResult)(interp, "control point position not specified",
                         (char *) ((void *)0));
        return 1;
    }

    if ( rev == ((void *)0) ) {
        (tclStubsPtr->tcl_AppendResult)(interp, "interpolation sense not specified",
                         (char *) ((void *)0));
        return 1;
    }

    if ( ! XParseColor(plFramePtr->display,
                       (((Tk_FakeWin *) (plFramePtr->tkwin))->atts.colormap), col, &xcol)) {
        (tclStubsPtr->tcl_AppendResult)(interp, "Couldn't parse color ", col,
                         (char *) ((void *)0));
        return 1;
    }

    r = ((unsigned) (xcol.red   & 0xFF00) >> 8) / 255.0;
    g = ((unsigned) (xcol.green & 0xFF00) >> 8) / 255.0;
    b = ((unsigned) (xcol.blue  & 0xFF00) >> 8) / 255.0;

    plRGB_HLS(r, g, b, &h, &l, &s);

    p = atof(pos) / 100.0;
    reverse = atoi(rev);

    if ( (pls->cmap1cp[i].h != h) ||
         (pls->cmap1cp[i].l != l) ||
         (pls->cmap1cp[i].s != s) ||
         (pls->cmap1cp[i].p != p) ||
         (pls->cmap1cp[i].rev != reverse) ) {

        pls->cmap1cp[i].h = h;
        pls->cmap1cp[i].l = l;
        pls->cmap1cp[i].s = s;
        pls->cmap1cp[i].p = p;
        pls->cmap1cp[i].rev = reverse;
        *p_changed = 1;
    }
    return 0;
}









static int
Cmd(Tcl_Interp *interp, register PlFrame *plFramePtr,
    int argc, char **argv)
{
    PLStream *pls = plFramePtr->pls;
    int length;
    char c3;
    int result = 0;
    char cmdlist[] = "plgcmap0 plgcmap1 plscmap0 plscmap1 plscol0 plscol1";














    if (argc == 0) 
        return plTclCmd(cmdlist, interp, argc, argv);



    if ( ! plFramePtr->tkwin_initted) {
        (tclStubsPtr->tcl_VarEval)(plFramePtr->interp, "update", (char *) ((void *)0));
    }



    c_plsstrm(plFramePtr->ipls);

    c3 = argv[0][2];
    length = strlen(argv[0]);




    if ((c3 == 'g') && (strncmp(argv[0], "plgcmap0", length) == 0)) {
        int i;
        unsigned long plcolor;
        char str[10];

        sprintf(str, "%d", (int) pls->ncol0);
        (tclStubsPtr->tcl_AppendElement)(interp, str);
        for (i = 0; i < pls->ncol0; i++) {
            plcolor = ((pls->cmap0[i].r << 16) | 
                       (pls->cmap0[i].g << 8) |
                       (pls->cmap0[i].b));

            sprintf(str, "#%06lx", (plcolor & 0xFFFFFF));
            (tclStubsPtr->tcl_AppendElement)(interp, str);
        }
        result = 0;
    }





    else if ((c3 == 'g') && (strncmp(argv[0], "plgcmap1", length) == 0)) {
        int i;
        unsigned long plcolor;
        char str[10];
        PLFLT h, l, s, r, g, b;
        int r1, g1, b1;

        sprintf(str, "%d", (int) pls->ncp1);
        (tclStubsPtr->tcl_AppendElement)(interp, str);
        for (i = 0; i < pls->ncp1; i++) {
            h = pls->cmap1cp[i].h;
            l = pls->cmap1cp[i].l;
            s = pls->cmap1cp[i].s;

            plHLS_RGB(h, l, s, &r, &g, &b);

            r1 = (((0) > ((((255) < ((int) (256. * r))) ? (255) : ((int) (256. * r))))) ? (0) : ((((255) < ((int) (256. * r))) ? (255) : ((int) (256. * r)))));
            g1 = (((0) > ((((255) < ((int) (256. * g))) ? (255) : ((int) (256. * g))))) ? (0) : ((((255) < ((int) (256. * g))) ? (255) : ((int) (256. * g)))));
            b1 = (((0) > ((((255) < ((int) (256. * b))) ? (255) : ((int) (256. * b))))) ? (0) : ((((255) < ((int) (256. * b))) ? (255) : ((int) (256. * b)))));

            plcolor = ((r1 << 16) | (g1 << 8) | (b1));

            sprintf(str, "#%06lx", (plcolor & 0xFFFFFF));
            (tclStubsPtr->tcl_AppendElement)(interp, str);

            sprintf(str, "%02d", (int) (100*pls->cmap1cp[i].p));
            (tclStubsPtr->tcl_AppendElement)(interp, str);

            sprintf(str, "%01d", (int) (pls->cmap1cp[i].rev));
            (tclStubsPtr->tcl_AppendElement)(interp, str);
        }
        result = 0;
    }




    else if ((c3 == 's') && (strncmp(argv[0], "plscmap0", length) == 0)) {
        int i, changed = 1, ncol0 = atoi(argv[1]);
        char *col;

        if (ncol0 > 16 || ncol0 < 1) {
            (tclStubsPtr->tcl_AppendResult)(interp, "illegal number of colors in cmap0: ",
                             argv[1], (char *) ((void *)0));
            return 1;
        }

        pls->ncol0 = ncol0;
        for (i = 0; i < pls->ncol0; i++) {
            col = strtok(argv[2+i], " ");
            if ( col == ((void *)0) )
                break;

            if (scol0(interp, plFramePtr, i, col, &changed) != 0)
                return 1;

        }

        if (changed)
            plP_state(5);
    }




    else if ((c3 == 's') && (strncmp(argv[0], "plscmap1", length) == 0)) {
        int i, changed = 1, ncp1 = atoi(argv[1]);
        char *col, *pos, *rev;

        if (ncp1 > 32 || ncp1 < 1) {
            (tclStubsPtr->tcl_AppendResult)(interp,
                             "illegal number of control points in cmap1: ",
                             argv[1], (char *) ((void *)0));
            return 1;
        }

        col = strtok(argv[2], " ");
        pos = strtok(((void *)0), " ");
        rev = strtok(((void *)0), " ");
        for (i = 0; i < ncp1; i++) {
            if ( col == ((void *)0) )
                break;

            if (scol1(interp, plFramePtr,
                      i, col, pos, rev, &changed) != 0)
                return 1;

            col = strtok(((void *)0), " ");
            pos = strtok(((void *)0), " ");
            rev = strtok(((void *)0), " ");
        }

        if (changed) {
            plsc->ncp1 = ncp1;
            plcmap1_calc();
        }
    }




    else if ((c3 == 's') && (strncmp(argv[0], "plscol0", length) == 0)) {
        int i = atoi(argv[1]), changed = 1;

        if (i > pls->ncol0 || i < 0) {
            (tclStubsPtr->tcl_AppendResult)(interp, "illegal color number in cmap0: ",
                             argv[1], (char *) ((void *)0));
            return 1;
        }

        if (scol0(interp, plFramePtr, i, argv[2], &changed) != 0)
            return 1;

        if (changed)
            plP_state(5);
    }




    else if ((c3 == 's') && (strncmp(argv[0], "plscol1", length) == 0)) {
        int i = atoi(argv[1]), changed = 1;

        if (i > pls->ncp1 || i < 0) {
            (tclStubsPtr->tcl_AppendResult)(interp, "illegal control point number in cmap1: ",
                             argv[1], (char *) ((void *)0));
            return 1;
        }

        if (scol1(interp, plFramePtr,
                  i, argv[2], argv[3], argv[4], &changed) != 0)
            return 1;

        if (changed) 
            plcmap1_calc();
    }

























    else 
        result = plTclCmd(cmdlist, interp, argc, argv);

    c_plflush();
    return result;
}

static void ActiveState(register PlFrame *plFramePtr, int on) {
    if(on ) {
        if(!plFramePtr->isActive) {
            Tk_CreateEventHandler(plFramePtr->tkwin, (1L<<2),
                                  PlFrameButtonPressEH, (ClientData) plFramePtr);

            Tk_CreateEventHandler(plFramePtr->tkwin, (1L<<6),
                                  PlFrameMotionEH, (ClientData) plFramePtr);
        
            Tk_CreateEventHandler(plFramePtr->tkwin, (1L<<4),
                                  PlFrameEnterEH, (ClientData) plFramePtr);
        
            Tk_CreateEventHandler(plFramePtr->tkwin, (1L<<5),
                                  PlFrameLeaveEH, (ClientData) plFramePtr);
        
            Tk_CreateEventHandler(plFramePtr->tkwin, (1L<<0),
                                  PlFrameKeyEH, (ClientData) plFramePtr);
        
        
            Tk_DefineCursor(plFramePtr->tkwin, plFramePtr->xhair_cursor);
        }
    } else {
        if(plFramePtr->isActive) {
            Tk_DeleteEventHandler(plFramePtr->tkwin, (1L<<2),
                                  PlFrameButtonPressEH, (ClientData) plFramePtr);
            Tk_DeleteEventHandler(plFramePtr->tkwin, (1L<<6),
                                  PlFrameMotionEH, (ClientData) plFramePtr);
    
            Tk_DeleteEventHandler(plFramePtr->tkwin, (1L<<4),
                                  PlFrameEnterEH, (ClientData) plFramePtr);
    
            Tk_DeleteEventHandler(plFramePtr->tkwin, (1L<<5),
                                  PlFrameLeaveEH, (ClientData) plFramePtr);
            Tk_DeleteEventHandler(plFramePtr->tkwin, (1L<<0),
                                  PlFrameKeyEH, (ClientData) plFramePtr);
        
        
            Tk_DefineCursor(plFramePtr->tkwin, plFramePtr->cursor);
        }
    }
}























static int
ConfigurePlFrame(Tcl_Interp *interp, register PlFrame *plFramePtr,
                 int argc, char **argv, int flags)
{
    register Tk_Window tkwin = plFramePtr->tkwin;
    PLStream *pls = plFramePtr->pls;
    TkwDev *dev = (TkwDev *) pls->dev;
    TkwDisplay *tkwd = (TkwDisplay *) dev->tkwd;
    XGCValues gcValues;
    unsigned long mask;
    int need_redisplay = 0;












    ;

    if (Tk_ConfigureWidget(interp, tkwin, configSpecs,
            argc, argv, (char *) plFramePtr, flags) != 0) {
        return 1;
    }







    c_plsstrm(plFramePtr->ipls);
    if(PLColor_from_XColor_Changed(&pls->cmap0[0], 
      Tk_3DBorderColor(plFramePtr->border))) {
    	need_redisplay = 1;
    	
        plFramePtr->flags |= 4;
    }
    plX_setBGFG(pls);

    Tk_SetWindowBackground(tkwin, tkwd->cmap0[0].pixel);
    Tk_SetWindowBorder(tkwin, tkwd->cmap0[0].pixel);

    

    gcValues.background = tkwd->cmap0[0].pixel;
    gcValues.foreground = 0xFF;
    gcValues.function = 0x6;
    mask = (1L<<2) | (1L<<3) | (1L<<0);

    if (plFramePtr->xorGC != ((void *)0))
        Tk_FreeGC(plFramePtr->display, plFramePtr->xorGC);

    plFramePtr->xorGC = Tk_GetGC(plFramePtr->tkwin, mask, &gcValues);



    Tk_SetInternalBorder(tkwin, plFramePtr->borderWidth);
    if ((plFramePtr->width > 0) || (plFramePtr->height > 0)) {
        Tk_GeometryRequest(tkwin, plFramePtr->width, plFramePtr->height);
        if ((plFramePtr->width != plFramePtr->prevWidth) ||
            (plFramePtr->height != plFramePtr->prevHeight))
            need_redisplay = 1;
    }



    if ((((Tk_FakeWin *) (tkwin))->flags & 1)) {
        if (plFramePtr->xhairs) {
            if (! plFramePtr->drawing_xhairs)
                CreateXhairs(plFramePtr);
        }
        else {
            if (plFramePtr->drawing_xhairs)
                DestroyXhairs(plFramePtr);
        }
    }



    if ((((Tk_FakeWin *) (tkwin))->flags & 1)) {
        if (plFramePtr->rband) {
            if (! plFramePtr->drawing_rband)
                CreateRband(plFramePtr);
        }
        else {
            if (plFramePtr->drawing_rband)
                DestroyRband(plFramePtr);
        }
    }

    ActiveState(plFramePtr,plFramePtr->active_plot);
        
    if (!pls->nopixmap) {
	
	if(plFramePtr->double_buffer != pls->db) {
	    pls->db = plFramePtr->double_buffer;
	    
	    dev->write_to_window = !pls->db;
	}
    } else {
	plFramePtr->double_buffer = 0;
    }
	

    if (need_redisplay && (((Tk_FakeWin *) (tkwin))->flags & 1)
            && !(plFramePtr->flags & 1)) {
        (tclStubsPtr->tcl_DoWhenIdle)(DisplayPlFrame, (ClientData) plFramePtr);
        plFramePtr->flags |= 1;
        plFramePtr->flags |= 8|16;
    }

    return 0;
}








static int
Draw(Tcl_Interp *interp, register PlFrame *plFramePtr,
     int argc, char **argv)
{
    register Tk_Window tkwin = plFramePtr->tkwin;
    int result = 0;
    char c = argv[0][0];
    int length = strlen(argv[0]);



    if ( ! plFramePtr->tkwin_initted) {
        (tclStubsPtr->tcl_VarEval)(plFramePtr->interp, "update", (char *) ((void *)0));
    }



    if ((c == 'i') && (strncmp(argv[0], "init", length) == 0)) {
        Tk_DefineCursor(tkwin, plFramePtr->xhair_cursor);
    }



    else if ((c == 'e') && (strncmp(argv[0], "end", length) == 0)) {

        Tk_DefineCursor(tkwin, plFramePtr->cursor);
        if (plFramePtr->continue_draw) {
            XDrawLines((((Tk_FakeWin *) (tkwin))->display), (((Tk_FakeWin *) (tkwin))->window),
                       plFramePtr->xorGC, plFramePtr->pts, 5,
                       0);
            {(((Tk_FakeWin *) (tkwin))->display)->request++;};
        }

        plFramePtr->continue_draw = 0;
    }




    else if ((c == 'r') && (strncmp(argv[0], "rect", length) == 0)) {
        if (argc < 5) {
            (tclStubsPtr->tcl_AppendResult)(interp, "wrong # args: should be \"",
                             " draw rect x0 y0 x1 y1\"", (char *) ((void *)0));
            result = 1;
        }
        else {
            int x0, y0, x1, y1;
            int xmin = 0, xmax = (((Tk_FakeWin *) (tkwin))->changes.width) - 1;
            int ymin = 0, ymax = (((Tk_FakeWin *) (tkwin))->changes.height) - 1;

            x0 = atoi(argv[1]);
            y0 = atoi(argv[2]);
            x1 = atoi(argv[3]);
            y1 = atoi(argv[4]);

            x0 = (((xmin) > ((((xmax) < (x0)) ? (xmax) : (x0)))) ? (xmin) : ((((xmax) < (x0)) ? (xmax) : (x0))));
            y0 = (((ymin) > ((((ymax) < (y0)) ? (ymax) : (y0)))) ? (ymin) : ((((ymax) < (y0)) ? (ymax) : (y0))));
            x1 = (((xmin) > ((((xmax) < (x1)) ? (xmax) : (x1)))) ? (xmin) : ((((xmax) < (x1)) ? (xmax) : (x1))));
            y1 = (((ymin) > ((((ymax) < (y1)) ? (ymax) : (y1)))) ? (ymin) : ((((ymax) < (y1)) ? (ymax) : (y1))));

            if (plFramePtr->continue_draw) {
                XDrawLines((((Tk_FakeWin *) (tkwin))->display), (((Tk_FakeWin *) (tkwin))->window),
                           plFramePtr->xorGC, plFramePtr->pts, 5,
                           0);
                {(((Tk_FakeWin *) (tkwin))->display)->request++;};
            }

            plFramePtr->pts[0].x = x0; plFramePtr->pts[0].y = y0;
            plFramePtr->pts[1].x = x1; plFramePtr->pts[1].y = y0;
            plFramePtr->pts[2].x = x1; plFramePtr->pts[2].y = y1;
            plFramePtr->pts[3].x = x0; plFramePtr->pts[3].y = y1;
            plFramePtr->pts[4].x = x0; plFramePtr->pts[4].y = y0;

            XDrawLines((((Tk_FakeWin *) (tkwin))->display), (((Tk_FakeWin *) (tkwin))->window),
                       plFramePtr->xorGC, plFramePtr->pts, 5,
                       0);
            {(((Tk_FakeWin *) (tkwin))->display)->request++;};

            plFramePtr->continue_draw = 1;
        }
    }

    return result;
}








static int
Info(Tcl_Interp *interp, register PlFrame *plFramePtr,
     int argc, char **argv)
{
    int length;
    char c;
    int result = 0;



    if (argc == 0) {
        (tclStubsPtr->tcl_SetResult)(interp, "devkeys devnames", ((Tcl_FreeProc *) 0));
        return 0;
    }

    c = argv[0][0];
    length = strlen(argv[0]);



    if ((c == 'd') && (strncmp(argv[0], "devkeys", length) == 0)) {
        int i = 0;
        while (plFramePtr->devName[i] != ((void *)0)) 
            (tclStubsPtr->tcl_AppendElement)(interp, plFramePtr->devName[i++]);

        result = 0;
    }



    else if ((c == 'd') && (strncmp(argv[0], "devnames", length) == 0)) {
        int i = 0;
        while (plFramePtr->devDesc[i] != ((void *)0)) 
            (tclStubsPtr->tcl_AppendElement)(interp, plFramePtr->devDesc[i++]);

        result = 0;
    }



    else {
        (tclStubsPtr->tcl_AppendResult)(interp, "bad option to \"info\": must be ", 
         "devkeys, devnames", (char *) ((void *)0));

        result = 1;
    }

    return result;
}









static int
Openlink(Tcl_Interp *interp, register PlFrame *plFramePtr,
         int argc, char **argv)
{
































































#line 2539 "./../tk/plframe.c"

    return 0;
}








static int
Closelink(Tcl_Interp *interp, register PlFrame *plFramePtr,
         int argc, char **argv)
{














#line 2569 "./../tk/plframe.c"
    return 0;
}







static int
process_data(Tcl_Interp *interp, register PlFrame *plFramePtr)
{
    register PLRDev *plr = plFramePtr->plr;
    register PLiodev *iodev = plr->iodev;
    int result = 0;



    if (plr_process(plr) == -1) {
        (tclStubsPtr->tcl_AppendResult)(interp, "unable to read from ", iodev->typeName,
                         (char *) ((void *)0));
        result = 1;
    }



    if (plr->at_bop && plFramePtr->bopCmd != ((void *)0)) {
        plr->at_bop = 0;
        if ((tclStubsPtr->tcl_Eval)(interp, plFramePtr->bopCmd) != 0)
            fprintf((&_iob[2]), "Command \"%s\" failed:\n\t %s\n",
                    plFramePtr->bopCmd, interp->result);
    }



    if (plr->at_eop && plFramePtr->eopCmd != ((void *)0)) {
        plr->at_eop = 0;
        if ((tclStubsPtr->tcl_Eval)(interp, plFramePtr->eopCmd) != 0)
            fprintf((&_iob[2]), "Command \"%s\" failed:\n\t %s\n",
                    plFramePtr->eopCmd, interp->result);
    }

    return result;
}

void PlframeAtEop(Tcl_Interp *interp, register PlFrame *plFramePtr) {
    if (plFramePtr->eopCmd != ((void *)0)) {
        if ((tclStubsPtr->tcl_Eval)(interp, plFramePtr->eopCmd) != 0)
            fprintf((&_iob[2]), "Command \"%s\" failed:\n\t %s\n",
                    plFramePtr->eopCmd, interp->result);
    }
}

void PlframeAtBop(Tcl_Interp *interp, register PlFrame *plFramePtr) {
    if (plFramePtr->bopCmd != ((void *)0)) {
        if ((tclStubsPtr->tcl_Eval)(interp, plFramePtr->bopCmd) != 0)
            fprintf((&_iob[2]), "Command \"%s\" failed:\n\t %s\n",
                    plFramePtr->bopCmd, interp->result);
    }
}








static int
ReadData(ClientData clientData, int mask)
{
    register PlFrame *plFramePtr = (PlFrame *) clientData;
    register Tcl_Interp *interp = plFramePtr->interp;

    register PLRDev *plr = plFramePtr->plr;
    register PLiodev *iodev = plr->iodev;
    register PDFstrm *pdfs = plr->pdfs;
    int result = 0;

    if (mask & (1<<1)) {

    

        c_plsstrm(plFramePtr->ipls);
        
        if (pl_PacketReceive(interp, iodev, pdfs)) {
        

#line 2658 "./../tk/plframe.c"
            (tclStubsPtr->tcl_AppendResult)(interp, "Packet receive failed:\n\t %s\n",
                             interp->result, (char *) ((void *)0));
            return 1;
        }

    



        if (pdfs->bp == 0)
            return 0;

        plr->nbytes = pdfs->bp;
        pdfs->bp = 0;
        result = process_data(interp, plFramePtr);
    }

    return result;
}








static int
Orient(Tcl_Interp *interp, register PlFrame *plFramePtr,
       int argc, char **argv)
{
    int result = 0;



    c_plsstrm(plFramePtr->ipls);

    if (argc == 0) {
        PLFLT rot;
        char result_str[128];
        c_plgdiori(&rot);
        sprintf(result_str, "%f", rot);
        (tclStubsPtr->tcl_SetResult)(interp, result_str, ((Tcl_FreeProc *) 1));
    }



    else {
        c_plsdiori(atof(argv[0]));
        result = Redraw(interp, plFramePtr, argc-1, argv+1);
    }

    return result;
}














static int
Print(Tcl_Interp *interp, register PlFrame *plFramePtr,
       int argc, char **argv)
{
    PLINT ipls;
    int result = 0;
    char *sfnam;
    FILE *sfile;
    int pid;



    if ( ! plFramePtr->tkwin_initted) {
        (tclStubsPtr->tcl_AppendResult)(interp, "Error -- widget not plotted to yet", 
                         (char *) ((void *)0));
        return 1;
    }



    c_plmkstrm(&ipls);
    if (ipls < 0) {
        (tclStubsPtr->tcl_AppendResult)(interp, "Error -- cannot create stream", 
                         (char *) ((void *)0));
        return 1;
    }



    sfnam = (char *) tmpnam(((void *)0));

    if ((sfile = fopen(sfnam, "wb+")) == ((void *)0)) {
        (tclStubsPtr->tcl_AppendResult)(interp, 
                         "Error -- cannot open plot file for writing",
                         (char *) ((void *)0));
        c_plend1();
        return 1;
    }



    c_plsdev("plmeta");
    plsfile(sfile);
    c_plcpstrm(plFramePtr->ipls, 0);
    c_pladv(0);



    c_plreplot();
    c_plend1();
    c_plsstrm(plFramePtr->ipls);



    if (plFramePtr->plpr_cmd == ((void *)0))
        plFramePtr->plpr_cmd = plFindCommand("plpr");














#line 2797 "./../tk/plframe.c"
    return result;
}







static int 
NextPage(Tcl_Interp *interp, register PlFrame *plFramePtr,
     int argc, char **argv)
{
    TkwDev *dev = (TkwDev *) plFramePtr->pls->dev;
    int len = 0;
    if(argc == 0) {
    	dev->exit_eventloop = 1;
    } else {
        (tclStubsPtr->tcl_AppendResult)(interp, "wrong # args: should be \"",
                         " nextpage\"", (char *) ((void *)0));
        return 1;
    }
    return 0;
}









static int
Page(Tcl_Interp *interp, register PlFrame *plFramePtr,
     int argc, char **argv)
{



    c_plsstrm(plFramePtr->ipls);

    if (argc == 0) {
        PLFLT mar, aspect, jx, jy;
        char result_str[128];

        c_plgdidev(&mar, &aspect, &jx, &jy);
        sprintf(result_str, "%g %g %g %g", mar, aspect, jx, jy);
        (tclStubsPtr->tcl_SetResult)(interp, result_str, ((Tcl_FreeProc *) 1));
        return 0;
    }



    if (argc < 4) {
        (tclStubsPtr->tcl_AppendResult)(interp, "wrong # args: should be \"",
                         " page mar aspect jx jy\"", (char *) ((void *)0));
        return 1;
    } 

    c_plsdidev(atof(argv[0]), atof(argv[1]), atof(argv[2]), atof(argv[3]));
    return (Redraw(interp, plFramePtr, argc-1, argv+1));
}









static int
Redraw(Tcl_Interp *interp, register PlFrame *plFramePtr,
       int argc, char **argv)
{
    ;

    plFramePtr->flags |= 4;
    if ((plFramePtr->tkwin != ((void *)0)) &&
        !(plFramePtr->flags & 1)) {

        (tclStubsPtr->tcl_DoWhenIdle)(DisplayPlFrame, (ClientData) plFramePtr);
        plFramePtr->flags |= 1;
    }

    return 0;
}








static int
Save(Tcl_Interp *interp, register PlFrame *plFramePtr,
     int argc, char **argv)
{
    int length;
    char c;
    FILE *sfile;



    if ( ! plFramePtr->tkwin_initted) {
        (tclStubsPtr->tcl_AppendResult)(interp, "Error -- widget not plotted to yet", 
                         (char *) ((void *)0));
        return 1;
    }



    if (argc == 0) {
        if ( ! plFramePtr->ipls_save) {
            (tclStubsPtr->tcl_AppendResult)(interp, "Error -- no current save file", 
                             (char *) ((void *)0));
            return 1;
        }
        c_plsstrm(plFramePtr->ipls_save);
        c_plcpstrm(plFramePtr->ipls, 0);
        c_pladv(0);
        c_plreplot();
        c_plflush();
        c_plsstrm(plFramePtr->ipls);
        return 0;
    }

    c = argv[0][0];
    length = strlen(argv[0]);



    if ((c == 'a') && (strncmp(argv[0], "as", length) == 0)) {
        if (argc < 3) {
            (tclStubsPtr->tcl_AppendResult)(interp, "wrong # args: should be \"",
                             " save as device file\"", (char *) ((void *)0));
            return 1;
        } 

    

        if (plFramePtr->ipls_save) {
            c_plsstrm(plFramePtr->ipls_save);
            c_plend1();
        }

    

        c_plmkstrm(&plFramePtr->ipls_save);
        if (plFramePtr->ipls_save < 0) {
            (tclStubsPtr->tcl_AppendResult)(interp, "Error -- cannot create stream", 
                             (char *) ((void *)0));
            plFramePtr->ipls_save = 0;
            return 1;
        }

    

        if ((sfile = fopen(argv[2], "wb+")) == ((void *)0)) {
            (tclStubsPtr->tcl_AppendResult)(interp, "Error -- cannot open file ", argv[2],
                             " for writing", (char *) ((void *)0));
            plFramePtr->ipls_save = 0;
            c_plend1();
            return 1;
        }

    

        c_plsdev(argv[1]);
        plsfile(sfile);
        c_plcpstrm(plFramePtr->ipls, 0);
        c_pladv(0);

    

        c_plreplot();
        c_plflush();
        c_plsstrm(plFramePtr->ipls);
    }



    else if ((c == 'c') && (strncmp(argv[0], "close", length) == 0)) {

        if ( ! plFramePtr->ipls_save) {
            (tclStubsPtr->tcl_AppendResult)(interp, "Error -- no current save file", 
                             (char *) ((void *)0));
            return 1;
        }
        else {
            c_plsstrm(plFramePtr->ipls_save);
            c_plend1();
            plFramePtr->ipls_save = 0;
        }
    }



    else {
        (tclStubsPtr->tcl_AppendResult)(interp, "bad option to \"save\": must be ", 
         "as or close", (char *) ((void *)0));

        return 1;
    }

    return 0;
}








static int
View(Tcl_Interp *interp, register PlFrame *plFramePtr,
     int argc, char **argv)
{
    int length;
    int dontRedraw = 0;
    char c;
    PLFLT xl, xr, yl, yr;



    c_plsstrm(plFramePtr->ipls);

    if (argc == 0) {
        char result_str[128];
        c_plgdiplt(&xl, &yl, &xr, &yr);
        sprintf(result_str, "%g %g %g %g", xl, yl, xr, yr);
        (tclStubsPtr->tcl_SetResult)(interp, result_str, ((Tcl_FreeProc *) 1));
        return 0;
    }

    c = argv[0][0];
    length = strlen(argv[0]);




    if ((c == 'b') && (strncmp(argv[0], "bounds", length) == 0)) {
        char result_str[128];
        xl = 0.; yl = 0.;
        xr = 1.; yr = 1.;
        pldip2dc(&xl, &yl, &xr, &yr);
        sprintf(result_str, "%g %g %g %g", xl, yl, xr, yr);
        (tclStubsPtr->tcl_SetResult)(interp, result_str, ((Tcl_FreeProc *) 1));
        return 0;
    }



    if ((c == 'r') && (strncmp(argv[0], "reset", length) == 0)) {
        xl = 0.; yl = 0.;
        xr = 1.; yr = 1.;
        c_plsdiplt(xl, yl, xr, yr);
	if (argc > 1 && (strcmp(argv[1], "wait") == 0)) {
	    
	    dontRedraw = 1;
	}
    }




    else if ((c == 's') && (strncmp(argv[0], "select", length) == 0)) {
        if (argc < 5) {
            (tclStubsPtr->tcl_AppendResult)(interp, "wrong # args: should be \"",
                             " view select xmin ymin xmax ymax\"",
                             (char *) ((void *)0));
            return 1;
        }
        else {
            gbox(&xl, &yl, &xr, &yr, argv+1);
            c_plsdiplt(xl, yl, xr, yr);
        }
    }




    else if ((c == 'z') && (strncmp(argv[0], "zoom", length) == 0)) {
        if (argc < 5) {
            (tclStubsPtr->tcl_AppendResult)(interp, "wrong # args: should be \"",
                             " view zoom xmin ymin xmax ymax\"",
                             (char *) ((void *)0));
            return 1;
        } else {
            gbox(&xl, &yl, &xr, &yr, argv+1);
            pldid2pc(&xl, &yl, &xr, &yr);
            c_plsdiplz(xl, yl, xr, yr);
	    if (argc > 5 && (strcmp(argv[5], "wait") == 0)) {
		
		dontRedraw = 1;
	    }
        }
    }



    else {
        (tclStubsPtr->tcl_AppendResult)(interp, "bad option \"", argv[1],
         "\":  options to \"view\" are: bounds, reset, select, or zoom",
         (char *) ((void *)0));

        return 1;
    }



    c_plgdiplt(&xl, &yl, &xr, &yr);
    plFramePtr->xl = xl;
    plFramePtr->yl = yl;
    plFramePtr->xr = xr;
    plFramePtr->yr = yr;
    plFramePtr->flags |= 8|16;

    if (dontRedraw) {
        return 0;
    } else {
	return Redraw(interp, plFramePtr, argc, argv);
    }
}








static int
Scroll(Tcl_Interp *interp, register PlFrame *plFramePtr)
{
    c_plsstrm(plFramePtr->ipls);
    c_plsdiplt(plFramePtr->xl, plFramePtr->yl, plFramePtr->xr, plFramePtr->yr);

    plFramePtr->flags |= 8 | 16;
    return (Redraw(interp, plFramePtr, 0, ((void *)0)));
}









static int
report( Tcl_Interp *interp, register PlFrame *plFramePtr,
        int argc, char **argv )
{
    float x, y;


    if (argc == 0) {
        interp->result = "report what?";
        return 1;
    }

    if (!strcmp( argv[0], "wc" )) {

        TkwDev *dev = (TkwDev *) plFramePtr->pls->dev;
        PLGraphicsIn *gin = &(dev->gin);

        if (argc != 3) {
            interp->result = "Wrong # of args: report wc x y";
            return 1;
        }

        x = atof( argv[1] );
        y = atof( argv[2] );

        gin->dX = (PLFLT) x / (dev->width - 1);
        gin->dY = 1.0 - (PLFLT) y / (dev->height - 1);

    

        if (plTranslateCursor(gin)) {
            sprintf( interp->result, "%f %f", gin->wX, gin->wY );
            return 0;
        }

        interp->result = "Cannot locate";
        return 0;
    }

    interp->result = "nonsensical request.";
    return 1;
}











static void
UpdateVScrollbar(register PlFrame *plFramePtr)
{
    char string[60];
    int result;

    if (plFramePtr->yScrollCmd == ((void *)0))
        return;

    sprintf(string, " %f %f",1.-plFramePtr->yr,1.-plFramePtr->yl);

    result = (tclStubsPtr->tcl_VarEval)(plFramePtr->interp, plFramePtr->yScrollCmd, string,
                         (char *) ((void *)0));

    if (result != 0) {
        (tclStubsPtr->tcl_BackgroundError)(plFramePtr->interp);
    }
}







static void
UpdateHScrollbar(register PlFrame *plFramePtr)
{
    char string[60];
	int result;
	
    if (plFramePtr->xScrollCmd == ((void *)0))
        return;

    sprintf(string, " %f %f",plFramePtr->xl,plFramePtr->xr);

    result = (tclStubsPtr->tcl_VarEval)(plFramePtr->interp, plFramePtr->xScrollCmd, string,
                         (char *) ((void *)0));

    if (result != 0) {
        (tclStubsPtr->tcl_BackgroundError)(plFramePtr->interp);
    }
}








static void
gbox(PLFLT *xl, PLFLT *yl, PLFLT *xr, PLFLT *yr, char **argv)
{
    float x0, y0, x1, y1;

    x0 = atof(argv[0]);
    y0 = atof(argv[1]);
    x1 = atof(argv[2]);
    y1 = atof(argv[3]);

    x0 = (((0.) > ((((1.) < (x0)) ? (1.) : (x0)))) ? (0.) : ((((1.) < (x0)) ? (1.) : (x0))));
    y0 = (((0.) > ((((1.) < (y0)) ? (1.) : (y0)))) ? (0.) : ((((1.) < (y0)) ? (1.) : (y0))));
    x1 = (((0.) > ((((1.) < (x1)) ? (1.) : (x1)))) ? (0.) : ((((1.) < (x1)) ? (1.) : (x1))));
    y1 = (((0.) > ((((1.) < (y1)) ? (1.) : (y1)))) ? (0.) : ((((1.) < (y1)) ? (1.) : (y1))));



    *xl = (((x0) < (x1)) ? (x0) : (x1));
    *yl = (((y0) < (y1)) ? (y0) : (y1));
    *xr = (((x0) > (x1)) ? (x0) : (x1));
    *yr = (((y0) > (y1)) ? (y0) : (y1));
}




















#line 3297 "./../tk/plframe.c"















