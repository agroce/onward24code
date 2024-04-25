#include "binsearch.h"

#define MAX_SIZE 10

int main () {
  int a[MAX_SIZE];
  unsigned int SIZE = nondet_uint();
  __CPROVER_assume(SIZE > 0);
  __CPROVER_assume(SIZE <= MAX_SIZE);
  int k = nondet_int();
  int present = 0;
  for (int i = 0; i < SIZE; i++) {
    a[i] = nondet_int();
    if (i > 0) {
      __CPROVER_assume(a[i] >= a[i-1]);
    }
    if (a[i] == k) {
      present = 1;
    }
  }

  int r = binsearch(a, k, SIZE);
  if (r != -1) {
    assert(a[r] == k);
  } else {
    assert(!present);
  }
}
