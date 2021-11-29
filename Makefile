FLAGS=-Wall -Wextra -Wpedantic -Wconversion -Wformat-security -Werror
STD=-std=c17
CMD=gcc
PROG=VMTranslator
FILES=*.c

build:
	$(CMD) $(STD) $(FLAGS) $(FILES) -o $(PROG)
debug:
	$(CMD) $(STD) $(FLAGS) -g $(FILES) -o $(PROG)
run:
	./$(PROG) test.txt
clean:
	rm $(PROG)
