build:
	gcc -Wall -std=c17 ./src/*.c -o vm_translator
debug:
	gcc -Wall -std=c17 -g ./src/*.c -o vm_translator
launch:
	./vm_translator test.txt
clean:
	rm vm_translator
