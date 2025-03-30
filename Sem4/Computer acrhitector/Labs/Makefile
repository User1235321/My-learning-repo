build:
	clear
	grep -n -r "dell"
	g++ "$n"/*.cpp -o "$n"/main
run:
	make build n=""$n""
	./"$n"/main
	rm "$n"/main
vrun:
	make build n=""$n""
	valgrind ./"$n"/main
	rm "$n"/main
