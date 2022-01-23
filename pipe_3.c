#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <wait.h>
#include <time.h>

#define READ 0
#define WRITE 1

int main()
{
    srand(time(NULL));

    pid_t pid; // Fork
    int fd[2]; // Vector de posiciones
    int random_number;
    char readbuffer[80];
    int nbytes;

    // 0-Entrada Estandar
    // 1-Salida Estandar

    if (pipe(fd) == -1)
    {
        // Funcion que permite comunicar 2 procesos 0-entrada 1-salida
        perror("Creating pipe\n");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid < 0)
    {
        perror("fork() failed");
        exit(EXIT_FAILURE);
    }

    switch (pid)
    {
    case 0:
        // El proceso hijo ejecutará wc.
        // Cierra el descriptor de escritura de pipe.

        close(fd[WRITE]); // El proceso principal no necesita este extremo del pipe

        nbytes = read(fd[READ], readbuffer, sizeof(readbuffer));
        int numero = atoi(readbuffer);

        if (numero >= 500)
        {
            printf("Numero mayor o igual a 500 (%d)\n", numero);
        }
        else if (numero < 500)
        {
            printf("Numero menor a 500 (%d)\n", numero);
        }

        //printf("Read string: %s\n", readbuffer);

        break;

    default:

        // Numero aleatorio
        random_number = rand() % 1000 + 1;
        // Transformar en string
        char str[80];
        sprintf(str, "%d", random_number);

        close(fd[READ]);                          // El proceso hijo no necesita este extremo del pipe
        write(fd[WRITE], str, (strlen(str) + 1)); // Enviar str a traves del pipe

        break;
    }

    return 0;
}