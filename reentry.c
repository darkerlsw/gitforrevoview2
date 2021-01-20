#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAXSTRINGSZ    4096

static char envbuf[MAXSTRINGSZ];

extern char **environ;

char *getEnv(const char *name)
{
    int i, len;
    len = strlen(name);
    for(i = 0; environ[i] != NULL; i++) {
        if((strncmp(name, environ[i], len)) == 0 && (environ[i][len] == '=')) {
            strncpy(envbuf, &environ[i][len+1], MAXSTRINGSZ-1);
            return (envbuf);
        }
    }

    return (NULL);
}

int main(void)
{
    int i;
    char *p = getEnv("SHELL");
    for(i = 0; i < sizeof(p); i++) {
        printf("%c", p[i]);
    }

}