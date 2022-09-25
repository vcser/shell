#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <signal.h>
#include "shell.h"

// solucion temporal
#define BUF_SIZE 1000
char line[BUF_SIZE];

void sigint_handler(int parameter) {
    char answer;
    write(1 ,"\nDesea salir [Y/n] ",19);
    fflush(stdout);
    read(0,&answer, 1);

    if (answer != 'n') {
        exit(0);
    }
}

void sigusr_handler(int parameter) {
    write(1 ,"recibido señal SIGUSRX\n",25);
}

int main(int argc, char *argv[]) {
    signal(SIGINT, sigint_handler);
    signal(SIGUSR1, sigusr_handler);
    signal(SIGUSR2, sigusr_handler);

    size_t len = 0;
    ssize_t nread = 0;
    int pipes;
    while (1) {
        printf("$ ");
        shell_getline();
        pipes = shell_howmanypipes(line);
        char ***command = shell_parsepipe(line,(pipes+1));
        shell_executepipe(command);
        
    }
    return 0;
}

char *shell_getline() {
    if (fgets(line, BUF_SIZE, stdin) == NULL) {
        perror("shell");
    }
    return line;
}

char **shell_parse(char *line) {
    char *token;
    char **command = malloc(16 * sizeof(char *));
    unsigned int capacity = 16, size = 0;

    token = strtok(line, " \n");
    while (token) {
        int len = strlen(token);
        if (size + 1 >= capacity) {
            capacity *= 2;
            command = realloc(command, capacity * sizeof(char *));
        }
        command[size] = malloc(len * sizeof(char));
        strcpy(command[size++], token);
        token = strtok(NULL, " \n");
    }
    command[size] = NULL;
    return command;
}

void shell_exit(char **args) {
    exit(EXIT_SUCCESS);
}

void shell_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "cd: Missing arguments\n");
        return;
    }

    if (chdir(args[1]) == -1) {
        perror(args[1]);
    }
}

void shell_send_signal(char **args) {
    pid_t pid = getpid();
    int s = SIGUSR1;
    // parseo de los argumentos
    for (int i = 0; args[i] != NULL; i++) {
        for (int j = 0; args[i][j] != 0; j++) {
            if (args[i][j] == '-') {
                if (args[i][j+1] == 'p') {
                    if (args[i+1] == NULL) goto ERROR;
                    pid = atoi(args[i+1]);
                } else if (args[i][j+1] == 's') {
                    if (args[i+1] == NULL) goto ERROR;
                    int number = atoi(args[i+1]);
                    if (number == 1)
                        s = SIGUSR1;
                    else if (number == 2)
                        s = SIGUSR2;
                }
            }
        }
    }
    // enviar señal al proceso especificado
    if (kill(pid, s) == -1) {
        perror("send_signal");
    }
    return;
    ERROR: fprintf(stderr, "ERROR: send_signal\nusage: send_signal -p <pid> [-s <1|2>]\n");
}

void shell_pwd(char **args) {
    char cwd[PATH_MAX];
    if (getcwd(cwd, PATH_MAX) == NULL) {
        perror("pwd");
        return;
    }
    printf("%s\n", cwd);
}

int shell_howmanypipes(char *line){
    int count =0;
    for(int i=0;line[i]!='\0';i++){
        if(line[i]=='|'){
            count++;
        }
    }
    return count;
    
}

char ***shell_parsepipe(char *line, int pipes){
    char ***commands = malloc( pipes * sizeof(char**));
    char **command = malloc(pipes * sizeof(char*));
    char *token;
    int count =0;
    unsigned int size = 0;
    token = strtok(line, "|\n");

    while(token){
        int len = strlen(token);
        command[size] = malloc(len*sizeof(char));
        strcpy(command[size],token);
        size++;
        token = strtok(NULL,"|\n");
    }

    for(int i=0;i<size;i++){
        commands[i] = shell_parse(command[i]);
        count =0;
        while(commands[i][count]!= NULL){
            //printf("%s ",commands[i][count]);
            count++;
        }
        //printf("\n");
    }
    
    return commands;
}

void shell_executepipe(char ***command){
    if (command[0] == NULL) return;
    for (int i = 0; i < BUILTINS_SIZE; i++) {
        if (strcmp(command[0][0], builtins[i].name) == 0) {
            builtins[i].func(command[0]);
            return;
        }
    }

    int   p[2];
    pid_t pid;
    int   fd_in = 0;

    while (*command != NULL){
        pipe(p);
        if ((pid = fork()) == -1){
            exit(EXIT_FAILURE);
        }
        else if (pid == 0){
            dup2(fd_in, 0); 
            if (*(command + 1) != NULL)
                dup2(p[1], 1);
            close(p[0]);
            execvp((*command)[0], *command);
            exit(EXIT_FAILURE);
        }
        else{
            wait(NULL);
            close(p[1]);
            fd_in = p[0]; 
            command++;
        }
    }
    /*for (int i = 0; command[i] != NULL; i++) {
        for(int j=0;command[i][j] != NULL;j++){
            free(command[i][j]);
        }
        free(command[i]);
    }
    free(command);*/
}
