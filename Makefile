find:
	find -name "*.OBJ"
	find -name "*.EXE"
	find -name "*.SWP"
	find -name "*.BAK"
	find -name "*.o"
	find -name "*.acf"
	find -name "*.fit"
	find -name "*.cnf"
	find -name "*.hif"
	find -name "*.mmf"
	find -name "*.ndb"
	find -name "*.pin"
	find -name "*.pof"
	find -name "*.rpt"
	find -name "*.snf"
	find -name "*.DLS"
	find -name "*.sym"
	find -name "*.t"

clear:
	find -name "*.OBJ" -delete
	find -name "*.EXE" -delete
	find -name "*.SWP" -delete
	find -name "*.BAK" -delete
	find -name "*.o" -delete
	find -name "*.acf" -delete
	find -name "*.fit" -delete
	find -name "*.cnf" -delete
	find -name "*.hif" -delete
	find -name "*.mmf" -delete
	find -name "*.ndb" -delete
	find -name "*.pin" -delete
	find -name "*.pof" -delete
	find -name "*.rpt" -delete
	find -name "*.snf" -delete
	find -name "*.DLS" -delete
	find -name "*.sym" -delete
	find -name "*.t" -delete
push:
	make clear
	git status
	git add Sem3/
	git status
	git commit -m "$c"
	git push
