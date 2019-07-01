// pick: offer choice on each argument

#include <stdio.h>
#include <string.h>

char *progname; // program name for error message

int main(int argc, char *argv[]) {

    void pick(char *s);

    int i;
    char buf[BUFSIZ];

    progname = argv[0];

    if (argc == 2 && strcmp(argv[1], "-") == 0) {  // pick -
        while (fgets(buf, sizeof buf, stdin) != NULL) {
            buf[strlen(buf)-1] = '\0';  // drop newline
            pick(buf);
        }
    } else {
        for (i = 1; i < argc; i++)
            pick(argv[i]);
    }
    return 0;
}

// offer choice of s
void pick(char *s) {
    int ttyin();

    fprintf(stderr, "%s? ", s);
    if (ttyin() == 'y')
        printf("%s\n", s);
}

// process response from /dev/tty (version 1)
int ttyin() {
    char buf[BUFSIZ];
    FILE *efopen();
    static FILE *tty = NULL;

    if (tty == NULL)
        tty = efopen("/dev/tty", "r");
    if (fgets(buf, BUFSIZ, tty) == NULL || buf[0] == 'q')
        return 0;
    else
        return buf[0];
}
