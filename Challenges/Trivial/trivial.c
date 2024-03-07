#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void easy() { system("cat flag.txt"); }

int main() {
  setbuf(stdin, NULL);
  setbuf(stdout, NULL);
  char buffer[50];
  write(1, "I love binaries!\n", 17);
  char address[50];
  write(1, "What do you think about:\n", 25);
  sprintf(address, "%p\n", easy);
  write(1, address, 9);
  write(1, ">", 1);
  gets(buffer);
}
