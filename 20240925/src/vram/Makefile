CROSS = x86_64-elf-

all: boot.bin boot.dis

boot.o: boot.s
	$(CROSS)gcc -m32 -c boot.s

boot.bin: boot.o
	$(CROSS)objcopy -O binary boot.o  boot.bin

boot.dis: boot.o
	$(CROSS)objdump -D -mi8086 boot.o > boot.dis

qemu:
	-qemu-system-x86_64 boot.bin
#	-qemu-system-i386 -drive file=boot.bin,format=raw

clean:
	rm -f boot.bin boot.o *~ \#*\# boot.dis 
