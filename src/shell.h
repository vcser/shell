#ifndef SHELL_H
#define SHELL_H

char *shell_getline();
char **shell_parse(char *line);
void shell_execute(char **command);
void shell_exit();

struct builtin {
    char *name;
    void (*func)(char **args);
};

struct builtin builtins[] = {
    {"exit", shell_exit},
};

#endif
