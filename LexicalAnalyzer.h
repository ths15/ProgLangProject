#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

#include <stdio.h>

int Lexan(FILE *fp);
void FinishDecl();
void StartDecl();
void ChangeFirstSearch();
void SetLastLexeme(char fittedLexeme[], int idSize);
char * ReadLastLexeme();

#endif
