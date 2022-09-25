#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <signal.h>
#include <string>
#include "shell.h"


using std::string;

// solucion temporal
#define BUF_SIZE 1000
char line[BUF_SIZE];

void sigint_handler(int parameter) {
    char answer;
    write(1 ,"\nDesea salir [Y/n] ",19);
    read(0,&answer, 1);

    if (answer != 'n') {
        exit(0);
    }
}

int main(int argc, char *argv[]) {
    signal(SIGINT, sigint_handler);

    size_t len = 0;
    ssize_t nread = 0;
    while (1) {
        fflush(stdin);
        fflush(stdout);
        printf("$ ");
        shell_getline();
        vector<vector<char*>> command = shell_parse(line);

        // DEBUG
        printf("tamaño: %d\n", command.size());
        int count = 1;
        for (auto i : command) {
            printf("command %d\n", count++);
            for (auto x = i.begin(); *x != NULL; x++) {
                printf("%s\n", *x);
            }
            printf("\n");
        }
        // DEBUG

        shell_execute(command);
    }
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

vector<vector<char*>> shell_parse(char *line) {
    char *token;
    char *comm;
    vector<vector<char *>> commands;
    vector<string> lines;

    comm = strtok(line, "|\n");
    while (comm) {
        lines.push_back(string(comm));
        comm = strtok(NULL, "|\n");
    }

    for (string comm : lines) {
        vector<char*> command;
        token = strtok((char *)comm.c_str(), " \n");
        while (token) {
            int len = strlen(token);
            command.push_back((char *) malloc((len+1) * sizeof(char)));
            strcpy(command.back(), token);
            token = strtok(NULL, " \n");
        }
        command.push_back(NULL);
        commands.push_back(command);
    }
    return commands;
}

void shell_execute(vector<vector<char*>> &command) {
    if (command[0][0] == NULL) return;

    for (int i = 0; i < BUILTINS_SIZE; i++) {
        if (strcmp(command[0][0], builtins[i].name) == 0) {
            builtins[i].func((char **)&command[0][0]);
            return;
        }
    }

    pid_t child_pid = fork();

    if (child_pid == 0) {
        if (execvp((char *)command[0][0], (char**)&command[0][0]) == -1) {
            perror(command[0][0]);
        }
    } else if (child_pid > 0) {
        int status;
        //waitpid(child_pid, &status, WUNTRACED);
        wait(NULL);
    } else {
        perror("shell");
    }

    // no estoy seguro si liberar la memoria aqui o fuera de la funcion
    for (int i = 0; command[0][i] != NULL; i++) {
        free(command[0][i]);
    }
    //free(command[0]);
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
    //printf("pipes: %d\n",count);
    return count;
    
}

char ***shell_parsepipe(char *line){
    return NULL;
}
void shell_executepipe(char ***command){

}