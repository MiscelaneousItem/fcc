/*
  Tokens:
  Tokens are defined as a memory zone starting with 16 bits which describe the type, then a byte which describes the size of the value stored by the token, then that value
*/
#include "ftypes.h"
#include "ftokens.h"
#include "fstd.h"

char* inittoken(uint16 token, char dsize, void* data);

extern const uint16 TOKEN_ALPHA     ;
extern const uint16 TOKEN_COLON     ;
extern const uint16 TOKEN_BRA_OPEN  ;
extern const uint16 TOKEN_BRA_CLOSE ;
extern const uint16 TOKEN_PIPE      ;
extern const uint16 TOKEN_SEMICOLON ;
extern const uint16 TOKEN_PERCENT   ;
extern const uint16 TOKEN_C_ZONE    ;

/*
  nterms: 
  nterms are defined in the same way as tokens.
*/
extern const uint16 NTERM_OFFSET;
// NTERM_OFFSET + 0 is reserved as the terminating character for nterms
extern const uint16 root            ;
extern const uint16 alpha_list      ;
extern const uint16 ntdef           ;
extern const uint16 rule            ;
extern const uint16 rule_list       ;
extern const uint16 NTERM_VOID      ; // always having NTERM_VOID as the last nterm allows us to know the number of nterms

/*
  The array nterm_methods[]() gives access to the methods associated to each expression of an nterm. These methods should return a correctly formatted nterm, print the fasm code to the output and do any additional incompiler actions necessary: add a var/function, etc...
*/

extern char* const (*nterm_methods[])(char* tokens);

/* 
   The array nterms[] is the corespondance table for nterms, it is formatted as follows :
   [nterm to define][index of the method associated to the rule in nterm_methods[]() ][tokens / nterms that make up the nterm ][NTERM_OFFSET][...][NTERM_VOID];
*/
extern const uint16 nterms[];
