fBuild:
	clear
	grep -n -r "dell"
	g++ "$n"/*.cpp "$n"/*.f -o "$n"/main -lsfml-graphics -lsfml-window -lsfml-system
fRun:
	make fBuild n=""$n""
	./"$n"/main
	rm "$n"/main

build:
	clear
	grep -n -r "dell"
	g++ "$n"/*.cpp -o "$n"/main -lsfml-graphics -lsfml-window -lsfml-system
run:
	make build n=""$n""
	./"$n"/main
	rm "$n"/main
