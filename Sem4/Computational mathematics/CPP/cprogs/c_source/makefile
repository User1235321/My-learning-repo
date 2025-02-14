# makefile for Genetic Algorithm optimizer
# P. Jacobs
# Department of Mechanical Engineering
# University of Queensland
#
# Updated...
# 20-July-96 : emx version
# 17-Feb-98  : Win32 flags
#
#--------------------------------------------------------
#
BIN=.
CC=gcc

#----------------------------------------------------------------------
# EMX/OS/2 executables

for_emx : $(BIN)/ga_optd.exe $(BIN)/ga_optd2.exe

$(BIN)/ga_optd2.exe : $(BIN)/ga_optd2.o $(BIN)/ga_opt.o $(BIN)/random.o
        $(CC) -Zwin32 $(BIN)/ga_optd2.o $(BIN)/ga_opt.o $(BIN)/random.o \
                -o $(BIN)/ga_optd2.exe
        pestack -s8192 -c4096 $(BIN)/ga_optd2.exe

$(BIN)/ga_optd.exe : $(BIN)/ga_optd.o $(BIN)/ga_opt.o $(BIN)/random.o
        $(CC) -Zwin32 $(BIN)/ga_optd.o $(BIN)/ga_opt.o $(BIN)/random.o \
                -o $(BIN)/ga_optd.exe
        pestack -s8192 -c4096 $(BIN)/ga_optd.exe

#---------------------------------------------------------------------
# Object files.

$(BIN)/ga_optd.o : ga_optd.c ga_opt.h
        $(CC) -c -O -Zwin32 ga_optd.c -o $(BIN)/ga_optd.o

$(BIN)/ga_optd2.o : ga_optd2.c ga_opt.h
        $(CC) -c -O -Zwin32 ga_optd2.c -o $(BIN)/ga_optd2.o

$(BIN)/ga_opt.o : ga_opt.c ga_opt.h
        $(CC) -c -O -Zwin32 ga_opt.c -o $(BIN)/ga_opt.o

$(BIN)/random.o : random.c random.h
        $(CC) -c -O -Zwin32 random.c -o $(BIN)/random.o

