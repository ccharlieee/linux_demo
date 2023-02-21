KERNELDIR := /home/edu/work/work/git/linux_demo/linux_kernel
CURRENT_PATH := $(shell pwd)
obj-m := driver.o

build: kernel_modules

kernel_modules:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) modules
clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) clean
