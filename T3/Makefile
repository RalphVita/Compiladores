File=err3_01
Exec=trab3
FolderOut=test/out3
FolderIn=test/in

all: bison flex gcc

bison: parser.y
	bison parser.y

flex: scanner.l
	flex scanner.l
	
gcc: menssagem.h ./entity/funcao.h ./entity/variavel.h ./hash/tables.c scanner.c parser.c 
	gcc -Wall -w -o $(Exec) ./entity/funcao.c ./entity/variavel.c ./hash/tables.c scanner.c parser.c -ly -lm	 

executa:
	./$(Exec) < ./$(FolderIn)/$(File).cm

clean:
	rm -f *.out scanner.c parser.h parser.c $(Exec)

diff:
	./$(Exec) < ./$(FolderIn)/$(File).cm | diff ./$(FolderOut)/$(File).out -
loop:
	$(foreach f, $(sort $(wildcard ./$(FolderIn)/*.cm)), \
	echo $(notdir $(basename $f));\
	./$(Exec) < ./$(FolderIn)/$(notdir $(basename $f)).cm | diff ./$(FolderOut)/$(notdir $(basename $f)).out -;)

