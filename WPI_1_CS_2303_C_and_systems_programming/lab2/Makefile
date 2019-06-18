#Ezra Davis
#CS2303 makefile: lab2

edavis_lab2: main.o motion.o print_field.o
	gcc -o edavis_lab2 main.o motion.o print_field.o
	
main.o: main.c motion.h
	gcc -Wall -c main.c

motion.o: motion.c motion.h
	gcc -Wall -c motion.c

print_field.o: print_field.c motion.h
	gcc -Wall -c print_field.c

clean:
	rm *.o
	
##end of Makefile
