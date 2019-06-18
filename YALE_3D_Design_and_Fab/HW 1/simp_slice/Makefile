src = $(wildcard *.cc)
obj = $(src:.cc=.o)

CC = g++

FLAGS = 

# Include boost headers for unit test library
TESTFLAGS = $(FLAGS) -I/usr/local/Cellar/boost/1.65.0/include -L/usr/local/Cellar/boost/1.65.0/

simp_slice: $(obj)
	$(CC) $(FLAGS) -o $@ $^

tests: $(obj) test.cpp
	# Tests geometry
	$(CC) $(TESTFLAGS) -lboost_unit_test_framework -o $@ Geometry.o test.cpp

.PHONY: clean test

test: tests
	./tests

clean:
	rm -f $(obj) simp_slice tests tests.o