obj-m := hello.o
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

CPP := g++
cflags := 

hello:
	$(MAKE) -C $(KDIR) M=$(PWD) modules


#Multipurpose make command for simple files
%.c: %.cc header.h
	$(CPP) $(CFLAGS) -o $@ $<

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
