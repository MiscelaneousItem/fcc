#include "ftypes.h"

void* fstd_h_malloc(uint32 size) {
}

void* fstd_h_calloc(uint32 nmemb, uint32 size) {
  char* output = fstd_h_malloc(size);
  for(int i = 0; i < size; i++) {
    output[i] = 0;
  }
  return output;
}

void* fstd_h_realloc(void* src, uint32 size) {
}

void* fstd_h_memcpy(void* dest, const void* src, uint32 size) {
}

int fstd_pow(short a, short n) {
  int output = a;
  for(int i = 0; i < n; i++) {
    output *= a;
  }
  return output;
}

void fstd_h_print(char* input) {
}

void fstd_assert(unsigned short test) {
}

short fstd_isdigit(char a) {
  if(a >= 48 && a <= 57) return 1;
  return 0;
}
