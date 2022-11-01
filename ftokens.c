/*
  Tokens:
  Tokens are defined as a memory zone starting with 16 bits which describe the type, then a byte which describes the size of the value stored by the token, then that value
*/
#include "ftypes.h"
#include "ftokens.h"

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
   [nterm to define][tokens / nterms that make up the nterm ][NTERM_VOID][...][NTERM_END];
*/
const uint16 nterms[] = {
  nt_root,
    nt_expr,
    NTERM_OFFSET,
  nt_root,
    nt_root, nt_expr,
    NTERM_OFFSET,
    
  nt_expr,
    nt_var_assignment, TOKEN_SEMICOLON,
    NTERM_OFFSET,
  nt_expr,
    nt_var_declaration, TOKEN_SEMICOLON,
    NTERM_OFFSET,
    
  nt_var_declaration,
    TOKEN_TYPE, TOKEN_ALPHA,
    NTERM_OFFSET,
    
  nt_var_assignment,
    nt_var_declaration, TOKEN_EQUALS, TOKEN_NUM,
    NTERM_OFFSET,
    
  NTERM_VOID };

/*
  The array nterm_methods[]() gives access to the methods associated to each expression of an nterm. These methods should return a correctly formatted nterm, print the fasm code to the output and do any additional incompiler actions necessary: add a var/function, etc...
*/
const char* nt_var_assignment_method(char* tokens, uint16 ntokens) {
  return NULL;
}

const char* (*nterm_methods[])(char* tokens, uint16 ntokens) = { &nt_var_assignment_method };
