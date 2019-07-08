File=c13
Exec=trab5
FolderOut=test/out5
FolderAst=test/ast
FolderIn=test/in

all: bison flex gcc

bison: parser.y
	bison parser.y

flex: scanner.l
	flex scanner.l

gcc: menssagem.h ./ast/ast.h ./entity/funcao.h ./entity/variavel.h ./hash/tables.c ./porting/interpreter.h scanner.c parser.c
	gcc -Wall -w -o $(Exec) ./ast/ast.c ./entity/funcao.c ./entity/variavel.c ./hash/tables.c ./porting/interpreter.c scanner.c parser.c -ly -lm

run:
	./$(Exec) < ./$(FolderIn)/$(File).cm

ast: all
	./$(Exec) -d < ./$(FolderIn)/$(File).cm
	
dot: all
	./$(Exec) -d < ./$(FolderIn)/$(File).cm > out.dot
	dot -Tpdf out.dot -o out.pdf

dot_diff:
	./$(Exec) -d < ./$(FolderIn)/$(File).cm | diff ./$(FolderAst)/$(File).dot -

dot_loop:
	$(foreach f, $(sort $(wildcard ./$(FolderIn)/*.cm)), \
	echo $(notdir $(basename $f));\
	./$(Exec) -d < ./$(FolderIn)/$(notdir $(basename $f)).cm | diff ./$(FolderAst)/$(notdir $(basename $f)).dot -;)

clean:
	rm -f *.out *.dot *.pdf scanner.c parser.h parser.c $(Exec)

diff:
	./$(Exec) < ./$(FolderIn)/$(File).cm | diff ./$(FolderOut)/$(File).out -
loop:
	$(foreach f, $(sort $(wildcard ./$(FolderIn)/*.cm)), \
	echo $(notdir $(basename $f));\
	./$(Exec) < ./$(FolderIn)/$(notdir $(basename $f)).cm | diff ./$(FolderOut)/$(notdir $(basename $f)).out -;)

up: clean
	git add .
	git commit -m "commit automático"
	git push origin master
