/*
  Tokens:
  Tokens are defined as a memory zone starting with 16 bits which describe the type, then a byte which describes the size of the value stored by the token, then that value
*/
#include "ftypes.h"


// inittoken returns a token
char* inittoken(uint16 token, char dsize, void* data);

extern const uint16 TOKEN_SEMICOLON;
extern const uint16 TOKEN_EQUALS;
extern const uint16 TOKEN_PLUS;
extern const uint16 TOKEN_MINUS;
extern const uint16 TOKEN_MULTIPLICATION;
extern const uint16 TOKEN_DIVISION;
extern const uint16 TOKEN_TYPE;

extern const char* typen[];
extern const uint16 type_size[];

extern const uint16 TOKEN_NUM;
extern const uint16 TOKEN_ALPHA;

/*
  nterms: 
  nterms are defined in the same way as tokens.
*/
extern const uint16 NTERM_OFFSET;
extern const uint16 NTERM_VOID;
extern const uint16 nt_root;
extern const uint16 nt_expr;
extern const uint16 nt_var_declaration;
extern const uint16 nt_var_assignment;
/* 
   The array nterms[] is the corespondance table for nterms, it is formatted as follows :
   [nterm to define][index of the method associated to the rule in nterm_methods[]() ][tokens / nterms that make up the nterm ][NTERM_VOID][...][NTERM_END];
*/
extern const uint16 nterms[];
/*
  The array nterm_methods[]() gives access to the methods associated to each expression of an nterm. These methods should return a correctly formatted nterm, print the fasm code to the output and do any additional incompiler actions necessary: add a var/function, etc...
*/
extern char* const ( *nterm_methods[])(char* tokens);
