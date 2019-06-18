obj-m := antivirus.o
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

test:
	cat cleanfile.txt
	cat infectedfile.txt
	tail -50 /var/log/syslog > ./syslog-results
