build:
	clear
	g++ -I. -I /usr/local/systemc-2.3.3/include -L. -L/usr/local/systemc-2.3.3/lib-linux64 -Wl,-rpath=/usr/local/systemc-2.3.3/lib-linux64 -lsystemc -lm -o main src/*.cpp
run:
	make build
	./main
	rm main
