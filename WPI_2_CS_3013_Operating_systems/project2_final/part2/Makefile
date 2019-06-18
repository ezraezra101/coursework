obj-m := smite_module.o
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm smite unsmite spin -f

spin: spin.c
	gcc -o spin spin.c

smite: smite.c
	gcc -o smite smite.c
	
unsmite: unsmite.c
	gcc -o unsmite unsmite.c
	
build:
	make clean all smite unsmite spin

test:
	sudo insmod smite_module.ko
	sudo ./smite 1001 > smite_output.txt
	sleep 1
	sudo ./unsmite < smite_output.txt
	sudo rmmod smite_module.ko
	tail -50 /var/log/syslog > ./syslog-results
	
run:
	make build test
