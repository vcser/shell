#ifndef SHELL_H
#define SHELL_H

char *shell_getline();
char **shell_parse(char *line);
void shell_execute(char **command);

// shell builtins
void shell_exit(char **args);
void shell_cd(char **args);
void shell_pwd(char **args);

struct builtin {
    char *name;
    void (*func)(char **args);
};

struct builtin builtins[] = {
    {"exit", shell_exit},
    {"cd", shell_cd},
    {"pwd", shell_pwd},
};

#endif
