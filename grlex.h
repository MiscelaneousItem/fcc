/*
  fcc_lex(void* tokens, char* code_input); , is the lexer for our compiler, it takes code as input and returns and array of tokens formatted as :
  [type (2 bytes)][value size (1 byte)][value (dynamic)]...[terminator type (-1)][0][]
  fcc_lex does NOT check for syntax errors.
*/
#include "ftypes.h"

char* fcc_lex(char* code_input);
