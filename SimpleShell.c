#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void shell(){

    char response[80];

    int children = 0;
    int a = 0;
    int r;

    while(1) {
        printf("prompt> ");

        fgets(response, 80, stdin);

        pid_t pid;
        pid = fork();
        int status;

        if(strncmp(response, "quit", 4) != 0) {

            if (pid == 0) {
                char* argv[20];

                int i = 0;

                argv[i] = strtok(response, " \n");

                while (argv[i] != NULL) {
                    argv[++i] = strtok(NULL, " \n");
                }

                i = 0;
                int ind = 0;
                int red = 0;
                char* argvv[20];

                while(argv[i] != NULL){
                    if(strncmp(argv[i], ">",1) == 0) {
                        red = 1;
                        FILE *file = fopen(argv[i + 1], "w");
                        int fd = fileno(file);
                        close(1);
                        dup(fd);
                        close(fd);
                    }
                    else if(strncmp(argv[i], "<", 1) == 0) {
                        red = 1;
                        FILE *file = fopen(argv[i + 1], "r");
                        int fd = fileno(file);
                        close(0);
                        dup(fd);
                        close(fd);
                    }
                    else if (red != 1){
                        argvv[ind] = argv[i];
                        ind++;
                    }
                    if (strncmp(argv[i], "&", 1) == 0){
                        a = 1;
                    }
                    i++;
                }

                argvv[ind] = NULL;

                execv(argvv[0], argvv);

                perror("Return  from execv() not  expected");
                exit(0);
            }
            else if (a == 1){
                r = waitpid(pid, &status, 0);
                children++;
            }
            else {
                wait();
            }
        }
    }
}

int main(){
    shell();
    return 0;
}