/*
 * En este documento se describe el desarrollo de
 * OS Tarea Pipe utilizando lo visto en clase.
 * Emplearemos los temas Pipe y los archivos en Unix desde el lenguaje C.
 *
 * Codigo fuente: https://docs.google.com/document/d/1pnsiXOkwBmcnt0VFh1EEjMiodIcnXgejeT4BJcEjF2g/edit?usp=sharing
 *
 * Modificado por: Johan Ruiz - johan.andres.ruiz@correounivalle.edu.co
 *                 Ingrid Echeverri - ingrid.echeverri@correounivalle.edu.co
 * Fecha: 24/01/2022
 */

#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <time.h>

#define READ 0
#define WRITE 1

/*
  * El proceso padre generará un núo 
  * entero aleatorio entre 1 y 1000, y
  * este número será enviado a . 
  * El hijo imprimirá el mensaje “menor0” 
  * si el número es menor que 500 y “mayor o igu0” 
  * si el número es mayor o igual a 500.
  *
  * @param[out] int retorna 0, en caso de una ejecucion correcta
 */

int main()
{
    pid_t pid;
    int fd[2];
    char buffer[80];
    srand(time(NULL));

    if (pipe(fd) == -1)
    {
        perror("Creating pipe");
        exit(EXIT_FAILURE);
    }

    switch (pid = fork())
    {

    case 0: //Hijo

        close(fd[WRITE]);

        read(fd[READ], buffer, sizeof(buffer));
        int num = atoi(buffer);
        int file = open("resultado.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        if (file >= 0)
        {
            if (num >= 500)
                sprintf(buffer, "%d es mayor o igual a 500\n", num);

            else
                sprintf(buffer, "%d es menor que 500\n", num);

            printf("Registro de respuesta disponible en 'resultado.txt'. Usa: \n\t> cat resultado.txt\n");
            write(file, buffer, strlen(buffer));
        }
        else
        {
            perror("Error Archivo");
            exit(EXIT_FAILURE);
        }
        break;

    case -1: // Error
        perror("fork() failed");
        exit(EXIT_FAILURE);
    default:; // Padre
        int random_number = sprintf(buffer, "%d", rand() % 1000 + 1);
        close(fd[READ]);
        write(fd[WRITE], buffer, random_number);
        break;
    }
    return 0;
}
