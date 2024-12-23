find:
#Borland c++
	find -name "*.OBJ"
	find -name "*.EXE"
	find -name "*.SWP"
	find -name "*.BAK"

#MaxPlusII
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

#Quartus2
	find -name "*.done"
	find -name "*.fit.*"
	find -name "*.map.*"
	find -name "*.qsf"
	find -name "*.sof"
	find -name "*.tan.*"
	find -name "*.qws"
	find -name "*.qpf"
	find -name "*.vwf"
	find -name "*.bsf"
	find -name "db"
	find -name "cmp_state.ini"

#LaTeX
	find -name "*.log"
	find -name "*.tex"
	find -name "*.aux"

#Java (bullshit)
	find -name "*.class"

#Other files
	find -name "*.o"
	find -name "*.t"

clear:
#Borland c++
	find -name "*.OBJ" -delete
	find -name "*.EXE" -delete
	find -name "*.SWP" -delete
	find -name "*.BAK" -delete

#MaxPlusII
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

#Quartus2
	find -name "*.done" -delete
	find -name "*.fit.*" -delete
	find -name "*.map.*" -delete
	find -name "*.qsf" -delete
	find -name "*.sof" -delete
	find -name "*.tan.*" -delete
	find -name "*.qws" -delete
	find -name "*.qpf" -delete
	find -name "*.vwf" -delete
	find -name "*.bsf" -delete
	find -name "cmp_state.ini" -delete

#LaTeX
	find -name "*.log" -delete
	find -name "*.aux" -delete

#Java (bullshit)
	find -name "*.class" -delete

#Other files
	find -name "*.o" -delete
	find -name "*.t" -delete

push:
	make clear
	git status
	git add Sem3/
	git status
	git commit -m "$c"
	git push
