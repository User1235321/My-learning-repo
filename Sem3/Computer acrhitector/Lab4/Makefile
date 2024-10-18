cbuild:
	g++ main.cpp -o main
crun:
	make cbuild
	valgrind ./main
	rm main
abuild:
	nasm -f elf64 1.asm -o 1.o
	ld -o 1 1.o
