/*
  The parser (fcc_parse) generates the Abstract Syntax Tree (AST) from the tokens returned by the the lexer (fcc_lex).
  It compacts the tokens into nterms (non terminals), nterms and the ast are described in detail in fgrammar.h .
*/

void fcc_parse(char* code);
