#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>

int main(int argc, char *argv[]) {
  struct timeval inicial, fin;
  gettimeofday(&inicial, NULL);
   int child= fork();
  if (child< 0) {
    fprintf(stderr, "el fork fallo\n");
    exit(1);
  } else if (child== 0) {
        execlp(argv[1], argv[1], NULL);
        return 1;
  } else {
    wait(NULL);
    gettimeofday(&fin, NULL);
    printf("tiempo transcurrido(ms): %ld ms. \n", fin.tv_usec - inicial.tv_usec);
  }
  return 0;
}
