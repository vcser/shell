#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

int main(int argc, char *argv[]) {
    char *line = NULL;
    size_t len = 0;
    ssize_t nread = 0;
    while (1) {
        printf("$ ");
        //shell_getline();
        if (getline(&line, &len, stdin) == -1) {
            // error
        }
        char ***commands = shell_parse(&line);
        shell_execute();
        free(commands);
    }
    free(line);
    return 0;
}

char ***shell_parse(char **line) {

}

void shell_getline() {

}

void shell_execute() {

}

void shell_exit() {

}
