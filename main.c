#include <stdio.h>
#include <stdlib.h>

#include "LexicalAnalyzer.h"
#include "Parser.h"
#include "SymbolTable.h"

#define ID    300
#define NUM   301
#define BEGIN 400
#define END   401
#define NOT_FOUND 402
#define ERROR 403
#define INT 404

int lookahead;

int main(int argc, char *argv[]) 
{
	FILE *fp;
	char name[50];
	strcpy(name, argv[1]);

	//Open file
	fp = fopen(name, "r");
   	if( fp == NULL ) {
      perror("Error in opening file");
      return(-1);
   	}
	
	name[3] = 'o';
	name[4] = 'u';
	name[5] = 't';

	FILE *out;
	out = fopen(name, "w+");

	Insert(BEGIN, "begin");
	Insert(END, "end.");
	Insert(INT, "int");
	
	lookahead = Lexan(fp);
	
	ChangeFirstSearch();
	
	if(lookahead != ERROR)
	{
		Match(BEGIN, fp);
		
		while(lookahead == INT)
			DeclStmt(fp);
		
		while(lookahead != ERROR && lookahead != END)
			AssignStmt(fp, out);
			
		if(lookahead != ERROR)
			Match(END, fp);
		else
			if(remove(name) == 0)
			{
				printf("deleted successfully\n\n");
			}
	}
	
	fclose(out);	
	fclose(fp);
	
	return 0;
}

