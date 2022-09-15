#ifndef SHELL_H
#define SHELL_H

char ***shell_parse(char **line);
void shell_getline();
void shell_execute();
void shell_exit();

struct builtin {
    char *name;
    void (*func)(char **args);
};

struct builtin builtins[] = {
    {"exit", shell_exit},
};

#endif
