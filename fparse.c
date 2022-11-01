#include "flex.h"
#include "ftypes.h"
#include "fstd.h"
#include "ftokens.h"
#include "fparse.h"

static char cmp_tokens(const uint16* a, const uint16* b, const uint32 n) {
  for(uint32 i = 0; i < n; i++) {
    if(a[i] != b[i]) return 0;
  }
  return 1;
}

/* get_nterm() searches through the corespondance table and returns the index of the nterm it finds or NTERM_VOID if it finds none*/
static uint16 get_nterm(uint16* tokens, uint16 ntokens) {
  uint16 i = 0;
  while(nterms[i] != NTERM_VOID) {
    uint16 j = 0;
    while(nterms[i + j] != NTERM_OFFSET) {
      j++;
    }
    if(j == ntokens + 1 && cmp_tokens(tokens, nterms + i + 1, ntokens)) return i;
    i += j + 1;
  }
  return NTERM_VOID;
}

void fcc_parse(char* code) {
  uint32 ast_s = 0;
  char* ast = NULL;
  
  uint32 ntokens = 0;
  uint16* tokentypes = NULL;

  char* ctoken = fcc_lex(code);

  while(ctoken) {
    ast = fstd_h_realloc(ast, ast_s + 3 + ctoken[2]);
    fstd_h_memcpy(ast + ast_s, ctoken, 3 + ctoken[2]);
    ast_s += 3 + ctoken[2];

    tokentypes = fstd_h_realloc(tokentypes, (ntokens + 1) * sizeof(uint16));
    fstd_h_memcpy(tokentypes + ntokens, ctoken, sizeof(uint16));
    ntokens++;
    for(uint32 i = 0; i < ntokens; i++) {
	printf("%i ", tokentypes[i]);
    }
    printf("\n");
    fstd_h_free(ctoken);
    for(int32 k = 0; k < ntokens; k++) {
      uint16 cnterm = get_nterm(tokentypes + k, ntokens - k);
      if(cnterm != NTERM_VOID) {
	tokentypes[k] = nterms[cnterm];
	for(uint32 j = 0; j < k; j++) {
	  tokentypes[1 + j + k] = tokentypes[ntokens - k + j];
	}
	ntokens = k + 1;
	printf("%i : ", cnterm);
	for(uint32 i = 0; i < ntokens; i++) {
	  printf("%i ", tokentypes[i]);
	}
	printf("\n");
	k = -1;
      }
    }

    ctoken = fcc_lex(code);
  }
  for(uint32 i = 0; i < ntokens; i++) {
    printf("%i ", tokentypes[i]);
  }
  printf("\n");
  free(ast);
}
