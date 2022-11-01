/*
  This file is used to define types for fC. 
  These types are designed to be simplified version of C types.
  This file SHOULD NOT BE INCLUDED when boot straping the compiler, as these types are hardcoded into it. 
*/
#ifdef GCC

#include <stdlib.h>

typedef u_int16_t uint16;
typedef u_int32_t uint32;
typedef u_int64_t uint64;

typedef int16_t   int16;
typedef int32_t   int32;
typedef int64_t   int64;
#endif
