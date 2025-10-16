#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

static int failures = 0;

static void ok(const char *name){
  printf("[OK] %s\n", name);
}

static void fail(const char *name){
  printf("[FAIL] %s\n", name);
  failures++;
}

static void t_calc_ok(const char *name, int x, const char *op, int y, int expect){
  int got = 0;
  int rc = calculate(x, y, (char*)op, &got);
  if(rc == 0 && got == expect) ok(name); else fail(name);
}

static void t_calc_err(const char *name, int x, const char *op, int y){
  int out = 0xdeadbeef;
  int rc = calculate(x, y, (char*)op, &out);
  if(rc < 0) ok(name); else fail(name);
}

int
main(int argc, char *argv[])
{
  // valid operations
  t_calc_ok("add basic", 10, "+", 5, 15);
  t_calc_ok("sub basic", 10, "-", 3, 7);
  t_calc_ok("mul basic", 4,  "*", 7, 28);
  t_calc_ok("div basic", 15, "/", 3, 5);

  // edge cases
  t_calc_ok("add zero", 1, "+", 0, 1);
  t_calc_ok("mul zero", 123, "*", 0, 0);
  t_calc_ok("negatives mul", -6, "*", -7, 42);
  t_calc_ok("negatives add", -6, "+", 7, 1);

  // error cases
  t_calc_err("div by zero", 1, "/", 0);
  t_calc_err("invalid op letter", 2, "x", 3);
  t_calc_err("multi-char op", 2, "++", 3);

  // null pointer arguments (expect error)
  {
    int rc;
    int res;
    rc = calculate(1, 2, 0, &res);
    if(rc < 0) ok("null op pointer"); else fail("null op pointer");
    rc = calculate(1, 2, "+", 0);
    if(rc < 0) ok("null result pointer"); else fail("null result pointer");
  }

  if(failures){
    printf("calctest: %d failure(s)\n", failures);
    exit(1);
  }
  printf("calctest: all tests passed\n");
  exit(0);
}


