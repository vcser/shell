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

void shell_parse(char s[], char ** parseS) {
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

void shell_getline() {

}

void shell_execute() {

}
