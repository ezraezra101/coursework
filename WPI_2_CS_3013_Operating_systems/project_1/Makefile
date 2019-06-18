# Gareth Solbeck & Ezra Davis
# HW 1 for CS 3013: Operating Systems

CC := gcc
cflags := 
executables := runCommand shell shell2

all:
	@ $(MAKE) $(executables)


shell2: shell2.c joblist.c shell2.h
	$(CC) $(CFLAGS) -o shell2 shell2.c joblist.c

#Multipurpose simple make command
%: %.c
	$(CC) $(CFLAGS) -o $@ $<

test: $(executables)
	./shell2 <test_shell2.txt >output_shell2.txt
	./shell2 --no-stats <test_shell2_no-stats.txt >output_shell2_no-stats.txt
	./shell  <test_shell.txt  >output_shell.txt
	./test_runCommand.sh      >output_runCommand.txt

clean:
	rm $(executables)