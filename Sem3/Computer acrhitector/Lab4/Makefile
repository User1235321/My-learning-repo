1build:
	nasm -f elf64 1.asm -o 1.o
	ld -o 1 1.o
build:
	grep -n -r "dell"
	g++ "$n".cpp -o "$n"
run:
	make build n=""$n""
	valgrind ./"$n"
	rm "$n"
