# Gareth Solbeck & Ezra Davis
# HW 3 for CS 3013: Operating Systems

CC := gcc
CFLAGS := -pthread -lpthread
executables := part1 part2

all:
	@ $(MAKE) $(executables)


#Multipurpose simple make command
%: %.c
	$(CC) $(CFLAGS) -o $@ $<

test: $(executables)
	echo "Uses random numbers. Just run executables to test."

clean:
	rm $(executables)
