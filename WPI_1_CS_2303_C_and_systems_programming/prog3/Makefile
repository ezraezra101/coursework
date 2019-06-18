#Ezra Davis
#Program 3 CS 2303
#February 2014

SOURCEFOLDER = source/
OBJFOLDER = objs/
TESTFOLDER = testfiles/
OBJS = 	$(OBJFOLDER)create_node.o $(OBJFOLDER)main.o $(OBJFOLDER)create_eventList.o $(OBJFOLDER)listcpy.o $(OBJFOLDER)print_node.o\
$(OBJFOLDER)print_eventList.o $(OBJFOLDER)insert_node.o $(OBJFOLDER)fcfs.o $(OBJFOLDER)insert_node_queue.o $(OBJFOLDER)round_robin.o\
$(OBJFOLDER)enqueue.o $(OBJFOLDER)dequeue.o $(OBJFOLDER)isEmpty.o $(OBJFOLDER)print_to_file.o $(OBJFOLDER)variance.o $(OBJFOLDER)mean.o\
$(OBJFOLDER)free_list.o $(OBJFOLDER)preemptive_rr.o $(OBJFOLDER)options_to_file.o

CC = gcc
CFLAGS = -Wall -c
LFLAGS = -Wall

edavis_prog3: $(OBJFOLDER)  $(OBJS)
	$(CC) $(LFLAGS) -o edavis_prog3 $(OBJS)
	
$(OBJFOLDER):
	mkdir $(OBJFOLDER)

$(OBJFOLDER)create_eventList.o: $(OBJFOLDER)  $(SOURCEFOLDER)create_eventList.c $(SOURCEFOLDER)header.h
	$(CC) $(CFLAGS) $(SOURCEFOLDER)create_eventList.c -o $(OBJFOLDER)create_eventList.o

$(OBJFOLDER)create_node.o: $(OBJFOLDER)  $(SOURCEFOLDER)create_node.c $(SOURCEFOLDER)header.h
	$(CC) $(CFLAGS) $(SOURCEFOLDER)create_node.c -o $(OBJFOLDER)create_node.o

$(OBJFOLDER)dequeue.o: $(OBJFOLDER)  $(SOURCEFOLDER)dequeue.c $(SOURCEFOLDER)header.h
	$(CC) $(CFLAGS) $(SOURCEFOLDER)dequeue.c -o $(OBJFOLDER)dequeue.o

$(OBJFOLDER)enqueue.o: $(OBJFOLDER)  $(SOURCEFOLDER)enqueue.c $(SOURCEFOLDER)header.h
	$(CC) $(CFLAGS) $(SOURCEFOLDER)enqueue.c -o $(OBJFOLDER)enqueue.o
	
$(OBJFOLDER)fcfs.o: $(OBJFOLDER)  $(SOURCEFOLDER)fcfs.c $(SOURCEFOLDER)header.h
	$(CC) $(CFLAGS) $(SOURCEFOLDER)fcfs.c -o $(OBJFOLDER)fcfs.o

$(OBJFOLDER)free_list.o: $(OBJFOLDER)  $(SOURCEFOLDER)free_list.c $(SOURCEFOLDER)header.h
	$(CC) $(CFLAGS) $(SOURCEFOLDER)free_list.c -o $(OBJFOLDER)free_list.o

$(OBJFOLDER)insert_node_queue.o: $(OBJFOLDER)  $(SOURCEFOLDER)insert_node_queue.c $(SOURCEFOLDER)header.h
	$(CC) $(CFLAGS) $(SOURCEFOLDER)insert_node_queue.c -o $(OBJFOLDER)insert_node_queue.o

$(OBJFOLDER)insert_node.o: $(OBJFOLDER)  $(SOURCEFOLDER)insert_node.c $(SOURCEFOLDER)header.h
	$(CC) $(CFLAGS) $(SOURCEFOLDER)insert_node.c -o $(OBJFOLDER)insert_node.o

$(OBJFOLDER)isEmpty.o: $(OBJFOLDER)  $(SOURCEFOLDER)isEmpty.c $(SOURCEFOLDER)header.h
	$(CC) $(CFLAGS) $(SOURCEFOLDER)isEmpty.c -o $(OBJFOLDER)isEmpty.o

