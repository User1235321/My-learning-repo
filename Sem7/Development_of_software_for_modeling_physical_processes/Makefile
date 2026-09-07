build:
	g++ -std=c++17 -O2 -Wall -Wextra -Wpedantic -I Lab1/src/solver Lab1/src/solver.cpp Lab1/src/solver/*.cpp -o solver
	g++ -std=c++17 -O2 -Wall -Wextra -Wpedantic -I Lab1/src/visualizer Lab1/src/visualizer.cpp Lab1/src/visualizer/*.cpp -o visualizer -lsfml-graphics -lsfml-window -lsfml-system

run:
	clear
	clang-format -i Lab1/src/*.cpp Lab1/src/solver/*.cpp Lab1/src/solver/*.hpp Lab1/src/visualizer/*.cpp Lab1/src/visualizer/*.hpp Lab1/tests/*.cpp
	make build
	./solver Lab1/input/config.txt
	./visualizer Lab1/results/solution.csv Lab1/results/solution.png
	./visualizer Lab1/results/convergence.csv Lab1/results/convergence.png
	./visualizer Lab1/results/performance.csv Lab1/results/performance.png
	rm solver visualizer

test:
	g++ -std=c++17 -O2 -Wall -Wextra -Wpedantic -I Lab1/src/solver Lab1/src/solver/*.cpp Lab1/tests/*.cpp -o tests
	./tests
	rm tests
