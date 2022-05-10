
all:boot.img

boot.img:boot.bin loader.bin kernel.bin
	rm -rf boot.img
	rm -rf /mnt/os/HuangOS/bin/boot.bin
	echo "1\nfd\n1.44M\nboot.img\n" | bximage
	dd if=boot.bin of=boot.img bs=512 count=1 conv=notrunc
	mkdir tmp
	mount -t vfat -o loop boot.img tmp/
	cp loader.bin tmp/
	cp kernel.bin tmp/
	sync
	umount tmp/
	rmdir tmp
	cp ./boot.img /mnt/os/HuangOS/bin
	@echo "boot.img has been created successful!!!"
	@echo "done!!!"

clean:
	rm -rf *.bin
