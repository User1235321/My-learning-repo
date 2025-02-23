build:
	clear
	grep -n -r "dell"
	g++ "$n"/*.cpp "$n"/*.f -o "$n"/main -lsfml-graphics -lsfml-window -lsfml-system
run:
	make build n=""$n""
	./"$n"/main
	rm "$n"/main
