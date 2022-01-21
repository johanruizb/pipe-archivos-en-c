#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#define READ  0
#define WRITE 1

int main() {

  pid_t pid;//fork
  int   fd[2];//Vector de posiciones
  char *argv[] = {"/etc/hosts/",0}; 
//0-Entrada Estandar
  //1-Salida Estandar
  if (pipe(fd) == -1) { //Funcion que permite comunicar 2 procesos 0-entrada 1-salida
	perror("Creating pipe");
	exit(EXIT_FAILURE);
  }

  switch(pid = fork()) {

  case 0:
	// The child process will execute wc.
	// Close the pipe write descriptor.
	close(fd[WRITE]);//
	// Redirect STDIN to read from the pipe.
	dup2(fd[READ], STDIN_FILENO); //Sobreescribir la salida estandar del hijo
// Execute wc
        //printf("Parent: %c", STDIN_FILENO);
	execvp("sort",argv);

  case -1:
	perror("fork() failed)");
	exit(EXIT_FAILURE);

  default:
	// The parent process will execute ls.
	// Close the pipe read descriptor.
	close(fd[READ]);
	// Redirect STDOUT to write to the pipe.
	dup2(fd[WRITE], STDOUT_FILENO);//Sobreescribir la salida estandar
	// Execute ls -l
	execvp("cat", argv);
  }
}

