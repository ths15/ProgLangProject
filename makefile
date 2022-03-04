runProgram: main.o LexicalAnalyzer.o Parser.o SymbolTable.o
	gcc main.o LexicalAnalyzer.o Parser.o SymbolTable.o -o runProgram

main.o: main.c
	gcc -c main.c

LexicalAnalyzer.o: LexicalAnalyzer.c LexicalAnalyzer.h
	gcc -c LexicalAnalyzer.c

Parser.o: Parser.c Parser.h
	gcc -c Parser.c

SymbolTable.o: SymbolTable.c SymbolTable.h
	gcc -c SymbolTable.c

clean:
	rm *.o runProgram
	rm *.out

run: 
	./runProgram a1.in 
	./runProgram a2.in
	./runProgram a3.in 
	./runProgram a4.in
	./runProgram a5.in
	./runProgram a6.in
	./runProgram a7.in
	./runProgram a8.in
