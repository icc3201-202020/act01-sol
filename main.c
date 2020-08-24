#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#define READ_END 0
#define WRITE_END 1

int main(int argc, char** argv) {
    
    int fd[2];
    int errnum;

    printf("Iniciando proceso padre, pid '%d'.\n", getpid());

    if (pipe(fd) < 0) {
        fprintf(stderr, "Error al crear pipe!\n");    
        exit(EXIT_FAILURE);
    }

    int child = fork();
    
    if (child == -1) {
        fprintf(stderr, "Error creando proceso hijo!\n");
        exit(EXIT_FAILURE);
    }

    if (!child) {
        char arg[17];
        sprintf(arg, "%d", fd[READ_END]);
        char* args[] = { "./child", arg, NULL };
        execv("./child", args);

        errnum = errno;
        
        perror("Error: ");
        fprintf(stderr, "Valor de errno: %d\n", errno);
    }
    else {
        char buff[128]; 
        int rc = 0;
        close(fd[READ_END]);

        while ((rc = read(STDIN_FILENO, buff, 128)) > 0) {
            write(fd[WRITE_END], buff, rc);
        }

        wait(&child);
        printf("Proceso padre ha terminado.\n");
    }

    exit(EXIT_SUCCESS);
}
