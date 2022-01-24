#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#define READ  0
#define WRITE 1

int main() {

  pid_t pid;
  int   fd[2];
  char buffer[80];
  srand(time(NULL));
  if (pipe(fd) == -1) {
        perror("Creating pipe");
        exit(EXIT_FAILURE);
  }

  switch(pid = fork()) {

  case 0: //Hijo

        close(fd[WRITE]);

        read(fd[READ], buffer, sizeof(buffer));
        int num = atoi(buffer);
        int file = open("resultado.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
        if(file >= 0){
                if(num >= 500){
                        sprintf(buffer, "%d es mayor o igual a 500\n", num);
                }else{
                        sprintf(buffer,"%d es menor que 500\n", num);
                }
                write(file, buffer, strlen(buffer));
        }else{
                perror("Error Archivo");
                exit(EXIT_FAILURE);
        }
        break;


  case -1: //Error

        perror("fork() failed)");
        exit(EXIT_FAILURE);

  default: ; //Padre

        int random_number = sprintf(buffer, "%d", rand() % 1000 + 1);
        close(fd[READ]);
        write(fd[WRITE], buffer, random_number);
        break;
}
        return 0;
}
