#include <stdint.h>
#include <stdio.h>

int fib(int n)
{
  if (n == 1)
  {
    return 0;
  }
  if (n == 2)
  {
    return 1;
  }
  return fib(n - 1) + fib(n - 2);
}

int main(int argc, char **argv)
{
  int f;
  f = fib(8);
  printf("f=%ld\n", f);
  return 0;
}
