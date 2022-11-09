KDIR := $(HOME)/linux-5.4.222/kernel_out
RUNDIR := $(PWD)
export BUSYBOX := $(HOME)/qemu/initramfs/busybox
TESTDIR := $(PWD)/test

MODULE := a2A hello
obj-m := $(foreach n, $(MODULE), $(n).o)
QEMU := qemu-system-x86_64
SHELL := /bin/bash

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	$(MAKE) -C $(TESTDIR)

dir:
	mkdir -p $(BUSYBOX)/mod
	cp *.ko $(BUSYBOX)/mod/
	$(MAKE) -C $(TESTDIR) dir
	$(RM) -f $(RUNDIR)/initramfs.cpio
	cd $(BUSYBOX) && find . | cpio -H newc -o > $(RUNDIR)/initramfs.cpio
	$(RM) -f $(RUNDIR)/initramfs.igz
	cat $(RUNDIR)/initramfs.cpio | gzip > $(RUNDIR)/initramfs.igz

qemu:
	sudo $(QEMU) \
		-kernel $(KDIR)/arch/x86/boot/bzImage \
		-initrd $(RUNDIR)/initramfs.igz \
		-nographic \
		-append "console=ttyS0" \
		-m 512

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) modules clean
	$(MAKE) -C $(TESTDIR) clean
	$(RM) -f *.o *.mod.o *.mod.c *.symvers *.markers *.unsigned *.order
