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
    if(j == ntokens + 2 && cmp_tokens(tokens, nterms + i + 2, ntokens)) return i;
    i += j + 1;
  }
  return i;
}

static uint16 min(uint16 a, uint16 b) {
  if(a < b) return a;
  return b;
}

/* 
   get_sr_conflicts() searches through the grammar as defined in nterms[] to find shift reduce conflicts.
*/

static void get_sr_conflicts() {
  uint16 i = 0;
  uint16 ith_nterm = 0;
  while(nterms[i] != NTERM_VOID) {
    uint16 j = 0;
    while(nterms[i + j] != NTERM_OFFSET) {
      j++;
    }
    uint16 k = i + j;
    uint16 kth_nterm = ith_nterm;
    while(nterms[k] != NTERM_VOID) {
      uint16 l = 0;
      while(nterms[k + l] != NTERM_OFFSET) {
	l++;
      }
      if(cmp_tokens(nterms + i + 2, nterms + k + 2, min(j, l) - 2)) {
	fprintf(stderr, "shift reduce conflict between rules %d and %d\n", ith_nterm, kth_nterm);
	exit(1);
      }
      k += l + 1;
      kth_nterm++;
    }
    i += j + 1;
    ith_nterm++;
  }
}

void fcc_parse(char* code) {

  get_sr_conflicts();
  
  uint32 ast_s = 0;
  char* ast = NULL;
  
  uint32 ntokens = 0;
  uint16* tokentypes = NULL;
  
  char* ctoken = fcc_lex(code);

  while(ctoken) {
    ast = fstd_h_realloc(ast, ast_s + 3 + ctoken[2]);
    fstd_h_memcpy(ast + ast_s, ctoken, 3 + ctoken[2]);

    tokentypes = fstd_h_realloc(tokentypes, (ntokens + 1) * sizeof(uint16));
    fstd_h_memcpy(tokentypes + ntokens, ctoken, sizeof(uint16));
    ntokens++;
    for(uint32 i = 0; i < ntokens; i++) {
	printf("%i ", tokentypes[i]);
    }
    printf("\n");
    for(int32 k = 0; k < ntokens; k++) {
      uint16 cnterm = get_nterm(tokentypes + k, ntokens - k);
      if(nterms[cnterm] != NTERM_VOID) {
	
	tokentypes[k] = nterms[cnterm];
	char* nnterm = nterm_methods[nterms[cnterm + 1]](ast + ast_s);
	
	for(uint32 j = 0; j < k; j++) {
	  tokentypes[1 + j + k] = tokentypes[ntokens - k + j];
	}
	ast_s = 0;
	for(uint32 j = 0; j < k; j++) {
	  ast_s += 3 + ast[ast_s + 2];
	}
	ast = fstd_h_realloc(ast, ast_s + 3 + nnterm[2]);
	memcpy(ast + ast_s, nnterm, 3 + nnterm[2]);
	free(nnterm);
	ntokens = k + 1;
	k = -1;
      }
    }
    
    ast_s += 3 + ctoken[2];
    fstd_h_free(ctoken);
    ctoken = fcc_lex(code);
  }
  free(ast);
  free(tokentypes);
}
