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

int main(int argc, char *argv[]) {
    signal(SIGINT, sigint_handler);

    size_t len = 0;
    ssize_t nread = 0;
    int pipes;
    while (1) {
        printf("$ ");
        shell_getline();
        //shell_prueba(line);
        if(pipes = shell_howmanypipes(line)>0){
            char ***command = shell_parsepipe(line,(pipes+1));
            shell_executepipe(command);
        }
        else{
            char **command = shell_parse(line);
            shell_execute(command);
        }
        /* for (int i = 0; command[i] != NULL; i++) {
            printf("Token %d = %s\n", i+1, command[i]);
        } */
        
        //free(commands);
    }
    //free(line);
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
        //printf("%s\n", token);
        token = strtok(NULL, " \n");
    }
    command[size] = NULL;
    return command;
}

void shell_execute(char **command) {
    if (command[0] == NULL) return;
    
    //podriamos dejar el builtins_size como una constante?, para que no haga la operación cada vez
    int builtins_size = sizeof(builtins) / sizeof(struct builtin);
    //printf("builtin size: %d",builtins_size);
    for (int i = 0; i < builtins_size; i++) {
        if (strcmp(command[0], builtins[i].name) == 0) {
            builtins[i].func(command);
            return;
        }
    }

    pid_t child_pid = fork();

    if (child_pid == 0) {
        if (execvp(command[0], command) == -1) {
            perror(command[0]);
        }
    } else if (child_pid > 0) {
        int status;
        //waitpid(child_pid, &status, WUNTRACED);
        wait(NULL);
    } else {
        perror("shell");
    }

    // no estoy seguro si liberar la memoria aqui o fuera de la funcion
    for (int i = 0; command[i] != NULL; i++) {
        free(command[i]);
    }
    free(command);
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
    printf("pipes: %d\n",count);
    return count;
    
}

char ***shell_parsepipe(char *line, int pipes){
    printf("%d\n",pipes);
    char ***command = malloc( pipes * sizeof(char**));
    char *token;
    int size_com;
    token =strtok(line, "|");

    printf("%ld %d\n",strlen(token),pipes);
    for(int i=0;i<pipes;i++){
        command[i] = shell_parse(token);
        size_com = strlen(token);
        token = strtok(NULL, " \n");
        for (int j = 0; j <size_com ; j++)
        {
            printf("%s", command[i][j]);
        }
    }

    return NULL;
}

void shell_executepipe(char ***command){

}