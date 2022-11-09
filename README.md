# LAB: Linux Kernel Module & Device

## Need

- Linux Kernel Source & Image
- BusyBox
- QEMU (qemu-system-x86_64)

# Build Initramfs

Setup and complie busybox: 

```bash
# Init config
make defconfig
# Fix following config:
# Settings
# - Build Options
# [*] Build BusyBox as a static binary (no shared libs)
# - Installation Options ("make install" behavior)
# (./_install) Destination path for 'make install'
make menuconfig
make -j2
make install
```

Build initramfs (DIR: busybox):

```bash
mkdir -p busybox
cd busybox
mkdir -pv {bin,dev,sbin,etc,proc,sys/kernel/debug,usr/{bin,sbin},lib,lib64,mnt/root,root}
# where "_install" is
cp -av $PATH_BUSYBOX/_install/* ./
sudo cp -av /dev/{null,console,tty,sda1} ./dev/
```

Write init script:

```bash
vim ./init
chmod +x ./init
```

```sh
#!/bin/sh
 
mount -t proc none /proc
mount -t sysfs none /sys
mount -t debugfs none /sys/kernel/debug
 
echo -e "\nBoot took $(cut -d' ' -f1 /proc/uptime) seconds\n"
 
exec /bin/sh
```

Value: $(BUSYBOX) in Makefile should set to the directory below.

## Complie & Run Qemu

Edit Makefile manually:

  - KDIR: root of kernel source
  - BUSYBOX: where is initramfs
  - RUNDIR: where to put initramfs image

Run following commands:

```bash
make
make dir
make qemu
```

## Module & Device

Commands for module:

```bash
# Install
insmod mod/module_name.ko
# List
lsmod
# Remove
rmmod module_name
```

Commands for device:

```bash
# Get Dev Number
cat /proc/devices | grep dev_name
# Make Dev File
mknod [-m MODE] NAME TYPE [MAJOR MINOR]
mknod /dev/a2A c 248 0
```

Commands for test:

```bash
# Write, end with C-d
dd of=/dev/dev_name
# Read
dd if=/dev/dev_name bs=1 count=20
# Use tools
./test_mod/a2A.out a2A_dev_name
```