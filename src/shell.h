#ifndef SHELL_H
#define SHELL_H

char *shell_getline();
char **shell_parse(char *line);
char ***shell_parsepipe(char *line, int pipes);
void shell_executepipe(char ***command);
void shell_execute(char **command);
int shell_howmanypipes(char *line);

// shell builtins
void shell_exit(char **args);
void shell_cd(char **args);
void shell_pwd(char **args);
void shell_send_signal(char **args);

struct builtin {
    char *name;
    void (*func)(char **args);
};

struct builtin builtins[] = {
    {"exit", shell_exit},
    {"cd", shell_cd},
    {"pwd", shell_pwd},
    {"send_signal", shell_send_signal},
};


#define BUILTINS_SIZE sizeof(builtins) / sizeof(struct builtin)
#endif