$(OBJFOLDER)listcpy.o: $(OBJFOLDER)  $(SOURCEFOLDER)listcpy.c $(SOURCEFOLDER)header.h
	$(CC) $(CFLAGS) $(SOURCEFOLDER)listcpy.c -o $(OBJFOLDER)listcpy.o

$(OBJFOLDER)main.o: $(OBJFOLDER)  $(SOURCEFOLDER)main.c $(SOURCEFOLDER)header.h
	$(CC) $(CFLAGS) $(SOURCEFOLDER)main.c -o $(OBJFOLDER)main.o

$(OBJFOLDER)mean.o: $(OBJFOLDER)  $(SOURCEFOLDER)mean.c $(SOURCEFOLDER)header.h
	$(CC) $(CFLAGS) $(SOURCEFOLDER)mean.c -o $(OBJFOLDER)mean.o

$(OBJFOLDER)options_to_file.o: $(OBJFOLDER)  $(SOURCEFOLDER)options_to_file.c $(SOURCEFOLDER)header.h
	$(CC) $(CFLAGS) $(SOURCEFOLDER)options_to_file.c -o $(OBJFOLDER)options_to_file.o

$(OBJFOLDER)preemptive_rr.o: $(OBJFOLDER)  $(SOURCEFOLDER)preemptive_rr.c $(SOURCEFOLDER)header.h
	$(CC) $(CFLAGS) $(SOURCEFOLDER)preemptive_rr.c -o $(OBJFOLDER)preemptive_rr.o

$(OBJFOLDER)print_eventList.o: $(OBJFOLDER)  $(SOURCEFOLDER)print_eventList.c $(SOURCEFOLDER)header.h
	$(CC) $(CFLAGS) $(SOURCEFOLDER)print_eventList.c -o $(OBJFOLDER)print_eventList.o

$(OBJFOLDER)print_node.o: $(OBJFOLDER)  $(SOURCEFOLDER)print_node.c $(SOURCEFOLDER)header.h
	$(CC) $(CFLAGS) $(SOURCEFOLDER)print_node.c -o $(OBJFOLDER)print_node.o
	
$(OBJFOLDER)print_to_file.o: $(OBJFOLDER)  $(SOURCEFOLDER)print_to_file.c $(SOURCEFOLDER)header.h
	$(CC) $(CFLAGS) $(SOURCEFOLDER)print_to_file.c -o $(OBJFOLDER)print_to_file.o
	
$(OBJFOLDER)round_robin.o: $(OBJFOLDER)  $(SOURCEFOLDER)round_robin.c $(SOURCEFOLDER)header.h
	$(CC) $(CFLAGS) $(SOURCEFOLDER)round_robin.c -o $(OBJFOLDER)round_robin.o
	
$(OBJFOLDER)variance.o: $(OBJFOLDER)  $(SOURCEFOLDER)variance.c $(SOURCEFOLDER)header.h
	$(CC) $(CFLAGS) $(SOURCEFOLDER)variance.c -o $(OBJFOLDER)variance.o

#$(OBJFOLDER)##.o: $(OBJFOLDER)  $(SOURCEFOLDER)##.c $(SOURCEFOLDER)header.h
#	$(CC) $(CFLAGS) $(SOURCEFOLDER)##.c

debuggable: $(OBJFOLDER)  $(OBJS)
	$(CC) $(LFLAGS) -o debuggable -g $(OBJS)

clean:
	rm $(OBJS)
	[ "`ls -A $(OBJFOLDER)`" ] && echo $(OBJFOLDER) not empty || rmdir $(OBJFOLDER)
	
	
#builds, runs tests, and cleans
run: edavis_prog3
	make test
	make clean

test: edavis_prog3 $(TESTFOLDER)testData.txt $(TESTFOLDER)prog3.dat
	./edavis_prog3 5 1 <$(TESTFOLDER)testData.txt #testfiles and options
	./edavis_prog3 36 5 <$(TESTFOLDER)prog3.dat
	#cat testfile
	
