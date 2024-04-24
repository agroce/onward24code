#include <algorithm>
#include <deepstate/DeepState.hpp>

#include "binsearch.h"

using namespace deepstate;

#define MAX_SIZE 32

TEST(Run, Bentley) {
  int a[MAX_SIZE];
  unsigned int SIZE = DeepState_UIntInRange(1, MAX_SIZE);
  int k = DeepState_Int();
  int present = 0;

  LOG(TRACE) << "SIZE = " << SIZE;

  for (int i = 0; i < SIZE; i++) {
    a[i] = DeepState_Int();
    LOG(TRACE) << "a[" << i << "] = " << a[i];
    if (a[i] == k) {
      present = 1;
    }
  }
  LOG(TRACE) << "Sorting...";  
  std::sort(std::begin(a), &a[SIZE]);
  for (int i = 0; i < SIZE; i++) {
    LOG(TRACE) << "a[" << i << "] = " << a[i];
  }
  if (!present && DeepState_Bool()) {
    k = a[DeepState_UIntInRange(0, SIZE-1)];
    present = 1;
  }

  LOG(TRACE) << "k = " << k;
  LOG(TRACE) << "present = " << present;

  int r = binsearch(a, k, SIZE);
  LOG(TRACE) << "r = " << r;
  if (r != -1) {
    assert(a[r] == k);
  } else {
    assert(!present);
  }
}
