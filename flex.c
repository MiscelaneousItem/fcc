#include "ftypes.h"
#include "fstd.h"
#include "ftokens.h"
#include "flex.h"

static short isin(char a, char* b) {
  // returns 1 if a is present in b, with strlen(b) < 65
  uint32 i = 0;
  while(b[i] != '\0') {
    fstd_assert(i < 65);
    if(a == b[i]) return 1;
    i++;
  }
  return 0;
}

char* fcc_lex(char* code_input) {

  fstd_assert(code_input != NULL);

  static uint64 i = 0;
  uint32 j = 0;
  char* buffer = fstd_h_calloc(65, 1);
  while(isin(code_input[i], " \n\t") && code_input[i] != '\0') { // get to the first non space/newline/tab character
    i++;
  }
  while(code_input[i] != '\0') {
    if(isin(code_input[i], " ;+-*/=\n\t") || code_input[i] == '\0') {
      if(isin(code_input[i], ";+-*/=") && j == 0) {
	char c = code_input[i];
	#ifdef ULTRA_VERBOSE
	printf("%c\n", (char)c);
	#endif
	i++;
	fstd_h_free(buffer);
	if(c == ';') return inittoken(TOKEN_SEMICOLON,      0, NULL);
	if(c == '+') return inittoken(TOKEN_PLUS,           0, NULL);
	if(c == '-') return inittoken(TOKEN_MINUS,          0, NULL);
	if(c == '*') return inittoken(TOKEN_MULTIPLICATION, 0, NULL);
	if(c == '/') return inittoken(TOKEN_DIVISION,       0, NULL);
	if(c == '=') return inittoken(TOKEN_EQUALS,         0, NULL);
      }
      #if ULTRA_VERBOSE
      printf("%s\n", buffer);
      #endif
      if(fstd_isdigit(buffer[0])) {
	uint32 alpha = atoi(buffer);
	fstd_h_free(buffer);
	return inittoken( TOKEN_NUM, sizeof(uint32), &alpha);
      }
      
      if(fstd_isalpha(buffer[0])) {
	uint32 k = 0;
	while(typen[k]) {
	  if(fstd_strcmp(typen[k], buffer) == 0) {
	    fstd_h_free(buffer);
	    return inittoken( TOKEN_TYPE, sizeof(uint16), &k);
	  }
	  k++;
	}	
	char* token = inittoken( TOKEN_ALPHA, fstd_strlen(buffer) + 1, buffer);
	fstd_h_free(buffer);
	return token;
      }
      
    }
    buffer[j] = code_input[i];
    j++;
    i++;
    fstd_assert(i < fstd_pow(8 * sizeof(uint32), 2));
    fstd_assert(j < 65);
    
  }
  fstd_h_free(buffer);
  return NULL;
}
