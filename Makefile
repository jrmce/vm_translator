FLAGS=-Wall -Wextra -Wpedantic -Wconversion -Wformat-security -Werror
STD=-std=c17
CMD=gcc
PROG=vm_translator
FILES=./src/*.c

build:
	$(CMD) $(STD) $(FLAGS) $(FILES) -o $(PROG)
debug:
	$(CMD) $(STD) $(FLAGS) -g $(FILES) -o $(PROG)
run:
	./$(PROG) src/test.txt
clean:
	rm $(PROG)
