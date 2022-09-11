#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "shell.h"

int main(int argc, char *argv[]) {
    while (1) {
        printf("$ ");
        shell_getline();
        shell_parse();
        shell_execute();
    }
    return 0;
}

void shell_parse() {

}

void shell_parse() {

}

void shell_execute() {
    
}
