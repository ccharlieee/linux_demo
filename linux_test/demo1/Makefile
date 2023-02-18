obj-m := driver.o
KERNELDIR = /home/edu/work/linux
PWD = $(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=`pwd`
	rm -rf *.order *.mod.* *.o *.symvers
clean:
	rm -rf *.ko