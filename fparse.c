#include "flex.h"
#include "ftypes.h"
#include "fstd.h"
#include "ftokens.h"
#include "fparse.h"

static uint16 get_nth_rule(uint16 index) { // returns the number of the rule which can be found at nterms[index]
  uint16 nth_rule = 0;
  for(uint32 i = 0; i < index; i++) {
    if(nterms[i] == NTERM_OFFSET) nth_rule++;
  }
  return nth_rule;
}

static uint16 min(uint16 a, uint16 b) {
  if(a < b) return a;
  return b;
}

static char cmp_tokens(const uint16* a, const uint16* b, const uint32 n) { // compares two arrays of uint16
  for(uint32 i = 0; i < n; i++) {
    if(a[i] != b[i]) return 0;
  }
  return 1;
}

/* 
   get_sr_conflicts() searches through the grammar as defined in nterms[] to find shift/reduce conflicts and returns an array of arrays formattes like so :
   [index of conflicting token in nterms[]][...][0]
   as such a rule with no S/R conflicts would just be : 
   [0]
*/
static uint16** get_sr_conflicts() {
  uint16** output = calloc(50, sizeof(uint16*)); // arbitrary constant should be replaced with number of rules 
  uint16 i = 0;
  uint16 ith_nterm = 0;
  while(nterms[i] != NTERM_VOID) {
    uint16 nconflicts = 0;
    uint16 j = 0;
    while(nterms[i + j] != NTERM_OFFSET) {
      j++;
    }
    uint16 k = 0;
    uint16 kth_nterm = 0;
    while(nterms[k] != NTERM_VOID) {
      uint16 l = 0;
      while(nterms[k + l] != NTERM_OFFSET) {
	l++;
      }
      if(i != k && cmp_tokens(nterms + i + 2, nterms + k + 2, min(j, l) - 2)) {
       	output[ith_nterm] = realloc(output[ith_nterm], (nconflicts + 1) * sizeof(uint16));
	output[ith_nterm][nconflicts] = k + min(j, l);
	printf("output[%i][%i] = %i (%i) -> %i\n", ith_nterm, nconflicts, output[ith_nterm][nconflicts], kth_nterm, nterms[output[ith_nterm][nconflicts]]);
	nconflicts++;
      }
      k += l + 1;
      kth_nterm++;
    }
    
    output[ith_nterm] = realloc(output[ith_nterm], (nconflicts + 1) * sizeof(uint16));
    output[ith_nterm][nconflicts] = 0; // we use 0 here because we consider that no rule should have a S/R conflict with root
    
    i += j + 1;
    ith_nterm++;
  }
  return output;
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

void fcc_parse(char* code) {

  uint16** sr_conflict_table = get_sr_conflicts();
  
  uint32 ast_s = 0;
  char* ast = NULL;
  
  uint32 ntokens = 0;
  uint16* tokentypes = NULL;
  
  char* ctoken  = fcc_lex(code);
  char* nxtoken = fcc_lex(code);

  while(ctoken) {
    ast = fstd_h_realloc(ast, ast_s + 3 + ctoken[2]);
    fstd_h_memcpy(ast + ast_s, ctoken, 3 + ctoken[2]);
    ast_s += 3 + ctoken[2];

    tokentypes = fstd_h_realloc(tokentypes, (ntokens + 1) * sizeof(uint16));
    fstd_h_memcpy(tokentypes + ntokens, ctoken, sizeof(uint16));
    ntokens++;

    for(int32 k = 0; k < ntokens; k++) {
      uint16 cnterm = get_nterm(tokentypes + k, ntokens - k);

      
      if(nterms[cnterm] != NTERM_VOID) {

	
	char sr_conflict_resolved = 1;

	uint16 index = get_nth_rule(cnterm);
	
	if(sr_conflict_table[index][0] != 0 ) { // if the current rule has sr conflicts
	  uint16 i = 0;
	  uint16 nxtoken_id = 0;
	  if(nxtoken) fstd_h_memcpy(&nxtoken_id, nxtoken, 2);
#ifdef PARSE_ULTRA_VERBOSE
	  printf("sr_conflict : %i ; nx_token = %i\n", index, nxtoken_id);
#endif
	  while(sr_conflict_table[index][i] != 0) { // search through the S\R conflict table and check whether the next token is contained in the list of conflicting tokens
#ifdef PARSE_ULTRA_VERBOSE
	    printf("sr_conflict_table[%i][%i] = %i\n", index, i, sr_conflict_table[index][i]);
#endif
	    if( nterms[sr_conflict_table[index][i]] == nxtoken_id) {
#ifdef PARSE_VERBOSE
	      printf("forced shift due to conflict\n");
#endif
	      sr_conflict_resolved = 0; // if it is we should not reduce, but shift
	      break;
	    }
	    i++;
	  }
	}

	if(sr_conflict_resolved) {
	
	  tokentypes[k] = nterms[cnterm];
	  ntokens = k + 1;
    
	  ast_s = 0;
	  for(uint32 j = 0; j < k; j++) {
	    ast_s += 3 + ast[ast_s + 2];
	  }
	  char* nnterm = nterm_methods[nterms[cnterm + 1]](ast + ast_s);
	
	  ast = fstd_h_realloc(ast, ast_s + 3 + nnterm[2]);
	  fstd_h_memcpy(ast + ast_s, nnterm, 3 + nnterm[2]);
	
	  ast_s += 3 + nnterm[2];
	
#ifdef PARSE_ULTRA_VERBOSE
	  printf("ast_s = %d\n", ast_s);
	  for(uint32 u = 0; u < ast_s; u++) {
	    printf("%d ", ast[u]);
	  }
	  printf("\n");
#endif

	  free(nnterm);
	  ntokens = k + 1;
	  k = -1;

 	}
      }
      
    }
#ifdef PARSE_VERBOSE
    printf("tokens : ");
    for(uint32 u = 0; u < ntokens; u++) {
      printf("%d ", tokentypes[u]);
    }
    printf("\n"); 
#endif
    fstd_h_free(ctoken);
    ctoken = nxtoken;
    nxtoken = fcc_lex(code);
  }
  free(ast);
  free(tokentypes);
}
