FLAGS=-Wall -Wextra -Wpedantic -Wconversion -Wformat-security -Werror
STD=-std=c17
CMD=gcc
PROG=VMTranslator
FILES=*.c

build: clean
	$(CMD) $(STD) $(FLAGS) $(FILES) -o $(PROG)
debug: clean
	$(CMD) $(STD) $(FLAGS) -g $(FILES) -o $(PROG)
run:
	./$(PROG) test.txt
clean:
	rm -f $(PROG)
	rm -f *.asm
