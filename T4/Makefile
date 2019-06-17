File=c01
Exec=trab4
FolderOut=test/out4
FolderIn=test/in

all: bison flex gcc

bison: parser.y
	bison parser.y

flex: scanner.l
	flex scanner.l
	
gcc: menssagem.h ./ast/ast.h ./entity/funcao.h ./entity/variavel.h ./hash/tables.c scanner.c parser.c 
	gcc -Wall -w -o $(Exec) ./ast/ast.c ./entity/funcao.c ./entity/variavel.c ./hash/tables.c scanner.c parser.c -ly -lm	 

executa:
	./$(Exec) < ./$(FolderIn)/$(File).cm

dot:
	./$(Exec) < ./$(FolderIn)/$(File).cm > out.dot

clean:
	rm -f *.out scanner.c parser.h parser.c $(Exec)

diff:
	./$(Exec) < ./$(FolderIn)/$(File).cm | diff ./$(FolderOut)/$(File).out -
loop:
	$(foreach f, $(sort $(wildcard ./$(FolderIn)/*.cm)), \
	echo $(notdir $(basename $f));\
	./$(Exec) < ./$(FolderIn)/$(notdir $(basename $f)).cm | diff ./$(FolderOut)/$(notdir $(basename $f)).out -;)

