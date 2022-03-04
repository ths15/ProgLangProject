#include "LexicalAnalyzer.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ID    300
#define NUM   301
#define BEGIN 400
#define END   401
#define NOT_FOUND 402
#define ERROR 403
#define INT 404

int lineNo = 1;
bool finishedDecl = true;
bool firstSearch = true;
char lastLexeme[50] = {0};

int Lexan(FILE *fp)
{
	char ch;
	char idLexeme[25] = {0};
	char numLexeme[25] = {0};
	
	while(true)
	{
		
		ch = getc(fp);
		
		if(ch == ' ' || ch == '\t') //checks if 'ch' is a space or tab
		{
			//Nothing should occur here
		}
		else if(ch == '~')   //Checks if line is commented
		{
			while(ch != '\n')
				ch = getc(fp);
				
			lineNo++;
		}
		else if(ch == '\n') //checks if 'ch' is an endline character
			lineNo++;
		else if(isdigit(ch)) //checks is 'ch' is a digit
		{
			int numLexemePos = 0;
			
			while(isdigit(ch))
			{
				numLexeme[numLexemePos] = ch;
				
				numLexemePos++;
				
				ch = getc(fp);
			}
			
			SetLastLexeme(numLexeme, numLexemePos);
				
			ungetc(ch, fp);
			
			return NUM;
		}
		else if(isalpha(ch)) //checks if 'ch' is a letter
		{
			
			int idLexemePos = 0;
			int consecUnderscore = 0;
			
			//get identifier into idLexeme
			while(isalpha(ch) || isdigit(ch) || ch == '_' || ch == '.')
			{
				idLexeme[idLexemePos] = ch;
				//printf("");
				if(ch == '_')
					consecUnderscore++;
				else
					consecUnderscore = 0;
					
				if(consecUnderscore > 1)
				{
					printf("\nLine %d contains an error <Terms can not have consecutive '_'>\n", lineNo);
					return ERROR;
				}
				
				idLexemePos++;
				ch = getc(fp);
			}
			
			ungetc(ch, fp);
			
			if(idLexeme[idLexemePos - 1] == '_')   //Check if last position is an underscore
			{
				printf("\nLine %d contains an error <Last character of %s was an underscore>\n", lineNo, idLexeme);
				return ERROR;
			}
			else
			{	
				char fittedLexeme[50] = {0};
				
				int j;
				for(j = 0; j < idLexemePos; j++)
					fittedLexeme[j] = idLexeme[j]; 
				
				int type = Lookup(fittedLexeme);
				if(type == NOT_FOUND) //must be adjusted for undefined after declaration statements
				{
					if(firstSearch)
						return ID;
					if(!finishedDecl)
					{
						//insert value into symbolTable
						Insert(ID, fittedLexeme);
						SetLastLexeme(fittedLexeme, idLexemePos);
						return ID;
					}
					if(finishedDecl)
					{
						printf("\nLine %d contains an error <Variable %s is undefined>\n", lineNo, fittedLexeme);
						return ERROR;
					}
				}
				else if(!finishedDecl)
				{
					printf("\nLine %d contains an error <Variable %s is being redefined>\n", lineNo, fittedLexeme);
					return ERROR;
				}
				else
				{
					SetLastLexeme(fittedLexeme, idLexemePos);
					return type;
				}
			}
			
		}
		else if(ch == EOF) //Checks if 'ch' is the end of file
		{
			printf("\nLine %d contains an error <Expected 'end.' statemtn>\n", lineNo);
			return ERROR;
		}
		else if(ch == '_')
		{
			printf("\nLine %d contains an error <Terms can't start with '_'>\n", lineNo);
			return ERROR;
		}
		else
			return ch;
	
	}
}

void FinishDecl()
{
	finishedDecl = true;
}

void StartDecl()
{
	finishedDecl = false;
}
void ChangeFirstSearch()
{
	firstSearch = false;
}

void SetLastLexeme(char fittedLexeme[], int idSize)
{
	int i = 0;
	for(i = 0; i < 50; i++)
	{
		lastLexeme[i] = 0;
	}
	
	for(i = 0; i < idSize; i++)
	{
		lastLexeme[i] = fittedLexeme[i];
	//	printf("\nLastLexeme[%d] = %c\n", i, lastID[i]);
	}
}

char * ReadLastLexeme()
{
	return lastLexeme;
}
