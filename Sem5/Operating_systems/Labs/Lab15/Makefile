static:
	clear
	gcc -c matrix.c -o matrix.o
	ar rcs libmatrix.a matrix.o
	gcc main.c -L. -lmatrix -o program_static
	./program_static
	rm program_static

dynamic:
	clear
	gcc -c -fPIC matrix.c -o dynMatrix.o
	gcc -shared -o libmatrix.so dynMatrix.o
	gcc main.c -L. -lmatrix -Wl,-rpath,. -o program_dynamic
	./program_dynamic
	rm program_dynamic
