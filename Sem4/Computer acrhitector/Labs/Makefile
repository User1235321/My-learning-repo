build:
	clear
	grep -n -r "dell"
	g++ $n/*.cpp -o $n/main $f
run:
	make build n="$n" flags="$f"
	./$n/main
	rm $n/main
Lab1:
	make run n=Lab1
Lab2:
	make run n=Lab2 f="-mavx"
Lab3:
	make run n=Lab3 f="-I/usr/local/include -L/usr/local/lib -openmp"
Lab4:
	make run n=Lab4 f="-I/usr/local/include -L/usr/local/lib -lfmod -lfmodstudio"
Lab5:
	make run n=Lab5 f="`pkg-config --cflags --libs opencv4`"
Lab6:
	make run n=Lab6
