/* p:  print input in chunks (version 2) */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAGESIZE 22
char *progname;  /* program name for error message */

int main(int argc, char *argv[]) {

    void print(FILE *fp, int pagesize);
    int ttyin();

    int i;
    int pagesize = PAGESIZE;
    FILE *fp, *efopen();

    progname = argv[0];

    if (argc > 1 && argv[1][0] == '-') {
        pagesize = atoi(&argv[1][1]);
            argc--;
            argv++;
    }

    if (argc == 1)
        print(stdin, pagesize);
    else
        for (i = 1; i < argc; i++) {
            fp = efopen(argv[i], "r");
            print(fp, pagesize);
            fclose(fp);
        }
    return 0;
}

/* fopen file, die if can't */
FILE *efopen(char *file, char *mode) {
    FILE *fp, *fopen();
    extern char *progname;

    if ((fp = fopen(file, mode)) != NULL)
        return fp;
    else
        fprintf(stderr, "%s: can't open file %s mode %s\n", progname, file, mode);
        exit(1);
}

/* print fp in pagesize chunks */
void print(FILE *fp, int pagesize) {
    int ttyin();

    static int lines = 0; /* nu,ber of lines so far */
    char buf[BUFSIZ];

    while (fgets(buf, sizeof buf, fp) != NULL) {
        if (++lines < pagesize) {
            fputs(buf, stdout);
        } else {
            buf[strlen(buf)-1] = '\0';
            fputs(buf, stdout);
            fflush(stdout);
            ttyin();
            lines = 0;
        }
    }
}

/* process response from /dev/tty (version 2) */
int ttyin() {
    char buf[BUFSIZ];
    FILE *efopen();
    static FILE *tty = NULL;

    if (tty == NULL)
        tty = efopen("/dev/tty", "r");
    if (fgets(buf, BUFSIZ, tty) == NULL || buf[0] == 'q')
        exit(0);
    else if (buf[0] == '!') {
        system(buf+1); /* BUG: here is a bug */
        printf("!\n");
    }
    else    /* ordinary line */
        return buf[0];
}
