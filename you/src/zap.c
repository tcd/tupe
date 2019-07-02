/* zap:  interactive process killer */

#include <stdio.h>
#include <signal.h>
#include <string.h>

char *progname;  /* program name for error message */
char *ps = "ps aux"; /* system dependent */

int main(int argc, char *argv[]) {

    int strindex(char *s, char *t);
    int ttyin();

    FILE *fin, *popen();
    char buf[BUFSIZ];
    int pid;

    progname = argv[0];

    if ((fin = popen(ps, "r")) == NULL) {
        fprintf(stderr, "%s: can't run %s\n", progname, ps);
        return 1;
    }

    fgets(buf, sizeof buf, fin);  /* get header line */
    fprintf(stderr, "%s", buf);
    while (fgets(buf, sizeof buf, fin) != NULL) {
        if (argc == 1 || strindex(buf, argv[1]) >= 0) {
            buf[strlen(buf)-1] = '\0';  /* suppress \n */
            fprintf(stderr, "%s? ", buf);
            if (ttyin() == 'y') {
                scanf(buf, "%d", &pid);
                kill(pid, SIGKILL);
            }
        }
    }
    return 0;
}

// return index of t in s, -1 if none
int strindex(char *s, char *t) {
    int i, n;

    n = strlen(t);
    for (i = 0; s[i] != '\0'; i++)
        if (strncmp(s+i, t, n) == 0)
            return 1;
    return -1;
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
