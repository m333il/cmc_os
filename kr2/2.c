#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdint.h>
#include <sys/mman.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>


int find(char * arr, char g) {
    for (int i = 0; i < 16; i++) {
        if (arr[i] == g) {
            return i;
        }
  }
  return 0;
}

int
main(int argc, char ** argv) {
  char arr[] = {"0123456789abcdef"};

  char num1[33] = "f";
  char num2[33] = "0";
  sscanf(argv[1], "%s", num1 + 1);
  sscanf(argv[2], "%s", num2 + 1);

  int z = 0;
  for (int i = 32; i > 0; i--) {
    int dist = find(arr, num1[i]) - find(arr, b[i]) - z;
    if (dist < 0) {
      z = 1;
      dist += 16;
    }
    num1[i] = arr[dist];
  }

  for (int i = 1; i < 33; i++) {
    printf("%c", num1[i]);
  }
  printf("\n");

  return 0;
}
