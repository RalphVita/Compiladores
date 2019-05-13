File=c02
Exec=trab1

all: compila

compila:
	 flex -o executavel.c scanner.l
	 gcc executavel.c -o $(Exec) -lfl
	 

executa:
	./$(Exec) < ./in/$(File).cm

clean:
	rm -f *.out *.c $(Exec)

diff:
	./$(Exec) < ./in/$(File).cm | diff ./out1/$(File).out -
loop:
	$(foreach f, $(sort $(wildcard ./in/*.cm)), \
	echo $(notdir $(basename $f));\
	./$(Exec) < ./in/$(notdir $(basename $f)).cm | diff ./out1/$(notdir $(basename $f)).out -;)


