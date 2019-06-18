OBJS = 	create_node.o main.o create_eventList.o listcpy.o print_node.o\
print_eventList.o insert_node.o fcfs.o insert_node_queue.o round_robin.o\
enqueue.o dequeue.o isEmpty.o print_to_file.o variance.o mean.o

CC = gcc
CFLAGS = -Wall -c
LFLAGS = -Wall

edavis_prog3: $(OBJS)
	$(CC) $(LFLAGS) -o edavis_prog3 $(OBJS)

create_node.o: create_node.c header.h
	$(CC) $(CFLAGS) create_node.c

create_eventList.o: create_eventList.c header.h
	$(CC) $(CFLAGS) create_eventList.c

main.o: main.c header.h
	$(CC) $(CFLAGS) main.c

listcpy.o: listcpy.c header.h
	$(CC) $(CFLAGS) listcpy.c

print_node.o: print_node.c header.h
	$(CC) $(CFLAGS) print_node.c

print_eventList.o: print_eventList.c header.h
	$(CC) $(CFLAGS) print_eventList.c

insert_node.o: insert_node.c header.h
	$(CC) $(CFLAGS) insert_node.c
	
fcfs.o: fcfs.c header.h
	$(CC) $(CFLAGS) fcfs.c
	
insert_node_queue.o: insert_node_queue.c header.h
	$(CC) $(CFLAGS) insert_node_queue.c

round_robin.o: round_robin.c header.h
	$(CC) $(CFLAGS) round_robin.c

enqueue.o: enqueue.c header.h
	$(CC) $(CFLAGS) enqueue.c

dequeue.o: dequeue.c header.h
	$(CC) $(CFLAGS) dequeue.c

isEmpty.o: isEmpty.c header.h
	$(CC) $(CFLAGS) isEmpty.c

print_to_file.o: print_to_file.c header.h
	$(CC) $(CFLAGS) print_to_file.c
	
mean.o: mean.c header.h
	$(CC) $(CFLAGS) mean.c

variance.o: variance.c header.h
	$(CC) $(CFLAGS) variance.c

##.o: ##.c header.h
#	$(CC) $(CFLAGS) ##.c

clean:
	rm $(OBJS)
	
	
#builds, runs tests, and cleans
run: edavis_prog3
	make
	make test
	make clean

test: edavis_prog3 testData.txt #testfiles go here
	./edavis_prog3 5 1 <testData.txt #testfiles and options
	./edavis_prog3 36 5 <prog3.dat
	#cat testfile
	
#Ezra Davis
#Lab 3 CS 2303
#January 2014