/*
  Tokens:
  Tokens are defined as a memory zone starting with 16 bits which describe the type, then a byte which describes the size of the value stored by the token, then that value
*/
#include "ftypes.h"
#include "ftokens.h"
#include "fstd.h"

char* inittoken(uint16 token, char dsize, void* data) {
  char* output = fstd_h_malloc( 2 + 1 + dsize);
  fstd_h_memcpy(output, &token, 2);
  uint16 offset = 2;
  *(output + offset) = dsize;
  offset++;
  fstd_h_memcpy(output + offset, data, dsize);
  return output;
}

const uint16 TOKEN_SEMICOLON      = 0;
const uint16 TOKEN_EQUALS         = 1;
const uint16 TOKEN_PLUS           = 2;
const uint16 TOKEN_MINUS          = 3;
const uint16 TOKEN_MULTIPLICATION = 4;
const uint16 TOKEN_DIVISION       = 5;
const uint16 TOKEN_TYPE           = 6;

const char* typen[] = { "char", "uint16", "uint32", NULL };
const uint16 type_size[] = { 1, 2, 4 };

const uint16 TOKEN_NUM            = 7;
const uint16 TOKEN_ALPHA          = 8;

/*
  nterms: 
  nterms are defined in the same way as tokens.
*/
const uint16 NTERM_OFFSET = 512;
const uint16 NTERM_VOID = 65535;
// NTERM_OFFSET + 0 is reserved as the terminating character for nterms
const uint16 nt_root            = 1 + NTERM_OFFSET;
const uint16 nt_expr            = 2 + NTERM_OFFSET;
const uint16 nt_var_declaration = 3 + NTERM_OFFSET;
const uint16 nt_var_assignment  = 4 + NTERM_OFFSET;
const uint16 nt_add_expr        = 5 + NTERM_OFFSET;
/* 
   The array nterms[] is the corespondance table for nterms, it is formatted as follows :
   [nterm to define][index of the method associated to the rule in nterm_methods[]() ][tokens / nterms that make up the nterm ][NTERM_VOID][...][NTERM_END];
*/
const uint16 nterms[] = {
  nt_root, 0,
    nt_expr,
    NTERM_OFFSET,
  nt_root, 0,
    nt_root, nt_expr,
    NTERM_OFFSET,
    
  nt_expr, 1,
    nt_var_assignment, TOKEN_SEMICOLON,
    NTERM_OFFSET,
  nt_expr, 1,
    nt_var_declaration, TOKEN_SEMICOLON,
    NTERM_OFFSET,
    
  nt_var_declaration, 2,
    TOKEN_TYPE, TOKEN_ALPHA,
    NTERM_OFFSET,
    
  nt_var_assignment, 3,
    nt_var_declaration, TOKEN_EQUALS, TOKEN_NUM,
    NTERM_OFFSET,
    
  NTERM_VOID };

/*
  The array nterm_methods[]() gives access to the methods associated to each expression of an nterm. These methods should return a correctly formatted nterm, print the fasm code to the output and do any additional incompiler actions necessary: add a var/function, etc...
*/

static char* nt_root_method(char* tokens) {
  printf("root\n");
  return inittoken(nt_root, 0, NULL);
}

static char* nt_expr_method(char* tokens) {
  printf("expr\n");
  return inittoken(nt_expr, 0, NULL);;
}

static char* nt_var_declaration_method(char* tokens) {
  printf("var declaration\n");
  return inittoken(nt_var_declaration, 0, NULL);
}

static char* nt_var_declaration_assignment_method(char* tokens) {
  printf("declaration + assignment\n");
  return inittoken(nt_var_assignment, 0, NULL);;
}



char* const (*nterm_methods[])(char* tokens) = {
  &nt_root_method,
  &nt_expr_method,
  &nt_var_declaration_method,
  &nt_var_declaration_assignment_method
};
