build:
	javac -d . "$n"/src/*.java
run:
	make build n=""$n""
	java "$n".MainFile
	rm -r "$n"/*.class
