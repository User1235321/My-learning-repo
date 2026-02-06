generate:
	g++ generateRandomParams.cpp -o gen
	./gen
	rm gen
run:
	g++ main.cpp -o main -lbdd -O3
	./main
	rm main

full:
	make generate
	make run
