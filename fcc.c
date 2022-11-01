/*
  This is a second attempt at a compiler for 'phasm' architecture.
  It aims at producing assembly for the 'f' cpu (fasm) from a subset of C called 'fC'.
  It also aims at being completly bootstrapable, and thus must be written in the subset of C it aims to compile.

  fC's main characteristics are as defined below :
  - NO structures or typedefs
  - pointers and memory allocation
  - boolean and arithmetic operators
  - functions
  - function pointers
  - NO recursion
 
  fC's standard library is as defined in fstd.h, and more precise information can be found in that header, however in general terms it seeks to mimic the C standard library.

  Additionally, due to the nature of the target cpu one should PRIVILEGE 16 BIT VARIABLES where possible.
  Lastly, due to the nature of the nature of fC's restrictions, writing self documenting code is much harder, so one should pay great attention to commenting and explaining how memory is laid out, but also to write checks and asserts to produce safe and reliable code.
*/

#include "ftypes.h"
#include "fparse.h"

int main(int argc, char** argv) {
  char* code = "uint32 toto = 400; uint32 tata = 90; uint32 lolo;";
  fcc_parse(code);
  return 0;
}
