build:
	clear
	grep -n -r "dell"
	g++ *.cpp -o main
run:
	make build
	./main
test:
	make build
	valgrind ./main
	rm main
print:
	clear
	cat *.cpp
	cat *.hpp
