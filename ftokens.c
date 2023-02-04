/*
  Tokens:
  Tokens are defined as a memory zone starting with 16 bits which describe the type, then a byte which describes the size of the value stored by the token, then that value
*/
#include "ftypes.h"
#include "ftokens.h"
#include "fstd.h"

char* inittoken(uint16 token, char dsize, void* data) {
  char* output = fstd_h_malloc(3 + dsize);
  fstd_h_memcpy(output, &token, 2);
  *(output + 2) = dsize;
  fstd_h_memcpy(output + 3, data, dsize);
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
// NTERM_OFFSET + 0 is reserved as the terminating character for nterms
const uint16 nt_root            = 1 + NTERM_OFFSET;
const uint16 nt_expr            = 2 + NTERM_OFFSET;
const uint16 nt_var_declaration = 3 + NTERM_OFFSET;
const uint16 nt_var_assignment  = 4 + NTERM_OFFSET;

const uint16 nt_mul_expr        = 5 + NTERM_OFFSET;
const uint16 nt_add_expr        = 6 + NTERM_OFFSET;
const uint16 nt_num_expr        = 7 + NTERM_OFFSET;
const uint16 NTERM_VOID         = 8 + NTERM_OFFSET; // always having NTERM_VOID as the last nterm allows us to know the number of nterms

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

static char* nt_var_declaration_assignment(char* tokens) {
  printf("declaration + assignment\n");
  return inittoken(nt_var_assignment, 0, NULL);
}

static char* nt_num__token(char* tokens) {
  uint32 alpha = 0;
  fstd_h_memcpy(&alpha, tokens + 3, tokens[2]);
  return inittoken(nt_num_expr, 4, tokens + 3);
}

static char* nt_mul__multiply(char* tokens) {
  printf("mul op\n");
  uint32 alpha = 0;
  fstd_h_memcpy(&alpha, tokens + 3, tokens[2]);
  uint32 beta = 0;
  fstd_h_memcpy(&beta, tokens + 9 + tokens[2], tokens[8 + tokens[2]]);
  alpha *= beta;
  return inittoken(nt_mul_expr, tokens[2], &alpha);
}

static char* nt_mul__divide(char* tokens) {
  printf("div op\n");
  uint32 alpha = 0;
  fstd_h_memcpy(&alpha, tokens + 3, tokens[2]);
  uint32 beta = 0;
  fstd_h_memcpy(&beta, tokens + 9 + tokens[2], tokens[8 + tokens[2]]);
  alpha /= beta;
  return inittoken(nt_mul_expr, tokens[2], &alpha);
}

static char* nt_add__plus(char* tokens) {
  printf("plus op\n");
  uint32 alpha = 0;
  fstd_h_memcpy(&alpha, tokens + 3, tokens[2]);
  uint32 beta = 0;
  fstd_h_memcpy(&beta, tokens + 9 + tokens[2], tokens[8 + tokens[2]]);
  alpha += beta;
  return inittoken(nt_add_expr, tokens[2], &alpha);

}

char* const (*nterm_methods[])(char* tokens) = {
  &nt_root_method,
  &nt_expr_method,
  &nt_var_declaration_method,
  &nt_var_declaration_assignment,
  &nt_num__token,
  &nt_mul__multiply,
  &nt_mul__divide,
  &nt_add__plus
};

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
  nt_expr, 1,
    nt_add_expr, TOKEN_SEMICOLON,
    NTERM_OFFSET,
    
  nt_var_declaration, 2,
    TOKEN_TYPE, TOKEN_ALPHA,
    NTERM_OFFSET,
    
  nt_var_assignment, 3,
    nt_var_declaration, TOKEN_EQUALS, TOKEN_NUM,
    NTERM_OFFSET,
    
  nt_num_expr, 4,
    TOKEN_NUM,
    NTERM_OFFSET,

  nt_mul_expr, 4,
    nt_num_expr,
    NTERM_OFFSET,
    
  nt_mul_expr, 5,
    nt_mul_expr, TOKEN_MULTIPLICATION, nt_num_expr,
    NTERM_OFFSET,

  nt_mul_expr, 6,
    nt_mul_expr, TOKEN_DIVISION, nt_num_expr,
    NTERM_OFFSET,
    
  nt_add_expr, 4,
    nt_mul_expr,
    NTERM_OFFSET,

  nt_add_expr, 7,
    nt_add_expr, TOKEN_PLUS, nt_add_expr,
    NTERM_OFFSET,
    
    
  NTERM_VOID };
