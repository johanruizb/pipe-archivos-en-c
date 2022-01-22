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

// int argc, char const *argv[]

int main()
{
	printf("Init main \n");
	srand(time(NULL));

	pid_t pid; // fork
	int fd[2]; // Vector de posiciones
	int random_number;
	char *argv[] = {"cat", "/etc/hosts", 0};
	char *argv1[] = {"sort", "/etc/hosts", 0};
	// 0-Entrada Estandar
	// 1-Salida Estandar

	printf("Pipe create\n");
	if (pipe(fd) == -1)
	{
		// Funcion que permite comunicar 2 procesos 0-entrada 1-salida
		perror("Creating pipe \n");
		exit(EXIT_FAILURE);
	}

	// printf("Fork create \n");

	pid = fork();

	if (pid < 0)
	{
		perror("fork() failed");
		exit(EXIT_FAILURE);
	}

	switch (pid)
	{
	case 0:
		printf("Process hijo create\n");
		// El proceso hijo ejecutará wc.
		// Cierra el descriptor de escritura de pipe.
		close(fd[WRITE]);
		// Redirigir STDIN para leer desde el pipe.
		dup2(fd[READ], STDIN_FILENO); //Sobreescribir la salida estandar del hijo
		// Execute wc
		// printf("Parent: %c", STDIN_FILENO);
		execvp(argv1[0], argv1);
		perror("Error inesperado de execvp()");
		exit(EXIT_FAILURE);
		break;

	default:
		// El proceso padre ejecutará ls.
		// Cierra el descriptor de lectura de pipe.
		// wait(NULL);
		random_number = rand() % 1000 + 1;
		printf("Random number is %d\n", random_number);

		close(fd[READ]);
		// Redirigir STDOUT para escribir en el pipe.
		dup2(fd[WRITE], STDOUT_FILENO); //Sobreescribir la salida estandar
		// Execute cat
		execvp(argv[0], argv);
		perror("Error inesperado de execvp()");
		exit(EXIT_FAILURE);
		break;
	}

	return 0;
}