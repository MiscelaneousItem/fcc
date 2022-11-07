/* 
   fC's standard library functions are defined as fstd(_groups)_name.
   Groups are a single character qualifier that gives information on the function, for example _h(ard) functions call fasm code directly.
   Groups are defined as :
   - _h, hard, functions that call fasm code directly
*/ 

#ifdef GCC

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>

     /* memory managment functions */
#define fstd_h_malloc(size)            malloc(size)
#define fstd_h_calloc(nmemb, size)     calloc(nmemb, size)
#define fstd_h_realloc(ptr, size)      realloc(ptr, size)
#define fstd_h_free(ptr)               free(ptr)
#define fstd_h_memcpy(dest, src, size) memcpy(dest, src, size)
#define fstd_memcmp(ptra, ptrb, size)  memcmp(ptra, ptrb, size)
#define fstd_h_memset(dest, val, size) memset(dest, val, size)
     /* math functions */
#define fstd_pow(a, b)                 pow(a, b)
      /*char type functions*/
#define fstd_isdigit(c)                isdigit(c)
#define fstd_isalpha(c)                isalpha(c)
     /* string manipulation functions */
#define fstd_atoi(a)                   atoi(a)
#define fstd_strcmp(a, b)              strcmp(a, b)
#define fstd_strlen(a)                 strlen(a)
     /* misc functions */
#define fstd_assert(c)                 assert(c)
#define fstd_h_print(t)                printf(t)
 
#else

    /* memory managment functions */
void*  fstd_h_malloc(uint32 size);
void*  fstd_h_calloc(uint32 nmemb, uint32 size);
void*  fstd_h_realloc(void* src, uint32 size);
void   fstd_h_free(void* ptr);
void   fstd_h_memcpy(void* dest, const void* src, uint32 size);
uint16 fstd_memcmp(void* ptra, void* ptrb, size);
    /* math functions */
uint32 fstd_pow(short a, short n);
    /* char type functions */
char   fstd_isdigit(char a);
char   fstd_isalpha(char a);
    /* string manipulation functions */
uint32 fstd_atoi(char* input);
char   fstd_strcmp(char* a, char* b);
char   fstd_strlen(char* a);
    /* misc functions */
void   fstd_h_print(char* input);
void   fstd_assert(unsigned short test);

#endif
