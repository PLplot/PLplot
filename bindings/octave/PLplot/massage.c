/*
 * Add online help to functions, parsing 'tmp_stub' and looking
 * for function definitions; for each function found, scans directory
 * 'etc/po' for a file with the same basename as function and extension
 * '.txt'; if found, incorporates it as the function online help, else,
 * looks in 'etc/plplot.doc' for a one-line description of the function
 * and adds it.
 */

#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include  <sys/stat.h>
#include  "errno.h"

main()
{
	char	*p1, *p2;
	char	doc[150][1024], b[1024], tok[80];
	int		item=0, j;
	FILE	*fp, *fp1;
	struct stat buf;
	
	if ((fp = fopen("etc/plplot.doc","r")) == NULL) {
		perror("etc/plplot.doc not found:");
		exit(1);
	}

	while (! feof(fp)){
		fgets(b, sizeof(b), fp);
		if (strchr(b, '-'))
			if (strchr(b, '-'))
				strcpy(doc[item++], b);
	}
	fclose(fp);

	if ((fp = fopen("tmp_stub", "r")) == NULL) {
		perror("tmp_stub not found:");
		exit(1);
	}

	while (! feof(fp)){
		fgets(b, sizeof(b), fp);
			if (p2 = strchr(b, '(')){		// function ... = ...(
				p1 = p2;
				while(*p1-- != ' ');p1+=2;
				if (*(p1+1) == '_')			// c_...
					p1+=2;
				strncpy(tok, p1, p2-p1);
				*(tok+(int)(p2-p1))='\0';
				printf("%s", b);
				fgets(b, sizeof(b), fp);printf("%s%%\n", b);	// % function ... = ...(
				
				sprintf(b,"etc/txt/%s.txt", tok);
				if (stat(b, &buf) && errno == ENOENT) {
					fprintf(stderr,"%s not found, using plplot.doc\n", b);
					strcat(tok, "\t");
					for (j=0; j<item; j++){
						if (strncmp(doc[j], tok, strlen(tok)) == 0){
							printf("%% %s", &doc[j][strlen(tok)+4]);	// strip func --
							break;}
						if (j == item) {
							fprintf(stderr,"%s not found in plplot.doc!", tok);
							printf("%% %s -- No online help yet\n", tok);
						}
					}
				}
				else {
					printf("%%   Original PLplot call documentation:\n%%\n");
					fp1 = fopen(b,"r");
					while(!feof(fp1)) {
						fgets(b, sizeof(b), fp1);printf("%% %s", b);
					}
					fclose(fp1);
				}
 				fgets(b, sizeof(b), fp);	// % No doc...
				fgets(b, sizeof(b), fp);printf("%s", b); // plplot_oct...
				fgets(b, sizeof(b), fp);printf("%s\n", b); // endfunction
			}
	}
	fclose(fp);
	exit(0);
}
