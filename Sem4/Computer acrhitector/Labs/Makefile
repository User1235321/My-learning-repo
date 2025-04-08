build:
	clear
	grep -n -r "dell"
	g++ "$n"/*.cpp -o "$n"/main -I/usr/local/include -L/usr/local/lib -lfmod -lfmodstudio -mavx -fopenmp
run:
	make build n=""$n""
	./"$n"/main
	rm "$n"/main
vrun:
	make build n=""$n""
	valgrind ./"$n"/main
	rm "$n"/main
