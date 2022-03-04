#include "SymbolTable.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ID    300
#define NUM   301
#define BEGIN 400
#define END   401
#define NOT_FOUND 402
#define ERROR 403
#define INT 404

#define HASHSIZE 25

struct SymbolData
{
	int type;
	char value[];
};

struct SymbolData* hashArray[HASHSIZE];

int hashIndex = 0;

void Insert(int type, char value[])
{
	
	struct SymbolData *newSymbol = (struct SymbolData*) malloc(sizeof(struct SymbolData) + sizeof(char)*strlen(value));
	newSymbol->type = type;
	strcpy(newSymbol->value, value);
	
	int i = 0;
	while(hashArray[i] != NULL)
	{
		i++;
		hashIndex++;
	}	
		
   hashArray[i] = newSymbol;
	
}

int Lookup(char value[])
{
	
	int i = 0;	
   	while(hashArray[i] != NULL) 
	{
      	if(strcmp(hashArray[i]->value, value) == 0)
         return hashArray[i]->type; 
			
      	i++;
		
   	}        
	
   return NOT_FOUND; 
	
}

void Display()
{
	int i = 0;
	while(hashArray[i] != NULL)
	{
		printf("Type: %d Value: %s\n", hashArray[i]->type, hashArray[i]->value);
		i++;
	}	
		
}

