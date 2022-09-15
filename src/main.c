#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

// solucion temporal
#define BUF_SIZE 1000
char line[BUF_SIZE];

int main(int argc, char *argv[]) {
    size_t len = 0;
    ssize_t nread = 0;
    while (1) {
        printf("$ ");
        shell_getline();
        char **commands = shell_parse(line);
        shell_execute(commands);
        free(commands);
    }
    free(line);
    return 0;
}

char *shell_getline() {
    if (fgets(line, BUF_SIZE, stdin) == NULL) {
        // ERROR
        perror("shell");
    }
    // if (getline(&line, &len, stdin) == -1) {
    //     // error
    // }
    return line;
}

char **shell_parse(char *line) {
    char tmpW[s_len];
    int tmpIndex = 0;
    int parseIndex = 0;
    for(int i = 0; i<strlen(s);i++){
        if(s[i] != '\n' && s[i] != ' '){
            tmpW[tmpIndex] = s[i];
            tmpIndex++;
        }
        else if((s[i] == '\n' || s[i] == ' ') && tmpIndex > 0){
            tmpW[tmpIndex] = '\0';
            parseS[parseIndex] = (char *) malloc((strlen(tmpW))+1;
            if(parseS[parseIndex] == NULL){
                freeArr(parseS);
                fprint(stderr,"Malloc Failed");
                exit(1);
            }
                                                    
              strcpy(parseS[parseIndex], tmpW);
              parseIndex++;
              tmpIndex = 0;                                   
                                                                                            
             }
     }
                                                 
}

void shell_execute(char **command) {
    pid_t child_pid = fork();

    if (child_pid == 0) {
        execvp(command[0], command);
    } else if (child_pid > 0) {
        int status;
        waitpid(child_pid, &status, WUNTRACED);
    } else {
        perror("shell");
    }
}

void shell_exit() {

}
