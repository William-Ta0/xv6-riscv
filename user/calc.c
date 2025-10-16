#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

static void
usage(void)
{
  fprintf(2, "usage: calc <x> <op> <y>\n");
}

// calculate is now a syscall; no user-space implementation here

int
main(int argc, char *argv[])
{
  if(argc != 4){
    usage();
    exit(1);
  }

  int x = atoi(argv[1]);
  char *op = argv[2];
  int y = atoi(argv[3]);
  int result = 0;

  if(calculate(x, y, op, &result) < 0){
    fprintf(2, "calc: invalid operation or divide by zero\n");
    exit(1);
  }

  printf("%d\n", result);
  exit(0);
}


