obj-m += virtual_sensor.o
virtual_sensor-y := vs_main.o vs_chrdev.o vs_fops.o

KDIR ?= /lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean