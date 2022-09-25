#ifndef SHELL_H
#define SHELL_H
#include <vector>

using std::vector;

char *shell_getline();
vector<vector<char*>> shell_parse(char *line);
char ***shell_parsepipe(char *line);
void shell_executepipe(char ***command);
void shell_execute(vector<vector<char*>> &command);
int shell_howmanypipes(char *line);
void shell_prueba(char *line);

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

#define BUILTINS_SIZE sizeof(builtins)/sizeof(struct builtin)

#endif
