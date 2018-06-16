# RamDisk - Makefile
#

# Kernel modules will be build with "kbuild"

# obj-m -- object modules, += append
obj-m += SystemCallHooking.o

# set compiler flags for this module, $(EXTRA_CFLAGS) is for backward compatibility
#	see: http://stackoverflow.com/a/11237805
ccflags-y  += $(EXTRA_CFLAGS) -std=gnu11

# mfd-objs := mfd.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
