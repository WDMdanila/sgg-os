GPPPARAMS = -m32 -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti \
	-fno-exceptions -fno-leading-underscore -Wno-write-strings
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = obj/loader.o \
	obj/std.o \
	obj/gdt.o \
	obj/Ports/Port.o \
	obj/Ports/Port8Bit.o \
	obj/Ports/Port16Bit.o \
	obj/Ports/Port32Bit.o \
	obj/interrupts.o \
	obj/interruptstubs.o \
	obj/Drivers/InterruptHandler.o \
	obj/Drivers/KeyboardDriver.o \
	obj/Drivers/MouseDriver.o \
	obj/kernel.o

build/kernel.iso: kernel.bin
	mkdir -p build
	mkdir -p iso/boot/grub
	mv $< iso/boot
	echo 'set timeout=0' >> iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo '' >> iso/boot/grub/grub.cfg
	echo 'menuentry "SGG Operating System" {' >> iso/boot/grub/grub.cfg
	echo '	multiboot /boot/kernel.bin' >> iso/boot/grub/grub.cfg
	echo '	boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso/

kernel.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

obj/%.o: src/%.cpp
	mkdir -p $(@D) || true
	g++ $(GPPPARAMS) -o $@ -c $<

obj/%.o: src/%.s
	mkdir -p $(@D) || true
	as $(ASPARAMS) -o $@ $<

.PHONY: clean
clean:
	rm -rf obj || true
	rm -rf iso || true
	rm -rf build || true

.PHONY: all
all: build/kernel.iso
	echo "Build done!"

.PHONY: run
run: all
	qemu-system-i386 -cdrom ./build/kernel.iso &