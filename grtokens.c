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

const uint16 TOKEN_ALPHA       = 0;
const uint16 TOKEN_COLON       = 1;
const uint16 TOKEN_PIPE        = 2;
const uint16 TOKEN_SEMICOLON   = 3;
const uint16 TOKEN_PERCENT     = 4;
const uint16 TOKEN_C_ZONE      = 5;

/*
  nterms: 
  nterms are defined in the same way as tokens.
*/
const uint16 NTERM_OFFSET = 512;
// NTERM_OFFSET + 0 is reserved as the terminating character for nterms
const uint16 root            = 1 + NTERM_OFFSET;
const uint16 alpha_list      = 2 + NTERM_OFFSET;
const uint16 ntdec           = 3 + NTERM_OFFSET;
const uint16 ntdef           = 4 + NTERM_OFFSET;
const uint16 rule            = 5 + NTERM_OFFSET;
const uint16 rule_list       = 6 + NTERM_OFFSET;
const uint16 token_def       = 7 + NTERM_OFFSET;
const uint16 NTERM_VOID      = 8 + NTERM_OFFSET; // always having NTERM_VOID as the last nterm allows us to know the number of nterms

/*
  The array nterm_methods[]() gives access to the methods associated to each expression of an nterm. These methods should return a correctly formatted nterm, print the fasm code to the output and do any additional incompiler actions necessary: add a var/function, etc...
*/

char* _root(char* tokens) {
  return inittoken(root, 0, NULL);
}

char* _alpha_list__TOKEN_ALPHA(char* tokens) {
  return inittoken(alpha_list, tokens[2], tokens + 3);
}

char* _alpha_list__alpha_list_TOKEN_ALPHA(char* tokens) {
  char* buffer = calloc(tokens[2] + tokens[tokens[2] + 5], 1); // -1 to compensate for the extra terminal char but +1 for the space separator
  strncpy(buffer, tokens + 3, tokens[2]);
  buffer[strlen(buffer)] = ' ';
  strncat(buffer, tokens + tokens[2] + 6, tokens[tokens[2] + 5]);
  return inittoken(alpha_list, strlen(buffer) + 1, buffer);
}

char* current_dec;

char* _nt_dec(char* tokens) {
  free(current_dec);
  current_dec = calloc(tokens[2] + 1, 1);
  strncpy(current_dec, tokens + 3, tokens[2]);
  return inittoken(ntdec, 0, NULL); 
}

char* _nt_def(char* tokens) {
  return inittoken(ntdef, 0, NULL); 
}

uint16 rule_function_index = 0;

char* _rule(char* tokens) {
  char* rule_function_name = calloc(strlen(current_dec) + tokens[2] + 3, 1);
  strncpy(rule_function_name, current_dec, strlen(current_dec));
  strcat(rule_function_name, "__");
  char* buffer = calloc(tokens[2] + 1, 1);
  memcpy(buffer, tokens + 3, tokens[2]);
  for(uint16 i = 0; i < tokens[2]; i++) {
    if(buffer[i] == ' ') buffer[i] = '_'; 
  }
  strncat(rule_function_name, buffer, tokens[2]);
  free(buffer);
  buffer = calloc(tokens[ tokens[2] + 5 ] + 1, 1);
  memcpy(buffer, tokens + tokens[2] + 6, tokens[tokens[2] + 5]);

  char* rule_function = calloc(strlen(rule_function_name) + strlen(buffer) + strlen("char* (char* tokens)") + 1, 1);
  strcpy(rule_function, "char* ");
  strcat(rule_function, rule_function_name);
  strcat(rule_function, "(char* tokens)");
  strcat(rule_function, buffer);

  free(buffer);
  buffer = calloc(tokens[2] + 1, 1);
  memcpy(buffer, tokens + 3, tokens[2]);
  for(uint16 i = 0; i < tokens[2]; i++) {
    if(buffer[i] == ' ') buffer[i] = ','; 
  }
  char* rule_function_index_a = calloc(100, 1);
  snprintf(rule_function_index_a, 100, "%d", rule_function_index);
  char* rule_definition = calloc(strlen(current_dec) + strlen(rule_function_index_a) + strlen(buffer) + strlen(",,,NTERM_OFFSET,") + 1, 1);
  
  strcpy(rule_definition, current_dec);
  strcat(rule_definition, ",");
  strcat(rule_definition, rule_function_index_a);
  strcat(rule_definition, ",");
  strcat(rule_definition, buffer);
  strcat(rule_definition, ",NTERM_OFFSET,");
    
  rule_function_index++;

  printf("%s\n", rule_function);
  printf("%s\n", rule_definition);
  
  return inittoken(rule, 0, NULL);
}

uint16 token_count = 0;

char* _token_def(char* tokens) {
  char* token_count_a = calloc(100, 1);
  snprintf(token_count_a, 100, "%d", token_count);
  char* definition = calloc(tokens[2] + strlen("const uint16  = ;") + strlen(token_count_a), 1);
  
  strcat(definition, "const uint16 "); 
  strncat(definition, tokens + 3, tokens[2]);
  strcat(definition, " = ");
  strcat(definition, token_count_a);
  strcat(definition, ";");

  char* declaration = calloc(tokens[2] + strlen("extern const uint16;") + 1, 1);
  
  strcat(declaration, "extern const uint16 "); 
  strncat(declaration, tokens + 3, tokens[2]);
  strcat(declaration, ";");
  
  token_count++;

  printf("%s\n", definition); 
  printf("%s\n", declaration);
  
  return inittoken(token_def, 0, NULL);
}

char* _rule_list(char* tokens) {
  return inittoken(rule_list, 0, NULL);
}

char* const (*nterm_methods[])(char* tokens) = {
  &_root,
  &_alpha_list__TOKEN_ALPHA,
  &_alpha_list__alpha_list_TOKEN_ALPHA,
  &_nt_dec,
  &_nt_def,
  &_rule,
  &_rule_list,
  &_token_def,
};

/* 
   The array nterms[] is the corespondance table for nterms, it is formatted as follows :
   [nterm to define][index of the method associated to the rule in nterm_methods[]() ][tokens / nterms that make up the nterm ][NTERM_OFFSET][...][NTERM_VOID];
*/
const uint16 nterms[] = {
  
  root,        0,
    token_def, TOKEN_PERCENT, TOKEN_C_ZONE, TOKEN_PERCENT, ntdef,
    NTERM_OFFSET,

  root,        0,
    root, ntdef,
    NTERM_OFFSET,

  ntdec,       3,
    TOKEN_ALPHA, TOKEN_COLON,
    NTERM_OFFSET,

  ntdef,       4,
    ntdec, rule_list, TOKEN_SEMICOLON,
    NTERM_OFFSET,

  rule_list,   6,
    rule,
    NTERM_OFFSET,

  rule_list,   6,
    rule_list, TOKEN_PIPE, rule,
    NTERM_OFFSET,

  rule,        5,
    alpha_list, TOKEN_C_ZONE, 
    NTERM_OFFSET,

  alpha_list,  1,
    TOKEN_ALPHA,
    NTERM_OFFSET,

  alpha_list,  2,
    alpha_list, TOKEN_ALPHA,
    NTERM_OFFSET,

  token_def,   7,
    TOKEN_ALPHA, TOKEN_SEMICOLON,
    NTERM_OFFSET,
    
  NTERM_VOID };
