build:
	clear
	grep -n -r "dell"
	g++ *.cpp additionalSystem/*.cpp mainSystem/*.cpp -o main
run:
	make build
	./main
	rm main
valgrind:
	make build
	valgrind ./main
gdb:
	make build
	gdb ./main
print:
	cat main.cpp >> main_cpp.txt
	cat mainSystem/*.cpp >> mainSystem_cpp.txt
	cat mainSystem/*.hpp >> mainSystem_hpp.txt
	cat additionalSystem/*.cpp >> additionalSystem_cpp.txt
	cat additionalSystem/*.hpp >> additionalSystem_hpp.txt
clear:
	rm *.txt
