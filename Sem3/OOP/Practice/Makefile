build:
	javac -d . "$n"/*.java
run:
	make build n=""$n""
	java "$n".MainFile
	rm -r "$n"/*.class
