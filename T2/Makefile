File=c02
Exec=trab2
FolderOut=out2

all: bison flex gcc

bison: parser.y
	bison parser.y

flex: scanner.l
	flex scanner.l
	
gcc: scanner.c parser.c
	gcc -Wall -o $(Exec) scanner.c parser.c -ly -lm	 

executa:
	./$(Exec) < ./in/$(File).cm

clean:
	rm -f *.out *.h *.c $(Exec)

diff:
	./$(Exec) < ./in/$(File).cm | diff ./$(FolderOut)/$(File).out -
loop:
	$(foreach f, $(sort $(wildcard ./in/*.cm)), \
	echo $(notdir $(basename $f));\
	./$(Exec) < ./in/$(notdir $(basename $f)).cm | diff ./$(FolderOut)/$(notdir $(basename $f)).out -;)

