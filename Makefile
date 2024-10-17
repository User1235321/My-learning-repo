find:
	find -name "*.OBJ"
	find -name "*.EXE"
	find -name "*.SWP"
	find -name "*.BAK"
	find -name "*.o"
clear:
	find -name "*.OBJ" -delete
	find -name "*.EXE" -delete
	find -name "*.SWP" -delete
	find -name "*.BAK" -delete
	find -name "*.o" -delete
push:
	make clear
	git status
	git add Sem3/
	git status
	git commit -m "$c"
	git push
