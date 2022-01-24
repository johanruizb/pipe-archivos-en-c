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
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <wait.h>
#include <time.h>

#define READ 0
#define WRITE 1

 /*
  * En esta segunda actividad modificar el programa que se trabajó 
  * en el ítem anterior de modo que:
  *
  *	Se cambie el uso de la función execlp por execvp.
  *	El padre se encarga de ejecutar el comando “cat /etc/hosts” y el hijo ejecute “sort”.
  *
  *
  * @param[out] int retorna 0, en caso de una ejecucion correcta
  */

int main() {
	srand(time(NULL));

	pid_t pid;
	int fd[2];
	int random_number;
	char* argv[] = { "cat", "/etc/hosts", 0 };


	printf("Pipe create\n");
	if (pipe(fd) == -1) {
		perror("Creating pipe \n");
		exit(EXIT_FAILURE);
	}

	pid = fork();

	if (pid < 0) {
		perror("fork() failed");
		exit(EXIT_FAILURE);
	}

	switch (pid) {
	case 0:
		printf("Process hijo create\n");

		close(fd[WRITE]);
		dup2(fd[READ], STDIN_FILENO);


		char* argv1[] = { "sort", STDIN_FILENO, NULL };
		execvp(argv1[0], argv1);
		perror("Error inesperado de execvp()");
		exit(EXIT_FAILURE);
		break;

	default:

		random_number = rand() % 1000 + 1;
		printf("Random number is %d\n", random_number);

		close(fd[READ]);
		dup2(fd[WRITE], STDOUT_FILENO);

		execvp(argv[0], argv);
		perror("Error inesperado de execvp()");
		exit(EXIT_FAILURE);
		break;
	}

	return 0;
}
