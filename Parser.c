#include "Parser.h"

#include <stdio.h>
#include <stdlib.h>

#define ID    300
#define NUM   301
#define BEGIN 400
#define END   401
#define NOT_FOUND 402
#define ERROR 403
#define INT 404

extern int lookahead;
extern int lineNo;

int registerCounter;
char postfix[50] = {0};
int postfixPos = 0;

void DeclStmt(FILE *fp)
{
	StartDecl();
	
	Match(INT, fp);

	if(lookahead == ERROR)
		return;

	Match(ID, fp);
	
	if(lookahead == ERROR)
		return;

	while(lookahead == ',')
	{
		Match(',', fp);
		
		if(lookahead == ERROR)
			return;
		
		Match(ID, fp);
		
		if(lookahead == ERROR)
			return;
	}
	
	if(lookahead == ERROR)
		return;
	
	FinishDecl();
	
	Match(';', fp);
}

void AssignStmt(FILE *fp, FILE *out)
{
	registerCounter = 0;
	
	//printf("R%d = %s\n", registerCounter, ReadLastLexeme());
	
	size_t len = strlen(ReadLastLexeme());
	char *assigned = malloc(len + 1);
	
	memcpy(assigned, ReadLastLexeme(), len + 1);
	
	Match(ID, fp);
	
	//registerCounter++;
	
	if(lookahead == ERROR)
		return;
	
	if(lookahead != '=')
	{
		printf("\nLine %d contains an error <Missing '='>\n", lineNo);
		lookahead = ERROR;
		return;
	}
	else
	{
		Match(lookahead, fp);
	
		if(lookahead == ERROR)
			return;
	
		Expression(fp, out);
	
		if(lookahead == ERROR)
			return;
	
		Match(';', fp);
	
		if(lookahead == ERROR)
			return;
			
		registerCounter--;
		fprintf(out, "%s = R%d\n", assigned, registerCounter);
		
		postfix[postfixPos - 1] = 0;
		fprintf(out, "*****[%s]*****\n", postfix);
		
		int i;
		for(i = 0; i < 50; i++)
		{
			postfix[i] = 0;
		}
		
		postfixPos = 0;
	
	}
}

void Expression(FILE *fp, FILE *out)
{
	Term(fp, out);
	
	if(lookahead == ERROR)
		return;
	
	
	while(lookahead == '+' || lookahead == '-')
	{
		char modifier = lookahead;
		
		Match(lookahead, fp);
	
		if(lookahead == ERROR)
			return;
	
		Term(fp, out);
		
		registerCounter--;
		fprintf(out, "R%d = R%d %c R%d\n", (registerCounter - 1), (registerCounter - 1), modifier, (registerCounter));
		
		postfix[postfixPos] = modifier;
		postfixPos++;
		
		postfix[postfixPos] = ',';
		postfixPos++;
	
		if(lookahead == ERROR)
			return;
	
	}
	
}

void Term(FILE *fp, FILE *out)
{
	Factor(fp, out);
	
	if(lookahead == ERROR)
		return;
	
	
	while(lookahead == '*' || lookahead == '/')
	{
		char modifier = lookahead;
		
		Match(lookahead, fp);
	
		if(lookahead == ERROR)
			return;
	
		Factor(fp, out);
	
		registerCounter--;
		fprintf(out, "R%d = R%d %c R%d\n", (registerCounter - 1), (registerCounter - 1), modifier, (registerCounter));
	
		postfix[postfixPos] = modifier;
		postfixPos++;
		
		postfix[postfixPos] = ',';
		postfixPos++;
	
		if(lookahead == ERROR)
			return;
	
	}
	
}

void Factor(FILE *fp, FILE *out)
{
	
	if(lookahead == ID)
	{
		fprintf(out, "R%d = %s\n", registerCounter, ReadLastLexeme());
		
		size_t len = strlen(ReadLastLexeme());
		char *temp = malloc(len + 1);
	
		memcpy(temp, ReadLastLexeme(), len + 1);
		
		int i;
		for(i = 0; i < len;i++)
		{
			postfix[postfixPos] = temp[i];
			postfixPos++;
		}
		postfix[postfixPos] = ',';
		postfixPos++;
		
		Match(ID, fp);
		registerCounter++;
	}
	else if(lookahead == NUM)
	{
		fprintf(out, "R%d = %s\n", registerCounter, ReadLastLexeme());
		
		size_t len = strlen(ReadLastLexeme());
		char *temp = malloc(len + 1);
	
		memcpy(temp, ReadLastLexeme(), len + 1);
		
		int i;
		for(i = 0; i < len;i++)
		{
			postfix[postfixPos] = temp[i];
			postfixPos++;
		}
		postfix[postfixPos] = ',';
		postfixPos++;
		
		Match(NUM, fp);
		registerCounter++;
	}
	else if(lookahead == '(')
	{
		Match('(', fp);
	
		if(lookahead == ERROR)
			return;
	
		Expression(fp, out);
	
		if(lookahead == ERROR)
			return;
	
		Match(')', fp);
	
		if(lookahead == ERROR)
			return;
	
	}
	else
	{
		printf("\nLine %d contains an error <Expected factor>\n", lineNo);
		lookahead = ERROR; //error statement needed
	}
	
	if(lookahead == ERROR)
		return;
	
	
}

void Match(int t, FILE *fp)
{
	
	if(lookahead == t)
	{
		if(lookahead == END)
		{
			printf("\nSuccess\n\nResults:\n");
			Display();
		}
		else
			lookahead = Lexan(fp);
		
	}
	else
		{
		if(t == BEGIN)
			printf("\nLine %d contains an error <Expected 'begin' statement>\n", lineNo);
		else if(t == END)
			printf("\nLine %d contains an error <Expected 'end.' statement>\n", lineNo);
		else
			printf("\nLine %d contains an error <Expected '%c'>\n", lineNo, t);
		lookahead = ERROR;
		return;
		}
	
}

