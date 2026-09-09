build:
	g++ Lab"$n"/src/*.cpp -lglut -lGL -lGLU -O2 -o main

run:
	clear
	clang-format -i Lab"$n"/src/*.cpp
	make build
	./main
	rm main
