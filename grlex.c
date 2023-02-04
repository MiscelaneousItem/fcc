#include "ftypes.h"
#include "fstd.h"
#include "grtokens.h"
#include "grlex.h"

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
    if(isin(code_input[i], " \n\t:;|{%") || code_input[i] == '\0') {
      if(isin(code_input[i], ":;|{%") && j == 0) {
	char c = code_input[i];
	#ifdef LEX_VERBOSE
	printf("%c\n", (char)c);
	#endif
	i++;
	fstd_h_free(buffer);
	if(c == ':') return inittoken(TOKEN_COLON,      0, NULL);
	if(c == ';') return inittoken(TOKEN_SEMICOLON,  0, NULL);
	if(c == '|') return inittoken(TOKEN_PIPE,       0, NULL);
	if(c == '%') return inittoken(TOKEN_PERCENT,    0, NULL);
	
	// code zone handling
	uint16 zone_size = 100;
	char* code_zone = calloc(zone_size, 1);
	code_zone[0] = '{';
	j = 1;
	uint16 open = 1;
	while(open) {
	  i++;
	  if(j > zone_size) code_zone = realloc(code_zone, zone_size += 100);
	  if(code_input[i] == '{') open++;
	  else if(code_input[i] == '}') open--;
	  code_zone[j] = code_input[i];
	  j++;
	}
	i++;
#ifdef LEX_VERBOSE
	printf("code_zone %s\n", code_zone);
#endif
	return inittoken(TOKEN_C_ZONE, zone_size, code_zone);
	
      }
#ifdef LEX_VERBOSE
      printf("%s\n", buffer);
#endif      
      
      if(fstd_isalpha(buffer[0])) {	
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
