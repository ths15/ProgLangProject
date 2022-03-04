#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>

void DeclStmt(FILE *fp);
void AssignStmt(FILE *fp, FILE *out);
void Expression(FILE *fp, FILE *out);
void Term(FILE *fp, FILE *out);
void Factor(FILE *fp, FILE *out);
void Match(int t, FILE *fp);

#endif
