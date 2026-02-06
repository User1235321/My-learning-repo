run:
	gcc main.cpp -o main
	echo "\n"
	./main list.dat 10 &
	./main list.dat 20
	echo "\n"
	hexdump -C list.dat
	echo "\n"
	rm main list.dat
