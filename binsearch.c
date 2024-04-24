#include "binsearch.h"

int binsearch(int* a, int key, int size) {
  
  int low = 0;
  int high = size - 1;
  
  while (low <= high) {
    int mid = (low + high) / 2;
    int midVal = a[mid];

    if (midVal < key)
      low = mid + 1;
    else if (midVal > key)
      high = mid - 1;
    else {
      return mid; // key found
    }
  }
    
  return -1;  // key not found.
}
