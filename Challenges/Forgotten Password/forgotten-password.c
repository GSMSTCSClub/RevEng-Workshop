#include <stdio.h>
#include <string.h>

void decryptPassword(char *buf, int len) {
  int i;
  for (i = 0; i < len; ++i) {
    buf[i] = buf[i] ^ 39;
  }
}

int main(int argc, char *argv[]) {
  setbuf(stdin, NULL);
  setbuf(stdout, NULL);
  // The values for flag[] and name are redacted, but
  // you may set them at your own convenience.
  char flag[] = "<testFlagHere>";
  char name[] = "<testNameHere>";
  char password[] = {0x13, 0x1e, 0x12, 0x43, 0x44, 0x12, 0x12,
                     0x45, 0x1e, 0x10, 0x10, 0x1e, 0x00};
  decryptPassword(password, 12);
  printf("Enter your name: ");
  char input[100];
  scanf("%s", input);
  if (strcmp(input, name) != 0) {
    printf("Invalid name\n");
    return 1;
  }
  printf("Enter your password: ");
  scanf("%s", input);
  if (strcmp(input, password) != 0) {
    printf("Invalid password\n");
    return 1;
  }
  printf("Welcome %s, here is your flag: %s\n", name, flag);
  return 0;
}
