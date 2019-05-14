File=c06
Exec=aula8
FolderOut=out8
FolderIn=in

all: bison flex gcc

bison: parser.y
	bison parser.y

flex: scanner.l
	flex scanner.l
	
gcc: ./hash/tables.c scanner.c parser.c 
	gcc -Wall -o $(Exec) ./hash/tables.c scanner.c parser.c -ly -lm	 

executa:
	./$(Exec) < ./$(FolderIn)/$(File).cm

clean:
	rm -f *.out parser.h parser.c $(Exec)

diff:
	./$(Exec) < ./$(FolderIn)/$(File).cm | diff ./$(FolderOut)/$(File).out -
loop:
	$(foreach f, $(sort $(wildcard ./$(FolderIn)/*.cm)), \
	echo $(notdir $(basename $f));\
	./$(Exec) < ./$(FolderIn)/$(notdir $(basename $f)).cm | diff ./$(FolderOut)/$(notdir $(basename $f)).out -;)

