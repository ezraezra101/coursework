#Ezra Davis
#CS2303 makefile: prog2

prog2: controller.o model.o view.o header.h
	gcc -o prog2 controller.o model.o view.o
	make clean
	
controller.o: controller.c header.h
	gcc -Wall -c controller.c

model.o: model.c header.h
	gcc -Wall -c model.c

view.o: view.c header.h
	gcc -Wall -c view.c

clean:
	rm *.o
	
##end of Makefile
